#ifndef LOTUS_SESSION_LISTENER_H
#define LOTUS_SESSION_LISTENER_H

#include <functional>
#include <memory>
#include <session.hxx>

#include "attachable.hxx"
#include "conn.hxx"

namespace lotus::core
{
    class SessionListener : public IAttachment
    {
    public:
        typedef std::function<void(const char*, size_t)>
            data_received_handler_t;
        typedef std::function<void(std::unique_ptr<Session>&&)>
            new_session_callback_t;
        new_session_callback_t on_new_session;

    public:
        SessionListener() noexcept;
        virtual ~SessionListener() noexcept;

    public:
        static SessionListener& bind(
            IServerSideConnection& conn) noexcept;

    public:
        bool parse(const char* data, size_t length) noexcept;

    public:
        bool _parse_payload_single_pack(
            const char* data,
            size_t length) noexcept;
        void _ensure_tmp_pack_data_container();

    private:
        enum class _parse_state_e : int
        {
            PS_Idle = 0,
            PS_Len0,
            PS_Len1,
            PS_Payload,
        } _parse_state;

        std::vector<char*> _tmp_pack_data;
        size_t _tmp_pack_length;
        size_t _tmp_readlength;
    };
}

#endif // !LOTUS_SESSION_LISTENER_H
