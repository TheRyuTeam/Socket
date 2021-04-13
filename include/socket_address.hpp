#pragma once
#include "defines.hpp"

class socket_address
{
private:
    using storage_t = sockaddr_storage;
    storage_t m_storage { };
    socklen_t m_size    { MAX_SIZE };

public:
    static constexpr socklen_t MAX_SIZE { sizeof(sockaddr_storage) };

    socket_address() = default;
    socket_address(const sockaddr*, socklen_t);
    socket_address(storage_t, socklen_t);
    socket_address(domain_t, const char*, port_t);
    socket_address(const socket_address&) = default;
    socket_address(socket_address&&) noexcept;

    socket_address& operator=(const socket_address&) = default;
    socket_address& operator=(socket_address&&) noexcept;

    sockaddr* sockaddr_ptr() { return (sockaddr*)&m_storage; }
    const sockaddr* sockaddr_ptr() const { return (sockaddr*)&m_storage; }

    operator const sockaddr*() const { return sockaddr_ptr(); }
    operator sockaddr*() { return sockaddr_ptr(); }

    domain_t domain() const { return m_storage.ss_family; }
    domain_t domain()       { return m_storage.ss_family; }
    in_addr sin_addr()const { return ((sockaddr_in*)&m_storage)->sin_addr; }
    in_addr sin_addr()      { return ((sockaddr_in*)&m_storage)->sin_addr; }
    addr_t addr()     const { return ((sockaddr_in*)&m_storage)->sin_addr.S_un.S_addr; }
    addr_t addr()           { return ((sockaddr_in*)&m_storage)->sin_addr.S_un.S_addr; }
    port_t port()     const { return ntohs(((sockaddr_in*)&m_storage)->sin_port); }
    port_t port()           { return ntohs(((sockaddr_in*)&m_storage)->sin_port); }
    socklen_t size()  const { return m_size; }
    socklen_t size()        { return m_size; }

    friend bool operator==(const socket_address&, const socket_address&);
    friend bool operator!=(const socket_address&, const socket_address&);

    void clear();
};

enum class DOMAIN {
    IPV4 = AF_INET
};

template<DOMAIN Domain>
class sock_address : public socket_address
{
private:
    using base = socket_address;
public:
    sock_address(const char* addr, port_t port)
        : base(static_cast<domain_t>(Domain), addr, port)
    {}
    sock_address(const sock_address<Domain>&) = default;
    sock_address(sock_address<Domain>&& addr) noexcept
        : base(std::move(addr))
    {}

    sock_address& operator=(const sock_address<Domain>&) = default;
    sock_address& operator=(sock_address<Domain>& addr) noexcept
    {
        base::operator=(addr);
        return *this;
    }

};

namespace ipv4 {
    using address = sock_address<DOMAIN::IPV4>;
}