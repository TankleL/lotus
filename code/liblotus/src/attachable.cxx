#include "attachable.hxx"

namespace lotus::core
{
    void Attachable::attach(
        int index,
        std::unique_ptr<IAttachment>&& attachment) 
    {
        //_attachments[index] = std::move(attachment);
        _attachments.emplace(std::make_pair(index, std::move(attachment)));
    }

    IAttachment* Attachable::attachment(int index) 
    {
        const auto& found = _attachments.find(index);
        if (found != _attachments.cend())
        { // found
            return found->second.get();
        }
        else
        { // not found
            return nullptr;
        }
    }

    void Attachable::detach(int index) 
    {
        _attachments.erase(index);
    }

    void Attachable::clear() 
    {
        _attachments.clear();
    }
}


