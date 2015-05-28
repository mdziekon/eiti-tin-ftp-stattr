#ifndef TIN_UTILS_MAGICMEMBER_HPP
#define TIN_UTILS_MAGICMEMBER_HPP

namespace tin { namespace utils
{
    template<typename T>
    class MagicMember
    {
    public:
        MagicMember() = default;
        MagicMember(const MagicMember& rhs) = default;
        MagicMember(const T& rhs)
        {
            this->setter(value);
        }

        T& operator()()
        {
            return this->getter();
        }

        void operator()(const T& value)
        {
            this->setter(value);
        }

        MagicMember& operator=(const T& rhs)
        {
            this->setter(rhs);
            return *this;
        }

        MagicMember& operator=(const MagicMember& rhs)
        {
            this->setter(rhs());
            return *this;
        }

        operator T()
        {
            return getter();
        }

        bool operator==(const T& rhs) const
        {
            return this->value == rhs;
        }
        bool operator!=(const T& rhs) const
        {
            return !(*this == rhs);
        }

    private:
        T value;

        virtual T& getter()
        {
            return this->value;
        }

        virtual void setter(const T& val)
        {
            this->value = val;
        }

        // template <typename TT>
        // friend std::ostream & operator<<(std::ostream &os, const MagicMember<TT>& p);

        // template <typename TT>
        // friend std::istream & operator>>(std::istream &is, const MagicMember<TT>& p);
    };

    // template <typename T>
    // std::ostream & operator<<(std::ostream &os, const LambdaSetter<T>& p)
    // {
    //     os << p.getter();
    //     return os;
    // }

    // template <typename TT>
    // std::istream & operator>>(std::istream &is, const LambdaSetter<TT>& p)
    // {
    //     TT value;
    //     is >> value;
    //     p.setter(value);
    //     return is;
    // }
}}

#endif
