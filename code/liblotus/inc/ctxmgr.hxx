#ifndef LOTUS_CTXMGR_H
#define LOTUS_CTXMGR_H

#include <queue>
#include <unordered_map>

#include "attachable.hxx"
#include "conn.hxx"

namespace lotus::core
{
    struct Context
    {
        uint32_t id;
    };

    class ContextManager final : public IAttachment
    {
    public:
        static ContextManager& bind(IConnection& conn) noexcept;
        void push(Context&& ctx) noexcept;

    private:
        std::unordered_map<uint32_t, Context> _ctxmap;
        IConnection* _conn;
    };
} // namespace lotus::core

#endif // !LOTUS_CTXMGR_H
