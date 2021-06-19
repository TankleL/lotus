#ifndef LOTUS_SESSION_LISTENER_H
#define LOTUS_SESSION_LISTENER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "attachable.hxx"
#include "conn.hxx"
#include "proto-session-lstnr.hxx"
#include "session.hxx"
#include "uuid.hxx"

namespace lotus::core
{
    class SessionListener : public IAttachment
    {
    public:
        using session_req_t =
            protocols::proto_session_lstnr::SessionReq<
            protocols::ZeroBased>;
        typedef std::function<void(const char*, size_t)>
            data_received_handler_t;
        typedef std::function<void(std::unique_ptr<Session>&&)>
            new_session_callback_t;
        typedef std::function<void(void)>
            online_session_callback_t;
        new_session_callback_t on_new_session;

    public:
        SessionListener() noexcept;
        virtual ~SessionListener() noexcept;

    public:
        static SessionListener& bind(
            IConnection* conn)
            noexcept;

    public:
        bool parse(const char* data, size_t length) noexcept;

    public:
        bool _parse_payload_single_pack(
            const char* data,
            size_t length) noexcept;
        void _ensure_tmp_pack_data_container();
        void _new_session(const session_req_t& req);

    private:
        IConnection* _conn;
        // TODO: add session manager later
        std::unordered_map<
            uint32_t,
            std::unique_ptr<Session>> _sessions; 
        uint32_t _last_sid;

    private:
        enum class _parse_state_e : int
        {
            idle = 0,
            len0,
            len1,
            payload,
        } _parse_state;

        std::vector<char> _tmp_pack_data;
        size_t _tmp_pack_length;
        size_t _tmp_readlength;
    };
}

#endif // !LOTUS_SESSION_LISTENER_H
