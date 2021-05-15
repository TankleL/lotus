#ifndef LOTUS_UUID_H
#define LOTUS_UUID_H

#include <memory>
#include <string>
#include <random>
#include <cstdint>
#include <array>

namespace lotus::core
{
    class UUID
    {
    public:
        typedef std::array<uint8_t, 16> data_t;

    public:
        UUID();
        UUID(const UUID& id);
        UUID(UUID&& id) noexcept;
        UUID(const data_t& data);
        UUID(data_t&& data);
        ~UUID();

        static UUID generate_v4();

        std::string to_string() const;
        std::string to_string(char splitter, bool uppercase) const;
        const data_t& data() const;
        unsigned int hash_code() const;

        UUID& operator=(const UUID& rhs);
        UUID& operator=(UUID&& rhs) noexcept;
        bool operator==(const UUID& rhs) const noexcept;
        bool operator!=(const UUID& rhs) const noexcept;
        uint8_t& operator[](int idx);

    private:
        data_t _data;

    private:
        static std::default_random_engine* _rendeng;
        static std::uniform_int_distribution<int> _renddis;
    };

    struct uuid_hasher
    {
        unsigned int operator()(const UUID& uuid) const
        {
            return uuid.hash_code();
        }
    };
} // lotus::core

#endif // LOTUS_UUID_H
