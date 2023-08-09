#ifndef HAVE_TEA2_H
#define HAVE_TEA2_H

#include <inttypes.h>

uint64_t expand_iv(uint32_t dwFrameNumbers);

void tea2(uint32_t dwFrameNumbers, uint8_t *lpKey, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

#endif /* HAVE_TEA2_H */
