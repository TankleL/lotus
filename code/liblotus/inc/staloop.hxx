/**
 * staloop - single thread async loop
 * the design of this component is intended to provide capabilities
 * of async IO, thread pool, etc.
*/

#ifndef LOTUS_STALOOP_H
#define LOTUS_STALOOP_H

#include <memory>

namespace lotus::core
{
    namespace _internal_impl
    {
        struct ISTALoopInternalProps
        {
            virtual ~ISTALoopInternalProps() noexcept {}
        };
    }

    class STALoop
    {
    public:
        STALoop() noexcept;

    public:
        void run();

    private:
        std::unique_ptr<_internal_impl::ISTALoopInternalProps>
            _props;
    };
}

#endif // LOTUS_STALOOP_H


