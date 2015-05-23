#ifndef TIN_UTILS_EVENT_HPP
#define TIN_UTILS_EVENT_HPP

namespace tin { namespace utils
{
    template <typename Visitor>
    struct Event
    {
        virtual ~Event() {};
        virtual void accept(Visitor& visitor) = 0;
    };
}}

#endif	/* TIN_UTILS_EVENT_HPP */

