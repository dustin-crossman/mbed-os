/*
 * $ Copyright Cypress Semiconductor Apache2 $
 */
/*
 * Byte order utilities
 *
 * This file by default provides proper behavior on little-endian architectures.
 * On big-endian architectures, IL_BIGENDIAN should be defined.
 */

#ifndef INCLUDED_WHD_ENDIAN_H
#define INCLUDED_WHD_ENDIAN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Reverse the bytes in a 16-bit value */
#define SWAP16(val) \
    ( (uint16_t)( ( ( (uint16_t)(val) & (uint16_t)0x00ffU ) << 8 ) | \
                  ( ( (uint16_t)(val) & (uint16_t)0xff00U ) >> 8 ) ) )

/* Reverse the bytes in a 32-bit value */
#define SWAP32(val) \
    ( (uint32_t)( ( ( (uint32_t)(val) & (uint32_t)0x000000ffU ) << 24 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0x0000ff00U ) <<  8 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0x00ff0000U ) >>  8 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0xff000000U ) >> 24 ) ) )

/* Reverse the two 16-bit halves of a 32-bit value */
#define SWAP32BY16(val) \
    ( (uint32_t)( ( ( (uint32_t)(val) & (uint32_t)0x0000ffffU ) << 16 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0xffff0000U ) >> 16 ) ) )

extern uint16_t swap16(uint16_t val);
extern uint32_t swap32(uint32_t val);
extern uint32_t swap32by16(uint32_t val);
extern void     swap16_buf(uint16_t *buf, uint32_t len);
extern void     htol16_ua_store(uint16_t val, uint8_t *bytes);
extern void     htol32_ua_store(uint32_t val, uint8_t *bytes);
extern void     hton16_ua_store(uint16_t val, uint8_t *bytes);
extern void     hton32_ua_store(uint32_t val, uint8_t *bytes);
extern uint16_t ltoh16_ua(const void *bytes);
extern uint32_t ltoh32_ua(const void *bytes);
extern uint16_t ntoh16_ua(const void *bytes);
extern uint32_t ntoh32_ua(const void *bytes);

#ifndef hton16
#ifndef IL_BIGENDIAN
#define HTON16(i) SWAP16(i)
#define HTON32(i) SWAP32(i)
#define NTOH16(i) SWAP16(i)
#define NTOH32(i) SWAP32(i)
#define hton16(i) swap16(i)
#define hton32(i) swap32(i)
#define ntoh16(i) swap16(i)
#define ntoh32(i) swap32(i)
#define HTOL16(i) (i)
#define HTOL32(i) (i)
#define ltoh16(i) (i)
#define ltoh32(i) (i)
#define htol16(i) (i)
#define htol32(i) (i)
#else
#define HTON16(i) (i)
#define HTON32(i) (i)
#define hton16(i) (i)
#define hton32(i) (i)
#define ntoh16(i) (i)
#define ntoh32(i) (i)
#define HTOL16(i) SWAP16(i)
#define HTOL32(i) SWAP32(i)
#define ltoh16(i) swap16(i)
#define ltoh32(i) swap32(i)
#define htol16(i) swap16(i)
#define htol32(i) swap32(i)
#endif   /* IL_BIGENDIAN */
#endif  /* hton16 */

#ifndef IL_BIGENDIAN
#define ltoh16_buf(buf, i)
#define htol16_buf(buf, i)
#else
#define ltoh16_buf(buf, i) swap16_buf( (uint16_t *)buf, i )
#define htol16_buf(buf, i) swap16_buf( (uint16_t *)buf, i )
#endif  /* IL_BIGENDIAN */


#define _LTOH16_UA(cp)    ( (cp)[0] | ( (cp)[1] << 8 ) )
#define _LTOH32_UA(cp)    ( (cp)[0] | ( (cp)[1] << 8 ) | ( (cp)[2] << 16 ) | ( (cp)[3] << 24 ) )
#define _NTOH16_UA(cp)    ( ( (cp)[0] << 8 ) | (cp)[1] )
#define _NTOH32_UA(cp)    ( ( (cp)[0] << 24 ) | ( (cp)[1] << 16 ) | ( (cp)[2] << 8 ) | (cp)[3] )

#define ltoh_ua(ptr) \
    (sizeof(*(ptr) ) == sizeof(uint8_t) ? *(const uint8_t *)ptr : \
     sizeof(*(ptr) ) == sizeof(uint16_t) ? _LTOH16_UA( (const uint8_t *)ptr ) : \
     sizeof(*(ptr) ) == sizeof(uint32_t) ? _LTOH32_UA( (const uint8_t *)ptr ) : \
     0xfeedf00d)

#define ntoh_ua(ptr) \
    (sizeof(*(ptr) ) == sizeof(uint8_t) ? *(const uint8_t *)ptr : \
     sizeof(*(ptr) ) == sizeof(uint16_t) ? _NTOH16_UA( (const uint8_t *)ptr ) : \
     sizeof(*(ptr) ) == sizeof(uint32_t) ? _NTOH32_UA( (const uint8_t *)ptr ) : \
     0xfeedf00d)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INCLUDED_WHD_ENDIAN_H */

