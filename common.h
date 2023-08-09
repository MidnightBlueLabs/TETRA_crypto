#ifndef HAVE_COMMON_H
#define HAVE_COMMON_H



#define rol32(x, n) ((x << n) | (x >> (32 - n)))

#define BYTE0(x) ((x) & 0xff)
#define BYTE1(x) (((x) >> 8) & 0xff)
#define BYTE2(x) (((x) >> 16) & 0xff)
#define BYTE3(x) (((x) >> 24) & 0xff)
#define BYTE4(x) (((x) >> 32) & 0xff)
#define BYTE5(x) (((x) >> 40) & 0xff)
#define BYTE6(x) (((x) >> 48) & 0xff)
#define BYTE7(x) (((x) >> 56) & 0xff)

#define BYTEN(x,n) (((x) >> (n * 8)) & 0xff)

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define be32(x) __builtin_bswap32(x)
#define be16(x) __builtin_bswap16(x)
#define le32(x) (x)
#define le16(x) (x)
#else
#define be32(x) (x)
#define be16(x) (x)
#define le32(x) __builtin_bswap32(x)
#define le16(x) __builtin_bswap16(x)
#endif

typedef struct {
    uint8_t  tn; // timeslot, 1 to 4
    uint8_t  fn; // frame, 1 to 18
    uint8_t  mn; // multiframe, 1 to 60
    uint16_t hn; // hyperframe, 0 to 0xFFFF, although only 15 bits are used
    uint8_t  dir; // 0 or 1; 0 = downlink, 1 = uplink
} FrameNumbers;


uint32_t build_iv(FrameNumbers *f);

#endif /* HAVE_COMMON_H */
