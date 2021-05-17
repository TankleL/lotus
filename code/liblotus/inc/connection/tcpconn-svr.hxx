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
    
    // TCPServerSideConnection  -------------------------------------

    class TCPServerSideConnection final :
        public IServerSideConnection
    {
    private:
        TCPServerSideConnection() noexcept;

    public:
        static constexpr int AttIdx_SessionListener = 1;

    public:
        typedef std::function<
            bool(
                TCPServerSideConnection& conn,
                const char* data,
                size_t len)>
            data_received_callback_t;
        data_received_callback_t on_data_received;

    public:
        ~TCPServerSideConnection();

        void write() override {}
        void read() override;
        void on_error() override {}

        void close() noexcept;

    public:
        static std::shared_ptr<TCPServerSideConnection>
            accept(void* tcp_server_handle) noexcept;

    private:
        std::shared_ptr<void> _tcp_client_handle;
        bool _is_connected;
    };
} // namespace lotus::core::connection

#endif // !LOTUS_TCPCONN_SVR_H

