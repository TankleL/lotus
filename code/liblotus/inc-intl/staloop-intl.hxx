#ifndef LOTUS_STALOOP_INTL_H
#define LOTUS_STALOOP_INTL_H

#include "staloop.hxx"
#include "uvw.hpp"

namespace lotus::core::_internal
{
    class STALoopImpl : public STALoop
    {
    public:
        STALoopImpl() ;

    public:
        void run()  override;

    public:
        uvw::Loop* _native_loop() ;

    private:
        std::shared_ptr<uvw::Loop> _loop;
    }; // class STALoopImpl

    // utiliies
    inline STALoopImpl* expose_impl(STALoop* loop)
    {
        return static_cast<STALoopImpl*>(loop->_impl.get());
    }

} // lotus::core::_internal

#endif // LOTUS_STALOOP_INTL_H