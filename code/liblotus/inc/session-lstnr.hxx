#ifndef LOTUS_SESSION_LISTENER_H
#define LOTUS_SESSION_LISTENER_H

#include <functional>
#include <memory>
#include <session.hxx>
#include "conn.hxx"

namespace lotus::core
{
    class SessionListener
    {
    public:
        typedef std::function<void(const char*, size_t)>
            data_received_handler_t;
        typedef std::function<void(std::unique_ptr<Session>&&)>
            new_session_callback_t;
        new_session_callback_t on_new_session;

    public:
        SessionListener() noexcept;

    public:
        static SessionListener& bind(
            IServerSideConnection& conn) noexcept;

    public:
        bool eat(const char* data, size_t length) noexcept;
    };
}

#endif // !LOTUS_SESSION_LISTENER_H
