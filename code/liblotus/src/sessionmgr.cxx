#include "ctxmgr.hxx"
#include "proto-session.hxx"
#include "sessionmgr.hxx"

namespace lotus::core
{
    namespace pt = lotus::core::protocols;
    namespace pts = lotus::core::protocols::proto_session;

    struct SessionReqCtx : public Context
    {
        pts::SessionReq<pt::ZeroBased> req_info;
    };

    SessionManager::SessionManager() noexcept
        : _conn(nullptr)
        , _last_sid(0)
    {}

    SessionManager::~SessionManager() noexcept
    {}

    SessionManager& SessionManager::bind(IConnection& conn) noexcept
    {
        auto instance = std::make_unique<SessionManager>();
        auto& retval = *instance;
        retval._conn = &conn;
        conn.attach(conn.ATTID_SessionManager, std::move(instance));
        return retval;
    }

    void SessionManager::associate_with(
        protocols::ProtoListener& listener) noexcept
    {
        listener.add_request_callback(
            pts::SessionReq<pt::ZeroBased>::REQ_ID,
            [this](auto&& req, auto&& pack, auto offset)->bool
        {
            return _handle_session_req(
                std::move(req),
                std::move(pack),
                std::move(offset));
        });

        listener.add_response_callback(
            pts::SessionRsp<pt::ZeroBased>::RSP_ID,
            [this](auto&& rsp, auto&& pack, auto offset)->bool
        {
            return _handle_session_rsp(
                std::move(rsp),
                std::move(pack),
                std::move(offset));
        });
    }

    void SessionManager::begin_session(begin_session_cb cb)
    {
        auto ctxmgr =
            _conn->attachment<ContextManager>(
                _conn->ATTID_ContextManager);
        auto& ctx =
            ctxmgr->push_t(std::make_unique<SessionReqCtx>());

        ctx.req_info.intention =
            pts::SessionReq<pt::ZeroBased>::Intention::begin_session;
        ctx.req_info.ctx_id = ctx.id;

        auto package = ctx.req_info.pack();
        _conn->write(package.data(), package.length());
    }

    bool SessionManager::_handle_session_req(
        protocols::ProtocolRequest<protocols::ProtocolBase>&& req,
        protocols::ListenedPackage&& pack,
        size_t offset)
    {
        pts::SessionReq<pt::ProtocolRequest<pt::ProtocolBase>>
            sess_req(std::move(req));
        sess_req.unpack(
            pack.data() + offset,
            pack.size() - offset);
        
        switch(sess_req.intention)
        {
        case pts::SessionReq<pt::ZeroBased>::Intention::begin_session:
            return _handle_begin_session_req(sess_req);
            break;

        case pts::SessionReq<pt::ZeroBased>::Intention::end_session:
            break;

        case pts::SessionReq<pt::ZeroBased>::Intention::session_data:
            break;
            
        default:
            break;
        }
        return true;
    }

    bool SessionManager::_handle_begin_session_req(
        protocols::proto_session::SessionReq<
            protocols::ProtocolRequest<
            protocols::ProtocolBase>>& req)
    {
        if (_last_sid <= std::numeric_limits<uint32_t>::max())
        {
            auto sid = ++_last_sid;
            auto sess = std::make_unique<Session>(sid, _conn);
            _sessmap.emplace(sid, std::move(sess));

            pts::SessionRsp<pt::ZeroBased> rsp;
            rsp.ctx_id = req.ctx_id;
            rsp.response_id = 200;
            rsp.session_id = sid;

            auto package = rsp.pack();
            if(_conn->write(package.data(), package.length()
                == package.length()))
                return true;
            return false;
        }
        else
        {
            // TODO: add trace logs
            return false;
        }
    }

    bool SessionManager::_handle_session_rsp(
        protocols::ProtocolResponse<protocols::ProtocolBase>&& rsp,
        protocols::ListenedPackage&& pack,
        size_t offset)
    {
        pts::SessionRsp<pt::ProtocolResponse<pt::ProtocolBase>>
            sess_rsp(std::move(rsp));
        sess_rsp.unpack(pack.data() + offset, pack.size() - offset);

        auto ctxmgr =
            _conn->attachment<ContextManager>(
                _conn->ATTID_ContextManager);
        auto ctx = ctxmgr->pop_t<SessionReqCtx>(sess_rsp.ctx_id);
        if (!ctx)
            return false;

        switch (ctx->req_info.intention)
        {
        case pts::SessionReq<pt::ZeroBased>::Intention::begin_session:
            return _handle_begin_session_rsp(sess_rsp);
            break;

        case pts::SessionReq<pt::ZeroBased>::Intention::end_session:
            break;

        case pts::SessionReq<pt::ZeroBased>::Intention::session_data:
            break;

        default:
            return false;
        }

        return true;
    }
} // namespace lotus::core



