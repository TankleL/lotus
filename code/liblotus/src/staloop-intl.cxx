
#include "staloop-intl.hxx"

namespace lotus::core::_internal
{
    STALoopImpl::STALoopImpl() noexcept
        : _loop(uvw::Loop::create())
    {}

    void STALoopImpl::run() noexcept
    {
        _loop->run();
    }

    uvw::Loop* STALoopImpl::_native_loop() noexcept
    {
        return _loop.get();
    }
}
