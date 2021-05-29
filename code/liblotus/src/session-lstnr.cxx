#include <cassert>
#include "session-lstnr.hxx"
#include "proto-session-lstnr.hxx"

namespace lotus::core
{
    SessionListener::SessionListener() noexcept
        : _parse_state(_parse_state_e::PS_Idle)
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
            case _parse_state_e::PS_Idle:
                // initialize temporary fields
                _tmp_pack_length = 0;
                _tmp_readlength = 0;
                
                // next state
                _parse_state = _parse_state_e::PS_Len0;

                [[fallthrough]]; // this is important

            case _parse_state_e::PS_Len0:
                // low 8 bytes of the length
                _tmp_pack_length = curval;

                // next state
                _parse_state = _parse_state_e::PS_Len1;
                break;

            case _parse_state_e::PS_Len1:
                // high 8 bytes of the length
                wideval = curval;
                _tmp_pack_length |= wideval << 8;

                // next state
                _parse_state = _parse_state_e::PS_Payload;
                break;

            case _parse_state_e::PS_Payload:
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

            ControlHeaderReq req;
            if (!req.unpack(data, length))
                return false;
            if (req.request_id != ControlHeaderReq::REQ_ID)
                return false;

            switch(req.intention)
            {
            case ControlHeaderReq::intention_e::NewSession:
                break;

            default:
                // bad or not supported intentions
                return false;
            }
        }
    }

    void SessionListener::_ensure_tmp_pack_data_container()
    {

    }

    SessionListener& SessionListener::bind(
        IServerSideConnection& conn) noexcept
    {
        auto instance = std::make_unique<SessionListener>();
        auto* retval = instance.get();

        conn.attach(conn.ATTID_SessionListener, std::move(instance));
        conn.on_data_received = []
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

