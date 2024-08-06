#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>

#define IS_MAC_BROADCAST_ADDR(mac)                              \
    (mac[0] == 0xFF  &&  mac[1] == 0xFF && mac[2] == 0xFF &&    \
     mac[3] == 0xFF  &&  mac[4] == 0xFF && mac[5] == 0xFF)

char* apply_mask(char *prefix, char mask);
void layer2_fill_with_broadcast_mac(char *mac_array);
char* convert_ip_from_int_to_str(int binary_prefix);
uint32_t convert_ip_from_str_to_int(char *ip_addr);
#endif
