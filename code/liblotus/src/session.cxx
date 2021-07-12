#include <cassert>

#include "proto-session.hxx"
#include "session.hxx"

namespace lotus::core
{
    namespace pt = protocols;
    namespace pts = protocols::proto_session;

    Session::Session(IConnection* conn) 
        : _id(0)
        , _conn(conn)
    {}

    Session::Session(uint32_t id, IConnection* conn) 
        : _id(id)
        , _conn(conn)
    {}

    void Session::send_msg(const char* data, size_t length) 
    {
        assert(
            length > 0 &&
            length <= std::numeric_limits<uint32_t>::max());

        pts::SessionReq<pt::ZeroBased> req;
        req.session_id = _id;
        req.intention =
            pts::SessionReq<pt::ZeroBased>::Intention::session_data;
        req.payload.resize(length);
        memcpy(req.payload.data(), data, length);

        _send_req(req);
    }

    void Session::send_msg(std::vector<char>&& data) 
    {
        assert(
            data.size() > 0 &&
            data.size() <= std::numeric_limits<uint32_t>::max());

        pts::SessionReq<pt::ZeroBased> req;
        req.session_id = _id;
        req.intention =
            pts::SessionReq<pt::ZeroBased>::Intention::session_data;
        req.payload = std::move(data);

        _send_req(req);
    }

    uint32_t Session::get_id() const 
    {
        return _id;
    }

    void Session::_send_req(session_req_t& req) 
    {
        assert(_conn != nullptr);
        auto package = req.pack();
        _conn->write(package.data(), package.length());
    }
}

