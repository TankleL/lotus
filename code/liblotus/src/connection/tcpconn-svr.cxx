#include "tcpconn-svr.hxx"
#include "connstr.hxx"
#include "staloop-intl.hxx"

namespace lotus::core::connection
{
    // TCPConnectionListener ----------------------------------------

    TCPConnectionListener::TCPConnectionListener(
        std::weak_ptr<STALoop> loop)
        : _loop(loop)
    {}

    void TCPConnectionListener::listen(
        const std::string& connstr) noexcept
    {
        using namespace _internal;

        auto loop = _loop.lock();
        if(loop != nullptr)
        {
            ConnString cs(connstr);

            auto l = expose_impl(loop.get())->_native_loop();
            auto tcp = l->resource<uvw::TCPHandle>();
            tcp->on<uvw::ListenEvent>([this]
            (const uvw::ListenEvent&, uvw::TCPHandle& svr) {
                auto conn = TCPServerSideConnection::accept(&svr);
                if(this->on_create_connection != nullptr)
                    this->on_create_connection(conn);
            });

            tcp->bind(cs.host(), cs.port());
            tcp->listen();
        }
    }

    // TCPServerSideConnection --------------------------------------
    TCPServerSideConnection::TCPServerSideConnection() noexcept
        : _is_connected(false)
    {}

    TCPServerSideConnection::~TCPServerSideConnection()
    {
        close();
    }

    std::shared_ptr<TCPServerSideConnection>
        TCPServerSideConnection::accept(
            void* tcp_server_handle) noexcept
    {
        auto tcp_svr = static_cast<uvw::TCPHandle*>(
            tcp_server_handle);
        auto conn = std::shared_ptr<TCPServerSideConnection>(
            new TCPServerSideConnection);

        auto client = tcp_svr->loop().resource<uvw::TCPHandle>();
        client->on<uvw::DataEvent>([connection = std::weak_ptr(conn)]
        (const uvw::DataEvent& de, uvw::TCPHandle& tcp) {
            bool close_conn = false;
            auto conn = connection.lock();
            if(conn != nullptr && conn->on_data_received != nullptr)
            {
                close_conn = conn->on_data_received(
                    *conn,
                    de.data.get(),
                    de.length);
            }
            else
            {
                close_conn = true;
            }

            if(close_conn)
                conn->close();
        });

        conn->_tcp_client_handle = client;

        tcp_svr->accept(*client);
        return conn;
    }

    void TCPServerSideConnection::read()
    {
        const auto& tcp =
            std::static_pointer_cast<uvw::TCPHandle>(
                _tcp_client_handle);
        tcp->read();
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


