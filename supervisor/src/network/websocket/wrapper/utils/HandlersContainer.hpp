#ifndef TIN_NETWORK_WEBSOCKET_WRAPPER_UTILS_HANDLERSCONTAINER_HPP
#define	TIN_NETWORK_WEBSOCKET_WRAPPER_UTILS_HANDLERSCONTAINER_HPP

#include <unordered_map>
#include <functional>

namespace tin { namespace network { namespace websocket { namespace wrapper { namespace utils
{
    template <typename T>
    class HandlersContainer
    {
        typedef typename std::unordered_map<unsigned int, const std::function<T>> unordered_map;
        typedef typename std::unordered_map<unsigned int, const std::function<T>>::const_iterator const_iterator;
        typedef typename std::unordered_map<unsigned int, const std::function<T>>::size_type size_type;

    private:
        unordered_map handlers;

        unsigned int nextHandlerID = 0;
    public:
        unsigned int insert(std::function<T>& handler);
        unsigned int insert(std::function<T>&& handler);

        void erase(unsigned int index);
        void clear();

        const std::function<T>& at(unsigned int index) const;
        const std::function<T>& operator[](unsigned int index) const;

        size_type size() const;
        bool empty() const;

        const_iterator begin() const;
        const_iterator end() const;
    };
}}}}}

#include "HandlersContainer.impl.hpp"

#endif	/* TIN_NETWORK_WEBSOCKET_WRAPPER_UTILS_HANDLERSCONTAINER_HPP */

