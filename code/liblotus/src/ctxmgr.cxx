#include "ctxmgr.hxx"

namespace lotus::core
{
    ContextManager::ContextManager() noexcept
        : _last_id(0)
        , _conn(nullptr)
    {}

    ContextManager& ContextManager::bind(IConnection& conn) noexcept
    {
        auto instance = std::make_unique<ContextManager>();
        auto& retval = *instance;
        retval._conn = &conn;
        conn.attach(conn.ATTID_ContextManager, std::move(instance));
        return retval;
    }

    Context& ContextManager::push(
        std::unique_ptr<Context>&& ctx) noexcept
    {
        auto& retval = *ctx;
        ctx->id = ++_last_id;
        _ctxmap.insert(std::pair(ctx->id, std::move(ctx)));
        return retval;
    }

    std::unique_ptr<Context> ContextManager::pop(
        uint32_t id) noexcept
    {
        const auto& it = _ctxmap.find(id);
        if (it != _ctxmap.end())
        {
            auto retval = std::move(it->second);
            _ctxmap.erase(it);
            return retval;
        }
        else
        {
            return nullptr;
        }
    }
} // namespace lotus::core

