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
        auto* smgr = conn->attachment<SessionManager>(conn->ATTID_SessionManager);
        for (int i = 0; i < 10000; ++i)
        {
            smgr->begin_session([idx=i](auto rescode, auto session)
            {
                //std::cout << idx << "--------------------------------------" << std::endl;
                //std::cout << "result_code = " << rescode << std::endl;
                //std::cout << "session_id = " << session->get_id() << std::endl;
            });
        }
    });

    loop->run();

    return 0;
}
