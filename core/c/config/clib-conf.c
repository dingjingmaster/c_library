/*************************************************************************
> FileName: clib-conf.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 11时33分54秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "clib-conf.h"

#include "c/log/clib-log.h"
#include "c/common/clib-ret.h"

typedef struct
{
    uint32_t k_bpos;
    uint32_t v_bpos;

} conf_kv_t;

typedef struct 
{
    uint32_t kv_asize;
    uint32_t kv_count;
    conf_kv_t *kv_array;

    uint32_t bytes_asize;
    uint32_t bytes_count;
    uint8_t *bytes_array;

} conf_data_t;

#define CONF ((conf_data_t *) conf)

int conf_load(void *conf, const char *path_name, uint32_t buf_size, uint32_t kv_asize)
{
    int c;
    FILE *file_fp;
    uint32_t mem_size;
    uint32_t line_len, cid, k_bpos, k_epos, k_len, v_bpos, v_epos, v_len;
    uint8_t line_buf[CLIB_CONF_MAX_LINE_BYTES];
    uint8_t *mem_cur;

    mem_size = sizeof(conf_data_t);
    mem_size += sizeof(conf_kv_t) * kv_asize;
    if (buf_size < mem_size) {
        return DJCLIB_EINVAL;
    } else {
        mem_cur = (uint8_t *) conf + sizeof(conf_data_t);
        CONF->kv_asize = kv_asize;
        CONF->kv_count = 0;
        CONF->kv_array = (conf_kv_t *) mem_cur;
        mem_cur += sizeof(conf_kv_t) * kv_asize;

        CONF->bytes_asize = buf_size - mem_size;
        CONF->bytes_count = 0;
        CONF->bytes_array = (uint8_t *) mem_cur;
    }

    file_fp = fopen(path_name, "rb");
    if (file_fp == NULL) {
        return DJCLIB_EFILE;
    }
    for (;;) {
        /* get line text */
        c = EOF;
        line_len = 0;
        while (line_len < CLIB_CONF_MAX_LINE_BYTES) {
            c = fgetc(file_fp);
            if ((c == EOF) || (c == '\n')) {
                break;
            } else if (c != '\r') {
                line_buf[line_len++] = c;
            }
        }
        if (line_len > 0) {
            /* by pass comments */
            if ((line_buf[0] == '#') || (line_buf[0] == '[')) {
                continue;
            }

            /* get key, trim spaces. */
            cid = 0;
            while (cid < line_len) {
                if (line_buf[cid] > ' ') {
                    break;
                }
                cid++;
            }
            k_bpos = cid;
            k_epos = cid;
            while (cid < line_len) {
                if (line_buf[cid] <= ' ') {
                    if (k_bpos == k_epos) {
                        k_epos = cid;
                    }
                } else if (line_buf[cid] == '=') {
                    if (k_bpos == k_epos) {
                        k_epos = cid;
                    }
                    cid++;
                    break;
                }
                cid++;
            }

            /* get value, trim spaces. */
            while (cid < line_len) {
                if (line_buf[cid] > ' ') {
                    break;
                }
                cid++;
            }
            v_bpos = cid;
            v_epos = cid;
            while (cid < line_len) {
                if (line_buf[cid] <= ' ') {
                    if (v_bpos == v_epos) {
                        v_epos = cid;
                    }
                }
                cid++;
            }
            if (v_bpos == v_epos) {
                v_epos = cid;
            }

            if ((k_bpos < k_epos) && (v_bpos < v_epos)) {
                k_len = k_epos - k_bpos;
                v_len = v_epos - v_bpos;

                if (CONF->kv_count >= CONF->kv_asize) {
                    break;
                }

                /* '\0' terminate value string, add one more byte. */
                if ((CONF->bytes_count + k_len + v_len + 1) > CONF->bytes_asize) {
                    break;
                }

                CONF->kv_array[CONF->kv_count].k_bpos = CONF->bytes_count;
                memcpy(CONF->bytes_array + CONF->bytes_count, line_buf + k_bpos, k_len);
                CONF->bytes_count += k_len;

                CONF->kv_array[CONF->kv_count].v_bpos = CONF->bytes_count;
                memcpy(CONF->bytes_array + CONF->bytes_count, line_buf + v_bpos, v_len);
                CONF->bytes_count += v_len;

                *(CONF->bytes_array + CONF->bytes_count) = 0;
                CONF->bytes_count += 1;

                ++(CONF->kv_count);
            }
        }

        /* trap end of file */
        if (c == EOF) {
            break;
        }
    }
    fclose(file_fp);

    return DJCLIB_OK;
}

const char *conf_value(void *conf, const char *key, const char *def) 
{
    uint32_t kv_id, kv_len, key_len;

    key_len = strlen(key);
    for (kv_id = 0; kv_id < CONF->kv_count; kv_id++) {
        kv_len = CONF->kv_array[kv_id].v_bpos - CONF->kv_array[kv_id].k_bpos;
        if (key_len == kv_len) {
            if (memcmp(key, CONF->bytes_array + CONF->kv_array[kv_id].k_bpos, key_len) == 0) {
                return (char *) (CONF->bytes_array + CONF->kv_array[kv_id].v_bpos);
            }
        }
    }

    return def;
}

uint64_t conf_udec(void *conf, const char *key, uint64_t def) 
{
    const char *value;

    value = conf_value(conf, key, NULL);
    if (value != NULL) {
        return strtoul(value, NULL, 10);
    }
    return def;
}

uint64_t conf_uhex(void *conf, const char *key, uint64_t def) 
{
    const char *value;

    value = conf_value(conf, key, NULL);
    if (value != NULL) {
        return strtoul(value, NULL, 16);
    }
    return def;
}

void conf_dump(void *conf) {
    uint32_t kv_id;
#if 0
    for (kv_id = 0; kv_id < CONF->kv_count; kv_id++) {
        INFO("[%.*s]=[%s]", /**/
        CONF->kv_array[kv_id].v_bpos - CONF->kv_array[kv_id].k_bpos, /**/
        CONF->bytes_array + CONF->kv_array[kv_id].k_bpos, /**/
        CONF->bytes_array + CONF->kv_array[kv_id].v_bpos);
    }
#endif
}
