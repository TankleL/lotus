#include <iostream>
#include <lotus.hxx>

int main(int argc, char** argv)
{
    using namespace lotus::core;


    auto loop = std::unique_ptr<STALoop>(STALoop::create());
    auto conn = connection::TCPClientSideConnection(loop.get());

    conn.connect("host=127.0.0.1; port=50500", []
    (IClientSideConnection* conn)
    {
        auto& pl = protocols::ProtoListener::bind(*conn);
        auto& smgr = SessionManager::bind(*conn);
        for (int i = 0; i < 10000; ++i)
        {
            smgr.begin_session([](auto rescode, auto session)
            {
                std::cout << "--------------------------------------" << std::endl;
                std::cout << "result_code = " << rescode << std::endl;
                std::cout << "session_id = " << session->get_id() << std::endl;
            });
        }
    });

    loop->run();

    return 0;
}
