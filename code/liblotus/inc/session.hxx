#ifndef LOTUS_SESSION_H
#define LOTUS_SESSION_H

#include <memory>
#include <functional>

#include "conn.hxx"
#include "proto-session.hxx"

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
            protocols::proto_session::SessionReq<
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

        uint32_t get_id() const noexcept;

    private:
        void _send_req(session_req_t& req)
            noexcept;

    private:
        uint32_t _id;
        IConnection* _conn;
    };
} // namespace lotus::core

#endif // LOTUS_SESSION_H
