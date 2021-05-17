#include "attachable.hxx"

namespace lotus::core
{
    void Attachable::attach(
        int index,
        std::unique_ptr<std::any>&& attachment) noexcept
    {
        _attachments[index] = std::move(attachment);
    }

    std::optional<std::any*> Attachable::attachment(
        int index) noexcept
    {
        const auto& found = _attachments.find(index);
        if (found != _attachments.cend())
        { // found
            return std::make_optional(found->second.get());
        }
        else
        {
            return std::optional<std::any*>();
        }
    }

    void Attachable::detach(int index) noexcept
    {
        _attachments.erase(index);
    }

    void Attachable::clear() noexcept
    {
        _attachments.clear();
    }
}


