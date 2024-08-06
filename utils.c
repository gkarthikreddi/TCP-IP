#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int binary_str_to_int(char *binaryStr, int bit_size) {
    int result = 0;

    for (int i = 0; i < bit_size; i++) {
        result <<= 1;
        if (binaryStr[i] == '1') result += 1;
    }

    return result;
}

char* int_to_binary_str(int num, int bit_size) {
    char *bin = malloc((bit_size+1)*sizeof(char));

    for (int i = bit_size - 1; i >= 0; i--) {
        bin[bit_size - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
    bin[bit_size] = '\0'; // Null-terminate the string

    return bin;
}

uint32_t convert_ip_from_str_to_int(char *ip_addr) {
    char *arr[4];
    char *bin = (char *)malloc(32 + 1);

    int i = 0, idx = 0;
    while (1) {
        if (*(ip_addr+i) == '.' || *(ip_addr+i) == '\0') {
            arr[idx] = malloc(i*sizeof(char));
            strncpy(arr[idx], ip_addr, i);

            strcat(bin, int_to_binary_str(atoi(arr[idx++]), 8));

            if (*(ip_addr+i) == '\0') break;

            ip_addr += i + 1;
            i = -1;
        }
        i++;
    }

    return binary_str_to_int(bin, 32); 
}


char *convert_ip_from_int_to_str(int binary_prefix) {
    char *bin = (char *)malloc(32 + 1);
    char *ans = (char *)malloc(16 + 1);

    bin = int_to_binary_str(binary_prefix, 32);
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

char* apply_mask(char *prefix, char mask) {
    if(mask == 32) return prefix;

    uint32_t binary_prefix = 0;
    uint32_t subnet_mask = 0xFFFFFFFF;

    /*Convert Given IP address into binary format*/
    binary_prefix = convert_ip_from_str_to_int(prefix);

    /*Compute Mask in binary format as well*/
    subnet_mask = subnet_mask << (32 - mask);

    /*Perform logical AND to apply mask on IP address*/
    binary_prefix = binary_prefix & subnet_mask;
    
    return convert_ip_from_int_to_str(binary_prefix);
}

void layer2_fill_with_broadcast_mac(char *mac_array){
    mac_array[0] = 0xFF;
    mac_array[1] = 0xFF;
    mac_array[2] = 0xFF;
    mac_array[3] = 0xFF;
    mac_array[4] = 0xFF;
    mac_array[5] = 0xFF;
}
