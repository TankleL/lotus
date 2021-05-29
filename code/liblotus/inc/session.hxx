#ifndef LOTUS_SESSION_H
#define LOTUS_SESSION_H

#include <memory>
#include <functional>
#include "conn.hxx"

namespace lotus::core
{
    /*
     * Session is aiming to reuse the connection for business
     * purposes. Session maintains a context for shor-term usages.
     */
    class Session
    {
    public:
        typedef std::function<void()> recv_callback_t;

    public:
        Session() = delete;
        Session(std::weak_ptr<IConnection> conn);

    public:
        void on_msg_sent();
        void on_msg_received();
        void on_error();

        void send_msg();

    private:
        std::weak_ptr<IConnection> _conn;
    };
} // namespace lotus::core

#endif // LOTUS_SESSION_H
