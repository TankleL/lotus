#include "session-lstnr.hxx"

namespace lotus::core
{
    SessionListener::SessionListener() noexcept
    {}

    bool SessionListener::eat(
        const char* data,
        size_t length) noexcept
    {
        return true;
    }
}

