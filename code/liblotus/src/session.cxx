#include "session.hxx"

namespace lotus::core
{
    Session::Session(const UUID& id,
        std::weak_ptr<IConnection> conn)
        : _id(id)
        , _conn(conn)
    {}

    void Session::send_msg()
    {}
}

