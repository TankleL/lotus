#include <cassert>

#include "connstr.hxx"
#include "staloop-intl.hxx"
#include "tcpconn-cli.hxx"
#include "proto-session-lstnr.hxx"

namespace lotus::core::connection
{
    TCPClientSideConnection::TCPClientSideConnection(
        STALoop* loop)
        : _loop(loop)
        , _tcp_handle(nullptr)
        , _is_connected(false)
    {}

    void TCPClientSideConnection::connect(
        const std::string conn_str,
        connect_callback_t cb)
    {
        assert(_loop != nullptr);
        using namespace lotus::core::_internal;
        ConnString cs(conn_str);

        auto _l = expose_impl(_loop);
        auto tcp = _l
            ->_native_loop()
            ->resource<uvw::TCPHandle>();

        tcp->on<uvw::ErrorEvent>(
            [](const uvw::ErrorEvent&, uvw::TCPHandle&){
        });

        tcp->once<uvw::ConnectEvent>(
            [](const uvw::ConnectEvent&, uvw::TCPHandle& tcp){
        });

        tcp->connect(cs.host(), cs.port());

        _tcp_handle = tcp;
    }

    void TCPClientSideConnection::disconnect() noexcept
    {
    }

    void TCPClientSideConnection::write()
    {
    }
}

