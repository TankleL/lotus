#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;

    auto loop = std::shared_ptr<STALoop>(STALoop::create());

    connection::TCPConnectionListener listener(loop);
    std::vector<std::shared_ptr<connection::TCPServerSideConnection>>
        conn_collection;

    listener.on_create_connection = [&conn_collection]
    (std::shared_ptr<connection::TCPServerSideConnection>& conn){
        conn_collection.emplace_back(conn);
        conn->read();
    };
    listener.listen("host=127.0.0.1; port=50500");

    loop->run();

    return 0;
}

