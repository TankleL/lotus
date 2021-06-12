#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;


    auto loop = std::unique_ptr<STALoop>(STALoop::create());
    auto conn = connection::TCPClientSideConnection(loop.get());

    conn.connect("host=127.0.0.1; port=50500", [connection = &conn]()
    {
        std::vector<char> msg({'H', 'E', 'L', 'L', 'O'});
        Session session(connection);
        session.send_msg(std::move(msg));
    });

    loop->run();

    return 0;
}
