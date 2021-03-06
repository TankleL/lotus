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

        Context()  : id(0) {}
        virtual ~Context() {}
    };

    class ContextManager final : public IAttachment
    {
    public:
        ContextManager() ;

    public:
        static ContextManager& bind(IConnection& conn) ;
        Context& push(std::unique_ptr<Context>&& ctx) ;
        std::unique_ptr<Context> pop(uint32_t id) ;

        template <typename TCtx>
        TCtx& push_t(std::unique_ptr<TCtx>&& ctx) 
        {
            return static_cast<TCtx&>(push(std::move(ctx)));
        }

        template <typename TCtx>
        std::unique_ptr<TCtx> pop_t(uint32_t id) 
        {
            return std::unique_ptr<TCtx>(
                static_cast<TCtx*>(pop(id).release()));
        }

    private:
        std::unordered_map<
            uint32_t,
            std::unique_ptr<Context>> _ctxmap;
        uint32_t _last_id;
        IConnection* _conn;
    };
} // namespace lotus::core

#endif // !LOTUS_CTXMGR_H
