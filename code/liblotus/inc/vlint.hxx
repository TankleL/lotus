#ifndef LOTUS_VLINT_H
#define LOTUS_VLINT_H

#include "vluint.hxx"

namespace lotus::core::variant_length
{
    class vlint : private vluint
    {
    public:
        vlint() ;
        vlint(std::int32_t val) ;
        vlint(const vlint& rhs) ;
        vlint(vlint&& rhs) ;
        vlint& operator=(const vlint& rhs) ;
        vlint& operator=(vlint&& rhs) ;
        ~vlint() ;

    public:
        std::int32_t value() const ;
        int decoded_size() const ;
        
        void value(std::int32_t val) ;

        digest_state_e digest(const std::uint8_t& byte) ;
        int encoded_size() const ;
        const std::uint8_t* encoded_data() const ;

    private:
        static std::uint32_t _zz_map(std::int32_t n) ;
        static std::int32_t _zz_unmap(std::uint32_t n) ;
    };

    std::vector<uint8_t>& operator<<(
        std::vector<uint8_t>& dest,
        const vlint& vuint);
} // namespace lotus::core::_internal

#endif // !LOTUS_VLINT_H


