#include "session.hxx"

namespace lotus::core
{
    Session::Session(std::weak_ptr<IConnection> conn)
        : _conn(conn)
    {}

    void Session::send_msg()
    {}
}

