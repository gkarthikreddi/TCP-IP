#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

char* int_to_8bit_binary(int num) {
    char *bin = malloc(9*sizeof(char));
    for (int i = 7; i >= 0; i--) {
        bin[7-i] = (num & (1 << i)) ? '1' : '0';
    }
    bin[8] = '\0';
    return bin;
}

int binary_str_to_int(char *binaryStr, int bit_size) {
    int result = 0;

    for (int i = 0; i < bit_size; i++) {
        result <<= 1;
        if (binaryStr[i] == '1') result += 1;
    }

    return result;
}

uint32_t convert_ip_from_str_to_int(char *ip_addr) {
    char *arr[4];
    char *bin = (char *)malloc(32 + 1);
    int i = 0, idx = 0;
    while (1) {
        if (*(ip_addr+i) == '.' || *(ip_addr+i) == '\0') {
            arr[idx] = malloc(i*sizeof(char));
            strncpy(arr[idx], ip_addr, i);
            strcat(bin, int_to_8bit_binary(atoi(arr[idx++])));
            if (*(ip_addr+i) == '\0') break;
            ip_addr = ip_addr + i + 1;
            i = -1;
        }
        i++;
    }
    return binary_str_to_int(bin, 32); 
}

void int_to_binary(int number, char *binaryStr, int bitSize) {
    for (int i = bitSize - 1; i >= 0; i--) {
        binaryStr[bitSize - 1 - i] = (number & (1 << i)) ? '1' : '0';
    }
    binaryStr[bitSize] = '\0'; // Null-terminate the string
}

char *convert_ip_from_int_to_str(int binary_prefix) {
    char *bin = (char *)malloc(32 + 1);
    char *ans = (char *)malloc(16 + 1);

    int_to_binary(binary_prefix, bin, 32);
    while (*bin != '\0') {
        char *tmp = (char *)malloc(8);
        char *num = (char *)malloc(4);
        strncpy(tmp, bin, 8);
        sprintf(num, "%d", binary_str_to_int(tmp, 8));
        bin += 8;
        strcat(ans, num);
        if (!(*bin == '\0')) strcat(ans, ".");
    }
    return ans;
}

void apply_mask(char *prefix, char mask, char *str_prefix) {
    if(mask == 32){
        strncpy(str_prefix, prefix, 16);
        str_prefix[15] = '\0';
        return;
    }

    uint32_t binary_prefix = 0;
    uint32_t subnet_mask = 0xFFFFFFFF;

    /*Convert Given IP address into binary format*/
    binary_prefix = convert_ip_from_str_to_int(prefix);

    /*Compute Mask in binary format as well*/
    subnet_mask = subnet_mask << (32 - mask);

    /*Perform logical AND to apply mask on IP address*/
    binary_prefix = binary_prefix & subnet_mask;

    strncpy(str_prefix, convert_ip_from_int_to_str(binary_prefix), 16);
}

void layer2_fill_with_broadcast_mac(char *mac_array){
    mac_array[0] = 0xFF;
    mac_array[1] = 0xFF;
    mac_array[2] = 0xFF;
    mac_array[3] = 0xFF;
    mac_array[4] = 0xFF;
    mac_array[5] = 0xFF;
}
