#ifndef LOTUS_CONN_H
#define LOTUS_CONN_H

#include <functional>
#include <string>
#include "attachable.hxx"

namespace lotus::core
{
    class IConnection
    {
    public:
        virtual ~IConnection() {}

    public:
        virtual void write() = 0;
        virtual void read() = 0;
        virtual void on_error() = 0;
    };

    class IClientSideConnection :
        public IConnection,
        public Attachable
    {
    public:
        virtual ~IClientSideConnection() {}

    public:
        typedef std::function<void()> connect_callback_t;

    public:
        virtual void connect(
            const std::string conn_str,
            connect_callback_t cb) = 0;
        virtual void disconnect() noexcept = 0;
    };

    class IServerSideConnection :
        public IConnection,
        public Attachable
    {
    public:
        virtual ~IServerSideConnection() {}
    };
    
} // namespace lotus::core


#endif // LOTUS_CONN_H