#ifndef LOTUS_ATTACHABLE_H
#define LOTUS_ATTACHABLE_H

#include <any>
#include <map>
#include <memory>
#include <optional>

namespace lotus::core
{
    class Attachable
    {
    public:
        void attach(
            int index,
            std::unique_ptr<std::any>&& attachment) noexcept;
        std::optional<std::any*> attachment(int index) noexcept;
        void detach(int index) noexcept;
        void clear() noexcept;

    public:
        template<typename ValType>
        constexpr std::optional<ValType*> att(int index) noexcept
        {
            const auto& att = attachment(index);
            if (att.has_value())
            {
                ValType& ref = std::any_cast<ValType&>(*(att.value()));
                return std::make_optional(&ref);
            }
            else
            {
                return std::optional<ValType*>();
            }
        }

    private:
        std::map<int, std::unique_ptr<std::any>> _attachments;
    };
}

#endif // !LOTUS_ATTACHABLE_H