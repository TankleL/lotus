#include <iostream>
#include <lotus.hxx>

constexpr int MAX_REPEATING = 10;
const std::string MSG_HELLO("Hello, lotus!");

int main(int argc, char** argv)
{
    using namespace lotus::core;

    auto loop = std::unique_ptr<STALoop>(STALoop::create());
    auto conn = connection::TCPClientSideConnection(loop.get());

    conn.connect("host=127.0.0.1; port=50500", []
    (IClientSideConnection* conn)
    {
        auto* smgr = conn->attachment<SessionManager>(conn->ATTID_SessionManager);
        for (int i = 0; i < MAX_REPEATING; ++i)
        {
            smgr->begin_session([conn, idx=i](int32_t rescode, Session* session)
            {
                //std::cout << idx << "--------------------------------------" << std::endl;
                //std::cout << "result_code = " << rescode << std::endl;
                //std::cout << "session_id = " << session->get_id() << std::endl;

                if (session)
                {
                    session->send_msg(MSG_HELLO.data(), MSG_HELLO.length());
                }

                if (idx == MAX_REPEATING - 1)
                {
                    conn->disconnect();
                }
            });
        }
    });

    loop->run();

    return 0;
}
