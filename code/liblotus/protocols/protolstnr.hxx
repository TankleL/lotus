#ifndef LOTUS_PROTO_LSTNR_H
#define LOTUS_PROTO_LSTNR_H

#include <functional>
#include <unordered_map>

#include "attachable.hxx"
#include "conn.hxx"
#include "protobase.hxx"

namespace lotus::core::protocols
{
    class ListenedPackage final
    {
    public:
        ListenedPackage() noexcept;
        explicit ListenedPackage(size_t size) noexcept;
        ListenedPackage(const ListenedPackage&) = delete;
        ListenedPackage(ListenedPackage&&) noexcept;
        ~ListenedPackage() noexcept;

        ListenedPackage& operator=(const ListenedPackage&) = delete;
        ListenedPackage& operator=(ListenedPackage&& rhs) noexcept;

    public:
        char* data() const noexcept;
        size_t size() const noexcept;
        ListenedPackage& acquire(ListenedPackage& src) noexcept;
        bool is_available() const noexcept;
        ListenedPackage& redefine(size_t size) noexcept;

    private:
        char* _buf;
        size_t _length;
    };

    class ProtoListener : public IAttachment
    {
    public:
        typedef std::function<
            bool(ProtocolRequest<ProtocolBase>&& req,
                ListenedPackage&& pack,
                size_t offset)> req_callback_t;
        typedef std::function<
            bool(ProtocolResponse<ProtocolBase>&& rsp,
                ListenedPackage&& pack,
                size_t offset)> rsp_callback_t;
        typedef std::function<bool(void)> notify_callback_t;
        typedef std::function<bool(void)> unknown_callback_t;

    public:
        ProtoListener() noexcept;
        ProtoListener(const ProtoListener&) = delete;
        ProtoListener(ProtoListener&&) = default;
        virtual ~ProtoListener() noexcept;

        ProtoListener& operator=(const ProtoListener&) = delete;
        ProtoListener& operator=(ProtoListener&&) = default;

    public:
        static ProtoListener& bind(IConnection* conn) noexcept;
        bool parse(const char* data, size_t length) noexcept;
        void add_request_callback(
            uint32_t req_id,
            const req_callback_t& cb) noexcept;
        void add_response_callback(
            uint32_t rsp_id,
            const rsp_callback_t& cb) noexcept;

    private:
        void _ensure_pack() noexcept;
        bool _parse_single_pack(
            const char* data,
            size_t length) noexcept;
        bool _dispatch(ListenedPackage&& pack) noexcept;
        
    private:
        IConnection* _conn;
        std::unordered_map<uint32_t, req_callback_t> _req_map;
        std::unordered_map<uint32_t, rsp_callback_t> _rsp_map;
        std::unordered_map<uint32_t, notify_callback_t> _notify_map;
        std::unordered_map<uint32_t, unknown_callback_t> _unknown_map;

    private:
        enum class ParseState : int
        {
            idle = 0,
            len0,
            len1,
            payload,
        } _parse_state;

        ListenedPackage _tmp_pack;
        size_t _tmp_pack_size;
        size_t _tmp_pack_read;
    };
} // namespace lotus::core

#endif // !LOTUS_PROTO_LSTNR_H
