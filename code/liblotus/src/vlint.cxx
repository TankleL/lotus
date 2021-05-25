#include "vlint.hxx"

namespace lotus::core::variant_length
{
    vlint::vlint() noexcept
    {}

    vlint::vlint(std::int32_t val) noexcept
        : vluint(_zz_map(val))
    {}

    vlint::vlint(const vlint& rhs) noexcept
        : vluint(rhs)
    {}

    vlint::vlint(vlint&& rhs) noexcept
        : vluint(std::move(rhs))
    {}

    vlint& vlint::operator=(const vlint& rhs) noexcept
    {
        vluint::operator=(rhs);
        return *this;
    }

    vlint& vlint::operator=(vlint&& rhs) noexcept
    {
        vluint::operator=(std::move(rhs));
        return *this;
    }

    vlint::~vlint()
    {}

    std::int32_t vlint::value() const noexcept
    {
        return _zz_unmap(vluint::value());
    }

    int vlint::decoded_size() const noexcept
    {
        return vluint::decoded_size();
    }

    void vlint::value(std::int32_t val) noexcept
    {
        vluint::value(_zz_map(val));
    }

    vlint::digest_state_e
    vlint::digest(const std::uint8_t& byte) noexcept
    {
        return vluint::digest(byte);
    }

    int vlint::encoded_size() const noexcept
    {
        return vluint::encoded_size();
    }

    const std::uint8_t* vlint::encoded_data() const noexcept
    {
        return vluint::encoded_data();
    }

    std::uint32_t vlint::_zz_map(std::int32_t n) noexcept
    {
        return (n << 1) ^ (n >> 31);
    }

    std::int32_t vlint::_zz_unmap(std::uint32_t n) noexcept
    {
        return (n >> 1) ^ -static_cast<int32_t>(n & 1);
    }

    std::vector<uint8_t>&
    operator<<(std::vector<uint8_t>& dest, const vlint& vint)
    {
        dest.insert(
            dest.end(),
            vint.encoded_data(),
            vint.encoded_data() + vint.encoded_size());
        return dest;
    }
}


