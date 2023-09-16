#ifndef COMMON
    #include "../_common.hpp"
#endif
#include "client/client.hpp"

#ifdef __linux__
    #include "socket_linux.hpp"
#else
    #include "socket_windows.hpp"
#endif