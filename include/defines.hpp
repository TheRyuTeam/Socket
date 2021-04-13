#pragma once

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else


#endif


using port_t = unsigned short;
using socklen_t = int;
using domain_t = short;
using addr_t = unsigned long;