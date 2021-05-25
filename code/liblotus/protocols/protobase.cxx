#include <msgpack.hpp>
#include "protobase.hxx"

namespace lotus::core::protocols
{
    // ProtocolPackage ----------------------------------------------
    
    ProtocolPackage::ProtocolPackage() noexcept
        : _data({0, 0}) // use the first 2 bytes to store length
    {}

    ProtocolPackage::~ProtocolPackage() noexcept
    {}

    size_t ProtocolPackage::length() noexcept
    {
        return _data.size();
    }

    size_t ProtocolPackage::payload_length() noexcept
    {
        return _data.size() - fixed_header_len;
    }

    const char* ProtocolPackage::data() noexcept
    {
        return _data.data();
    }

    void ProtocolPackage::append(
        const char* data,
        size_t length) noexcept
    {
        _data.insert(_data.end(), data, data + length);
        const auto payload_len =
            _data.size()
            - fixed_header_len
            + length;
        const auto masked_payload_len =
            payload_len & 0x0000ffff;
        _data[0] = static_cast<char>(masked_payload_len & 0x00ff);
        _data[1] = static_cast<char>(masked_payload_len >> 8);
    }

    // ProtocolBase -------------------------------------------------
    ProtocolPackage ProtocolBase::pack() noexcept
    {
        ProtocolPackage result;
        on_packing(result);
        return result;
    }

    // ProtocolRequest ----------------------------------------------
    ProtocolRequest::ProtocolRequest() noexcept
    {}

    ProtocolRequest::~ProtocolRequest() noexcept
    {}

    void ProtocolRequest::on_packing(ProtocolPackage& data) noexcept
    {}

    // ProtocolResponse ---------------------------------------------

    ProtocolResponse::ProtocolResponse() noexcept
        : result_code(0)
    {}

    ProtocolResponse::~ProtocolResponse() noexcept
    {}

    void ProtocolResponse::on_packing(ProtocolPackage& data) noexcept
    {
        msgpack::sbuffer buf;
        msgpack::packer pkr(&buf);

        pkr.pack(result_code);

        data.append(buf.data(), buf.size());
    }
} // namespace lotus::core::protocols



