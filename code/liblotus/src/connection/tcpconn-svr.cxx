#include "tcpconn-svr.hxx"
#include "connstr.hxx"
#include "ctxmgr.hxx"
#include "protolstnr.hxx"
#include "staloop-intl.hxx"

namespace lotus::core::connection
{
    // TCPConnectionListener ----------------------------------------
    TCPConnectionListener::TCPConnectionListener(STALoop* loop)
        : _loop(loop)
    {}

    void TCPConnectionListener::listen(
        const std::string& connstr) noexcept
    {
        using namespace _internal;
        ConnString cs(connstr);

        auto l = expose_impl(_loop)->_native_loop();
        auto tcp = l->resource<uvw::TCPHandle>();
        tcp->on<uvw::ListenEvent>([this]
        (const uvw::ListenEvent&, uvw::TCPHandle& svr) {
            auto conn = TCPServerSideConnection::accept(&svr);
            protocols::ProtoListener::bind(*conn);
            ContextManager::bind(*conn);
            if(this->on_create_connection != nullptr)
                this->on_create_connection(conn);
        });

        tcp->bind(cs.host(), cs.port());
        tcp->listen();
    }

    // TCPServerSideConnection --------------------------------------
    TCPServerSideConnection::TCPServerSideConnection() noexcept
        : _is_connected(false)
    {}

    TCPServerSideConnection::~TCPServerSideConnection()
    {
        close();
    }

    std::unique_ptr<TCPServerSideConnection>
        TCPServerSideConnection::accept(
            void* tcp_server_handle) noexcept
    {
        auto tcp_svr = static_cast<uvw::TCPHandle*>(
            tcp_server_handle);
        auto conn = std::unique_ptr<TCPServerSideConnection>(
            new TCPServerSideConnection());

        auto client = tcp_svr->loop().resource<uvw::TCPHandle>();
        client->on<uvw::DataEvent>([connection = conn.get()]
        (const uvw::DataEvent& de, uvw::TCPHandle& tcp) {
            bool close_conn = false;
            if(connection->on_read != nullptr)
            {
                close_conn = !connection->on_read(
                    *connection,
                    de.data.get(),
                    de.length);
            }
            else
            {
                close_conn = true;
            }

            if(close_conn)
                connection->close();
        });

        conn->_tcp_client_handle = client;

        tcp_svr->accept(*client);
        return std::move(conn);
    }

    void TCPServerSideConnection::close() noexcept
    {
        const auto& tcp =
            std::static_pointer_cast<uvw::TCPHandle>(
                _tcp_client_handle);

        if(tcp != nullptr && tcp->active())
            tcp->close();
    }
} // namespace lotus::core::connection


