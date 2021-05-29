#include <msgpack.hpp>
#include "proto-session-lstnr.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    ControlHeaderReq::ControlHeaderReq() noexcept
        : intention(intention_e::BadIntention)
    {
        request_id = REQ_ID;
    }


    void ControlHeaderReq::on_packing(ProtocolPackage& data) noexcept
    {
        msgpack::sbuffer sbuf;
        msgpack::packer pkr(&sbuf);

        int intention_val = static_cast<int>(intention);
        pkr.pack(intention_val);

        data.append(sbuf.data(), sbuf.size());
    }

    bool ControlHeaderReq::on_unpacking(void* native_pac) noexcept
    {
        if(ProtocolRequest::on_unpacking(native_pac))
        {
            auto pac = static_cast<msgpack::unpacker*>(native_pac);
            msgpack::object_handle objh;
            if (!pac->next(objh))
                return false;

            const auto& msgobj = objh.get();
            int intention_val = 0;
            if (!msgobj.convert_if_not_nil<int>(intention_val))
                return false;
            intention = static_cast<intention_e>(intention_val);
        }
        return false;
    }
} // namespace lotus::core::protocols::proto_session_lstnr




