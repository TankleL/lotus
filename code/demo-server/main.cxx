#include <cassert>
#include <unordered_map>
#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;

    auto loop = std::unique_ptr<STALoop>(STALoop::create());

    connection::TCPConnectionListener conn_listener(loop.get());
    std::unordered_map<size_t, std::unique_ptr<connection::TCPServerSideConnection>> conn_map;

    conn_listener.on_create_connection =
        [&conn_map]
    (std::unique_ptr<connection::TCPServerSideConnection>& conn){
        conn->on_error = 
            [](IConnection& conn, int code, const char* name)
        {
            auto& connection = static_cast<connection::TCPServerSideConnection&>(conn);
            connection.close();
        };

        conn->on_closed = 
            [&conn_map](IConnection& conn)
        {
            auto addr = (size_t)&conn;
            const auto& it = conn_map.find(addr);
            if (it != conn_map.cend())
            {
                conn_map.erase(it);
            }
        };

        conn_map[(size_t)conn.get()] = std::move(conn);
    };
    conn_listener.listen("host=127.0.0.1; port=50500");

    loop->run();

    return 0;
}

