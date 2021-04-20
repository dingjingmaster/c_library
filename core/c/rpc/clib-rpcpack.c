/*************************************************************************
> FileName: clib-rpcpack.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 16时30分08秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "c/common/clib-ret.h"
#include "clib-align.h"

#include "clib-rpcpack.h"

int rpcpack_tdown(rpcpack_trace_t *pack_trace, uint32_t node_id) 
{
    /* check trace depth, add node_id. */
    if (pack_trace->depth_count >= CLIB_RPCPACK_MAX_TRACE_DEPTH) {
        return DJCLIB_EINVAL;
    }
    if (pack_trace->depth_count == 0) {
        pack_trace->node_id0 = node_id;
    } else if (pack_trace->depth_count == 1) {
        pack_trace->node_id1 = node_id;
    } else if (pack_trace->depth_count == 2) {
        pack_trace->node_id2 = node_id;
    } else if (pack_trace->depth_count == 3) {
        pack_trace->node_id3 = node_id;
    } else if (pack_trace->depth_count == 4) {
        pack_trace->node_id4 = node_id;
    } else {
        pack_trace->node_id5 = node_id;
    }
    pack_trace->depth_count++;

    return DJCLIB_OK;
}

int rpcpack_tnode(rpcpack_trace_t *pack_trace, uint32_t depth_count, uint32_t *node_id) 
{
    /* check trace depth, get node_id. */
    if (depth_count >= pack_trace->depth_count) {
        return DJCLIB_EINVAL;
    }
    if (depth_count == 0) {
        *node_id = pack_trace->node_id0;
    } else if (depth_count == 1) {
        *node_id = pack_trace->node_id1;
    } else if (depth_count == 2) {
        *node_id = pack_trace->node_id2;
    } else if (depth_count == 3) {
        *node_id = pack_trace->node_id3;
    } else if (depth_count == 4) {
        *node_id = pack_trace->node_id4;
    } else {
        *node_id = pack_trace->node_id5;
    }

    return DJCLIB_OK;
}

void rpcpack_naddr(rpcpack_ninfo_t *pack_ninfo, char *addr_buf) 
{
    /* addr_buf should be [RPCPACK_ADDR_ISTR_BYTES] to hold ipv6 */
    if (pack_ninfo->node_flag & CLIB_RPCPACK_NODE_FLAG_IPV6) {
        snprintf(addr_buf, CLIB_RPCPACK_ADDR_ISTR_BYTES, /**/
        "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:" /**/
        "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", /**/
        pack_ninfo->addr[0], pack_ninfo->addr[1], /**/
        pack_ninfo->addr[2], pack_ninfo->addr[3], /**/
        pack_ninfo->addr[4], pack_ninfo->addr[5], /**/
        pack_ninfo->addr[6], pack_ninfo->addr[7], /**/
        pack_ninfo->addr[8], pack_ninfo->addr[9], /**/
        pack_ninfo->addr[10], pack_ninfo->addr[11], /**/
        pack_ninfo->addr[12], pack_ninfo->addr[13], /**/
        pack_ninfo->addr[14], pack_ninfo->addr[15]);
    } else {
        snprintf(addr_buf, CLIB_RPCPACK_ADDR_ISTR_BYTES, /**/
        "%u.%u.%u.%u", /**/
        pack_ninfo->addr[0], pack_ninfo->addr[1], /**/
        pack_ninfo->addr[2], pack_ninfo->addr[3]);
    }
}

