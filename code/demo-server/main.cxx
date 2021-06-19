#include <cassert>
#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;

    auto loop = std::unique_ptr<STALoop>(STALoop::create());

    connection::TCPConnectionListener conn_listener(loop.get());
    std::vector<std::unique_ptr<connection::TCPServerSideConnection>>
        conn_collection;

    conn_listener.on_create_connection =
        [&conn_collection]
    (std::unique_ptr<connection::TCPServerSideConnection>& conn){
        auto conn_ptr = conn.get();
        conn_collection.emplace_back(std::move(conn));

        SessionListener::bind(conn_ptr);
        //conn_ptr->read();
    };
    conn_listener.listen("host=127.0.0.1; port=50500");

    loop->run();

    return 0;
}

