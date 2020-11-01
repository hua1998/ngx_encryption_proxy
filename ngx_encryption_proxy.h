/* 
 * File:   ngx_http_subs_filter_module.h
 * Author: Administrator
 *
 * Created on 2014年10月10日, 上午10:45
 */

#ifndef NGX_HTTP_SUBS_FILTER_MODULE_H
#define	NGX_HTTP_SUBS_FILTER_MODULE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

#define SUBS_DEBUG 1

#ifndef NGX_HTTP_MAX_CAPTURES
#define NGX_HTTP_MAX_CAPTURES 9
#endif

#define ngx_buffer_init(b) b->pos = b->last = b->start;

typedef struct {
    ngx_flag_t once;
    ngx_flag_t regex;
    ngx_flag_t insensitive;

    /* If it has captured variables? */
    ngx_flag_t has_captured;

    ngx_str_t match;
#if (NGX_PCRE)
    ngx_regex_t *match_regex;
    int *captures;
    ngx_int_t ncaptures;
#endif

    ngx_str_t sub;
    ngx_array_t *sub_lengths;
    ngx_array_t *sub_values;

    unsigned matched;
} sub_pair_t;

typedef struct {
    ngx_hash_t types;
    ngx_array_t *sub_pairs; /* array of sub_pair_t     */
    ngx_array_t *types_keys; /* array of ngx_hash_key_t */
    ngx_array_t *bypass; /* array of ngx_http_complex_value_t */
    size_t line_buffer_size;
    ngx_bufs_t bufs;
} ngx_http_subs_loc_conf_t;

typedef struct {
    ngx_array_t *sub_pairs; /* array of sub_pair_t */

    ngx_chain_t *in;

    /* the line input buffer before substitution */
    ngx_buf_t *line_in;
    /* the line destination buffer after substitution */
    ngx_buf_t *line_dst;

    /* the last output buffer */
    ngx_buf_t *out_buf;
    /* point to the last output chain's next chain */
    ngx_chain_t **last_out;
    ngx_chain_t *out;

    ngx_chain_t *busy;

    /* the freed chain buffers. */
    ngx_chain_t *free;

    ngx_int_t bufs;

    unsigned last;
    
    
    unsigned char *lastPacket;
    int lastPacketLen;
    int lastDePacketLen;

} ngx_http_subs_ctx_t;




#ifdef	__cplusplus
}
#endif

#endif	/* NGX_HTTP_SUBS_FILTER_MODULE_H */

