#include "tcpconn-svr.hxx"
#include "connstr.hxx"
#include "ctxmgr.hxx"
#include "protolstnr.hxx"
#include "sessionmgr.hxx"
#include "staloop-intl.hxx"

namespace lotus::core::connection
{
    // TCPConnectionListener ----------------------------------------
    TCPConnectionListener::TCPConnectionListener(STALoop* loop)
        : _loop(loop)
    {}

    void TCPConnectionListener::listen(
        const std::string& connstr) 
    {
        using namespace _internal;
        ConnString cs(connstr);

        auto l = expose_impl(_loop)->_native_loop();
        auto tcp = l->resource<uvw::TCPHandle>();
        tcp->on<uvw::ListenEvent>([this]
        (const uvw::ListenEvent&, uvw::TCPHandle& svr) {
            auto conn = TCPServerSideConnection::accept(&svr);
            ContextManager::bind(*conn);
            auto& pl = protocols::ProtoListener::bind(*conn);
            auto& smgr = SessionManager::bind(*conn);
            smgr.associate_with(pl);
            if(this->on_create_connection != nullptr)
                this->on_create_connection(conn);
        });

        tcp->bind(cs.host(), cs.port());
        tcp->listen();
    }

    // TCPServerSideConnection --------------------------------------
    TCPServerSideConnection::TCPServerSideConnection() 
    {}

    TCPServerSideConnection::~TCPServerSideConnection()
    {
        close();
    }

    void TCPServerSideConnection::read() 
    {
        auto client =
            std::static_pointer_cast<uvw::TCPHandle>(
                _tcp_client_handle);
        client->read();
    }

    std::unique_ptr<TCPServerSideConnection>
        TCPServerSideConnection::accept(
            void* tcp_server_handle) 
    {
        auto tcp_svr = static_cast<uvw::TCPHandle*>(
            tcp_server_handle);
        auto conn = std::unique_ptr<TCPServerSideConnection>(
            new TCPServerSideConnection());

        auto client = tcp_svr->loop().resource<uvw::TCPHandle>();

        // date event
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

        // end event
        client->on<uvw::EndEvent>(
            [](const auto& ee, uvw::TCPHandle& client)
        {
            if(client.readable())
                client.read();
        });

        // error event
        client->on<uvw::ErrorEvent>(
            [connection = conn.get()](const uvw::ErrorEvent& ee, uvw::TCPHandle& tcp)
        {
            if(connection->on_error != nullptr)
            {
                connection->on_error(*connection, ee.code(), ee.name());
            }
        });

        // close event
        client->on<uvw::CloseEvent>(
            [connection = conn.get()]
        (const uvw::CloseEvent& ce, uvw::TCPHandle& tcp)
        {
            if (connection->on_closed != nullptr)
            {
                connection->on_closed(*connection);
            }
        });

        client->on<uvw::ShutdownEvent>([](const auto& se, auto& tcp)
        {
            static int count = 0;
            ++count;
        });

        conn->_tcp_client_handle = client;

        tcp_svr->accept(*client);
        client->read();
        return std::move(conn);
    }

    size_t TCPServerSideConnection::write(
        const char* data,
        size_t length)
    {
        auto tcp =
            std::static_pointer_cast<uvw::TCPHandle>(
                _tcp_client_handle);
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

    void TCPServerSideConnection::close() 
    {
        const auto& tcp =
            std::static_pointer_cast<uvw::TCPHandle>(
                _tcp_client_handle);

        if(tcp != nullptr)
            tcp->close();
    }
} // namespace lotus::core::connection


