#ifndef LOTUS_TCPCONN_SVR_H
#define LOTUS_TCPCONN_SVR_H

#include <functional>
#include <memory>
#include "conn.hxx"
#include "staloop.hxx"

namespace lotus::core::connection
{
    class TCPServerSideConnection;
    class TCPConnectionListener
    {
    public:
        typedef std::function<
            void(std::shared_ptr<TCPServerSideConnection>& conn)>
            create_conn_callback_t;
        create_conn_callback_t on_create_connection;

    public:
        TCPConnectionListener(std::weak_ptr<STALoop> loop);

    public:
        void listen(const std::string& connstr) noexcept;

    private:
        std::weak_ptr<STALoop> _loop;
    };

    class TCPServerSideConnection final :
        public IServerSideConnection
    {
    private:
        TCPServerSideConnection() noexcept;

    public:
        typedef std::function<void(const char* data, size_t len)>
            data_received_callback_t;
        data_received_callback_t on_data_received;

    public:
        void write() override {}
        void read() override;
        void on_error() override {}

    public:
        static std::shared_ptr<TCPServerSideConnection>
            accept(void* tcp_server_handle) noexcept;

    private:
        std::shared_ptr<void> _tcp_client_handle;
        bool _is_connected;
    };
} // namespace lotus::core::connection

#endif // !LOTUS_TCPCONN_SVR_H

