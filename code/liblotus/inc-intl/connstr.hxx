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
        static const std::string not_seg_val;

    public:
        ConnString(const std::string& connstr) ;

        std::string host() const ;
        uint16_t port() const ;
        int backlogs() const ;

        std::string segment(const std::string& name) const ;

    private:
        void _filter_common_segements() ;
        
    private:
        std::map<std::string, std::string> _segments;

        // common used segments
        std::string _host;
        uint16_t _port;
        int _backlogs;
    };
} // namespace lotus::core::_internal

#endif // LOTUS_CONNSTR_H
