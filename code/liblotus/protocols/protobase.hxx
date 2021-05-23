#ifndef LOTUS_PROTOBASE_H
#define LOTUS_PROTOBASE_H

#include <vector>

namespace lotus::core::protocols
{
    class ProtocolBase
    {
    public:
        typedef std::vector<char> packed_result_t;

    public:
        virtual ~ProtocolBase() noexcept {};
        virtual void pack() noexcept = 0;
        virtual const packed_result_t& data() noexcept = 0;
        virtual void release(packed_result_t&) noexcept = 0;
    };

    struct ProtocolRequest : public ProtocolBase
    {
    };

    struct ProtocolResponse : public ProtocolBase
    {
        int result_code;

        ProtocolResponse();
        ProtocolResponse(const ProtocolResponse&) = default;
        ProtocolResponse(ProtocolResponse&&) = default;
        ProtocolResponse& operator=(const ProtocolResponse&) = default;
        ProtocolResponse& operator=(ProtocolResponse&&) = default;
        ~ProtocolResponse();

        virtual void pack() noexcept override;
        virtual const packed_result_t& data() noexcept;
        virtual void release(packed_result_t&) noexcept;

    private:
        packed_result_t _cache;
    };
}

#endif //!LOTUS_PROTOBASE_H