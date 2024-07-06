#ifndef COMMON_DEFS_HPP
#define COMMON_DEFS_HPP

#include "config.hpp"

#if BNIK_EXCEPTIONS_ENABLED
# define BNIK_THROW(Ty, Mssg) throw Ty{Mssg}
#else
# define BNIK_THROW(Ty, Mssg) std::cerr << Mssg << std::endl; \
    std::exit(-1)
#endif // BNIK_EXCEPTIONS_ENABLED


#endif // COMMON_DEFS_HPP
