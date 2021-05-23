#ifndef LOTUS_PROTO_SESSION_LSTNR_H
#define LOTUS_PROTO_SESSION_LSTNR_H

#include "protobase.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    struct ControlHeaderReq : public ProtocolRequest 
    {
        enum struct Intention : int
        {
            NewSession = 0,
            RejoinSession = 1,
            LeaveSession = 2
        };
    };

    class ControlHeaderRsp
    {};
} // namespace lotus::core::protocols::proto_session_lstnr

#endif // !LOTUS_PROTO_SESSION_LSTNR_H


