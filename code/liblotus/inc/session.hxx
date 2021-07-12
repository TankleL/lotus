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
        typedef std::function<
            void(
                const char* data,
                size_t length)> recv_callback_t;
        recv_callback_t on_data_recv;

    public:
        Session() = delete;
        Session(IConnection* conn);
        Session(uint32_t id, IConnection* conn);

    public:
        void send_msg(const char* data, size_t length);
        void send_msg(std::vector<char>&& data);

        uint32_t get_id() const;

    private:
        void _send_req(session_req_t& req);

    private:
        uint32_t _id;
        IConnection* _conn;
    };
} // namespace lotus::core

#endif // LOTUS_SESSION_H
