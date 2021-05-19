#ifndef LOTUS_ATTACHABLE_H
#define LOTUS_ATTACHABLE_H

#include <any>
#include <map>
#include <memory>
#include <optional>

namespace lotus::core
{
    class IAttachment
    {
    public:
        virtual ~IAttachment() noexcept {}
    };

    class Attachable
    {
    public:
        void attach(
            int index,
            std::unique_ptr<IAttachment>&& attachment) noexcept;
        IAttachment* attachment(int index) noexcept;
        void detach(int index) noexcept;
        void clear() noexcept;

    public:
        template<typename ValType>
        constexpr ValType* attachment(int index) noexcept
        {
            IAttachment* att = attachment(index);
            if (att != nullptr)
            {
                return static_cast<ValType*>(att);
            }
            else
            {
                return nullptr;
            }
        }

    private:
        std::map<int, std::unique_ptr<IAttachment>> _attachments;
    };
}

#endif // !LOTUS_ATTACHABLE_H