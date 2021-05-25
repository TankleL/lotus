#ifndef LOTUS_PROTO_SESSION_LSTNR_H
#define LOTUS_PROTO_SESSION_LSTNR_H

#include "protobase.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    struct ControlHeaderReq final : public ProtocolRequest 
    {
        constexpr static int req_id = 1;
        enum struct intention_e : int
        {
            NewSession = 0,
            RejoinSession = 1,
            LeaveSession = 2
        } intention;


        void on_packing(
            ProtocolPackage& data) noexcept override;
    };

    class ControlHeaderRsp
    {};
} // namespace lotus::core::protocols::proto_session_lstnr

#endif // !LOTUS_PROTO_SESSION_LSTNR_H


