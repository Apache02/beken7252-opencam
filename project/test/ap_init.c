#include <rtthread.h>
//#include <rtdevice.h>
#include <stdio.h>
#include <string.h>

#include <lwip/ip.h>
#include <lwip/inet.h>
#include <lwip/netif.h>


static void usage(void) {
    rt_kprintf("usage: ap_init 192.168.11.1 \n");
}

static void print_netif(struct netif *iface, int index) {
    char index_buf[16];
    if (index == -1) {
        index_buf[0] = '_';
        index_buf[1] = '\0';
    } else {
        sprintf(index_buf, "%d", index);
    }

    rt_kprintf(
            "%s. %s #%d : %s | mask: %s | gw: %s \n",
            index_buf,
            iface->name,
            iface->num,
            inet_ntoa(iface->ip_addr),
            inet_ntoa(iface->netmask),
            inet_ntoa(iface->gw)
    );
}

int ap_init(int argc, char **argv) {

    if (argc != 2) {
        usage();
        return RT_EOK;
    }

    if (strcmp(argv[1], "print") == 0) {
        if (netif_default != RT_NULL) {
            print_netif(netif_default, -1);
        }

        struct netif *netif = netif_list;

        for (int i = 0; netif != RT_NULL; i++) {
            print_netif(netif, i);
            netif = netif->next;
        }

        return RT_EOK;
    }

    ip_addr_t ip_addr;
    ip_addr_t netmask;

    if (inet_aton(argv[1], &ip_addr) == 0) {
        rt_kprintf("Error: Invalid IP\n");
        return RT_EOK;
    }

    rt_kprintf("IP: %s\n", inet_ntoa(ip_addr));

    struct netif *netif = netif_find("ap1");
    if (netif == RT_NULL) {
        rt_kprintf("Error: netif ap not found\n");
        return RT_EOK;
    }

    netif_set_down(netif);

    print_netif(netif, 0);
    inet_aton("255.255.255.0", &netmask);
    netif->ip_addr = ip_addr;
    netif->gw = ip_addr;
    netif->netmask = netmask;
    print_netif(netif, 0);

    return RT_EOK;
}

MSH_CMD_EXPORT(ap_init, ap init test);
