#include"InetAddress.h"
#include"Endian.h"
#include"SocketsOps.h"
#include"base/Types.h"

#include<stddef.h>
#include<netdb.h>
#include<assert.h>


namespace study {

// 任意IP
static const in_addr_t kInaddrAny = INADDR_ANY;
// 环回IP
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

//     struct sockaddr_in6 {
//         sa_family_t     sin6_family;   /* address family: AF_INET6 */
//         uint16_t        sin6_port;     /* port in network byte order */
//         uint32_t        sin6_flowinfo; /* IPv6 flow information */
//         struct in6_addr sin6_addr;     /* IPv6 address */
//         uint32_t        sin6_scope_id; /* IPv6 scope-id */
//     };


InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6) {
    static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");
    static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");

    if(ipv6) {
        memZero(&addr6_, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        in6_addr ip =  loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr= ip;
        addr6_.sin6_port = sockets::hostToNetWork16(port);
    }
}

InetAddress::InetAddress(StringArg ip, uint16_t port, bool ipv6) {
    if(ipv6 || strchr(ip.c_str(), ':')) {
        memZero(&addr6_, sizeof(addr6_));
        sockets::fromIpPort(ip.c_str(), port, &addr6_);
    } else {
        memZero(&addr_, sizeof(addr_));
        sockets::fromIpPort(ip.c_str(), port, &addr_);
    }
}


std::string InetAddress::toIp() const {
    char buf[64] = "";
    sockets::toIp(buf, sizeof(buf), getSockAddr());
    return buf;
}

std::string InetAddress::toIpPort() const {
    char buf[64] = "";
    sockets::toIpPort(buf, sizeof(buf), getSockAddr());
    return buf;
}

uint16_t InetAddress::port() const {
    return sockets::networkToHost16(portNetEndian());
}

uint32_t InetAddress::ipv4NetEndian() const {
    assert(family() == AF_INET);
    return addr_.sin_addr.s_addr;
}

static __thread char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(StringArg hostname, InetAddress* result) {
    assert(result != NULL);
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    memZero(&hent, sizeof(hent));
    int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer,
                              sizeof(t_resolveBuffer), &he, &herrno);
    if(ret == 0 && he != NULL) {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        result->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    } else {
        if(ret) {
            // LOG_SYSERR ...
        }
        return false;
    }
}

void InetAddress::setScopedId(uint32_t scope_id) {


}

}