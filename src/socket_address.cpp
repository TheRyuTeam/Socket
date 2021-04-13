#include <stdexcept>
#include "../include/socket_address.hpp"


socket_address::socket_address(const sockaddr* addr, socklen_t size)
    : socket_address(*(sockaddr_storage*)addr, size)
{}

socket_address::socket_address(sockaddr_storage addr, socklen_t size)
{
    if (size > MAX_SIZE) {
        throw std::length_error("");
    }
    m_size = size;
    memcpy(&m_storage, &addr, m_size);
}

socket_address::socket_address(domain_t domain, const char* addr, port_t port)
{
    auto ptr = (sockaddr_in*)&m_storage;
    ptr->sin_family             = domain;
    ptr->sin_addr.S_un.S_addr   = inet_addr(addr);
    ptr->sin_port               = htons(port);
    m_size = sizeof(m_storage);
}

socket_address::socket_address(socket_address&& addr) noexcept
    : m_size{ addr.m_size }, m_storage{ addr.m_storage }
{
    ZeroMemory(&addr.m_storage, addr.m_size);
    addr.m_size = 0;
}

socket_address& socket_address::operator=(socket_address&& addr) noexcept
{
    if (this != &addr) {
        m_storage       = addr.m_storage;
        m_size          = addr.m_size;
        ZeroMemory(&addr.m_storage, m_size);
        addr.m_size     = 0;
    }
    return *this;
}

void socket_address::clear()
{
    ZeroMemory(&m_storage, m_size);
    m_size = 0;
}

bool operator==(const socket_address& lhs, const socket_address& rhs)
{
    return lhs.m_size == rhs.m_size && memcmp(&lhs.m_storage, &rhs.m_storage, rhs.m_size) == 0;
}

bool operator!=(const socket_address& lhs, const socket_address& rhs)
{
    return !(lhs == rhs);
}