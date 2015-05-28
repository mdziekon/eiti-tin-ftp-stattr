#ifndef TIN_UTILS_OBSERVERPATTERN_HPP
#define TIN_UTILS_OBSERVERPATTERN_HPP

#include <list>

namespace tin { namespace utils
{
    class Observer
    {
    public:
        virtual void update() = 0;
        virtual ~Observer() = default;
    };

    class Observable
    {
    public:
        void addObserver(Observer* o)
        {
            this->observers.push_back(o);
        }
        void removeObserver(Observer* o)
        {
            for(auto it = this->observers.begin(); it != this->observers.end(); ++it)
            {
                if ((*it) != o)
                {
                    return;
                }
                this->observers.erase(it);
                break;
            }
        }
        void notifyObservers()
        {
            for(auto& observer: this->observers)
            {
                observer->update();
            }
        }
    private:
        std::list<Observer*> observers;
    };
}}

#endif
