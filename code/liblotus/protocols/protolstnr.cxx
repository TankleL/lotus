#include <cassert>

#include "protobase.hxx"
#include "protolstnr.hxx"

namespace lotus::core::protocols
{
    // ListenedPackage ----------------------------------------------
    ListenedPackage::ListenedPackage() noexcept
        : _buf(nullptr)
        , _length(0)
    {}

    ListenedPackage::ListenedPackage(size_t size) noexcept
        : _buf(new char[size])
        , _length(size)
    {}

    ListenedPackage::ListenedPackage(ListenedPackage&& rhs) noexcept
        : _buf(std::move(rhs._buf))
        , _length(std::move(rhs._length))
    {}

    ListenedPackage::~ListenedPackage() noexcept
    {
        delete[] _buf;
        _length = 0;
    }

    ListenedPackage& ListenedPackage::operator=(
        ListenedPackage&& rhs) noexcept
    {
        _buf = std::move(rhs._buf);
        _length = std::move(rhs._length);
        return *this;
    }

    char* ListenedPackage::data() const noexcept
    {
        return _buf;
    }

    size_t ListenedPackage::size() const noexcept
    {
        return _length;
    }

    ListenedPackage& ListenedPackage::acquire(
        ListenedPackage& src) noexcept
    {
        _buf = src._buf;
        _length = src._length;

        src._buf = nullptr;
        src._length = 0;

        return *this;
    }

    bool ListenedPackage::is_available() const noexcept
    {
        return _buf != nullptr;
    }

    ListenedPackage& ListenedPackage::redefine(size_t size) noexcept
    {
        if(_buf == nullptr || _length != size)
        {
            delete[] _buf;
            _buf = new char[size];
            _length = size;
        }
        return *this;
    }

    // ProtoListener ------------------------------------------------
    ProtoListener::ProtoListener() noexcept
        : _conn(nullptr)
        , _parse_state(ParseState::idle)
        , _tmp_pack_size(0)
        , _tmp_pack_read(0)
    {}

    ProtoListener::~ProtoListener() noexcept
    {}

    ProtoListener& ProtoListener::bind(IConnection* conn) noexcept
    {
        auto instance = std::make_unique<ProtoListener>();
        auto& retval = *instance;
        retval._conn = conn;
        conn->attach(
            conn->ATTID_ProtoListener,
            std::move(instance));
        conn->on_read = []
            (IConnection& conn,
             const char* data,
             size_t len) -> bool
        {
            auto listener =
                conn.attachment<ProtoListener>(
                    conn.ATTID_ProtoListener);
            assert(listener != nullptr);
            return listener->parse(data, len);
        };
        return retval;
    }

    void ProtoListener::add_request_callback(
            uint32_t req_id,
            const req_callback_t& cb) noexcept
    {
        _req_map[req_id] = cb;
    }

    void ProtoListener::add_response_callback(
            uint32_t rsp_id,
            const rsp_callback_t& cb) noexcept
    {
        _req_map[rsp_id] = cb;
    }

    bool ProtoListener::parse(
        const char* data,
        size_t length) noexcept
    {
        size_t i = 0;
        while(i < length)
        {
            const char curval = data[i];
            size_t wideval = 0;
            switch (_parse_state)
            {
            case ParseState::idle:
            case ParseState::len0:
                // initialize temporary fields
                _tmp_pack_size = 0;
                _tmp_pack_read = 0;

                // low 8 bytes of the length
                _tmp_pack_size = curval;

                // next state
                _parse_state = ParseState::len1;
                ++i;
                break;

            case ParseState::len1:
                // high 8 bytes of the length
                wideval = curval;
                _tmp_pack_size |= wideval << 8;

                if (_tmp_pack_size >
                    std::numeric_limits<uint16_t>::max())
                {
                    return false;
                }

                // next state
                _parse_state = ParseState::payload;
                ++i;
                break;

            case ParseState::payload:
                if (_tmp_pack_size != 0)
                {
                    _ensure_pack();

                    const auto max_len = length - i;
                    const auto pack_left_len =
                        _tmp_pack_size - _tmp_pack_read;
                    const auto parse_len =
                        std::min(
                            pack_left_len,
                            max_len);

                    if(_parse_single_pack(data + i, parse_len))
                    {
                        i += parse_len;
                        if(_tmp_pack_read == _tmp_pack_size)
                            _parse_state = ParseState::idle;
                        continue;
                    }
                    else
                    {
                        // the connection seems really bad,
                        // try to kill it.
                        return false;
                    }
                }
                else
                {
                    // the connection seems really bad,
                    // try to kill it
                    return false;
                }
                break;
            }
        }

        return true;
    }

    void ProtoListener::_ensure_pack() noexcept
    {
        assert(_tmp_pack_size > 0);
        if(!_tmp_pack.is_available() ||
            _tmp_pack.size() != _tmp_pack_size)
        {
            _tmp_pack.redefine(_tmp_pack_size);
        }
    }

    bool ProtoListener::_parse_single_pack(
        const char* data,
        size_t length) noexcept
    {
        assert(length <= _tmp_pack_size &&
            _tmp_pack_read + length <= _tmp_pack_size);

        memcpy(_tmp_pack.data(), data, length);
        _tmp_pack_read += length;

        if (_tmp_pack_read == _tmp_pack_size)
        {
            ListenedPackage pack;
            pack.acquire(_tmp_pack);
            return _dispatch(std::move(pack));
        }
        return true;
    }

    bool ProtoListener::_dispatch(ListenedPackage&& pack) noexcept
    {
        ProtocolBase pb;
        size_t read = pb.unpack(pack.data(), pack.size());
        if(pb.UNPACK_ERROR == read)
            return false;

        try
        {
            switch (pb.proto_type)
            {
            case ProtocolBase::ProtocolType::request:
            {
                ProtocolRequest<ProtocolBase> req(std::move(pb));
                auto req_read = req.unpack(
                    pack.data() + read,
                    pack.size() - read);
                if (req.UNPACK_ERROR == req_read)
                    return false;

                const auto& entry = _req_map.find(req.request_id);
                if (entry != _req_map.cend())
                {
                    return entry->second(
                        std::move(req),
                        std::move(pack),
                        read + req_read);
                }
            }
                break;
            case ProtocolBase::ProtocolType::response:
            case ProtocolBase::ProtocolType::notification:
            case ProtocolBase::ProtocolType::unknown:
                break;

            default:
                assert(false);
                break;
            }
        }
        catch(UnpackError)
        {
            return false;
        }

        return true;
    }

} // namespace lotus::core



