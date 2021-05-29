#ifndef LOTUS_PROTO_SESSION_LSTNR_H
#define LOTUS_PROTO_SESSION_LSTNR_H

#include "protobase.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    struct ControlHeaderReq final : public ProtocolRequest 
    {
        static constexpr int REQ_ID = 1;

        ControlHeaderReq() noexcept;

        enum class intention_e : int
        {
            BadIntention = 0,
            NewSession = 1,
            StoreSession = 2,
            RejoinSession = 3,
            LeaveSession = 4
        } intention;

        void on_packing(
            ProtocolPackage& data) noexcept override;
        bool on_unpacking(void* native_pac) noexcept override;
    };

    class ControlHeaderRsp
    {};
} // namespace lotus::core::protocols::proto_session_lstnr

#endif // !LOTUS_PROTO_SESSION_LSTNR_H


