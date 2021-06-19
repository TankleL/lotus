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
            - fixed_header_len;
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

    bool ProtocolBase::unpack(
        const char* data,
        size_t length) noexcept
    {
        msgpack::unpacker pac;
        pac.reserve_buffer(length);
        memcpy(pac.buffer(), data, length);
        pac.buffer_consumed(length);

        return on_unpacking(&pac);
    }

    // ProtocolRequest ----------------------------------------------
    ProtocolRequest::ProtocolRequest() noexcept
        : request_id(0)
        , trx_id(0)
    {}

    ProtocolRequest::~ProtocolRequest() noexcept
    {}

    void ProtocolRequest::on_packing(ProtocolPackage& data) noexcept
    {
        //TODO: optimize this part into a zero-copy way in future
        msgpack::sbuffer buf;
        msgpack::packer pkr(&buf);

        pkr.pack(request_id);
        pkr.pack(trx_id);

        data.append(buf.data(), buf.size());
    }

    bool ProtocolRequest::on_unpacking(void* native_pac) noexcept
    {
        auto pac = static_cast<msgpack::unpacker*>(native_pac);
        msgpack::object_handle objh;

        try
        {
            // unpack request_id
            if (!pac->next(objh))
                return false;

            const auto& msg_reqid = objh.get();
            msg_reqid.convert(request_id);

            // unpack trx_id
            if (!pac->next(objh))
                return false;

            const auto& msg_trxid = objh.get();
            msg_trxid.convert(trx_id);
        }
        catch(msgpack::unpack_error uex)
        {
            return false;
        }

        return true;
    }

    // ProtocolResponse ---------------------------------------------

    ProtocolResponse::ProtocolResponse() noexcept
        : result_code(0)
        , trx_id(0)
    {}

    ProtocolResponse::~ProtocolResponse() noexcept
    {}

    void ProtocolResponse::on_packing(ProtocolPackage& data) noexcept
    {
        // TODO: optimize this part into a zero-copy way in future
        msgpack::sbuffer buf;
        msgpack::packer pkr(&buf);

        pkr.pack(result_code);
        pkr.pack(trx_id);

        data.append(buf.data(), buf.size());
    }

    bool ProtocolResponse::on_unpacking(void* native_pac) noexcept
    {
        auto pac = static_cast<msgpack::unpacker*>(native_pac);
        msgpack::object_handle objh;

        try
        {
            // unpack result_code
            if (!pac->next(objh))
                return false;

            const auto& msg_rescode = objh.get();
            msg_rescode.convert(result_code);

            // unpack trx_id
            if (!pac->next(objh))
                return false;

            const auto& msg_trxid = objh.get();
            msg_trxid.convert(trx_id);
        }
        catch(msgpack::unpack_error uex)
        {
            return false;
        }
        return true;
    }
} // namespace lotus::core::protocols



