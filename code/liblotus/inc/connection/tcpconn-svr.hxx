#ifndef LOTUS_TCPCONN_SVR_H
#define LOTUS_TCPCONN_SVR_H

#include <functional>
#include <memory>
#include "conn.hxx"
#include "staloop.hxx"

namespace lotus::core::connection
{
    // forward declaration ------------------------------------------
    class TCPServerSideConnection;

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
    
    // TCPServerSideConnection  -------------------------------------

    class TCPServerSideConnection final :
        public IServerSideConnection
    {
    private:
        TCPServerSideConnection() noexcept;

    public:
        ~TCPServerSideConnection();

        void write() override {}
        void read() override;
        void on_error() override {}

        void close() noexcept;

    public:
        static std::unique_ptr<TCPServerSideConnection>
            accept(void* tcp_server_handle) noexcept;

    private:
        std::shared_ptr<void> _tcp_client_handle;
        bool _is_connected;
    };
} // namespace lotus::core::connection

#endif // !LOTUS_TCPCONN_SVR_H

