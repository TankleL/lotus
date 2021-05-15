#include "tcpconn-cli.hxx"

namespace lotus::core::connection
{
    TCPClientSideConnection::TCPClientSideConnection(
        std::weak_ptr<STALoop> loop)
        : _loop(loop)
    {}

    void TCPClientSideConnection::connect(
        const std::string conn_str,
        connect_callback_t cb)
    {
    }

    void  TCPClientSideConnection::disconnect() noexcept
    {
    }
}

