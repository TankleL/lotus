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
        TCPServerSideConnection() ;

    public:
        ~TCPServerSideConnection();

        void read() override;
        size_t write(const char* data, size_t length) override;
        void close() ;

    public:
        static std::unique_ptr<TCPServerSideConnection>
            accept(void* tcp_server_handle) ;

    private:
        std::shared_ptr<void> _tcp_client_handle;
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
        void listen(const std::string& connstr) ;

    private:
        STALoop* _loop;
    };
    
} // namespace lotus::core::connection

#endif // !LOTUS_TCPCONN_SVR_H

