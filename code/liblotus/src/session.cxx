#include <cassert>

#include "proto-session-lstnr.hxx"
#include "session.hxx"

namespace lotus::core
{
    Session::Session(std::weak_ptr<IConnection> conn) noexcept
        : _id(UUID())
        , _conn(conn)
        , _nsrs_state(new_session_req_state_e::not_sent)
    {}

    Session::Session(const UUID& id,
        std::weak_ptr<IConnection> conn) noexcept
        : _id(id)
        , _conn(conn)
        , _nsrs_state(new_session_req_state_e::not_sent)
    {}

    void Session::send_msg(const char* data, size_t length) noexcept
    {
        assert(
            length > 0 &&
            length <= std::numeric_limits<uint32_t>::max());

        using namespace protocols::proto_session_lstnr;
        SessionReq req;
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

        using namespace protocols::proto_session_lstnr;
        SessionReq req;
        _ensure_nsrs(req);
        req.payload = std::move(data);

        _send_req(req);
    }

    void Session::_send_req(
        protocols::proto_session_lstnr::SessionReq& req) noexcept
    {
        auto conn = _conn.lock();
        if (conn != nullptr)
        {
            auto package = req.pack();
            conn->write(); // TODO:
        }
    }

    void Session::_ensure_nsrs(
        protocols::proto_session_lstnr::SessionReq& req) noexcept
    {
        using namespace protocols::proto_session_lstnr;
        switch (_nsrs_state)
        {
        case new_session_req_state_e::not_sent:
            req.intention = SessionReq::intention_e::new_session;
            break;

        case new_session_req_state_e::wait_for_rsp: [[fallthrough]];
        case new_session_req_state_e::remote_acked:
            req.intention = SessionReq::intention_e::session_data;
            break;
        }
    }
}

