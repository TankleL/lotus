#ifndef LOTUS_PROTOBASE_H
#define LOTUS_PROTOBASE_H

#include <vector>

namespace lotus::core::protocols
{
    class ProtocolPackage final
    {
    public:
        static constexpr size_t fixed_header_len = 2;

    public:
        ProtocolPackage() noexcept;
        ~ProtocolPackage() noexcept;
        ProtocolPackage(const ProtocolPackage&) = default;
        ProtocolPackage(ProtocolPackage&&) = default;
        ProtocolPackage& operator=(const ProtocolPackage&) = default;
        ProtocolPackage& operator=(ProtocolPackage&&) = default;

    public:
        size_t length() noexcept;
        size_t payload_length() noexcept;
        const char* data() noexcept;
        void append(const char* data, size_t length) noexcept;

    private:
        std::vector<char> _data;
    };

    class ProtocolBase
    {
    public:
        ProtocolPackage pack() noexcept;
        bool unpack(const char* data, size_t length) noexcept;

    public:
        virtual ~ProtocolBase() noexcept {};
        virtual void on_packing(ProtocolPackage& data) noexcept = 0;
        virtual bool on_unpacking(void* native_pac) noexcept = 0;
    };

    struct ProtocolRequest : public ProtocolBase
    {
        int32_t request_id;
        uint32_t trx_id;

        ProtocolRequest() noexcept;
        virtual ~ProtocolRequest() noexcept;

        ProtocolRequest(const ProtocolRequest&) = default;
        ProtocolRequest(ProtocolRequest&&) = default;
        ProtocolRequest& operator=(const ProtocolRequest&) = default;
        ProtocolRequest& operator=(ProtocolRequest&&) = default;

        virtual void on_packing(
            ProtocolPackage& data) noexcept override;
        virtual bool on_unpacking(void* native_pac) noexcept override;
    };

    struct ProtocolResponse : public ProtocolBase
    {
        int32_t result_code;
        uint32_t trx_id;

        ProtocolResponse() noexcept;
        virtual ~ProtocolResponse() noexcept;
        ProtocolResponse(const ProtocolResponse&) = default;
        ProtocolResponse(ProtocolResponse&&) = default;
        ProtocolResponse& operator=(const ProtocolResponse&) = default;
        ProtocolResponse& operator=(ProtocolResponse&&) = default;

        virtual void on_packing(
            ProtocolPackage& data) noexcept override;
        virtual bool on_unpacking(void* native_pac) noexcept override;
    };
}

#endif //!LOTUS_PROTOBASE_H