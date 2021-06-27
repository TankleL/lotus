#include <msgpack.hpp>
#include "protobase.hxx"

namespace lotus::core::protocols
{
    namespace utils
    {
        template<typename T>
        inline T msgpack_obj_as(void* native_obj_handle)
        {
            using namespace msgpack;
            try
            {
                auto oh =
                    static_cast<object_handle*>(native_obj_handle);
                const auto& o = oh->get();
                return o.as<T>();
            }
            catch (unpack_error)
            {
                throw UnpackError();
            }
            catch (type_error)
            {
                throw UnpackError();
            }
        }
    }

    // UnpackError --------------------------------------------------
    UnpackError::UnpackError()
        : std::runtime_error("Unpacking protocol data failed")
    {}

    // PackedPackage ------------------------------------------------------
    PackedPackage::PackedPackage() noexcept
        : _native_stream(new msgpack::sbuffer())
    {
        static constexpr char fixed_header[] = { 0, 0 };
        auto s = static_cast<msgpack::sbuffer*>(_native_stream);
        s->write(fixed_header, FIXED_HEADER_LENGTH);
    }

    PackedPackage::PackedPackage(PackedPackage&& rhs) noexcept
        : _native_stream(std::move(rhs._native_stream))
    {}

    PackedPackage::~PackedPackage() noexcept
    {
        delete static_cast<msgpack::sbuffer*>(_native_stream);
    }

    PackedPackage& PackedPackage::operator=(PackedPackage&& rhs) noexcept
    {
        _native_stream = std::move(rhs._native_stream);
        return *this;
    }

    void* PackedPackage::native_stream() const noexcept
    {
        return _native_stream;
    }

    void PackedPackage::refresh_header() noexcept
    {
        auto s = static_cast<msgpack::sbuffer*>(_native_stream);
        const size_t payload_len = s->size() - FIXED_HEADER_LENGTH;
        const uint16_t masked_payload_len = payload_len & 0x0000ffff;

        char* buf = s->data();
        buf[0] = static_cast<char>(masked_payload_len & 0x00ff);
        buf[1] = static_cast<char>(masked_payload_len >> 8);
    }

    const char* PackedPackage::data() const noexcept
    {
        auto s = static_cast<msgpack::sbuffer*>(_native_stream);
        return s->data();
    }

    size_t PackedPackage::length() const noexcept
    {
        auto s = static_cast<msgpack::sbuffer*>(_native_stream);
        return s->size();
    }

    // Packer -------------------------------------------------------
    Packer::Packer() noexcept
        : _native_pac(nullptr)
    {
        using namespace msgpack;
        auto pac = new packer<sbuffer>(
            static_cast<sbuffer*>(_package.native_stream()));
        _native_pac = pac;
    }

    Packer::Packer(Packer&& rhs) noexcept
        : _native_pac(std::move(rhs._native_pac))
        , _package(std::move(rhs._package))
    {}

    Packer::~Packer() noexcept
    {
        using namespace msgpack;
        delete static_cast<packer<sbuffer>*>(_native_pac);
    }

    Packer& Packer::operator=(Packer&& rhs) noexcept
    {
        _native_pac = std::move(rhs._native_pac);
        _package = std::move(rhs._package);
        return *this;
    }

    void Packer::pack_int32(int32_t val)
    {
        auto pac =
            static_cast<msgpack::packer<msgpack::sbuffer>*>(
                _native_pac);
        pac->pack_int32(val);
    }

    void Packer::pack_uint32(uint32_t val)
    {
        auto pac =
            static_cast<msgpack::packer<msgpack::sbuffer>*>(
                _native_pac);
        pac->pack_uint32(val);
    }

    void Packer::pack_blob(const std::vector<char>& blob)
    {
        auto pac =
            static_cast<msgpack::packer<msgpack::sbuffer>*>(
                _native_pac);

        uint32_t blen = static_cast<uint32_t>(blob.size());
        assert(blen <= std::numeric_limits<uint16_t>::max());
        pac->pack_bin(blen);
        if(blen > 0)
        {
            pac->pack_bin_body(blob.data(), blen);
        }
    }

