#ifndef LOTUS_VLINT_H
#define LOTUS_VLINT_H

#include "vluint.hxx"

namespace lotus::core::variant_length
{
    class vlint : private vluint
    {
    public:
        vlint() noexcept;
        vlint(std::int32_t val) noexcept;
        vlint(const vlint& rhs) noexcept;
        vlint(vlint&& rhs) noexcept;
        vlint& operator=(const vlint& rhs) noexcept;
        vlint& operator=(vlint&& rhs) noexcept;
        ~vlint() noexcept;

    public:
        std::int32_t value() const noexcept;
        int decoded_size() const noexcept;
        
        void value(std::int32_t val) noexcept;

        digest_state_e digest(const std::uint8_t& byte) noexcept;
        int encoded_size() const noexcept;
        const std::uint8_t* encoded_data() const noexcept;

    private:
        static std::uint32_t _zz_map(std::int32_t n) noexcept;
        static std::int32_t _zz_unmap(std::uint32_t n) noexcept;
    };

    std::vector<uint8_t>& operator<<(
        std::vector<uint8_t>& dest,
        const vlint& vuint);
} // namespace lotus::core::_internal

#endif // !LOTUS_VLINT_H