static void rpcpack_dump(rpcpack_data_t *pack_data, uint32_t pack_len, const char *msg) 
{
    uint32_t item_id, item_count;

#if 0
    INFO("%s pack_len %u " /**/
    "command %04x %02x %02x call_seq %016llx " /**/
    "trace %u %u_%u_%u_%u_%u_%u " /**/
    "item_count %u", /**/
    msg, /**/
    pack_len, /**/
    pack_data->pack_head->command_type, /**/
    pack_data->pack_head->command_ver, /**/
    pack_data->pack_head->command_flag, /**/
    (unsigned long long) pack_data->pack_head->call_seq, /**/
    (uint32_t) pack_data->pack_head->call_trace.depth_count, /**/
    (uint32_t) pack_data->pack_head->call_trace.node_id0, /**/
    (uint32_t) pack_data->pack_head->call_trace.node_id1, /**/
    (uint32_t) pack_data->pack_head->call_trace.node_id2, /**/
    (uint32_t) pack_data->pack_head->call_trace.node_id3, /**/
    (uint32_t) pack_data->pack_head->call_trace.node_id4, /**/
    (uint32_t) pack_data->pack_head->call_trace.node_id5, /**/
    pack_data->pack_head->item_count);
#endif

    item_count = pack_data->pack_head->item_count;
    for (item_id = 0; item_id < item_count; item_id++) {
        printf("%s item_id %u " /**/
        "off %u len %u type %u %u", /**/
        msg, /**/
        item_id, /**/
        pack_data->pos_array[item_id].off, /**/
        pack_data->pos_array[item_id].len, /**/
        pack_data->pos_array[item_id].type1, /**/
        pack_data->pos_array[item_id].type2);
    }
}

int rpcpack_begin(rpcpack_data_t *pack_data, uint32_t item_asize, void *pack_buf, uint32_t pack_len) 
{
    /* set packing buffer boundary */
    if (pack_len < sizeof(rpcpack_head_t)) {
        printf("rpcpack_begin pack_len error pack_len %u", pack_len);
        return DJCLIB_EINVAL;
    }
    if (CLIB_ALIGN_NG8(pack_buf) || CLIB_ALIGN_NG8(pack_len)) {
        printf("rpcpack_begin align error " /**/
        "pack_buf %016llx pack_len %u", /**/
        (unsigned long long) pack_buf, pack_len);
        return DJCLIB_EINVAL;
    }
    pack_data->pack_buf = (uint8_t *) pack_buf;
    pack_data->pack_cur = (uint8_t *) pack_buf;
    pack_data->pack_end = (uint8_t *) pack_buf + pack_len;

    /* output 8x bytes head, no needs to padding. */
    pack_data->pack_head = (rpcpack_head_t *) pack_data->pack_cur;
    pack_data->pack_cur += sizeof(rpcpack_head_t);

    /* set max item count */
    pack_data->item_asize = item_asize;

    /* zero header */
    pack_data->pack_head->pad1 = 0;
    pack_data->pack_head->pad2 = 0;
    pack_data->pack_head->pad3 = 0;
    pack_data->pack_head->item_count = 0;
    pack_data->pack_head->pos_off = 0;

    return DJCLIB_OK;
}

int rpcpack_trace(void *pack_buf, uint32_t pack_len, rpcpack_trace_t *call_trace) 
{
    rpcpack_head_t *pack_head;

    pack_head = (rpcpack_head_t *) pack_buf;

    /* update call trace in header */
    if (pack_len < sizeof(rpcpack_head_t)) {
        printf ("rpcpack_trace pack_len error pack_len %u", pack_len);
        return DJCLIB_EINVAL;
    }
    if (CLIB_ALIGN_NG8(pack_buf) || CLIB_ALIGN_NG8(pack_len)) {
        printf ("rpcpack_trace align error" /**/
        "pack_buf %016llx pack_len %u", /**/
        (unsigned long long) pack_buf, pack_len);
        return DJCLIB_EINVAL;
    }
    pack_head->call_trace.trace_sign = call_trace->trace_sign;

    return DJCLIB_OK;
}

void* rpcpack_put(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t item_len, uint32_t item_type1, uint32_t item_type2) 
{
    uint32_t item_id;
    uint8_t *item_buf, *pack_pad;

    /* output item, record position. */
    item_id = pack_data->pack_head->item_count;
    if (item_id >= pack_data->item_asize) {
        printf("rpcpack_put item_count error item_count %u", /**/
        pack_data->item_asize);
        return NULL;
    }
    item_buf = pack_data->pack_cur;
    pos_array[item_id].off = item_buf - pack_data->pack_buf;
    pos_array[item_id].len = item_len;
    pos_array[item_id].type1 = item_type1;
    pos_array[item_id].type2 = item_type2;
    pack_data->pack_cur += item_len;

    /* pad to 8 byte boundary, check buffer ending. */
    CLIB_ALIGN_EP8(pack_data->pack_cur, pack_pad, uint8_t);
    if (pack_pad > pack_data->pack_end) {
        printf("rpcpack_put pack_len error pack_len %lu", /**/
        pack_pad - pack_data->pack_buf);
        return NULL;
    }
    while (pack_data->pack_cur < pack_pad) {
        *(pack_data->pack_cur)++ = 0;
    }
    pack_data->pack_head->item_count++;

    return item_buf;
}

