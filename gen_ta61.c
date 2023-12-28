// Copyright 2023, Midnight Blue.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sodium.h>
#include <unistd.h>
#include <sys/sysinfo.h>

// Imports from tetra implementation
#include "common.h"
#include "taa1.h"

int main(int argc, const char* argv[]) {

    if (argc != 4) {
        printf("[+] TA61 identity encryption/decryption\n");
        printf("    Usage: %s e cck ssi\n", argv[0]);
        printf("    Usage: %s d cck esi\n", argv[0]);
        printf("    TA61 intermediate secret is supported\n");
        printf("    Example vectors:\n");
        printf("      %s e 11111111111111111111 100\n", argv[0]);
        printf("      %s d 11111111111111111111 12845063\n", argv[0]);
        exit(1);
    } 


    char bOp;
    int dwArgsParsed = sscanf(argv[1], "%c", &bOp);
    if (dwArgsParsed != 1 || (bOp != 'e' && bOp != 'd')) {
        printf("[-] Can't parse operation type, should be e for encrypt or d for decrypt\n");
        exit(1);
    }

    uint8_t abCck[10];
    int dwKeyLen;
    if (strlen(argv[2]) == 20) {
        dwKeyLen = 10;
    } else if (strlen(argv[2]) == 16) {
        dwKeyLen = 8;
    } else {
        printf("[-] Unexpected cck length, should be 8 or 10 bytes\n");
        exit(1);
    }

    for (int i = 0; i < dwKeyLen; i++) {
        if (!sscanf(&argv[2][2*i], "%02hhX", &abCck[i])) {
            printf("[-] Can't parse key %d\n", i);
            exit(1);
        }
    }

    int dwSsi, dwSsiOut;
    dwArgsParsed = sscanf(argv[3], "%d", &dwSsi);
    if (dwArgsParsed != 1 || (dwSsi > 0xFFFFFF)) {
        printf("[-] Can't parse SSI\n");
        exit(1);
    }

    uint8_t abSsi[3];
    abSsi[0] = (dwSsi >> 16) & 0xFF;
    abSsi[1] = (dwSsi >> 8) & 0xFF;
    abSsi[2] = (dwSsi) & 0xFF;

    printf("TA61 ");

    uint8_t abInt[8];
    if (bOp == 'e') {
        if (dwKeyLen == 10) {
            ta61_compute_c(abCck, abInt);
            ta61_inner(abInt, abSsi, abSsi);
            printf("intermediate: %02X%02X%02X%02X%02X%02X%02X%02X: ", 
                abInt[0], abInt[1], abInt[2], abInt[3], abInt[4], abInt[5], abInt[6], abInt[7]);
        } else {
            ta61_inner(abCck, abSsi, abSsi);
        }
    } else {
        if (dwKeyLen == 10) {
            ta61_compute_c(abCck, abInt);
            ta61_inner_inv(abInt, abSsi, abSsi);
            printf("intermediate: %02X%02X%02X%02X%02X%02X%02X%02X: ", 
                abInt[0], abInt[1], abInt[2], abInt[3], abInt[4], abInt[5], abInt[6], abInt[7]);

        } else {
            ta61_inner_inv(abCck, abSsi, abSsi);
        }
    }

    dwSsiOut = (abSsi[0] << 16) | (abSsi[1] << 8) | abSsi[2];
    printf("%d -> %d\n", dwSsi, dwSsiOut);

    return 0;
}
