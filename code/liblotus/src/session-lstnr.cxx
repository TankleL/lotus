#include <cassert>
#include "session-lstnr.hxx"

namespace lotus::core
{
    SessionListener::SessionListener() noexcept
    {}

    SessionListener::~SessionListener() noexcept
    {}

    bool SessionListener::eat(
        const char* data,
        size_t length) noexcept
    {
        return true;
    }

    SessionListener& SessionListener::bind(
        IServerSideConnection& conn) noexcept
    {
        auto instance = std::make_unique<SessionListener>();
        conn.attach(conn.ATTID_SessionListener, std::move(instance));
        conn.on_data_received = []
        (IServerSideConnection& conn,
            const char* data,
            size_t len) -> bool
        {
            auto listener =
                conn.att<SessionListener>(conn.ATTID_SessionListener);
            assert(listener.has_value());
            return listener.value()->eat(data, len);
        };

        return listener;
    }
}

