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
        virtual ~IAttachment()  {}
    };

    class Attachable
    {
    public:
        void attach(
            int index,
            std::unique_ptr<IAttachment>&& attachment) ;
        IAttachment* attachment(int index) ;
        void detach(int index) ;
        void clear() ;

    public:
        template<typename ValType>
        constexpr ValType* attachment(int index) 
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