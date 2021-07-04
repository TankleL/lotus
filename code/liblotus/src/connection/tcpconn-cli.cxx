#include <cassert>

#include "connstr.hxx"
#include "ctxmgr.hxx"
#include "protolstnr.hxx"
#include "sessionmgr.hxx"
#include "staloop-intl.hxx"
#include "tcpconn-cli.hxx"
#include "proto-session.hxx"

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

        // erro event 
        tcp->on<uvw::ErrorEvent>(
            [](const uvw::ErrorEvent&, uvw::TCPHandle&){
        });

        // end event
        tcp->on<uvw::EndEvent>(
            [this](const auto& ee, auto& tcp)
        {
            tcp.read();
        });

        // connect event
        tcp->once<uvw::ConnectEvent>(
            [this, cb]
        (const uvw::ConnectEvent& ce, uvw::TCPHandle& tcp)
        {
            _is_connected = true;
            ContextManager::bind(*this);
            auto& pl = protocols::ProtoListener::bind(*this);
            auto& smgr = SessionManager::bind(*this);
            smgr.associate_with(pl);
            cb(this);
            tcp.read();
        });

        tcp->connect(cs.host(), cs.port());

        _tcp_handle = tcp;
    }

    void TCPClientSideConnection::disconnect() noexcept
    {
        if(_is_connected)
        {
            _is_connected = false;
            // TODO:
        }
    }

    size_t TCPClientSideConnection::write(
        const char* data,
        size_t length)
    {
        if (!_is_connected)
        {
            return 0;
        }

        auto tcp =
            std::static_pointer_cast<uvw::TCPHandle>(_tcp_handle);
        if(tcp->writable())
        {
            tcp->write(
                const_cast<char*>(data),
                static_cast<unsigned int>(length));
            return length;
        }
        else
        {
            return 0;
        }
    }
}

