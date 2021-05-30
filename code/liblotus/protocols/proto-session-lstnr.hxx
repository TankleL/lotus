#ifndef LOTUS_PROTO_SESSION_LSTNR_H
#define LOTUS_PROTO_SESSION_LSTNR_H

#include "protobase.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    struct SessionReq final : public ProtocolRequest 
    {
        static constexpr int REQ_ID = 0x00000101;

        SessionReq () noexcept;

        enum class intention_e : int
        {
            BadIntention = 0,
            NewSession = 1,
            SessionData = 2,
            AbortSession = 3
        } intention;

        std::vector<char> payload;

        void on_packing(
            ProtocolPackage& data) noexcept override;
        bool on_unpacking(void* native_pac) noexcept override;
    };

    struct SessionRsp final : public ProtocolResponse
    {};
} // namespace lotus::core::protocols::proto_session_lstnr

#endif // !LOTUS_PROTO_SESSION_LSTNR_H


