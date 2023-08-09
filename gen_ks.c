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
#include "tea1.h"
#include "tea2.h"
#include "tea3.h"

#define KS_LEN 54

int main(int argc, const char* argv[]) {

    if (argc != 3 && argc < 7) {
        printf("[+] TEA keystream generation\n");
        printf("    Usage: %s tea_type  hn mn fn sn dir  eck\n", argv[0]);
        printf("    Usage: %s tea_type  hn mn fn sn dir  ck  cn la cc\n", argv[0]);
        printf("    TEA1 reduced key is supported\n");
        printf("    direction 0 = downlink, 1 = uplink\n");
        printf("    Example vector:\n");
        printf("      %s 2  110 30 06 1  0  11111111111111111111  1001 2 1\n", argv[0]);
        printf("      %s 1  110 30 06 1  0  11111111\n", argv[0]);
        exit(1);
    } 

    int dwTeaType;
    sscanf(argv[1], "%d", &dwTeaType);
    if (dwTeaType < 1 || dwTeaType > 3) {
        printf("[-] TEA type not supported\n");
        exit(1);
    }

    FrameNumbers f;
    if (    !sscanf(argv[2], "%hd", &f.hn) ||
            !sscanf(argv[3], "%hhd", &f.mn) ||
            !sscanf(argv[4], "%hhd", &f.fn) ||
            !sscanf(argv[5], "%hhd", &f.tn) ||
            !sscanf(argv[6], "%hhd", &f.dir)) {
        printf("[-] Can't parse hn/mn/fn/sn\n");
        exit(1);
    }

    uint8_t abCk[10];
    int dwCn, dwLa, dwCc;
    bool bTea1Reduced = false;
    memset(abCk, 0, sizeof(abCk));
    

    if (dwTeaType == 1 && strlen(argv[7]) == 8) {
        bTea1Reduced = true;
    } else if (strlen(argv[7]) != 20) {
        printf("[-] Invalid length key\n");
        exit(1);
    }
    
    int dwKeyLen = strlen(argv[7]) / 2;
    for (int i = 0; i < dwKeyLen; i++) {
        if (!sscanf(&argv[7][2*i], "%02hhX", &abCk[i])) {
            printf("[-] Can't parse key %d\n", i);
            exit(1);
        }
    }

    if (!bTea1Reduced) {
        if (    argc != 11 || 
                !sscanf(argv[8], "%d", &dwCn) ||
                !sscanf(argv[9], "%d", &dwLa) ||
                !sscanf(argv[10], "%d", &dwCc)) {
            printf("[-] Can't parse cn/la/cc\n");
            exit(1);
        }
    }

    uint8_t abKs[KS_LEN];
    uint32_t dwIv = build_iv(&f); 

    if (bTea1Reduced) {
        // Special case for reduced TEA1 key
        printf("TEA1_reduced hn %d mn %d fn %d tn %d eck %02X%02X%02X%02X\n",
            f.hn, f.mn, f.fn, f.tn, abCk[0],abCk[1],abCk[2],abCk[3]);
        uint64_t expIv = tea1_expand_iv(dwIv);
        uint32_t dwKeyReg = abCk[0] << 24 | abCk[1] << 16 | abCk[2] << 8 | abCk[3];
        tea1_inner(expIv, dwKeyReg, KS_LEN, abKs);
    } else {

        uint8_t abEck[10];
        uint8_t abCn[2] = {(dwCn >> 8) & 0xFF, dwCn & 0xFF};
        uint8_t abLa[2] = {(dwLa >> 8) & 0xFF, dwLa & 0xFF};
        uint8_t abCc[1] = {dwCc & 0xFF};
        tb5(abCn, abLa, abCc, abCk, abEck);

        printf("TEA%d hn %d mn %d fn %d tn %d ", dwTeaType, f.hn, f.mn, f.fn, f.tn);
        printf("     cn %d la %d cc %d ck ", dwCn, dwLa, dwCc);
        for (int i = 0; i < 10; i++) {
            printf("%02X", abCk[i]);
        }
        printf("\n     eck ");
        for (int i = 0; i < 10; i++) {
            printf("%02X", abEck[i]);
        }
        printf("     iv 0x%08X\n     ks ", dwIv);
        

        switch (dwTeaType) {
        case 1:
            tea1(dwIv, abEck, KS_LEN, abKs);
            break;

        case 2:
            tea2(dwIv, abEck, KS_LEN, abKs);
            break;

        case 3:
            tea3(dwIv, abEck, KS_LEN, abKs);
            break;
        }
    }

    for (int i = 0; i < KS_LEN; i++) {
        printf("%02X", abKs[i]);
    }
    printf("\n");
    return 0;
}
