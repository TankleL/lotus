#include <cassert>
#include "session-lstnr.hxx"
#include "proto-session-lstnr.hxx"

namespace lotus::core
{
    SessionListener::SessionListener() noexcept
        : _parse_state(_parse_state_e::idle)
        , _tmp_pack_length(0)
        , _tmp_readlength(0)
    {}

    SessionListener::~SessionListener() noexcept
    {}

    bool SessionListener::parse(
        const char* data,
        size_t length) noexcept
    {
        for (size_t i = 0; i < length; ++i)
        {
            const char curval = data[i];
            size_t wideval = 0;
            switch (_parse_state)
            {
            case _parse_state_e::idle:
                // initialize temporary fields
                _tmp_pack_length = 0;
                _tmp_readlength = 0;
                
                // next state
                _parse_state = _parse_state_e::len0;

                [[fallthrough]]; // this is important

            case _parse_state_e::len0:
                // low 8 bytes of the length
                _tmp_pack_length = curval;

                // next state
                _parse_state = _parse_state_e::len1;
                break;

            case _parse_state_e::len1:
                // high 8 bytes of the length
                wideval = curval;
                _tmp_pack_length |= wideval << 8;

                // next state
                _parse_state = _parse_state_e::payload;
                break;

            case _parse_state_e::payload:
                if (_tmp_pack_length != 0)
                {
                    _ensure_tmp_pack_data_container();

                    const auto remaining_len = length - i;
                    const auto len_to_parse =
                        std::min(remaining_len, _tmp_pack_length);

                    if (_parse_payload_single_pack(
                        data + i,
                        len_to_parse))
                    {
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

    bool SessionListener::_parse_payload_single_pack(
        const char* data,
        size_t length) noexcept
    {
        memcpy(_tmp_pack_data.data(), data, length);
        _tmp_readlength += length;

        if (_tmp_readlength == _tmp_pack_length)
        {
            using namespace protocols::proto_session_lstnr;

            SessionReq req;
            if (!req.unpack(data, length))
                return false;
            if (req.request_id != SessionReq::REQ_ID)
                return false;

            switch(req.intention)
            {
            case SessionReq::intention_e::new_session:
                _new_session();
                break;

            default:
                // bad or not supported intentions
                return false;
            }

            _parse_state = _parse_state_e::idle;
            return true;
        }
        return false;
    }

    void SessionListener::_ensure_tmp_pack_data_container()
    {
        assert(_tmp_pack_length > 0);
        _tmp_pack_data.resize(_tmp_pack_length);
    }

    void SessionListener::_new_session()
    {
        auto sid = UUID::generate_v4();
        auto sess = std::make_shared<Session>(sid, _conn);

        assert(_sessions.find(sid) == _sessions.cend());
        _sessions.insert(std::make_pair(sid, sess));
    }

    SessionListener& SessionListener::bind(
        const std::shared_ptr<IServerSideConnection>& conn) noexcept
    {
        auto instance = std::make_unique<SessionListener>();

        auto* retval = instance.get();
        retval->_conn = conn;

        conn->attach(conn->ATTID_SessionListener, std::move(instance));
        conn->on_data_received = []
        (IServerSideConnection& conn,
            const char* data,
            size_t len) -> bool
        {
            auto listener =
                conn.attachment<SessionListener>(
                    conn.ATTID_SessionListener);
            assert(listener != nullptr);
            return listener->parse(data, len);
        };

        return *retval;
    }
}

