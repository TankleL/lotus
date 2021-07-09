#ifndef LOTUS_PROTOBASE_H
#define LOTUS_PROTOBASE_H

#include <stdexcept>
#include <vector>

namespace lotus::core::protocols
{
    class UnpackError : public std::runtime_error
    {
    public:
        UnpackError();
        UnpackError(const UnpackError&) = default;
        UnpackError(UnpackError&&) = default;
    };

    class PackedPackage final
    {
    public:
        static constexpr size_t FIXED_HEADER_LENGTH = 2;

    public:
        PackedPackage() ;
        PackedPackage(const PackedPackage&) = delete;
        PackedPackage(PackedPackage&& rhs) ;
        ~PackedPackage() ;

        PackedPackage& operator=(const PackedPackage&) = delete;
        PackedPackage& operator=(PackedPackage&& rhs) ;

    public:
        void* native_stream() const ;
        void refresh_header() ;
        const char* data() const ;
        size_t length() const ;
        
    private:
        void* _native_stream;
    };

    class Packer final
    {
    public:
        Packer() ;
        Packer(const Packer&) = delete;
        Packer(Packer&& rhs) ;
        ~Packer() ;

        Packer& operator=(const Packer&) = delete;
        Packer& operator=(Packer&& rhs) ;

    public:
        PackedPackage& result();
        void pack_int32(int32_t val);
        void pack_uint32(uint32_t val);
        void pack_blob(const std::vector<char>& blob);

    private:
        PackedPackage _package;
        void* _native_pac;
    };

    class Unpacker final
    {
    public:
        Unpacker() ;
        Unpacker(const char* data, size_t length) ;
        Unpacker(const Unpacker&) = delete;
        Unpacker(Unpacker&& rhs) ;
        ~Unpacker() ;

        Unpacker& operator=(const Unpacker&) = delete;
        Unpacker& operator=(Unpacker&& rhs) ;

    public:
        void set_data(const char* data, size_t length) ;
        bool next() ;
        size_t parsed_size() const ;

        int32_t to_int32() const;
        uint32_t to_uint32() const;
        void to_blob(std::vector<char>& out) const;

    private:
        void* _native_pac;
        void* _native_obj_handle;

        size_t _buf_size;
    };

    struct ProtocolBase
    {
        static constexpr size_t UNPACK_ERROR = -1;

        enum class ProtocolType : int32_t
        {
            unknown = 0,
            request = 1,
            response = 2,
            notification = 3
        } proto_type;

        ProtocolBase() ;
        ProtocolBase(const ProtocolBase&) = delete;
        ProtocolBase(ProtocolBase&& rhs) ;
        ProtocolBase& operator=(const ProtocolBase&) = delete;
        ProtocolBase& operator=(ProtocolBase&& rhs) ;

        PackedPackage pack() ;
        size_t unpack(const char* data, size_t length);

        virtual ~ProtocolBase()  {};
        virtual void on_packing(Packer& pac) ;
        virtual size_t on_unpacking(Unpacker& pac);
    };

    struct ZeroBased;

    template<typename BaseProto = ZeroBased>
    struct ProtocolRequest;

    template<>
    struct ProtocolRequest<ProtocolBase> : public ProtocolBase
    {
        uint32_t request_id;
        uint32_t ctx_id;

        ProtocolRequest(ProtocolBase&& base) 
            : ProtocolBase(std::move(base))
            , request_id(0)
            , ctx_id(0)
        {}

        virtual ~ProtocolRequest() 
        {}

        ProtocolRequest(const ProtocolRequest&) = default;
        ProtocolRequest(ProtocolRequest&&) = default;
        ProtocolRequest& operator=(const ProtocolRequest&) = default;
        ProtocolRequest& operator=(ProtocolRequest&&) = default;

        virtual void on_packing(Packer& pac)  override
        {
            pac.pack_uint32(request_id);
            pac.pack_uint32(ctx_id);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            if (!pac.next())
                return pac.parsed_size();
            request_id = pac.to_uint32();

            if (!pac.next())
                return pac.parsed_size();
            ctx_id = pac.to_uint32();

            return pac.parsed_size();
        }
    };

    template<>
    struct ProtocolRequest<ZeroBased>
        : public ProtocolRequest<ProtocolBase>
    {
        ProtocolRequest() 
            : ProtocolRequest<ProtocolBase>(ProtocolBase())
        {
            proto_type = ProtocolType::request;
        }

        virtual void on_packing(Packer& pac)  override
        {
            ProtocolBase::on_packing(pac);
            ProtocolRequest<ProtocolBase>::on_packing(pac);
        }
        
        virtual size_t on_unpacking(Unpacker& pac) override
        {
            ProtocolBase::on_unpacking(pac);
            return ProtocolRequest<ProtocolBase>::on_unpacking(pac);
        }
    };

    template<typename BaseProto = ZeroBased>
    struct ProtocolResponse;

    template<>
    struct ProtocolResponse<ProtocolBase> : public ProtocolBase
    {
        uint32_t response_id;
        int32_t result_code;
        uint32_t ctx_id;

        ProtocolResponse(ProtocolBase&& base) 
            : ProtocolBase(std::move(base))
            , response_id(0)
            , result_code(0)
            , ctx_id(0)
        {}

        virtual ~ProtocolResponse() 
        {}

        ProtocolResponse(const ProtocolResponse&) = default;
        ProtocolResponse(ProtocolResponse&&) = default;
        ProtocolResponse& operator=(const ProtocolResponse&) = default;
        ProtocolResponse& operator=(ProtocolResponse&&) = default;

        virtual void on_packing(Packer& pac)  override
        {
            pac.pack_uint32(response_id);
            pac.pack_int32(result_code);
            pac.pack_uint32(ctx_id);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            if (!pac.next())
                return pac.parsed_size();
            response_id = pac.to_uint32();

            if (!pac.next())
                return pac.parsed_size();
            result_code = pac.to_int32();

            if (!pac.next())
                return pac.parsed_size();
            ctx_id = pac.to_uint32();

            return pac.parsed_size();
        }
    };

    template<>
    struct ProtocolResponse<ZeroBased>
        : public ProtocolResponse<ProtocolBase>
    {
        ProtocolResponse() 
            : ProtocolResponse<ProtocolBase>(ProtocolBase())
        {
            proto_type = ProtocolType::response;
        }

        virtual void on_packing(Packer& pac)  override
        {
            ProtocolBase::on_packing(pac);
            ProtocolResponse<ProtocolBase>::on_packing(pac);
        }

        virtual size_t on_unpacking(Unpacker& pac) override
        {
            ProtocolBase::on_unpacking(pac);
            return ProtocolResponse<ProtocolBase>::on_unpacking(pac);
        }
    };
}

#endif //!LOTUS_PROTOBASE_H