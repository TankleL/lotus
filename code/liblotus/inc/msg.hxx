#ifndef LOTUS_MSG_H
#define LOTUS_MSG_H

#include <string>
#include <cstdint>
#include "uuid.hxx"

/*
 * NOTE: This is a bad designed message class
 * We will change this file entirely once we finished
 * fundamental works
*/

namespace lotus::core
{
    class Message
    {
    public:
        enum interactive_method_e : uint8_t
        {
            im_unknown = 0x00,
            im_reserved = 0x09,

            im_request = 0x0a,
            im_response = 0x0b,
            im_notification = 0x0c,

            im_get = 0x1a,
            im_post = 0x1b,
            im_put = 0x1c,
            im_delete = 0x1d,
        };

    public:
        static const uint8_t VERSION = 0x01;

    public:
        Message();
        Message(const Message& rhs);
        Message(Message&& rhs) noexcept;

    public:
        void acquire(Message& rhs);
        Message& operator=(const Message& rhs);
        Message& operator=(Message&& rhs) noexcept;

    public:
        UUID        session_id;
        std::string session_payload; // max length = 0xffff = 65536
        std::string payload;         // max length = 0xffff = 65536
        uint8_t     version;
        interactive_method_e im;
    };

    class MessageSerializer
    {
    public:
        static const uint8_t SUPPORT_VERSION = 0x01;

    public:
        enum digest_state_e
        {
            ds_bad,
            ds_idle,
            ds_version,
            ds_im,
            ds_session_id,
            ds_session_payload_len_hi,
            ds_session_payload_len_lo,
            ds_session_payload,
            ds_payload_len_hi,
            ds_payload_len_lo,
            ds_payload
        };

    public:
        MessageSerializer();

    public:
        // deserialization
        digest_state_e digest(uint8_t byte);
        bool pull_message(Message& dest);
        void reset();

    public:
        // serialization
        static std::string serialize(const Message& msg);

    private:
        Message         _tmpmsg;
        digest_state_e  _ds;
        uint32_t        _memo_index;
        uint16_t        _memo_length;
    };
}

#endif // LOTUS_MSG_H
