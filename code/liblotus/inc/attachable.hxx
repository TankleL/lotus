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
        std::optional<IAttachment&> attachment(int index) noexcept;
        void detach(int index) noexcept;
        void clear() noexcept;

    public:
        template<typename ValType>
        constexpr std::optional<ValType&> att(int index) noexcept
        {
            const auto& att = attachment(index);
            if (att.has_value())
            {
                return std::make_optional<ValType&>(att.value());
            }
            else
            {
                return std::optional<ValType&>();
            }
        }

    private:
        std::map<int, std::unique_ptr<std::any>> _attachments;
    };
}

#endif // !LOTUS_ATTACHABLE_H