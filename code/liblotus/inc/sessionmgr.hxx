#ifndef LOTUS_SESSIONMGR_H
#define LOTUS_SESSIONMGR_H

#include <functional>
#include <unordered_map>

#include "attachable.hxx"
#include "conn.hxx"
#include "protolstnr.hxx"
#include "proto-session.hxx"
#include "session.hxx"

namespace lotus::core
{
    class SessionManager final : public IAttachment 
    {
    public:
        using begin_session_cb =
            std::function<void(
                int32_t result_code,
                Session* session)>;
        using end_session_cb =
            std::function<void(int32_t result_code)>;
        using new_session_cb =
            std::function<void(Session& session)>;

    public:
        new_session_cb on_new_session;

    public:
        SessionManager() ;
        ~SessionManager() ;

    public:
        static SessionManager& bind(IConnection& conn) ;
        void associate_with(
            protocols::ProtoListener& listener) ;

        void begin_session(const begin_session_cb& callback) ;
        void end_session(const end_session_cb& callback);

    private:
        bool _handle_session_req(
            protocols::ProtocolRequest<protocols::ProtocolBase>&& req,
            protocols::ListenedPackage&& pack,
            size_t offset);
        bool _handle_begin_session_req(
            protocols::proto_session::SessionReq<
                protocols::ProtocolRequest<
                protocols::ProtocolBase>>& req);
        bool _handle_session_data_req(
            protocols::proto_session::SessionReq<
                protocols::ProtocolRequest<
                protocols::ProtocolBase>>& req);

        bool _handle_session_rsp(
            protocols::ProtocolResponse<protocols::ProtocolBase>&& rsp,
            protocols::ListenedPackage&& pack,
            size_t offset);
        bool _handle_begin_session_rsp(
            protocols::proto_session::SessionRsp<
                protocols::ProtocolResponse<
                protocols::ProtocolBase>>& rsp,
            Context& ctx);

    private:
        IConnection* _conn;
        std::unordered_map<uint32_t, std::unique_ptr<Session>> _sessmap;
        uint32_t _last_sid;
    };
}

#endif // !LOTUS_SESSIONMGR_H
