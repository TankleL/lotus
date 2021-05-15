#ifndef LOTUS_CONNSTR_H
#define LOTUS_CONNSTR_H

#include <string>
#include <map>
#include <cstdint>

namespace lotus::core::_internal
{
    class ConnString
    {
    public:
        ConnString(const std::string& connstr) noexcept;

        std::string host() const noexcept;
        uint16_t port() const noexcept;

    private:
        void _filter_common_segements() noexcept;
        
    private:
        std::map<std::string, std::string> _segments;

        // common used segments
        std::string _host;
        uint16_t _port;
    };
} // namespace lotus::core::_internal

#endif // LOTUS_CONNSTR_H
