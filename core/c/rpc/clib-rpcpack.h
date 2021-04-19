/*************************************************************************
> FileName: clib-rpcpack.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 15时08分23秒
 ************************************************************************/
#ifndef _CLIB_RPCPACK_H
#define _CLIB_RPCPACK_H

#define CLIB_RPCPACK_MODE_LOG_DUMP              0x0000000000000004
#define CLIB_RPCPACK_MAX_TRACE_DEPTH            6

/**
 * @brief call trace
 */
typedef union {
    /* 8 */
    struct {
        /* 4 bit count */
        uint64_t depth_count :4;

        /* 10 bit node_id, [0, 1023]. */
        uint64_t node_id0 :10;
        uint64_t node_id1 :10;
        uint64_t node_id2 :10;
        uint64_t node_id3 :10;
        uint64_t node_id4 :10;
        uint64_t node_id5 :10;
    };
    uint64_t trace_sign;

} rpcpack_trace_t;


/* should be 16 and 32 bytes */
#define CLIB_RPCPACK_MAX_NSADDR_BYTES           16
#define CLIB_RPCPACK_MAX_NSDATA_BYTES           32

/* IP address in string format */
#define CLIB_RPCPACK_ADDR_ISTR_BYTES            64

/* node flag
 *
 * FLAG_IPV6        node address it is IPv6 address
 */
#define CLIB_RPCPACK_NODE_FLAG_IPV6             0x01

/* call node information */
typedef struct {
    /* 8 */
    rpcpack_trace_t call_trace;
    /* 36
     *
     * limit rpc_id and line_type to 16 bit
     * limit call_state and node_flag to 8 bit
     * 16 byte address for ipv6
     */
    uint32_t rpc_id;
    uint32_t line_type;
    uint8_t call_state;
    uint8_t node_flag;
    uint16_t port;
    uint8_t addr[CLIB_RPCPACK_MAX_NSADDR_BYTES];

    /* 48
     *
     * limit call and poll time to 16 bit, at most 65 seconds.
     * transmit length should be 32 bit for big packet
     */
    uint16_t ms_call;
    uint16_t ms_send;
    uint16_t ms_poll;
    uint16_t ms_recv;
    uint32_t tran_len;

    /* 80
     *
     * 32 byte pay load, should not overflow.
     */
    uint8_t data_buf[CLIB_RPCPACK_MAX_NSDATA_BYTES];

} rpcpack_ninfo_t;

/* header */
typedef struct {
    /* 24 */
    union {
        struct {
            /* 4 bytes body length, same as rpccall_nsend and rpccall_nrecv. */
            uint32_t head_len;
            uint16_t command_type;
            uint8_t command_ver;
            uint8_t command_flag;

            uint64_t call_seq;
            rpcpack_trace_t call_trace;
        };
        struct {
            uint64_t pad1;
            uint64_t pad2;
            uint64_t pad3;
        };
    };
    /* 32 */
    uint32_t item_count;
    uint32_t pos_off;
} rpcpack_head_t;

/* pack item position */
typedef struct {
    /* 16 */
    uint32_t off;
    uint32_t len;
    uint32_t type1;
    uint32_t type2;
} rpcpack_pos_t;

/* pack data */
typedef struct {
    uint8_t *pack_buf;
    uint8_t *pack_cur;
    uint8_t *pack_end;

    uint32_t item_asize;

    rpcpack_head_t *pack_head;
    rpcpack_pos_t *pos_array;
} rpcpack_data_t;


int rpcpack_tdown(rpcpack_trace_t *pack_trace, uint32_t node_id);
int rpcpack_tnode(rpcpack_trace_t *pack_trace, uint32_t depth_count, uint32_t *node_id);
void rpcpack_naddr(rpcpack_ninfo_t *pack_ninfo, char *addr_buf);

int rpcpack_begin(rpcpack_data_t *pack_data, uint32_t item_asize, void *pack_buf, uint32_t pack_len);
int rpcpack_trace(void *pack_buf, uint32_t pack_len, rpcpack_trace_t *call_trace);
void* rpcpack_put(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t item_len, uint32_t item_type1, uint32_t item_type2);
int rpcpack_cut(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t item_id, uint32_t item_len);
int rpcpack_mov(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array);
int rpcpack_copy(rpcpack_data_t *pack_data, void *pack_buf, uint32_t pack_len, void *copy_buf, uint32_t copy_len);
void rpcpack_error(rpcpack_data_t *pack_data, uint32_t *pack_len, uint64_t rpc_mode);
int rpcpack_end(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t *pack_len, uint64_t rpc_mode);

int rpcpack_decode(rpcpack_data_t *pack_data, void *pack_buf, uint32_t pack_len, uint64_t rpc_mode);
int rpcpack_head(rpcpack_data_t *pack_data, void *pack_buf, uint32_t pack_len, uint64_t rpc_mode);
void* rpcpack_get(rpcpack_data_t *pack_data, rpcpack_pos_t *pos_array, uint32_t item_id, uint32_t *item_len, uint32_t *item_type1, uint32_t *item_type2);

#endif