int rpcpack_cut(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t item_id, uint32_t item_len) 
{
    /* resize item */
    if (item_id >= pack_data->pack_head->item_count) {
        printf ("rpcpack_cut item_id error item_id %u %u", /**/
        item_id, pack_data->pack_head->item_count);
        return DJCLIB_EINVAL;
    }
    if (item_len > pos_array[item_id].len) {
        printf ("rpcpack_cut item_len error item_len %u %u", /**/
        item_len, pos_array[item_id].len);
        return DJCLIB_EINVAL;
    }
    pos_array[item_id].len = item_len;

    return DJCLIB_OK;
}

int rpcpack_mov(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array) 
{
    uint32_t item_id, item_count, item_off, item_len;
    uint8_t *pack_pad;

    /* reset pack buffer */
    pack_data->pack_cur = pack_data->pack_buf + sizeof(rpcpack_head_t);

    /* loop all items */
    item_count = pack_data->pack_head->item_count;
    for (item_id = 0; item_id < item_count; item_id++) {
        /* move item */
        item_len = pos_array[item_id].len;
        item_off = pack_data->pack_cur - pack_data->pack_buf;
        if (item_off > pos_array[item_id].off) {
            printf("rpcpack_mov item_off error item_off %u %u", /**/
            item_off, pos_array[item_id].off);
            return DJCLIB_EINVAL;
        } else if (item_off < pos_array[item_id].off) {
            memmove(pack_data->pack_buf + item_off, pack_data->pack_buf + pos_array[item_id].off, item_len);
            pos_array[item_id].off = item_off;
        }
        pack_data->pack_cur += item_len;

        /* pad to 8 byte boundary, check buffer ending. */
        CLIB_ALIGN_EP8(pack_data->pack_cur, pack_pad, uint8_t);
        if (pack_pad > pack_data->pack_end) {
            printf("rpcpack_mov pack_len error pack_len %lu", /**/
            pack_pad - pack_data->pack_buf);
            return DJCLIB_EINVAL;
        }
        while (pack_data->pack_cur < pack_pad) {
            *(pack_data->pack_cur)++ = 0;
        }
    }

    return DJCLIB_OK;
}

void rpcpack_error(rpcpack_data_t *pack_data, uint32_t *pack_len, uint64_t rpc_mode) 
{
    /* pack error, return just header data. */
    pack_data->pack_head->item_count = 0;
    pack_data->pack_head->pos_off = sizeof(rpcpack_head_t);
    pack_data->pos_array = (rpcpack_pos_t *) (pack_data->pack_buf + sizeof(rpcpack_head_t));
    *pack_len = sizeof(rpcpack_head_t);

    /* dump encoded header */
    if (rpc_mode & CLIB_RPCPACK_MODE_LOG_DUMP) {
        rpcpack_dump(pack_data, *pack_len, "rpcpack_error");
    }
}

int rpcpack_end(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t *pack_len, uint64_t rpc_mode) 
{
    uint32_t pos_size;

    /* check buffer ending */
    pos_size = sizeof(rpcpack_pos_t) * pack_data->pack_head->item_count;
    if ((pack_data->pack_cur + pos_size) > pack_data->pack_end) {
        printf("rpcpack_end pack_len error pack_len %lu", /**/
        pack_data->pack_cur + pos_size - pack_data->pack_buf);
        return DJCLIB_EINVAL;
    }

    /* output pos_array, always x8 bytes, no needs to padding. */
    pack_data->pack_head->pos_off = pack_data->pack_cur - pack_data->pack_buf;
    pack_data->pos_array = (rpcpack_pos_t *) pack_data->pack_cur;
    memcpy(pack_data->pack_cur, pos_array, pos_size);
    pack_data->pack_cur += pos_size;

    /* return total packet length */
    *pack_len = pack_data->pack_cur - pack_data->pack_buf;

    /* dump encoded header */
    if (rpc_mode & CLIB_RPCPACK_MODE_LOG_DUMP) {
        rpcpack_dump(pack_data, *pack_len, "rpcpack_end");
    }

    return DJCLIB_OK;
}

