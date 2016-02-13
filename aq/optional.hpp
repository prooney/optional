#pragma once

#include "optional/bad_optional_access.hpp"
#include "aligned_mem.hpp"

// this wont provide a rock solid guarantee that whatever c++11 feature used is supported
// but is good enough at this moment. Ok on clang & gcc
#define CPP11_SUPPORT  __cplusplus > 199711L

/*
// todo: exception specification
#ifdef CPP11_SUPPORT
#define NOEXCEPT  noexcept
#elif
#define NOEXCEPT  throw()
#endif
*/

namespace aq
{
    template<typename T>
    class optional
    {
        char scratch_[sizeof(T)]    ALIGNED8_;
        bool valid_;

        // get storage space as void pointer
        void* storage() { return scratch_; }
        const void* storage() const { return scratch_; }

        // get storage cast as a T
        T& get() { return *static_cast<T*>(storage()); }
        T const& get() const { return *static_cast<const T*>(storage()); }

        void throw_bad_access() const
        {
            throw bad_optional_access("access invalid optional");
        }

        T& get_or_throw()
        {
            if(!valid_) throw_bad_access();

            return get();
        }

        T const& get_or_throw() const
        {
            if(!valid_) throw_bad_access();

            return get();
        }

        void construct(T const& data)
        {
            new (storage()) T(data);
        }

        void destruct()
        {
            if(valid_)
            {
                get().~T();
                valid_ = false;
            }
        }

        friend void swap(optional<T>& first, optional<T>& second)
        {
            using std::swap;

            swap(first.valid_, second.valid_);
            swap(first.get(), second.get());  // must invoke T's copy constructor & assignment operator
        }

        public:

        optional()
            :valid_(false)
        {}

        explicit optional(T const& t)
            : valid_(false)
        {
            construct(t);
            valid_ = true;
        }

        ~optional()
        {
            destruct();
        }

        optional(optional<T> const& other)
            : valid_(false)
        {
            if(other.valid_)
            {
                construct(other.get());
                valid_ = true;
            }
        }

        optional& operator=(optional<T> other)
        {
            swap(*this, other);

            return *this;
        }

        optional& operator=(T const& val)
        {
            // wondering if object can be constructed directly
            optional<T> tmp(val);
            swap(*this, tmp);

            return *this;
        }


#if CPP11_SUPPORT

        // todo: move constructors/assignment operators

        explicit operator bool() const
        {
            return valid_;
        }

#else
        // used for bool test in conditional
        // e.g. if(x) { // blah }
        operator void*() const
        {
            return valid_ ? reinterpret_cast<void*>(1) : 0;
        }
#endif

        bool operator!() const
        {
            return (!valid_);
        }

        T& operator*()
        {
            return get_or_throw();
        }

        T const& operator*() const
        {
            return get_or_throw();
        }

        T* operator->()
        {
            return &get_or_throw();
        }

        T const* operator->() const
        {
            return &get_or_throw();
        }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, optional<T> const& opt)
    {
        os << *opt;
        return os;
    }

}