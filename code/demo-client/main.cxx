#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;

    auto loop = std::shared_ptr<STALoop>(STALoop::create());
    auto conn =
        std::make_shared<connection::TCPClientSideConnection>(loop);

    conn->connect("host=127.0.0.1; port=50500", []() {});

    Session session(conn);
    session.send_msg();


    loop->run();

    return 0;
}
