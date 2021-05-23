#include <msgpack.hpp>
#include "protobase.hxx"

namespace lotus::core::protocols
{
    ProtocolResponse::~ProtocolResponse()
    {}

    void
    ProtocolResponse::pack() noexcept
    {
        packed_result_t result;

        msgpack::sbuffer sbuf;
        msgpack::packer<msgpack::sbuffer> pkr(&sbuf);

        pkr.pack(result_code);
        sbuf.release();
    }

    const packed_result_t& ProtocolResponse::data() noexcept
    {}

    void ProtocolResponse::release(packed_result_t&) noexcept
    {}
} // namespace lotus::core::protocols



