#ifndef LOTUS_SESSION_H
#define LOTUS_SESSION_H

#include <memory>
#include <functional>

#include "conn.hxx"
#include "proto-session-lstnr.hxx"

namespace lotus::core
{
    /*
     * Session is aiming to reuse the connection for business
     * purposes. Session maintains a context for shor-term usages.
     */
    class Session
    {
    public:
        using session_req_t =
            protocols::proto_session_lstnr::SessionReq<
            protocols::ZeroBased>;
        typedef std::function<void()> recv_callback_t;

    public:
        Session() = delete;
        Session(IConnection* conn) noexcept;
        Session(uint32_t id, IConnection* conn) noexcept;

    public:
        void on_msg_sent();
        void on_msg_received();
        void on_error();

        void send_msg(const char* data, size_t length) noexcept;
        void send_msg(std::vector<char>&& data) noexcept;

    private:
        void _ensure_nsrs(session_req_t& req) noexcept;
        void _send_req(session_req_t& req)
            noexcept;

    private:
        uint32_t _id;
        IConnection* _conn;

    private:
        enum class new_session_req_state_e
        {
            not_sent = 0,
            wait_for_rsp,
            remote_acked 
        } _nsrs_state;
    };
} // namespace lotus::core

#endif // LOTUS_SESSION_H
