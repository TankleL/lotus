#ifndef LOTUS_VLUINT_H
#define LOTUS_VLUINT_H

#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

namespace lotus::core::variant_length
{
    class vluint
    {
    public:
        enum digest_state_e: int
        {
            ds_idle = 0,
            ds_decode_0,
            ds_decode_1,
            ds_decode_2,
            ds_decode_3,
            ds_bad
        };

    public:
        vluint() ;
        vluint(std::uint32_t val) ;
        vluint(const vluint& rhs) ;
        vluint(vluint&& rhs) ;
        vluint& operator=(const vluint& rhs) ;
        vluint& operator=(vluint&& rhs) ;
        ~vluint() ;

    public:
        std::uint32_t value() const ;
        int decoded_size() const ;
        
        void value(std::uint32_t val) ;

        digest_state_e digest(const std::uint8_t& byte) ;
        int encoded_size() const ;
        const std::uint8_t* encoded_data() const ;

    private:
        std::array<std::uint8_t, 6> _data;
        std::uint8_t _esize;        // encoded size
        std::uint8_t _dsize;        // decoded size
        digest_state_e _digst;
        std::uint32_t _value;
    };

    std::vector<uint8_t>& operator<<(
        std::vector<uint8_t>& dest,
        const vluint& vuint);
}

#endif // !LOTUS_VLUINT_H
