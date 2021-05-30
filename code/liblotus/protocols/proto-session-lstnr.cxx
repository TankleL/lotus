#include <msgpack.hpp>
#include "proto-session-lstnr.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    SessionReq::SessionReq() noexcept
        : intention(intention_e::BadIntention)
    {
        request_id = REQ_ID;
    }


    void SessionReq::on_packing(ProtocolPackage& data) noexcept
    {
        ProtocolRequest::on_packing(data);

        msgpack::sbuffer sbuf;
        msgpack::packer pkr(&sbuf);

        // pack intention
        int intention_val = static_cast<int>(intention);
        pkr.pack(intention_val);

        // pack payload
        uint32_t plen = static_cast<uint32_t>(payload.size());
        assert(plen <= std::numeric_limits<uint16_t>::max());
        pkr.pack_bin(plen);

        if (plen > 0)
        {
            pkr.pack_bin_body(payload.data(), plen);
        }

        data.append(sbuf.data(), sbuf.size());
    }

    bool SessionReq::on_unpacking(void* native_pac) noexcept
    {
        if(ProtocolRequest::on_unpacking(native_pac))
        {
            auto pac = static_cast<msgpack::unpacker*>(native_pac);
            msgpack::object_handle objh;

            try
            {
                // unpack intention
                if (!pac->next(objh))
                    return false;

                const auto& msg_intention = objh.get();
                int intention_val = 0;
                msg_intention.convert(intention_val);
                intention = static_cast<intention_e>(intention_val);

                // unpack payload
                if (!pac->next(objh))
                    return false;

                const auto& msg_payload = objh.get();
                assert(msg_payload.type == msgpack::type::BIN);
                assert(msg_payload.via.bin.size <=
                    std::numeric_limits<uint32_t>::max());
                msg_payload >> payload;
            }
            catch(msgpack::unpack_error uex)
            {
                return false;
            }

            return true;
        }
        return false;
    }
} // namespace lotus::core::protocols::proto_session_lstnr




