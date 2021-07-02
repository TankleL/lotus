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
        using begin_session_cb = std::function<void(void)>;
        using end_session_cb = std::function<void(void)>;
        using new_session_cb = std::function<void(void)>;

    public:
        SessionManager() noexcept;
        ~SessionManager() noexcept;

    public:
        static SessionManager& bind(IConnection& conn) noexcept;
        void associate_with(
            protocols::ProtoListener& listener) noexcept;

        void begin_session(begin_session_cb callback) ;
        void end_session(end_session_cb callback);
        void on_new_session() noexcept;
        void on_kill_session() noexcept;

    private:
        bool _handle_session_req(
            protocols::ProtocolRequest<protocols::ProtocolBase>&& req,
            protocols::ListenedPackage&& pack,
            size_t offset);
        bool _handle_begin_session_req(
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
                protocols::ProtocolBase>>& rsp);

    private:
        IConnection* _conn;
        std::unordered_map<uint32_t, std::unique_ptr<Session>> _sessmap;
        uint32_t _last_sid;
    };
}

#endif // !LOTUS_SESSIONMGR_H
