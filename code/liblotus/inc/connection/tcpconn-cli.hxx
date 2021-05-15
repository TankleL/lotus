#ifndef LOTUS_TCPCONN_CLI_H
#define LOTUS_TCPCONN_CLI_H

#include <memory>
#include "conn.hxx"
#include "staloop.hxx"

namespace lotus::core::connection
{
    class TCPClientSideConnection final :
        public IClientSideConnection
    {
    public:
        TCPClientSideConnection(std::weak_ptr<STALoop> loop);

    public:
        void connect(
            const std::string conn_str,
            connect_callback_t cb) override;

        void disconnect() noexcept override;

        void write() override {}
        void read() override {}
        void on_error() override {}

    private:
        std::weak_ptr<STALoop> _loop;
        std::weak_ptr<void> _tcp_handle;
        bool _is_connected;
    };
} // namespace lotus::core::connection

#endif // !LOTUS_TCPCONN_CLI_H

