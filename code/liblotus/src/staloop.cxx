#include "staloop.hxx"
#include <uvw.hpp>

namespace lotus::core
{
    namespace _internal_impl
    {
        struct STALoopIntlProps : public ISTALoopInternalProps
        {
            STALoopIntlProps() noexcept
                : loop(uvw::Loop::create())
            {}

            ~STALoopIntlProps() noexcept override
            {}

            std::shared_ptr<uvw::Loop> loop;
        };
    }

#define PROPS  \
(static_cast<_internal_impl::STALoopIntlProps*>(this->_props.get()))

    STALoop::STALoop() noexcept
        : _props(new _internal_impl::STALoopIntlProps())
    {}

    void STALoop::run()
    {
        PROPS->loop->run();
    }
    
} // namespace lotus::core



