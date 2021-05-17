#include <cassert>
#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;

    auto loop = std::shared_ptr<STALoop>(STALoop::create());

    connection::TCPConnectionListener conn_listener(loop);
    std::vector<std::shared_ptr<connection::TCPServerSideConnection>>
        conn_collection;

    conn_listener.on_create_connection =
        [&conn_collection]
    (std::shared_ptr<connection::TCPServerSideConnection>& conn){
        conn_collection.emplace_back(conn);

        conn->attach(
            conn->AttIdx_SessionListener,
            std::move(
                std::make_unique<std::any>(
                    std::make_any<SessionListener>())));
        
        conn->on_data_received = []
        (connection::TCPServerSideConnection& conn,
            const char* data,
            size_t len) {
            auto lstnr =
                conn.att<SessionListener>(conn.AttIdx_SessionListener);
            assert(lstnr.has_value());
            return lstnr.value()->eat(data, len);
        };

        conn->read();
    };
    conn_listener.listen("host=127.0.0.1; port=50500");

    loop->run();

    return 0;
}