int rpcpack_decode(rpcpack_data_t *pack_data, void *pack_buf, uint32_t pack_len, uint64_t rpc_mode) 
{
    uint32_t pos_size;

    /* set unpacking buffer beginning */
    if (pack_len < sizeof(rpcpack_head_t)) {
        printf("rpcpack_decode pack_len error pack_len %u", pack_len);
        return DJCLIB_EINVAL;
    }
    if (CLIB_ALIGN_NG8(pack_buf) || CLIB_ALIGN_NG8(pack_len)) {
        printf("rpcpack_decode align error " /**/
        "pack_buf %016llx pack_len %u", /**/
        (unsigned long long) pack_buf, pack_len);
        return DJCLIB_EINVAL;
    }
    pack_data->pack_buf = (uint8_t *) pack_buf;
    pack_data->pack_end = (uint8_t *) pack_buf + pack_len;

    /* get head */
    pack_data->pack_head = (rpcpack_head_t *) pack_data->pack_buf;
    pos_size = sizeof(rpcpack_pos_t) * pack_data->pack_head->item_count;
    if ((pack_data->pack_head->pos_off + pos_size) != pack_len) {
        printf( /**/
        "rpcpack_decode pack_pos error pack_pos %u pack_len %u", /**/
        pack_data->pack_head->pos_off + pos_size, pack_len);
        return DJCLIB_EINVAL;
    }

    /* set unpacking buffer ending */
    pack_data->pack_cur = (uint8_t *) pack_buf + pack_data->pack_head->pos_off;

    /* get head and pos_array */
    pack_data->pos_array = (rpcpack_pos_t *) pack_data->pack_cur;

    /* dump decoded header */
    if (rpc_mode & CLIB_RPCPACK_MODE_LOG_DUMP) {
        rpcpack_dump(pack_data, pack_len, "rpcpack_decode");
    }

    return DJCLIB_OK;
}

int rpcpack_head(rpcpack_data_t *pack_data, void *pack_buf, uint32_t pack_len, uint64_t rpc_mode) 
{
    /* reset header */
    if (pack_len < sizeof(rpcpack_head_t)) {
        printf("rpcpack_head pack_len error pack_len %u", pack_len);
        return DJCLIB_EINVAL;
    }
    if (CLIB_ALIGN_NG8(pack_buf) || CLIB_ALIGN_NG8(pack_len)) {
        printf("rpcpack_head align error " /**/
        "pack_buf %016llx pack_len %u", /**/
        (unsigned long long) pack_buf, pack_len);
        return DJCLIB_EINVAL;
    }
    pack_data->pack_buf = (uint8_t *) pack_buf;
    pack_data->pack_end = (uint8_t *) pack_buf + pack_len;

    pack_data->pack_head = (rpcpack_head_t *) pack_data->pack_buf;
    pack_data->pack_cur = (uint8_t *) pack_buf + pack_data->pack_head->pos_off;
    pack_data->pos_array = (rpcpack_pos_t *) pack_data->pack_cur;

    /* dump encoded header */
    if (rpc_mode & CLIB_RPCPACK_MODE_LOG_DUMP) {
        rpcpack_dump(pack_data, pack_len, "rpcpack_head");
    }

    return DJCLIB_OK;
}

void* rpcpack_get(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t item_id, uint32_t *item_len, uint32_t *item_type1, uint32_t *item_type2) 
{
    uint8_t *item_buf;

    if (pos_array == NULL) {
        pos_array = pack_data->pos_array;
    }

    /* zero item_len */
    *item_len = 0;

    /* get packing item by id, random access. */
    if (item_id >= pack_data->pack_head->item_count) {
        printf("rpcpack_get item_id error item_id %u", item_id);
        return NULL;
    }
    item_buf = pack_data->pack_buf + pos_array[item_id].off;
    if ((item_buf + pos_array[item_id].len) > pack_data->pack_cur) {
        printf("rpcpack_get pack_len error pack_len %lu", /**/
        item_buf + pos_array[item_id].len - pack_data->pack_buf);
        return NULL;
    }
    *item_len = pos_array[item_id].len;
    *item_type1 = pos_array[item_id].type1;
    *item_type2 = pos_array[item_id].type2;

    return item_buf;
}

