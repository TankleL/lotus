#ifndef LOTUS_PROTO_SESSION_LSTNR_H
#define LOTUS_PROTO_SESSION_LSTNR_H

namespace lotus::core::protocols::proto_session_lstnr
{
    class ControlHeaderReq
    {
    public:
        bool decode();
    };

    class ControlHeaderRsp
    {};
} // namespace lotus::core::protocols::proto_session_lstnr

#endif // !LOTUS_PROTO_SESSION_LSTNR_H


