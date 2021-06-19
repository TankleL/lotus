#ifndef LOTUS_CONN_H
#define LOTUS_CONN_H

#include <functional>
#include <string>
#include "attachable.hxx"

namespace lotus::core
{
    class IConnection : public Attachable
    {
    public:
        // reserved attachment id list
        static constexpr int ATTID_SessionListener = 0;

        // read callback
        typedef std::function<
            bool(
                IConnection& conn,
                const char* data,
                size_t len)>
            read_callback_t;
        read_callback_t on_read;

        // error callback
        typedef std::function<void(void)> error_callback_t;
        error_callback_t on_error;

    public:
        virtual ~IConnection() {}

    public:
        virtual size_t write(const char* data, size_t length) = 0;
    };

    class IClientSideConnection : public IConnection
    {
    public:
        virtual ~IClientSideConnection() {}

    public:
        typedef std::function<void(IClientSideConnection* conn)>
            connect_callback_t;

    public:
        virtual void connect(
            const std::string conn_str,
            connect_callback_t cb) = 0;
        virtual void disconnect() noexcept = 0;
    };

    class IServerSideConnection : public IConnection
    {
    public:
    public:
        virtual ~IServerSideConnection() {}
    };
    
} // namespace lotus::core


#endif // LOTUS_CONN_H