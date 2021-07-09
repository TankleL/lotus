
#include "staloop-intl.hxx"

namespace lotus::core::_internal
{
    STALoopImpl::STALoopImpl() 
        : _loop(uvw::Loop::create())
    {}

    void STALoopImpl::run() 
    {
        _loop->run();
    }

    uvw::Loop* STALoopImpl::_native_loop() 
    {
        return _loop.get();
    }
}
