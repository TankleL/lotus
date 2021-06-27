#include <cassert>

#include "proto-session-lstnr.hxx"
#include "session.hxx"

namespace lotus::core
{
    namespace pt = protocols;
    namespace pts = protocols::proto_session;

    Session::Session(IConnection* conn) noexcept
        : _id(0)
        , _conn(conn)
        , _nsrs_state(new_session_req_state_e::not_sent)
    {}

    Session::Session(uint32_t id, IConnection* conn) noexcept
        : _id(id)
        , _conn(conn)
        , _nsrs_state(new_session_req_state_e::not_sent)
    {}

    void Session::send_msg(const char* data, size_t length) noexcept
    {
        assert(
            length > 0 &&
            length <= std::numeric_limits<uint32_t>::max());

        pts::SessionReq<pt::ZeroBased> req;
        _ensure_nsrs(req);

        req.payload.resize(length);
        memcpy(req.payload.data(), data, length);

        _send_req(req);
    }

    void Session::send_msg(std::vector<char>&& data) noexcept
    {
        assert(
            data.size() > 0 &&
            data.size() <= std::numeric_limits<uint32_t>::max());

        pts::SessionReq<pt::ZeroBased> req;
        _ensure_nsrs(req);
        req.payload = std::move(data);

        _send_req(req);
    }

    void Session::_send_req(session_req_t& req) noexcept
    {
        assert(_conn != nullptr);

        auto package = req.pack();
        _conn->write(package.data(), package.length());
    }

    void Session::_ensure_nsrs(session_req_t& req) noexcept
    {
        //switch (_nsrs_state)
        //{
        //case new_session_req_state_e::not_sent:
        //    req.intention = session_req_t::Intention::new_session;
        //    break;
        //case new_session_req_state_e::wait_for_rsp:
        //case new_session_req_state_e::remote_acked:
        //    req.intention = session_req_t::Intention::session_data;
        //    break;
        //}
    }
}

