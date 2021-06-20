#ifndef LOTUS_TCPCONN_SVR_H
#define LOTUS_TCPCONN_SVR_H

#include <functional>
#include <memory>
#include "conn.hxx"
#include "staloop.hxx"

namespace lotus::core::connection
{
    // TCPServerSideConnection  -------------------------------------
    class TCPServerSideConnection final :
        public IServerSideConnection
    {
    private:
        TCPServerSideConnection() noexcept;

    public:
        ~TCPServerSideConnection();

        size_t write(const char* data, size_t length) override { return 0; }
        void close() noexcept;

    public:
        static std::unique_ptr<TCPServerSideConnection>
            accept(void* tcp_server_handle) noexcept;

    private:
        std::shared_ptr<void> _tcp_client_handle;
        bool _is_connected;
    };

    // TCPConnectionListener ----------------------------------------
    class TCPConnectionListener
    {
    public:
        typedef std::function<
            void(std::unique_ptr<TCPServerSideConnection>& conn)>
            create_conn_callback_t;
        create_conn_callback_t on_create_connection;

    public:
        TCPConnectionListener(STALoop* loop);

    public:
        void listen(const std::string& connstr) noexcept;

    private:
        STALoop* _loop;
    };
    
} // namespace lotus::core::connection

#endif // !LOTUS_TCPCONN_SVR_H

