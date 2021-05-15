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
    class STALoop
    {
    protected:
        STALoop() noexcept;

    private:
        STALoop(std::unique_ptr<STALoop>&& impl) noexcept;

    public:
        static STALoop* create() noexcept;
        virtual ~STALoop() noexcept;

    public:
        virtual void run() noexcept;

    public:
        std::unique_ptr<STALoop>  _impl;
    };
}

#endif // LOTUS_STALOOP_H


