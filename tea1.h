#ifndef HAVE_TEA1_H
#define HAVE_TEA1_H

#include <inttypes.h>


void tea1(uint32_t dwFrameNumbers, const uint8_t *lpKey, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

// Constants
extern const uint16_t g_awTea1LutA[8];
extern const uint16_t g_awTea1LutB[8];
extern const uint8_t g_abTea1Sbox[256];

// Internals
uint64_t tea1_expand_iv(uint32_t dwShortIv);
int32_t tea1_init_key_register(const uint8_t *lpKey);
uint8_t tea1_state_word_to_newbyte(uint16_t wSt, const uint16_t *awLut);
uint8_t tea1_reorder_state_byte(uint8_t bStByte);
void tea1_inner(uint64_t qwIvReg, uint32_t dwKeyReg, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

#endif /* HAVE_TEA1_H */
