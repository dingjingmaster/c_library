/**
 *  ctool_align.h
 *  enlarge buffer size at 2/4/8 byte boundary, memory block alignment.
 *
 */

#ifndef _CTOOL_ALIGN_H
#define _CTOOL_ALIGN_H

/* is 2/4/8 byte boundary */

#define CTOOL_ALIGN_OK2(s) ((((uint64_t) (s)) & 0x0000000000000001) == 0)
#define CTOOL_ALIGN_OK4(s) ((((uint64_t) (s)) & 0x0000000000000003) == 0)
#define CTOOL_ALIGN_OK8(s) ((((uint64_t) (s)) & 0x0000000000000007) == 0)

/* not 2/4/8 byte boundary */

#define CTOOL_ALIGN_NG2(s) ((((uint64_t) (s)) & 0x0000000000000001) != 0)
#define CTOOL_ALIGN_NG4(s) ((((uint64_t) (s)) & 0x0000000000000003) != 0)
#define CTOOL_ALIGN_NG8(s) ((((uint64_t) (s)) & 0x0000000000000007) != 0)

/* expend to 2/4/8 byte boundary, unsigned integer version. */

#define CTOOL_ALIGN_EU2(s1, s2) { \
    uint64_t s3; \
    /* addition may cause overflow !!! */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffffe; \
    if (s3 < ((uint64_t) (s1))) { \
        s3 += 2; \
    } \
    s2 = s3; \
}
#define CTOOL_ALIGN_EU4(s1, s2) { \
    uint64_t s3; \
    /* addition may cause overflow !!! */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffffc; \
    if (s3 < ((uint64_t) (s1))) { \
        s3 += 4; \
    } \
    s2 = s3; \
}
#define CTOOL_ALIGN_EU8(s1, s2) { \
    uint64_t s3; \
    /* addition may cause overflow !!! */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffff8; \
    if (s3 < ((uint64_t) (s1))) { \
        s3 += 8; \
    } \
    s2 = s3; \
}

/* expend to 2/4/8 byte boundary, pointer version. */

#define CTOOL_ALIGN_EP2(s1, s2, t) { \
    uint64_t s3; \
    /* addition may cause overflow !!! */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffffe; \
    if (s3 < ((uint64_t) (s1))) { \
        s3 += 2; \
    } \
    s2 = (t *) s3; \
}
#define CTOOL_ALIGN_EP4(s1, s2, t) { \
    uint64_t s3; \
    /* addition may cause overflow !!! */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffffc; \
    if (s3 < ((uint64_t) (s1))) { \
        s3 += 4; \
    } \
    s2 = (t *) s3; \
}
#define CTOOL_ALIGN_EP8(s1, s2, t) { \
    uint64_t s3; \
    /* addition may cause overflow !!! */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffff8; \
    if (s3 < ((uint64_t) (s1))) { \
        s3 += 8; \
    } \
    s2 = (t *) s3; \
}

/* down to 8 bit bitmap size, unsigned integer version. */

#define CTOOL_ALIGN_DU8(s1, s2) { \
    uint64_t s3; \
    /* divide before addition, would not overflow. */ \
    s3 = ((uint64_t) (s1)) & 0xfffffffffffffff8; \
    if (s3 < ((uint64_t) (s1))) { \
        s2 = (s3 >> 3) + 1; \
    } else { \
        s2 = s3 >> 3; \
    } \
}

#endif /* _CTOOL_ALIGN_H */

/******************************************************************************/
/*                                                                            */
/* end of file                                                                */
/*                                                                            */
/******************************************************************************/

