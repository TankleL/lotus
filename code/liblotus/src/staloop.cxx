#include "staloop-intl.hxx"
#include "staloop.hxx"

namespace lotus::core
{
    STALoop::STALoop() 
    {}

    STALoop::STALoop(std::unique_ptr<STALoop>&& impl) 
        : _impl(std::move(impl))
    {}

    STALoop::~STALoop() 
    {}

    STALoop* STALoop::create() 
    {
        auto impl = std::make_unique<_internal::STALoopImpl>();
        return new STALoop(std::move(impl));
    }

    void STALoop::run() 
    {
        _impl->run();
    }
    
} // namespace lotus::core



