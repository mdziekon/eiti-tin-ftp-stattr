#include <utility>

template<typename T>
unsigned int tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::insert(std::function<T>& handler)
{
    this->handlers.insert(std::make_pair(this->nextHandlerID, handler));

    return this->nextHandlerID++;
}

template<typename T>
unsigned int tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::insert(std::function<T>&& handler)
{
    this->handlers.insert(std::make_pair(this->nextHandlerID, std::move(handler)));

    return this->nextHandlerID++;
}

template<typename T>
void tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::erase(unsigned int index)
{
    this->handlers.erase(index);
}

template<typename T>
void tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::clear()
{
    this->handlers.clear();
}

template<typename T>
const std::function<T>& tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::at(unsigned int index) const
{
    return this->handlers.at(index);
}

template<typename T>
const std::function<T>& tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::operator[](unsigned int index) const
{
    return this->handlers[index];
}

template<typename T>
typename tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::size_type tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::size() const
{
    return this->handlers.size();
}

template<typename T>
bool tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::empty() const
{
    return this->handlers.empty();
}

template<typename T>
typename tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::const_iterator tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::begin() const
{
    return this->handlers.cbegin();
}

template<typename T>
typename tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::const_iterator tin::network::bsdsocket::wrapper::utils::HandlersContainer<T>::end() const
{
    return this->handlers.cend();
}
