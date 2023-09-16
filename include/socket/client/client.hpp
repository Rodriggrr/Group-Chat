#ifndef COMMON
    #include "../_common.hpp"
#endif

#ifdef __linux__
    #include "client_linux.hpp"
#else
    #include "client_windows.hpp"
#endif