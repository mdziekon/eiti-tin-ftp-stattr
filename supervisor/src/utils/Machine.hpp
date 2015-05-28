#ifndef TIN_UTILS_MACHINE_HPP
#define TIN_UTILS_MACHINE_HPP
#include <string>

#include "MagicMember.hpp"
#include "ObserverPattern.hpp"
#include "JSON.hpp"

#include <iostream>

namespace tin { namespace utils
{
    struct Machine;

    template <typename T>
    class MachineMagicMember: public MagicMember<T>
    {
        friend struct Machine;

    public:
        MachineMagicMember() = default;
        MachineMagicMember(const MachineMagicMember& rhs) = default;
        MachineMagicMember(const T& rhs): MagicMember<T>(rhs) {}

        MachineMagicMember& operator=(const T& rhs)
        {
            MagicMember<T>::operator=(rhs);
            return *this;
        }

        MachineMagicMember& operator=(const MagicMember<T>& rhs)
        {
            MagicMember<T>::operator=(rhs);
            return *this;
        }
    private:
        virtual void setter(const T& val)
        {
            this->value = val;
            if (this->machine != nullptr)
            {
                std::cout << "notify observers" << std::endl;
                this->machine->notifyObservers();
            }
            else
            {
                std::cout << "setter nullptr" << std::endl;
            }
        }

        Machine* machine = nullptr;
    };

    struct Machine: public tin::utils::Observable
    {
        Machine();
        Machine(const nlohmann::json& jsonObj);
    	Machine(std::string ip, std::string name, unsigned int port);
    	Machine(const int &id, const std::string &ip, const unsigned int &port);
        int incCounter();
        void updateMagicMembers();
        nlohmann::json    serialize();

    	tin::utils::MachineMagicMember<int> id;
        tin::utils::MachineMagicMember<std::string> name;
    	tin::utils::MachineMagicMember<std::string> ip;
        tin::utils::MachineMagicMember<unsigned int> port;
        tin::utils::MachineMagicMember<std::string> filterDevice = std::string("lo");
        tin::utils::MachineMagicMember<std::string> filterExpression = std::string("");
        tin::utils::MachineMagicMember<std::string> status = std::string("offline");
        tin::utils::MachineMagicMember<int> lastSynchronization = 0;
    };
}}


#endif	/* TIN_UTILS_MACHINE_HPP */

