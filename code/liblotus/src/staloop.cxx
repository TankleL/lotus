#include "staloop-intl.hxx"
#include "staloop.hxx"

namespace lotus::core
{
    STALoop::STALoop() noexcept
    {}

    STALoop::STALoop(std::unique_ptr<STALoop>&& impl) noexcept
        : _impl(std::move(impl))
    {}

    STALoop::~STALoop() noexcept
    {}

    STALoop* STALoop::create() noexcept
    {
        auto impl = std::make_unique<_internal::STALoopImpl>();
        return new STALoop(std::move(impl));
    }

    void STALoop::run() noexcept
    {
        _impl->run();
    }
    
} // namespace lotus::core



