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
        static inline constexpr int ATTID_ProtoListener = 0;
        static inline constexpr int ATTID_SessionManager = 1;
        static inline constexpr int ATTID_ContextManager = 2;

        // read callback
        typedef std::function<
            bool(
                IConnection& conn,
                const char* data,
                size_t len)>
            read_callback_t;
        read_callback_t on_read;

        // error callback
        typedef std::function<
            void(
                IConnection& conn,
                int error_code,
                const char* error_name)> error_callback_t;
        error_callback_t on_error;

        // closed callback
        typedef std::function<
            void(
                IConnection& conn)> closed_callback_t;
        closed_callback_t on_closed;

    public:
        virtual ~IConnection() {}

    public:
        virtual void read() = 0;
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
        virtual void disconnect()  = 0;
    };

    class IServerSideConnection : public IConnection
    {
    public:
    public:
        virtual ~IServerSideConnection() {}
    };
    
} // namespace lotus::core


#endif // LOTUS_CONN_H