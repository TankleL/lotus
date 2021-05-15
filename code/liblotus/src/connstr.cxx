#include <regex>
#include "connstr.hxx"

namespace lotus::core::_internal
{
    const std::regex g_rgx_connstr_segment("(\\w+)=((\\w|\\.)+)");
    const std::string g_connstr_seg_name_host("host");
    const std::string g_connstr_seg_name_port("port");

    ConnString::ConnString(const std::string& connstr) noexcept
        : _port(0)
    {
        auto seg_beg = std::sregex_iterator(
            connstr.begin(),
            connstr.end(),
            g_rgx_connstr_segment);
        auto seg_end = std::sregex_iterator();

        for(auto seg = seg_beg; seg != seg_end; ++seg)
        {
            std::smatch match = *seg;
            _segments[match[1]] = match[2];
        }

        _filter_common_segements();
    }

    void ConnString::_filter_common_segements() noexcept
    {
        if(_segments.find(g_connstr_seg_name_host) != _segments.cend())
        {
            _host = _segments[g_connstr_seg_name_host];
        }
        
        if (_segments.find(g_connstr_seg_name_port) != _segments.cend())
        {
            auto val = std::stoi(_segments[g_connstr_seg_name_port]);
            _port = static_cast<uint16_t>(val);
        }
    }

    std::string ConnString::host() const noexcept
    {
        return _host;
    }

    uint16_t ConnString::port() const noexcept
    {
        return _port;
    }


} // namespace lotus::core::_internal


