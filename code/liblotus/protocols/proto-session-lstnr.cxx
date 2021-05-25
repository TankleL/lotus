#include <msgpack.hpp>
#include "proto-session-lstnr.hxx"

namespace lotus::core::protocols::proto_session_lstnr
{
    void ControlHeaderReq::on_packing(ProtocolPackage& data) noexcept
    {
        msgpack::sbuffer sbuf;
        msgpack::packer pkr(&sbuf);

        pkr.pack(req_id);
        pkr.pack(intention);

        data.append(sbuf.data(), sbuf.size());
    }
} // namespace lotus::core::protocols::proto_session_lstnr




