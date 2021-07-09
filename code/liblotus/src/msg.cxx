#include "msg.hxx"

namespace lotus::core
{
    Message::Message()
        : version(VERSION)
        , im(im_unknown)
    {}

    Message::Message(const Message& rhs)
        : session_id(rhs.session_id)
        , session_payload(rhs.session_payload)
        , payload(rhs.payload)
        , version(rhs.version)
        , im(rhs.im)
    {}

    Message::Message(Message&& rhs) 
        : session_id(std::move(rhs.session_id))
        , session_payload(std::move(rhs.session_payload))
        , payload(std::move(rhs.payload))
        , version(std::move(rhs.version))
        , im(std::move(rhs.im))
    {}

    void Message::acquire(Message& rhs)
    {
        session_id = std::move(rhs.session_id);
        session_payload = std::move(rhs.session_payload);
        payload = std::move(rhs.payload);
        version = std::move(rhs.version);
        im = std::move(rhs.im);
    }

    Message& Message::operator=(const Message& rhs)
    {
        session_id = rhs.session_id;
        session_payload = rhs.session_payload;
        payload = rhs.payload;
        version = rhs.version;
        im = rhs.im;

        return *this;
    }

    Message& Message::operator=(Message&& rhs) 
    {
        session_id = std::move(rhs.session_id);
        session_payload = std::move(rhs.session_payload);
        payload = std::move(rhs.payload);
        version = std::move(rhs.version);
        im = std::move(rhs.im);

        return *this;
    }

    MessageSerializer::MessageSerializer()
        : _ds(ds_idle)
        , _memo_index(0)
        , _memo_length(0)
    {}

    void MessageSerializer::reset()
    {
        _tmpmsg = std::move(Message());
        _ds = ds_idle;
        _memo_index = 0;
        _memo_length = 0;
    }

    MessageSerializer::digest_state_e MessageSerializer::digest(uint8_t byte)
    {
        switch (_ds)
        {
        case ds_bad:
            break;

        case ds_idle:
            _ds = ds_version; // continue to ds_version
        case ds_version:
            if (byte <= SUPPORT_VERSION)
            {
                _tmpmsg.version = byte;
                _ds = ds_im;
            }
            else
            {
                _ds = ds_bad;
            }
            break;

        case ds_im:
            _tmpmsg.im = (Message::interactive_method_e)byte;
            _memo_length = 16;      // 16 is the length of session id
            _memo_index = 0;
            _ds = ds_session_id;
            break;

        case ds_session_id:
            if (_memo_index < (uint32_t)(_memo_length - 1))
            {
                _tmpmsg.session_id[_memo_index] = byte;
                ++_memo_index;
            }
            else
            {
                _tmpmsg.session_id[_memo_index] = byte;
                _ds = ds_session_payload_len_hi;
            }
            break;

        case ds_session_payload_len_hi:
            _memo_length = byte << 8;
            _ds = ds_session_payload_len_lo;
            break;

        case ds_session_payload_len_lo:
            _memo_index = 0;
            _memo_length |= byte;

            if (_memo_length > 0)
            {
                _ds = ds_session_payload;
                _tmpmsg.session_payload.resize(_memo_length);
            }
            else
            {
                _tmpmsg.session_payload.clear();
                _ds = ds_payload_len_hi;
            }

            break;

        case ds_session_payload:
            if (_memo_index < (uint32_t)(_memo_length - 1))
            {
                _tmpmsg.session_payload[_memo_index] = byte;
                ++_memo_index;
            }
            else
            {
                _tmpmsg.session_payload[_memo_index] = byte;
                _ds = ds_payload_len_hi;
            }
            break;

        case ds_payload_len_hi:
            _memo_length = byte << 8;
            _ds = ds_payload_len_lo;
            break;

        case ds_payload_len_lo:
            _memo_length |= byte;
            _memo_index = 0;

            if (_memo_length > 0)
            {
                _ds = ds_payload;
                _tmpmsg.payload.resize(_memo_length);
            }
            else
            {
                _ds = ds_idle;
            }
            break;

        case ds_payload:
            if (_memo_index < (uint32_t)(_memo_length - 1))
            {
                _tmpmsg.payload[_memo_index] = byte;
                ++_memo_index;
            }
            else
            {
                _tmpmsg.payload[_memo_index] = byte;
                _ds = ds_idle;
            }
            break;

        default:
            _ds = ds_bad;
            break;
        }

        return _ds;
    }

    bool MessageSerializer::pull_message(Message& dest)
    {
        if (_ds == ds_idle)
        {
            dest.acquire(_tmpmsg);
            _tmpmsg = std::move(Message());
            return true;
        }
        else
        {
            return false;
        }
    }

    std::string MessageSerializer::serialize(const Message& msg)
    {
        std::string output;
        output.resize(18);

        // [1] version ----------------------------------
        output[0] = 0x01; // version = 1

        // [2] im ---------------------------------------
        output[1] = msg.im;

        // [2] session id -------------------------------
        const UUID::data_t sessid = msg.session_id.data();
        for (auto i = 0; i < sessid.size(); ++i)
        {
            output[i + 2] = sessid[i];
        }

        // [3] session payload --------------------------
        const uint16_t slen = (uint16_t)msg.session_payload.length();
        output.push_back((uint8_t)(slen >> 8));   // high length
        output.push_back((uint8_t)(slen & 0xff)); // lo length
        output.append(msg.session_payload);

        // [4] payload ----------------------------------
        const uint16_t mlen = (uint16_t)msg.payload.length();
        output.push_back((uint8_t)(mlen >> 8));    // high length
        output.push_back((uint8_t)(mlen & 0xff));  // lo length
        output.append(msg.payload);

        return output;
    }
} // namespace lotus::core
