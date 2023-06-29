#pragma once

#include <net/if.h>
#include <netinet/in.h>
#define WG_KEY_SIZE 32

/* Name of this module for kernel control */
#define WG_CONTROL_NAME "com.wireguard.kext.tun"

#define SIOCSWG _IOW('i', 250, struct wg_data_io)
#define SIOCGWG _IOWR('i', 251, struct wg_data_io)

/* retrieve interface name */
#define WG_OPT_IFNAME 1

#define a_ipv4 a_addr.addr_ipv4
#define a_ipv6 a_addr.addr_ipv6

struct wg_aip_io {
    sa_family_t a_af;
    int a_cidr;
    union wg_aip_addr {
        struct in_addr addr_ipv4;
        struct in6_addr addr_ipv6;
    } a_addr;
};

/* flags for p_flags */
#define WG_PEER_HAS_PUBLIC (1 << 0)
#define WG_PEER_HAS_PSK (1 << 1)
#define WG_PEER_HAS_PKA (1 << 2)
#define WG_PEER_HAS_ENDPOINT (1 << 3)
#define WG_PEER_REPLACE_AIPS (1 << 4)
#define WG_PEER_REMOVE (1 << 5)
#define WG_PEER_UPDATE (1 << 6)

#define p_sa p_endpoint.sa_sa
#define p_sin p_endpoint.sa_sin
#define p_sin6 p_endpoint.sa_sin6

struct wg_peer_io {
    int p_flags;
    int p_protocol_version;
    uint8_t p_public[WG_KEY_SIZE];
    uint8_t p_psk[WG_KEY_SIZE];
    uint16_t p_pka;
    union wg_peer_endpoint {
        struct sockaddr sa_sa;
        struct sockaddr_in sa_sin;
        struct sockaddr_in6 sa_sin6;
    } p_endpoint;
    uint64_t p_txbytes;
    uint64_t p_rxbytes;
    struct timespec p_last_handshake;
    size_t p_aips_count;
    struct wg_aip_io p_aips[];
};

/* flags for i_flags */
#define WG_INTERFACE_HAS_PUBLIC (1 << 0)
#define WG_INTERFACE_HAS_PRIVATE (1 << 1)
#define WG_INTERFACE_HAS_PORT (1 << 2)
#define WG_INTERFACE_REPLACE_PEERS (1 << 3)

struct wg_interface_io {
    int i_flags;
    in_port_t i_port;
    uint8_t i_public[WG_KEY_SIZE];
    uint8_t i_private[WG_KEY_SIZE];
    size_t i_peers_count;
    struct wg_peer_io i_peers[];
};

struct wg_data_io {
    char wgd_name[IFNAMSIZ];
    /* total size of the memory pointed to by wgd_interface */
    size_t wgd_size;
    struct wg_interface_io* wgd_interface;
};