    PackedPackage& Packer::result()
    {
        return _package;
    }

    // Unpacker -----------------------------------------------------
    Unpacker::Unpacker() noexcept
        : _native_pac(new msgpack::unpacker())
        , _native_obj_handle(new msgpack::object_handle())
    {}

    Unpacker::Unpacker(const char* data, size_t length) noexcept
    {
        consume(data, length);
    }

    Unpacker::Unpacker(Unpacker&& rhs) noexcept
        : _native_pac(std::move(rhs._native_pac))
        , _native_obj_handle(std::move(rhs._native_obj_handle))
    {}

    Unpacker::~Unpacker() noexcept
    {
        using namespace msgpack;
        delete static_cast<object_handle*>(_native_obj_handle);
        delete static_cast<unpacker*>(_native_pac);
    }

    Unpacker& Unpacker::operator=(Unpacker&& rhs) noexcept
    {
        _native_pac = std::move(rhs._native_pac);
        _native_obj_handle = std::move(rhs._native_obj_handle);
        return *this;
    }

    void Unpacker::consume(const char* data, size_t length) noexcept
    {
        auto pac = static_cast<msgpack::unpacker*>(_native_pac);
        pac->reserve_buffer(length);
        memcpy(pac->buffer(), data, length);
        pac->buffer_consumed(length);
    }

    bool Unpacker::next() noexcept
    {
        using namespace msgpack;
        auto pac = static_cast<unpacker*>(_native_pac);
        auto oh = static_cast<object_handle*>(_native_obj_handle);
        return pac->next(*oh);
    }

    size_t Unpacker::parsed_size() const noexcept
    {
        auto pac = static_cast<msgpack::unpacker*>(_native_pac);
        return pac->parsed_size();
    }

    int32_t Unpacker::to_int32() const
    {
        return utils::msgpack_obj_as<int32_t>(_native_obj_handle);
    }

    uint32_t Unpacker::to_uint32() const
    {
        return utils::msgpack_obj_as<uint32_t>(_native_obj_handle);
    }

    void Unpacker::to_blob(std::vector<char>& out) const
    {
        using namespace msgpack;
        auto oh = static_cast<object_handle*>(_native_obj_handle);
        const auto& o = oh->get();
        if (o.type != type::BIN)
            throw UnpackError();
        if(o.via.bin.size >=
            std::numeric_limits<uint32_t>::max())
            throw UnpackError();
        o >> out;
    }

    // ProtocolBase -------------------------------------------------
    ProtocolBase::ProtocolBase() noexcept
        : proto_type(ProtocolType::unknown)
    {}

    ProtocolBase::ProtocolBase(ProtocolBase&& rhs) noexcept
        : proto_type(std::move(rhs.proto_type))
    {}

    ProtocolBase& ProtocolBase::operator=(ProtocolBase&& rhs) noexcept
    {
        proto_type = std::move(rhs.proto_type);
        return *this;
    }

    PackedPackage ProtocolBase::pack() noexcept
    {
        Packer pac;
        on_packing(pac);
        auto& ret = pac.result();
        ret.refresh_header();
        return std::move(ret);
    }

    size_t ProtocolBase::unpack(const char* data,size_t length)
    {
        Unpacker pac(data, length);
        return on_unpacking(pac);
    }

    void ProtocolBase::on_packing(Packer& pac) noexcept
    {
        pac.pack_int32(static_cast<int32_t>(proto_type));
    }

    size_t ProtocolBase::on_unpacking(Unpacker& pac)
    {
        if (pac.next())
            proto_type = static_cast<ProtocolType>(pac.to_int32());
        return pac.parsed_size();
    }
} // namespace lotus::core::protocols



