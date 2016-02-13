#pragma once

#include <stdexcept>

namespace aq
{
    class bad_optional_access : public std::logic_error
    {
        public:
        bad_optional_access(const char* msg)
            : std::logic_error(msg)
        {}
    };

}