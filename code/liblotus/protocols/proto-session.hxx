#ifndef LOTUS_PROTO_SESSION_LSTNR_H
#define LOTUS_PROTO_SESSION_LSTNR_H

#include "uuid.hxx"
#include "protobase.hxx"

namespace lotus::core::protocols::proto_session
{
    template<typename BaseProto = ZeroBased>
    struct SessionReq;

    template<>
    struct SessionReq<ProtocolRequest<ProtocolBase>>
        : public ProtocolRequest<ProtocolBase>
    {
        static constexpr int REQ_ID = 0x00000101;
        enum class Intention : int32_t
        {
            bad_intention= 0,
            begin_session= 1,
            end_session = 2,
            session_data = 3,
        } intention;
        uint32_t session_id;
        std::vector<char> payload;

        SessionReq(ProtocolRequest<ProtocolBase>&& base) 
            : ProtocolRequest<ProtocolBase>(std::move(base))
            , intention(Intention::bad_intention)
            , session_id(0)
        {
            request_id = REQ_ID;
        }

        virtual void on_packing(Packer& pac)  override
        {
            pac.pack_int32(static_cast<int32_t>(intention));
            pac.pack_uint32(session_id);
            pac.pack_blob(payload);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            if (!pac.next())
                return pac.parsed_size();
            intention = static_cast<Intention>(pac.to_int32());

            if (!pac.next())
                return pac.parsed_size();
            session_id = pac.to_uint32();
            
            if (!pac.next())
                return pac.parsed_size();
            pac.to_blob(payload);
            return pac.parsed_size();
        }
    };

    template<>
    struct SessionReq<ZeroBased> final
        : public SessionReq<ProtocolRequest<ProtocolBase>>
    {
        SessionReq() 
            : SessionReq<ProtocolRequest<ProtocolBase>>(
                ProtocolRequest<ZeroBased>())
        {}

        virtual void on_packing(Packer& pac)  override
        {
            ProtocolBase::on_packing(pac);
            SessionReq<ProtocolRequest<ProtocolBase>>
                ::ProtocolRequest
                ::on_packing(pac);
            SessionReq<ProtocolRequest<ProtocolBase>>
                ::on_packing(pac);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            ProtocolBase::on_unpacking(pac);
            SessionReq<ProtocolRequest<ProtocolBase>>
                ::ProtocolRequest
                ::on_unpacking(pac);
            return SessionReq<ProtocolRequest<ProtocolBase>>
                ::on_unpacking(pac);
        }
    };

    template<typename BaseProto = ZeroBased>
    struct SessionRsp;

    template<>
    struct SessionRsp<ProtocolResponse<ProtocolBase>>
        : public ProtocolResponse<ProtocolBase>
    {
        static constexpr int RSP_ID = 0x00000101;
        uint32_t session_id;
        
        SessionRsp(ProtocolResponse<ProtocolBase>&& base) 
            : ProtocolResponse<ProtocolBase>(std::move(base))
            , session_id(0)
        {
            response_id = RSP_ID;
        }

        virtual void on_packing(Packer& pac)  override
        {
            pac.pack_uint32(session_id);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            try
            {
                if (!pac.next())
                    return pac.parsed_size();
                session_id = pac.to_uint32();
                return pac.parsed_size();
            }
            catch (UnpackError)
            {
                return UNPACK_ERROR;
            }
        }
    };
    
    template<>
    struct SessionRsp<ZeroBased> final
        : public SessionRsp<ProtocolResponse<ProtocolBase>>
    {
        SessionRsp() 
            : SessionRsp<ProtocolResponse<ProtocolBase>>(
                ProtocolResponse<ZeroBased>())
        {}

        virtual void on_packing(Packer& pac)  override
        {
            ProtocolBase::on_packing(pac);
            SessionRsp<ProtocolResponse<ProtocolBase>>
                ::ProtocolResponse<ProtocolBase>
                ::on_packing(pac);
            SessionRsp<ProtocolResponse<ProtocolBase>>
                ::on_packing(pac);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            if (ProtocolBase::on_unpacking(pac) == UNPACK_ERROR)
                return UNPACK_ERROR;
            if (SessionRsp<ProtocolResponse<ProtocolBase>>
                ::ProtocolResponse<ProtocolBase>
                ::on_unpacking(pac) == UNPACK_ERROR)
                return UNPACK_ERROR;
            return SessionRsp<ProtocolResponse<ProtocolBase>>
                ::on_unpacking(pac);
        }
    };
} // namespace lotus::core::protocols::proto_session

#endif // !LOTUS_PROTO_SESSION_LSTNR_H


