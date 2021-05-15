#include "uuid.hxx"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace lotus::core
{
    std::default_random_engine* UUID::_rendeng = nullptr;
    std::uniform_int_distribution<int> UUID::_renddis(0, 127);

    UUID::UUID()
        : _data({ 0 })
    {}

    UUID::UUID(const UUID& id)
        : _data(id._data)
    {}

    UUID::UUID(UUID&& id) noexcept
        : _data(std::move(id._data))
    {}

    UUID::UUID(const UUID::data_t& data)
        : _data(data)
    {}

    UUID::UUID(UUID::data_t&& data)
        : _data(std::move(data))
    {}

    UUID::~UUID()
    {}

    UUID UUID::generate_v4()
    {
        // TODO: this is a temp impl
        // use system api to replace this impl in future

        // v4: xxxxxxxx-xxxx-40xx-xxxx-xxxxxxxxxxxx

        if (_rendeng == nullptr)
        {
            _rendeng = new std::default_random_engine(std::mt19937((unsigned int)std::time(0)));
        }

        std::array<uint8_t, 16> data;
        data[ 0] = _renddis(*_rendeng);  data[ 1] = _renddis(*_rendeng);  data[ 2] = _renddis(*_rendeng);  data[ 3] = _renddis(*_rendeng);
        data[ 4] = _renddis(*_rendeng);  data[ 5] = _renddis(*_rendeng);  data[ 6] = 0x40;                 data[ 7] = _renddis(*_rendeng);
        data[ 8] = _renddis(*_rendeng);  data[ 9] = _renddis(*_rendeng);  data[10] = _renddis(*_rendeng);  data[11] = _renddis(*_rendeng);
        data[12] = _renddis(*_rendeng);  data[13] = _renddis(*_rendeng);  data[14] = _renddis(*_rendeng);  data[15] = _renddis(*_rendeng);

        return UUID(std::move(data));
    }

    std::string UUID::to_string() const
    {
        std::stringstream ss;
        ss.unsetf(std::ios_base::uppercase);

        ss << std::hex << std::setfill('0')
            << std::setw(2) << (int)_data[ 0]  << std::setw(2) << (int)_data[ 1]        << std::setw(2) << (int)_data[ 2]  << std::setw(2) << (int)_data[ 3] << '-'
            << std::setw(2) << (int)_data[ 4]  << std::setw(2) << (int)_data[ 5] << '-' << std::setw(2) << (int)_data[ 6]  << std::setw(2) << (int)_data[ 7] << '-'
            << std::setw(2) << (int)_data[ 8]  << std::setw(2) << (int)_data[ 9] << '-' << std::setw(2) << (int)_data[10]  << std::setw(2) << (int)_data[11]
            << std::setw(2) << (int)_data[12]  << std::setw(2) << (int)_data[13]        << std::setw(2) << (int)_data[14]  << std::setw(2) << (int)_data[15];

        return ss.str();
    }

    std::string UUID::to_string(char splitter, bool uppercase) const
    {
        const char& s = splitter;
        std::stringstream ss;
        if (uppercase)
        {
            ss.setf(std::ios_base::uppercase);
        }
        else
        {
            ss.unsetf(std::ios_base::uppercase);
        }

        ss << std::hex << std::setfill('0')
            << std::setw(2) << (int)_data[ 0]  << std::setw(2) << (int)_data[ 1]      << std::setw(2) << (int)_data[ 2]  << std::setw(2) << (int)_data[ 3] << s
            << std::setw(2) << (int)_data[ 4]  << std::setw(2) << (int)_data[ 5] << s << std::setw(2) << (int)_data[ 6]  << std::setw(2) << (int)_data[ 7] << s
            << std::setw(2) << (int)_data[ 8]  << std::setw(2) << (int)_data[ 9] << s << std::setw(2) << (int)_data[10]  << std::setw(2) << (int)_data[11]
            << std::setw(2) << (int)_data[12]  << std::setw(2) << (int)_data[13]        << std::setw(2) << (int)_data[14]  << std::setw(2) << (int)_data[15];

        return ss.str();
    }

    unsigned int UUID::hash_code() const
    {
        struct s8_4_4_16
        {
            unsigned long  data1;
            unsigned short data2;
            unsigned short data3;
            unsigned char  data4[8];
        };

        const s8_4_4_16* t = reinterpret_cast<const s8_4_4_16*>(_data.data());

        return ((t->data1 ^ ((t->data2 << 0x10) | \
            (t->data3))) ^ ((t->data4[2] << 0x18) | t->data4[7]));
    }

    const UUID::data_t& UUID::data() const
    {
        return _data;
    }

    UUID& UUID::operator=(const UUID& rhs)
    {
        _data = rhs._data;
        return *this;
    }

    UUID& UUID::operator=(UUID&& rhs) noexcept
    {
        _data = std::move(rhs._data);
        return *this;
    }

    bool UUID::operator==(const UUID& rhs) const noexcept
    {
        return _data == _data;
    }

    bool UUID::operator!=(const UUID& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    uint8_t& UUID::operator[](int idx)
    {
        return _data[idx];
    }
} // namespace lotus::core

