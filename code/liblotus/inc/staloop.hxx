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
        STALoop() ;

    private:
        STALoop(std::unique_ptr<STALoop>&& impl) ;

    public:
        static STALoop* create() ;
        virtual ~STALoop() ;

    public:
        virtual void run() ;

    public:
        std::unique_ptr<STALoop>  _impl;
    };
}

#endif // LOTUS_STALOOP_H


