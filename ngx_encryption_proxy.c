#include "ngx_encryption_proxy.h"
#include "ngx_cep_httpfilter.h"
#include "ngx_cep_policy.h"
#include "policymanage.h"

static ngx_int_t ngx_http_subs_header_filter(ngx_http_request_t *r);
static ngx_int_t ngx_http_subs_init_context(ngx_http_request_t *r);

static ngx_int_t ngx_http_subs_body_filter(ngx_http_request_t *r,ngx_chain_t *in);
static ngx_int_t ngx_http_subs_body_filter_init_context(ngx_http_request_t *r,ngx_chain_t *in);
static ngx_int_t ngx_http_subs_body_filter_process_buffer(ngx_http_request_t *r,ngx_buf_t *b);
static ngx_int_t ngx_http_subs_match(ngx_http_request_t *r,ngx_http_subs_ctx_t *ctx);

#if (NGX_PCRE)
static ngx_int_t ngx_http_subs_match_regex_substituion(ngx_http_request_t *r,sub_pair_t *pair, ngx_buf_t *b, ngx_buf_t *dst);
#endif

static ngx_int_t ngx_http_subs_match_fix_substituion(ngx_http_request_t *r,sub_pair_t *pair, ngx_buf_t *b, ngx_buf_t *dst);
static ngx_buf_t * buffer_append_string(ngx_buf_t *b, u_char *s, size_t len,ngx_pool_t *pool);
static ngx_int_t ngx_http_subs_out_chain_append(ngx_http_request_t *r,ngx_http_subs_ctx_t *ctx, ngx_buf_t *b);
static ngx_int_t ngx_http_subs_get_chain_buf(ngx_http_request_t *r,ngx_http_subs_ctx_t *ctx);
static ngx_int_t ngx_http_subs_output(ngx_http_request_t *r,ngx_http_subs_ctx_t *ctx, ngx_chain_t *in);

static char * ngx_http_subs_filter(ngx_conf_t *cf, ngx_command_t *cmd,void *conf);
static ngx_int_t ngx_http_subs_filter_regex_compile(sub_pair_t *pair,ngx_http_script_compile_t *sc, ngx_conf_t *cf);


static void *ngx_http_subs_create_conf(ngx_conf_t *cf);
static char *ngx_http_subs_merge_conf(ngx_conf_t *cf, void *parent,void *child);

static ngx_int_t ngx_http_subs_filter_init(ngx_conf_t *cf);

#if (NGX_PCRE)
static ngx_int_t ngx_http_subs_regex_capture_count(ngx_regex_t *re);
#endif

static ngx_int_t ngx_http_p2s_urlquery_handler(ngx_http_request_t *r);

static ngx_command_t ngx_http_subs_filter_commands[] = {
    { 
        ngx_string("subs_filter"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_2MORE,
        ngx_http_subs_filter,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
     },{ 
        ngx_string("subs_filter_bypass"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
        ngx_http_set_predicate_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_subs_loc_conf_t, bypass),
        NULL
     },{ 
        ngx_string("subs_filter_types"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
        ngx_http_types_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_subs_loc_conf_t, types_keys),
        &ngx_http_html_default_types[0]
     },{ 
        ngx_string("subs_line_buffer_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
        ngx_conf_set_size_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_subs_loc_conf_t, line_buffer_size),
        NULL
     },{ 
        ngx_string("subs_buffers"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
        ngx_conf_set_bufs_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_subs_loc_conf_t, bufs),
        NULL
    },
    ngx_null_command
};


static ngx_http_module_t ngx_http_subs_filter_module_ctx = {
    NULL, /* preconfiguration */
    ngx_http_subs_filter_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_subs_create_conf, /* create location configuration */
    ngx_http_subs_merge_conf /* merge location configuration */
};


ngx_module_t ngx_http_subs_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_subs_filter_module_ctx, /* module context */
    ngx_http_subs_filter_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_http_output_header_filter_pt ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt ngx_http_next_body_filter;

extern volatile ngx_cycle_t *ngx_cycle;


static ngx_int_t ngx_http_subs_header_filter(ngx_http_request_t *r) {
    ngx_http_subs_loc_conf_t *slcf;
    printnginxlog("ngx_http_subs_header_filter\n");
    slcf = ngx_http_get_module_loc_conf(r, ngx_http_subs_filter_module); //获得当前的配置结构体。

    if (slcf->sub_pairs->nelts == 0 //没有设置过滤项
            || r->header_only //如果客户端仅仅需要我们返回头信息，那么我们就没有必要继续发送后面的报文体。
            || r->headers_out.content_type.len == 0 //content_type为空
            || r->headers_out.content_length_n == 0) //如果没有content或者是head请求，则直接跳过。
    {
        printnginxlog("ngx_http_subs_header_filter break 1\n");
        return ngx_http_next_header_filter(r);
    }

    if (ngx_http_test_content_type(r, &slcf->types) == NULL) {
        printnginxlog("HTTP Content type is not\n");
        return ngx_http_next_header_filter(r);
    }

    switch (ngx_http_test_predicates(r, slcf->bypass)) {

        case NGX_ERROR:
            /*pass through*/

        case NGX_DECLINED: //请求需要被转发到本阶段的下一个handler
            printnginxlog("ngx_http_subs_header_filter break NGX_DECLINED\n");
            return ngx_http_next_header_filter(r);

        default: /* NGX_OK */
            break;
    }

    /* Don't do substitution with the compressed content */
    if (r->headers_out.content_encoding
            && r->headers_out.content_encoding->value.len) {
        /*
        Content-Encoding是HTTP协议的响应报文头，一般形式如：
        Content-Encoding：gzip,deflate,compress
        Content-Encoding的说明中指出deflate指的是在RFC1950说明的zlib格式。也就是说当Content-Encoding为deflate时，内容应该为zlib格式。
        compress具说chrome支持，但还没见到哪个web服务器支持
        gzip,deflate,zlib的关系：
				
        deflate(RFC1951):一种压缩算法，使用LZ77和哈弗曼进行编码；  
        zlib(RFC1950):一种格式，是对deflate进行了简单的封装；  
        gzip(RFC1952):一种格式，也是对deflate进行的封装.
         */
        printnginxlog("Content-Encoding\n");

        return ngx_http_next_header_filter(r);
    }
    //print_ngx_str_t("server is               :", r->headers_in.server.data, r->headers_in.server.len);
    //print_ngx_str_t("http subs filter header :", r->unparsed_uri.data, r->unparsed_uri.len);

    if (ngx_http_subs_init_context(r) == NGX_ERROR) { //初始化上下文和配置信息
        printnginxlog("ngx_http_subs_header_filter break NGX_DECLINED\n");
        return NGX_ERROR;
    }

    r->filter_need_in_memory = 1; //未知

    if (r == r->main) {
        /*当前请求既有可能是用户发来的请求，也可能是派生出的子请求。 
         * 而main标识一系列相关的派生子请求的原始请求。 
         * 一般可通过main和当前请求的地址是否相等来判断当前请求是否为用户发来的原始请求。*/

        /*由于通常都会需要修改http body的长度，因此有两种方法：
                        1，用ngx_http_clear_content_length()清除content-length，然后立刻调用next_header_filter，从而使得数据以trunked的方式发送。参见：ngx_http_gzip_filter_module.c
                        2，在header filter里面不调next_header_filter，而是等body都处理完之后，设置了r->headers_out.content_length，再调next_header_filter。参见：ngx_http_image_filter_module.c*/
        ngx_http_clear_content_length(r);
        ngx_http_clear_last_modified(r); //抹去了last-modified的输出。
    }

    return ngx_http_next_header_filter(r);
}

static ngx_int_t ngx_http_subs_init_context(ngx_http_request_t *r) {
    ngx_uint_t i;
    sub_pair_t *src_pair, *dst_pair;
    ngx_http_subs_ctx_t *ctx;
    ngx_http_subs_loc_conf_t *slcf;
    printnginxlog("ngx_http_subs_init_context\n");

    slcf = ngx_http_get_module_loc_conf(r, ngx_http_subs_filter_module); //其作用是根据 module 的索引字段（ctx_index），找到 request 所请求的 location 配置。

    /* Everything in ctx is NULL or 0. */
    ctx = ngx_pcalloc(r->pool, sizeof (ngx_http_subs_ctx_t));
    if (ctx == NULL) {
        return NGX_ERROR;
    }

    ngx_http_set_ctx(r, ctx, ngx_http_subs_filter_module); //设置上下文

    ctx->sub_pairs = ngx_array_create(r->pool, slcf->sub_pairs->nelts,
            sizeof (sub_pair_t)); //创建数组
    if (slcf->sub_pairs == NULL) {
        return NGX_ERROR;
    }

    /* Deep copy sub_pairs from slcf to ctx, matched and captures need it */
    src_pair = (sub_pair_t *) slcf->sub_pairs->elts;

    for (i = 0; i < slcf->sub_pairs->nelts; i++) {

        dst_pair = ngx_array_push(ctx->sub_pairs);
        if (dst_pair == NULL) {
            return NGX_ERROR;
        }

        ngx_memcpy(dst_pair, src_pair + i, sizeof (sub_pair_t));
    }

    if (ctx->line_in == NULL) {

        ctx->line_in = ngx_create_temp_buf(r->pool, slcf->line_buffer_size);
        if (ctx->line_in == NULL) {
            return NGX_ERROR;
        }
    }

    if (ctx->line_dst == NULL) {

        ctx->line_dst = ngx_create_temp_buf(r->pool, slcf->line_buffer_size);
        if (ctx->line_dst == NULL) {
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}

static ngx_int_t ngx_http_subs_body_filter(ngx_http_request_t *r, ngx_chain_t *in) {
    ngx_int_t rc;
    ngx_log_t *log;
    ngx_chain_t *cl, *temp;
    ngx_http_subs_ctx_t *ctx;
    ngx_http_subs_loc_conf_t *slcf;
    
    printnginxlog("ngx_http_subs_body_filter\n");

    log = r->connection->log; //请求对应的客户端连接的日志指针

    slcf = ngx_http_get_module_loc_conf(r, ngx_http_subs_filter_module); //获取配置信息
    if (slcf == NULL) {
        return ngx_http_next_body_filter(r, in);
    }

    ctx = ngx_http_get_module_ctx(r, ngx_http_subs_filter_module); //获取上下文
    if (ctx == NULL) {
        return ngx_http_next_body_filter(r, in);
    }
    //print_ngx_str_t("http subs filter ",r->uri.data,r->uri.len );
    print_ngx_str_t("server is               :", r->headers_in.server.data, r->headers_in.server.len);
    print_ngx_str_t("http subs filter header :", r->unparsed_uri.data, r->unparsed_uri.len);

    char connections[20] = {0};
    sprintf(connections,"%08X",*(unsigned int*)r->connection);
    print_ngx_str_t("connection :", (u_char*)connections,strlen(connections));
    
    
    /* It doesn't intput anything, return */
    if (in == NULL && ctx->busy == NULL) {
        printnginxlog("ngx_http_subs_body_filter break 1\n");
        return ngx_http_next_body_filter(r, in);
    }

    //获取body内容
    if (ngx_http_subs_body_filter_init_context(r, in) != NGX_OK) {
        printnginxlog("ngx_http_subs_body_filter break 2\n");
        goto failed;
    }

    //替换body内容，body内容存储在ctx-in
    for (cl = ctx->in; cl; cl = cl->next) {

        if (cl->buf->last_buf || cl->buf->last_in_chain) {
            //最后一行内容
            ctx->last = 1;
        }
        //打印body内容
        //replace_ngx_str_t(r, cl->buf->start, ngx_buf_size(cl->buf));
        //print_ngx_str_t("[cl->buf]", cl->buf->start, ngx_buf_size(cl->buf));
        /* TODO: check the flush flag */
        rc = ngx_http_subs_body_filter_process_buffer(r, cl->buf);

        if (rc == NGX_DECLINED) {
            continue;
        } else if (rc == NGX_ERROR) {
            goto failed;
        }

        if (cl->next != NULL) {
            //如果还有后续包，继续循环；
            continue;
        }

        if (ctx->last) {
            //print_start2end("[cl->buf]" ,cl->buf->start ,cl->buf->end );
            // copy line_in to ctx->out. 
            /*if (ngx_buf_size(ctx->line_in) > 0) {

                //print_start2end("[cl->line_in]" ,ctx->line_in->start ,ctx->line_in->end );
                //printf(
                //    "[subs_filter] Lost last linefeed, output anyway.");

                if (ngx_http_subs_out_chain_append(r, ctx, ctx->line_in)
                        != NGX_OK) {
                    goto failed;
                }
            }*/

            if (ctx->out_buf == NULL) {

                //printf(
                //                "[subs_filter] The last buffer is zero size.");
                //
                // This is a zero buffer, it should not be set the temporary
                // or memory flag
                // 
                ctx->out_buf = ngx_calloc_buf(r->pool);
                if (ctx->out_buf == NULL) {
                    goto failed;
                }

                ctx->out_buf->sync = 1;

                temp = ngx_alloc_chain_link(r->pool);
                if (temp == NULL) {
                    goto failed;
                }

                temp->buf = ctx->out_buf;
                temp->next = NULL;

                *ctx->last_out = temp;
                ctx->last_out = &temp->next;
            }

            ctx->out_buf->last_buf = (r == r->main) ? 1 : 0;
            ctx->out_buf->last_in_chain = cl->buf->last_in_chain;

            //print_start2end("[outbuf]" ,ctx->out_buf->start ,ctx->out_buf->end );

            break;
        }
    }

      //替换body内容，body内容存储在ctx-in
    int out_len = 0;
    for (cl = ctx->out; cl; cl = cl->next) {

        out_len += ngx_buf_size(cl->buf);
        //打印body内容
        int real_len = ngx_buf_size(cl->buf);
        replace_ngx_str_t(r, ctx, cl->buf->start, ngx_buf_size(cl->buf),&real_len,cl->next);
        if(real_len < ngx_buf_size(cl->buf)){
            cl->buf->last = cl->buf->pos + real_len;
        }
        //print_ngx_str_t("[cl->buf]", cl->buf->start, ngx_buf_size(cl->buf));
   
        if (cl->next != NULL) {
            //如果还有后续包，继续循环；
            continue;
        }
        if (ctx->last) {
            break;
        }
    }
    for (cl = ctx->out; cl; cl = cl->next) {

        out_len += ngx_buf_size(cl->buf);
        //打印body内容
        int real_len = ngx_buf_size(cl->buf);
        replace_ngx_str_t(r, ctx, cl->buf->start, ngx_buf_size(cl->buf),&real_len,cl->next);
        if(real_len < ngx_buf_size(cl->buf)){
            cl->buf->last = cl->buf->pos + real_len;
        }
        //print_ngx_str_t("[cl->buf]", cl->buf->start, ngx_buf_size(cl->buf));
   
        if (cl->next != NULL) {
            //如果还有后续包，继续循环；
            continue;
        }
        if (ctx->last) {
            break;
        }
    }
    printnginxlog("out_len:%d\r\n",out_len);
    /*if(out_len > 0){
         ngx_chain_t* bufs = ctx->out;
         int body_len = 0;

        ngx_buf_t* buf = NULL;
        uint8_t* data_buf = NULL;

        data_buf = (uint8_t*) ngx_palloc(r->pool, out_len + 1);              //创建新buf存储body内容
        int buf_length = 0;

        while (bufs) {
            buf = bufs->buf;
            bufs = bufs->next;
            buf_length = buf->last - buf->pos;

            if (body_len + buf_length > out_len) {
                memcpy(data_buf + body_len, buf->pos, out_len - body_len);
                body_len = out_len;
                break;
            }
            memcpy(data_buf + body_len, buf->pos, buf->last - buf->pos);
            body_len += buf->last - buf->pos;
        }

        if (body_len) {
            data_buf[body_len] = '\0';
        }
        printf("data_buf:%s\r\n",data_buf);
        replace_ngx_str_t(r, data_buf, body_len);
        ngx_buf_t* b = ngx_create_temp_buf(r->pool, body_len);

        b->start = b->pos = data_buf;
        b->last = b->pos + body_len;

        ngx_chain_t *cl = ngx_alloc_chain_link(r->pool);
        cl->buf = b;
        cl->next = NULL;
        return ngx_http_next_body_filter(r, cl);
    }*/

    
    /* It doesn't output anything, return */
    if ((ctx->out == NULL) && (ctx->busy == NULL)) {
        printnginxlog("ngx_http_subs_body_filter break 3\n");
        return NGX_OK;
    }
    //修改后的body发送到客户端
    return ngx_http_subs_output(r, ctx, in);
failed:
    printnginxlog("[subs_filter] ngx_http_subs_body_filter error.");
    return NGX_ERROR;
}

static ngx_int_t ngx_http_subs_body_filter_init_context(ngx_http_request_t *r, ngx_chain_t *in) {
    ngx_http_subs_ctx_t *ctx;
    //printf("ngx_http_subs_body_filter_init_context\n");
    ctx = ngx_http_get_module_ctx(r, ngx_http_subs_filter_module);
    r->connection->buffered |= NGX_HTTP_SUB_BUFFERED;
    ctx->in = NULL;

    if (in) {
        //拷贝body内容到上下文中
        if (ngx_chain_add_copy(r->pool, &ctx->in, in) != NGX_OK) {
            return NGX_ERROR;
        }
    }
    ctx->last_out = &ctx->out;
    ctx->out_buf = NULL;

    return NGX_OK;
}

static ngx_int_t ngx_http_subs_body_filter_process_buffer(ngx_http_request_t *r, ngx_buf_t *b) {
    u_char *p, *last, *linefeed;
    ngx_int_t len, rc;
    ngx_http_subs_ctx_t *ctx;
    //printf("ngx_http_subs_body_filter_process_buffer\n");

    ctx = ngx_http_get_module_ctx(r, ngx_http_subs_filter_module);

    if (b == NULL) {
        return NGX_DECLINED;
    }

    p = b->pos;
    last = b->last;
    b->pos = b->last;

    /*printf(
                   "subs process in buffer: %p %uz, line_in buffer: %p %uz",
                   b, last - p,
                   ctx->line_in, (unsigned int)ngx_buf_size(ctx->line_in));*/
    //if (ngx_buf_size(ctx->line_in) > 0) {
    //            print_start2end("[cl->line_in]" ,ctx->line_in->start ,ctx->line_in->end );
    //}
    if ((last - p) == 0 && ngx_buf_size(ctx->line_in) == 0) {
        return NGX_OK;
    }

    if ((last - p) == 0 && ngx_buf_size(ctx->line_in) && ctx->last) {

        //printf(
        //               "the last zero buffer, try to do substitution");

        rc = ngx_http_subs_match(r, ctx);
        if (rc < 0) {
            return NGX_ERROR;
        }

        return NGX_OK;
    }

    while (p < last) {

        linefeed = memchr(p, LF, last - p); //查找/n字符

        //printf( "find linefeed: %p",
        //               linefeed);

        if (linefeed == NULL) {

            if (ctx->last) {
                linefeed = last - 1;
                //printf(
                //               "the last buffer, not find linefeed");
            }
        }

        if (linefeed) {

            len = linefeed - p + 1;

            if (buffer_append_string(ctx->line_in, p, len, r->pool) == NULL) {
                return NGX_ERROR;
            }

            p += len;

            rc = ngx_http_subs_match(r, ctx);
            if (rc < 0) {
                return NGX_ERROR;
            }

        } else {

            /* Not find linefeed in this chain, save the left data to line_in */
            if (buffer_append_string(ctx->line_in, p, last - p, r->pool)
                    == NULL) {
                return NGX_ERROR;
            }

            break;
        }
    }

    return NGX_OK;
}

/*
 * Do the substitutions from ctx->line_in
 * and output the chain buffers to ctx->out
 * */
static ngx_int_t ngx_http_subs_match(ngx_http_request_t *r, ngx_http_subs_ctx_t *ctx) {
    ngx_buf_t *src, *dst, *temp;
    ngx_log_t *log;
    ngx_int_t count, match_count;
    sub_pair_t *pairs, *pair;
    ngx_uint_t i;
    //printf("ngx_http_subs_match\n");


    count = 0;
    match_count = 0;

    log = r->connection->log;

    src = ctx->line_in;
    dst = ctx->line_dst;

    pairs = (sub_pair_t *) ctx->sub_pairs->elts;
    for (i = 0; i < ctx->sub_pairs->nelts; i++) {

        pair = &pairs[i];

        if (!pair->has_captured) {
            if (pair->sub.data == NULL) {
                if (ngx_http_script_run(r, &pair->sub, pair->sub_lengths->elts,
                        0, pair->sub_values->elts) == NULL) {
                    goto failed;
                }
            }

        } else {
            pair->sub.data = NULL;
            pair->sub.len = 0;
        }

        /* exchange the src and dst buffer */
        if (dst->pos != dst->last) {

            temp = src;
            src = dst;
            dst = temp;

            ngx_buffer_init(dst);
        }

        if ((!pair->regex)
                && ((ngx_uint_t) (src->last - src->pos) < pair->match.len)) {
            continue;
        }

        if (pair->once && pair->matched) {
            continue;
        }

        if (pair->sub.data == NULL && !pair->has_captured) {

            if (ngx_http_script_run(r, &pair->sub, pair->sub_lengths->elts, 0,
                    pair->sub_values->elts) == NULL) {
                goto failed;
            }
        }

        /* regex substitution */
        if (pair->regex || pair->insensitive) {
#if (NGX_PCRE)
            count = ngx_http_subs_match_regex_substituion(r, pair, src, dst);
            if (count == NGX_ERROR) {
                goto failed;
            }
#endif
        } else {
            /* fixed string substituion */
            count = ngx_http_subs_match_fix_substituion(r, pair, src, dst);
            if (count == NGX_ERROR) {
                goto failed;
            }
        }

        /* no match. */
        if (count == 0) {
            continue;
        }

        if (src->pos < src->last) {

            if (buffer_append_string(dst, src->pos, src->last - src->pos,
                    r->pool) == NULL) {
                goto failed;
            }

            src->pos = src->last;
        }

        /* match */
        match_count += count;
    }

    /* no match last time */
    if (dst->pos == dst->last) {
        dst = src;
    }

    if (ngx_http_subs_out_chain_append(r, ctx, dst) != NGX_OK) {
        goto failed;
    }

    ngx_buffer_init(ctx->line_in);
    ngx_buffer_init(ctx->line_dst);

    //printf("match counts: %i", (int)match_count);

    return match_count;

failed:

    //printf(
    //              "[subs_filter] ngx_http_subs_match error.");

    return -1;
}


#if (NGX_PCRE)

static ngx_int_t ngx_http_subs_match_regex_substituion(ngx_http_request_t *r, sub_pair_t *pair,
        ngx_buf_t *b, ngx_buf_t *dst) {
    ngx_str_t line;
    ngx_log_t *log;
    ngx_int_t rc, count = 0;
    //printf("ngx_http_subs_match_regex_substituion\n");

    log = r->connection->log;

    if (pair->captures == NULL || pair->ncaptures == 0) {
        pair->ncaptures = (NGX_HTTP_MAX_CAPTURES + 1) * 3;
        pair->captures = ngx_palloc(r->pool, pair->ncaptures * sizeof (int));
        if (pair->captures == NULL) {
            return NGX_ERROR;
        }
    }

    while (b->pos < b->last) {

        if (pair->once && pair->matched) {
            break;
        }

        line.data = b->pos;
        line.len = b->last - b->pos;

        rc = ngx_regex_exec(pair->match_regex, &line,
                (int *) pair->captures, pair->ncaptures);

        if (rc == NGX_REGEX_NO_MATCHED) {
            break;

        } else if (rc < 0) {
            printnginxlog(
                    " failed: %i",
                    (int) rc);
            print_ngx_str_t("on ", line.data, line.len);
            print_ngx_str_t("using ", pair->match.data, pair->match.len);
            return NGX_ERROR;
        }

        pair->matched++;
        count++;

        ngx_log_debug3(NGX_LOG_DEBUG_HTTP, log, 0,
                "regex match:%i, start:%d, end:%d ",
                rc, pair->captures[0], pair->captures[1]);

        if (pair->has_captured) {
            r->captures = pair->captures;
            r->ncaptures = pair->ncaptures;
            r->captures_data = line.data;

            if (ngx_http_script_run(r, &pair->sub, pair->sub_lengths->elts, 0,
                    pair->sub_values->elts) == NULL) {
                printnginxlog(
                        "[subs_filter] ngx_http_script_run error.");
                return NGX_ERROR;
            }
        }

        if (buffer_append_string(dst, b->pos, pair->captures[0],
                r->pool) == NULL) {
            return NGX_ERROR;
        }

        if (buffer_append_string(dst, pair->sub.data, pair->sub.len,
                r->pool) == NULL) {
            return NGX_ERROR;
        }

        b->pos = b->pos + pair->captures[1];
    }

    return count;
}
#endif

/*
 * Thanks to Laurent Ghigonis
 * Taken from FreeBSD
 * Find the first occurrence of the byte string s in byte string l.
 */
static void * subs_memmem(const void *l, size_t l_len, const void *s, size_t s_len) {
    register char *cur, *last;
    const char *cl = (const char *) l;
    const char *cs = (const char *) s;

    /* we need something to compare */
    if (l_len == 0 || s_len == 0) {
        return NULL;
    }

    /* "s" must be smaller or equal to "l" */
    if (l_len < s_len) {
        return NULL;
    }

    /* special case where s_len == 1 */
    if (s_len == 1) {
        return memchr(l, (int) *cs, l_len);
    }

    /* the last position where its possible to find "s" in "l" */
    last = (char *) cl + l_len - s_len;

    for (cur = (char *) cl; cur <= last; cur++) {
        if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0) {
            return cur;
        }
    }

    return NULL;
}

static ngx_int_t ngx_http_subs_match_fix_substituion(ngx_http_request_t *r,
        sub_pair_t *pair, ngx_buf_t *b, ngx_buf_t *dst) {
    u_char *sub_start;
    ngx_int_t count = 0;
    //printf("ngx_http_subs_match_fix_substituion\n");

    while (b->pos < b->last) {
        if (pair->once && pair->matched) {
            break;
        }

        sub_start = subs_memmem(b->pos, b->last - b->pos,
                pair->match.data, pair->match.len);
        if (sub_start == NULL) {
            break;
        }

        pair->matched++;
        count++;

        if (buffer_append_string(dst, b->pos, sub_start - b->pos,
                r->pool) == NULL) {
            return NGX_ERROR;
        }

        if (buffer_append_string(dst, pair->sub.data, pair->sub.len,
                r->pool) == NULL) {
            return NGX_ERROR;
        }

        //printf("fixed string match: %s\n", sub_start);

        b->pos = sub_start + pair->match.len;

        if ((ngx_uint_t) (b->last - b->pos) < pair->match.len)
            break;
    }

    return count;
}

static ngx_buf_t * buffer_append_string(ngx_buf_t *b, u_char *s, size_t len, ngx_pool_t *pool) {
    u_char *p;
    ngx_uint_t capacity, size;

    if (len > (size_t) (b->end - b->last)) {

        size = b->last - b->pos;

        capacity = b->end - b->start;
        capacity <<= 1;

        if (capacity < (size + len)) {
            capacity = size + len;
        }

        p = ngx_palloc(pool, capacity);
        if (p == NULL) {
            return NULL;
        }

        b->last = ngx_copy(p, b->pos, size);

        b->start = b->pos = p;
        b->end = p + capacity;
    }

    b->last = ngx_copy(b->last, s, len);

    return b;
}

static ngx_int_t ngx_http_subs_out_chain_append(ngx_http_request_t *r,
        ngx_http_subs_ctx_t *ctx, ngx_buf_t *b) {
    size_t len, capcity;
    //printf("ngx_http_subs_out_chain_append\n");

    if (b == NULL || ngx_buf_size(b) == 0) {
        return NGX_OK;
    }

    if (ctx->out_buf == NULL) {
        if (ngx_http_subs_get_chain_buf(r, ctx) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    while (1) {

        len = (size_t) ngx_buf_size(b);
        if (len == 0) {
            break;
        }

        capcity = ctx->out_buf->end - ctx->out_buf->last;

        if (len <= capcity) {
            ctx->out_buf->last = ngx_copy(ctx->out_buf->last, b->pos, len);
            b->pos += len;
            break;

        } else {
            ctx->out_buf->last = ngx_copy(ctx->out_buf->last,
                    b->pos, capcity);
        }

        b->pos += capcity;

        /* get more buffers */
        if (ngx_http_subs_get_chain_buf(r, ctx) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}

static ngx_int_t ngx_http_subs_get_chain_buf(ngx_http_request_t *r,
        ngx_http_subs_ctx_t *ctx) {
    ngx_chain_t *temp;
    ngx_http_subs_loc_conf_t *slcf;
    //printf("ngx_http_subs_get_chain_buf\n");

    slcf = ngx_http_get_module_loc_conf(r, ngx_http_subs_filter_module);

    if (ctx->free) {
        temp = ctx->free;
        ctx->free = ctx->free->next;

    } else {
        temp = ngx_alloc_chain_link(r->pool);
        if (temp == NULL) {
            return NGX_ERROR;
        }

        temp->buf = ngx_create_temp_buf(r->pool, slcf->bufs.size);
        if (temp->buf == NULL) {
            return NGX_ERROR;
        }

        temp->buf->tag = (ngx_buf_tag_t) & ngx_http_subs_filter_module;
        temp->buf->recycled = 1;

        /* TODO: limit the buffer number */
        ctx->bufs++;
    }

    temp->next = NULL;

    ctx->out_buf = temp->buf;
    *ctx->last_out = temp;
    ctx->last_out = &temp->next;

    return NGX_OK;
}

static ngx_int_t ngx_http_subs_output(ngx_http_request_t *r, ngx_http_subs_ctx_t *ctx,
        ngx_chain_t *in) {
    ngx_int_t rc;
    //printf("ngx_http_subs_output\n");
    /* ctx->out may not output all the data */
    //ctx->out为替换后新body内容，发送到客户端
    rc = ngx_http_next_body_filter(r, ctx->out);
    if (rc == NGX_ERROR) {
        return NGX_ERROR;
    }
    //发送内容成功后是否新body内容
#if defined(nginx_version) && (nginx_version >= 1001004)
    ngx_chain_update_chains(r->pool, &ctx->free, &ctx->busy, &ctx->out,
            (ngx_buf_tag_t) & ngx_http_subs_filter_module);
#else
    ngx_chain_update_chains(&ctx->free, &ctx->busy, &ctx->out,
            (ngx_buf_tag_t) & ngx_http_subs_filter_module);
#endif
    if (ctx->last) {
        r->connection->buffered &= ~NGX_HTTP_SUB_BUFFERED;
    }
    return rc;
}

static char * ngx_http_subs_filter(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_int_t n;
    ngx_uint_t i;
    ngx_str_t *value;
    ngx_str_t *option;
    sub_pair_t *pair;
    ngx_http_subs_loc_conf_t *slcf = conf;
    ngx_http_script_compile_t sc;
    printnginxlog("ngx_http_subs_filter\n");



    value = cf->args->elts;

    if (slcf->sub_pairs == NULL) {
        slcf->sub_pairs = ngx_array_create(cf->pool, 4, sizeof (sub_pair_t));
        if (slcf->sub_pairs == NULL) {
            return NGX_CONF_ERROR;
        }
    }

    pair = ngx_array_push(slcf->sub_pairs);
    if (pair == NULL) {
        return NGX_CONF_ERROR;
    }
    ngx_memzero(pair, sizeof (sub_pair_t));

    pair->match = value[1];

    n = ngx_http_script_variables_count(&value[2]);
    if (n != 0) {
        ngx_memzero(&sc, sizeof (ngx_http_script_compile_t));

        sc.cf = cf;
        sc.source = &value[2];
        sc.lengths = &pair->sub_lengths;
        sc.values = &pair->sub_values;
        sc.variables = n;
        sc.complete_lengths = 1;
        sc.complete_values = 1;

        if (ngx_http_script_compile(&sc) != NGX_OK) {
            return NGX_CONF_ERROR;
        }

        /* Dirty hack, if it has captured variables */
        if (sc.captures_mask) {
            pair->has_captured = 1;
        }

    } else {
        pair->sub = value[2];
    }

    if (cf->args->nelts > 3) {
        option = &value[3];
        for (i = 0; i < option->len; i++) {

            switch (option->data[i]) {
                case 'i':
                    pair->insensitive = 1;
                    break;

                case 'o':
                    pair->once = 1;
                    break;

                case 'r':
                    pair->regex = 1;
                    break;

                case 'g':
                default:
                    continue;
            }
        }
    }

    if (pair->regex || pair->insensitive) {
        if (ngx_http_subs_filter_regex_compile(pair, &sc, cf) == NGX_ERROR) {
            return NGX_CONF_ERROR;
        }
    }

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_subs_filter_regex_compile(sub_pair_t *pair,
        ngx_http_script_compile_t *sc, ngx_conf_t *cf) {
    printnginxlog("ngx_http_subs_filter_regex_compile\n");
    /* Caseless match can only be implemented in regex. */
#if (NGX_PCRE)
    u_char errstr[NGX_MAX_CONF_ERRSTR];
    ngx_int_t n, options;
    ngx_str_t err, *value;
    ngx_uint_t mask;

    value = cf->args->elts;

    err.len = NGX_MAX_CONF_ERRSTR;
    err.data = errstr;

    options = (pair->insensitive ? NGX_REGEX_CASELESS : 0);

    /* make nginx-0.8.25+ happy */
#if defined(nginx_version) && nginx_version >= 8025
    ngx_regex_compile_t rc;

    rc.pattern = pair->match;
    rc.pool = cf->pool;
    rc.err = err;
    rc.options = options;

    if (ngx_regex_compile(&rc) != NGX_OK) {

        print_ngx_str_t("ngx_conf_log_error ", rc.err.data, rc.err.len);
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%V", &rc.err);
        return NGX_ERROR;
    }

    pair->match_regex = rc.regex;

#else
    pair->match_regex = ngx_regex_compile(&pair->match, options,
            cf->pool, &err);
#endif

    if (pair->match_regex == NULL) {
        print_ngx_str_t("ngx_conf_log_error ", err.data, err.len);
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%V", &err);
        return NGX_ERROR;
    }

    n = ngx_http_subs_regex_capture_count(pair->match_regex);

    if (pair->has_captured) {
        mask = ((1 << (n + 1)) - 1);
        if (mask < sc->captures_mask) {
            /*printf(
                               "You want to capture too many regex substrings, "
                               "more than %i",
                               (int)n);
                                                print_ngx_str_t("in ",value[2].data,value[2].len );*/
            return NGX_ERROR;
        }
    }
#else
    //print_ngx_str_t("the using of the regex " ,pair->match.data,pair->match.len );
    //printf("requires PCRE library\n");

    return NGX_ERROR;
#endif

    return NGX_OK;
}


#if (NGX_PCRE)

static ngx_int_t ngx_http_subs_regex_capture_count(ngx_regex_t *re) {
    int rc, n;

    //printf("ngx_http_subs_regex_capture_count\n");
    n = 0;

#if defined(nginx_version) && nginx_version >= 1002002
    rc = pcre_fullinfo(re->code, NULL, PCRE_INFO_CAPTURECOUNT, &n);
#elif defined(nginx_version) && nginx_version >= 1001012
    rc = pcre_fullinfo(re->pcre, NULL, PCRE_INFO_CAPTURECOUNT, &n);
#else
    rc = pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT, &n);
#endif

    if (rc < 0) {
        return (ngx_int_t) rc;
    }

    return (ngx_int_t) n;
}
#endif

static void * ngx_http_subs_create_conf(ngx_conf_t *cf) {
    ngx_http_subs_loc_conf_t *conf;
    //printf("ngx_http_subs_create_conf\n");

    conf = ngx_pcalloc(cf->pool, sizeof (ngx_http_subs_loc_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->sub_pairs = NULL;
     *     conf->types = {NULL, 0};
     *     conf->types_keys = NULL;
     *     conf->bufs.num = 0;
     */

    conf->line_buffer_size = NGX_CONF_UNSET_SIZE;
    conf->bypass = NGX_CONF_UNSET_PTR;

    return conf;
}

static char * ngx_http_subs_merge_conf(ngx_conf_t *cf, void *parent, void *child) {
    ngx_http_subs_loc_conf_t *prev = parent;
    ngx_http_subs_loc_conf_t *conf = child;
    //printf("ngx_http_subs_merge_conf\n");

    if (conf->sub_pairs == NULL) {
        if (prev->sub_pairs == NULL) {
            conf->sub_pairs = ngx_array_create(cf->pool, 4, sizeof (sub_pair_t));
            if (conf->sub_pairs == NULL) {
                return NGX_CONF_ERROR;
            }
        } else {
            conf->sub_pairs = prev->sub_pairs;
        }
    }

    ngx_conf_merge_ptr_value(conf->bypass,
            prev->bypass, NULL);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
            &prev->types_keys, &prev->types,
            ngx_http_html_default_types)
            != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    ngx_conf_merge_size_value(conf->line_buffer_size,
            prev->line_buffer_size, 8 * ngx_pagesize);

    /* Default total buffer size is 128k */
    ngx_conf_merge_bufs_value(conf->bufs, prev->bufs,
            (128 * 1024) / ngx_pagesize, ngx_pagesize);

    return NGX_CONF_OK;
}

uint8_t* GetBodyData(ngx_http_request_t* r) {
    ngx_chain_t* bufs = r->request_body->bufs;
    size_t body_len = 0;
    ngx_buf_t* buf = NULL;
    uint8_t* data_buf = NULL;
    size_t content_length = 0;
    if(r->headers_in.content_length == NULL){
        return NULL;
    }

    content_length = atoi((char*) (r->headers_in.content_length->value.data));
    data_buf = (uint8_t*) ngx_palloc(r->pool, content_length + 1); //创建新buf存储body内容
    size_t buf_length = 0;

    while (bufs) {
        buf = bufs->buf;
        bufs = bufs->next;
        buf_length = buf->last - buf->pos;

        if (body_len + buf_length > content_length) {
            memcpy(data_buf + body_len, buf->pos, content_length - body_len);
            body_len = content_length;
            break;
        }
        memcpy(data_buf + body_len, buf->pos, buf->last - buf->pos);
        body_len += buf->last - buf->pos;
    }

    if (body_len) {
        data_buf[body_len] = '\0';
    }
    printnginxlog("data:%s\n", data_buf);
    //ngx_pfree(r->pool, data_buf);
    return data_buf;
}
/**
 * @brief Get the HTTP body data from the ngx_http_request_t struct.
 * @warning DONNOT release the return pointer.
 * @param[in] ngx_http_request_t * r -
 * The HTTP request of NGINX struct which holds the HTTP data.
 * @param[out] size_t * body_len - The body data length will stored here.
 * @return uint8_t* - A pointer to a memory where 
 * stored the HTTP body raw binary data.
 * The memory is allocated from nginx memory pool,
 * so the caller don't need to warry about the memory release work.
 */ 
/*
 * 
 * 
 * 
/getsites  {"host":""}--> {alldata};
/getsite  {"host":""}/{"id":""}--> {site};
/getform  {"id":""} --> {form};
/getfield {"id":""} --> {field};

/addsite {"host":"","proxyhost":"","key":"","desc":""} --> {"id":"123"}
/addform {site_id:'123',url:'/add.xx',desc:''} --> {id:'123'}
/addfield {form_id:'123',field_name:'name="111"/r/n/r/n',type:'',desc:''} --> {id:'123'}

/modifysite {id:'123',host:'',proxyhost:'',key:'',desc:''} --> {success:'true'}
/modifyform {id:'123',site_id:'123',url:'/add.xx',desc:''} --> {success:'true'}
/modifyfield {id:'123',form_id:'123',field_name:'name="111"/r/n/r/n',desc:''} --> {success:'true'}

/deletesite {id:'123',host:''} --> {success:'true'}
/deleteform {id:'123',url:'/add.xx'} --> {success:'true'}
/deletefield {id:'123',field_name:'name="111"/r/n/r/n'} --> {success:'true'}

 */
static void get_raw_http_body(ngx_http_request_t* r) {/*{{{*/
    printnginxlog("get_raw_http_body\n");
    ngx_chain_t* bufs = r->request_body->bufs;
    size_t body_len = 0;

    ngx_buf_t* buf = NULL;
    uint8_t* data_buf = NULL;
    size_t content_length = 0;

    if (r->headers_in.content_length == NULL && r->method == NGX_HTTP_POST) {
        printnginxlog("get_raw_http_body content_length NULL\n");
        return ;
    }

    // malloc space for data_buf
    char server[256] = {0};
    char url[1024] = {0};
    
    if(r->headers_in.server.len > sizeof(server)){
        return ;
    }
    memcpy(server,(char*)r->headers_in.server.data,r->headers_in.server.len);
    if(r->unparsed_uri.len > sizeof(url)){
        return ;
    }
    memcpy(url,(char*)r->unparsed_uri.data, r->unparsed_uri.len);
    
    printnginxlog("server:%s\r\n", server);
    printnginxlog("url:%s\r\n", url);
    
    if(strcmp(server,"getway.paraview.cn")==0){
        
        
        
        ngx_int_t rc = NGX_OK;
        ngx_chain_t *out;
        char *buf = NULL;
        if(strcmp(url,"/getsites") == 0){
            //data_buf = GetBodyData(r);
            buf = GetSitePolicyJson(NULL);
        }else if(strcmp(url,"/getsite") == 0){
            data_buf = GetBodyData(r);
            buf = GetSiteJson((char*)data_buf);
        }else if(strcmp(url,"/getform") == 0){
            data_buf = GetBodyData(r);
            buf = GetFormJson((char*)data_buf);
        }else if(strcmp(url,"/getfield") == 0){
            data_buf = GetBodyData(r);
            buf = GetFieldJson((char*)data_buf);
        }else if(strcmp(url,"/getforms") == 0){
            data_buf = GetBodyData(r);
            buf = GetFormsJson((char*)data_buf);
        }else if(strcmp(url,"/getfields") == 0){
            data_buf = GetBodyData(r);
            buf = GetFieldsJson((char*)data_buf);
        }else if(strcmp(url,"/addsite") == 0){
            data_buf = GetBodyData(r);
            buf = AddSite((char*)data_buf);
        }else if(strcmp(url,"/addform") == 0){
            data_buf = GetBodyData(r);
            buf = AddForm((char*)data_buf);
        }else if(strcmp(url,"/addfield") == 0){
            data_buf = GetBodyData(r);
            buf = AddField((char*)data_buf);
        }else if(strcmp(url,"/modifysite") == 0){
            data_buf = GetBodyData(r);
            buf = ModifySite((char*)data_buf);
        }else if(strcmp(url,"/modifyform") == 0){
            data_buf = GetBodyData(r);
            buf = ModifyForm((char*)data_buf);
        }else if(strcmp(url,"/modifyfield") == 0){
            data_buf = GetBodyData(r);
            buf = ModifyField((char*)data_buf);
        }else if(strcmp(url,"/deletesite") == 0){
            data_buf = GetBodyData(r);
            buf = DeleteSite((char*)data_buf);
        }else if(strcmp(url,"/deleteform") == 0){
            data_buf = GetBodyData(r);
            buf = DeleteForm((char*)data_buf);
        }else if(strcmp(url,"/deletefield") == 0){
            data_buf = GetBodyData(r);
            buf = DeleteField((char*)data_buf);
        }else{
            buf = (char*)malloc(strlen("ECP getway")+1);
            memset(buf,0,strlen("ECP getway")+1);
            strcpy(buf,"ECP getway");
        }
        out = createchain((unsigned char*) buf, (int) strlen(buf), r->pool);
        

        r->headers_out.status = NGX_HTTP_OK;
        r->headers_out.content_length_n = strlen(buf);
        r->headers_out.content_type.len = sizeof("application/json") - 1;
        r->headers_out.content_type.data = (u_char *) "application/json";

        rc = ngx_http_send_header(r);

        if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
        {
            ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
            return ;
        }

        ngx_http_output_filter(r, out);
        
        free(buf);
        if(data_buf != NULL){
            ngx_pfree(r->pool, data_buf);
        }
        InitPostPolicy();
        return;
    }
    
    FormPolicy* pPostPolicy = GetPostPolicy(server,url);
    if(pPostPolicy == NULL){
        printnginxlog("----------------not find policy----------------\r\n");
        return ;
    }

    if(r->method == NGX_HTTP_POST){

        content_length = atoi((char*) (r->headers_in.content_length->value.data));
        data_buf = (uint8_t*) ngx_palloc(r->pool, content_length + 1);              //创建新buf存储body内容
        size_t buf_length = 0;

        while (bufs) {
            buf = bufs->buf;
            bufs = bufs->next;
            buf_length = buf->last - buf->pos;

            if (body_len + buf_length > content_length) {
                memcpy(data_buf + body_len, buf->pos, content_length - body_len);
                body_len = content_length;
                break;
            }
            memcpy(data_buf + body_len, buf->pos, buf->last - buf->pos);
            body_len += buf->last - buf->pos;
        }

        if (body_len) {
            data_buf[body_len] = '\0';
        }
        //body原长度
        printnginxlog("old body len:%d\n", (int) body_len);
        int i = 0;
        for(i = 0 ; i < pPostPolicy->fieldPolicyLen ; i ++){
        //修改body内容
            if(findstrstr(data_buf,pPostPolicy->pFieldPolicy[i].field_name,body_len) == NULL){
                if(strstr(pPostPolicy->pFieldPolicy[i].field_name,"*") != NULL){
                    if(pPostPolicy->pFieldPolicy[i].type == POSTURLSW 
                            || pPostPolicy->pFieldPolicy[i].type == POSTFORMSW
                            || pPostPolicy->pFieldPolicy[i].type == POSTURLSPLIT
                            || pPostPolicy->pFieldPolicy[i].type == URLJSON
                            ){
                    
                    }else{
                        continue;
                    }
                }else{
                    continue;
                }
            }
            int iconv = 0;
            if(strcmp(pPostPolicy->url,"/sys/usersetup/new/opttree.xt?*") == 0 && strcmp(pPostPolicy->pFieldPolicy[i].field_name,"&name=") == 0){
                iconv = 1;
            }
            printnginxlog("start repacle:%s\n",pPostPolicy->pFieldPolicy[i].field_name);
            uint8_t * newdata_buf = replaceBuf(r, data_buf, &body_len,pPostPolicy->pFieldPolicy[i],iconv);
            if(newdata_buf == data_buf){
                continue;
            }
            ngx_pfree(r->pool, data_buf);
            data_buf = (uint8_t*) ngx_palloc(r->pool, body_len + 1); 
            memset((char*)data_buf, 0, body_len + 1);
            memcpy((char*)data_buf,newdata_buf,body_len);
            ngx_pfree(r->pool, newdata_buf);

            printnginxlog("new body len:%d\n", (int) body_len);
            printnginxlog("new body len:%d\n", (int) body_len);



        }
        printf("replace buf:%s\n", data_buf);
        printnginxlog("-------------------------------end\n");

        //替换body长度
        unsigned char * length = (unsigned char*) ngx_palloc(r->pool, 10);
        memset(length, 0, 10);
        sprintf((char*) length, "%d", (int) body_len);
        ngx_str_t contentlength;
        contentlength.len = strlen((char*) length);
        contentlength.data = length;
        r->headers_in.content_length->value = contentlength;
        r->headers_in.content_length_n = body_len;

        r->request_body->bufs = createchain((unsigned char*) data_buf, (int) body_len, r->pool);     //替换body内容
    }else if(r->method == NGX_HTTP_GET){
        int url_len = strlen(url);
        char *url_buf = url;              
        
       
        printnginxlog("old url len:%d\n", (int) url_len);
        int i = 0;
        for(i = 0 ; i < pPostPolicy->fieldPolicyLen ; i ++){
        //修改body内容
            if(findstrstr((u_char*)url_buf,pPostPolicy->pFieldPolicy[i].field_name,url_len) == NULL){
                continue;
            }
            printnginxlog("start repacle:%s\n",pPostPolicy->pFieldPolicy[i].field_name);
            char * newdata_buf = replaceGetBuf(r, url_buf, &url_len,pPostPolicy->pFieldPolicy[i]);
            if(newdata_buf == url_buf){
                continue;
            }
            ngx_pfree(r->pool, url_buf);
            url_buf = (char*) ngx_palloc(r->pool, url_len + 1); 
            memset((char*)url_buf, 0, url_len + 1);
            memcpy((char*)url_buf,newdata_buf,url_len);
            ngx_pfree(r->pool, newdata_buf);

            printnginxlog("new body len:%d\n", (int) url_len);
            printnginxlog("new body len:%d\n", (int) url_len);



        }
        printnginxlog("replace buf:%s\n", url_buf);
        printnginxlog("-------------------------------end\n");
        malloc_ngx_str_t(r,&r->unparsed_uri,url_buf);
    }
    return ;
}/*}}}*/



/**
 * Process the client request.
 * The client post data has stored in <code>r</code>
 */
int waiting_more_body = 0;
static void p2s_urlquery_process_handler(ngx_http_request_t *r) {/*{{{*/
    printnginxlog("p2s_urlquery_process_handler\n");
    //ngx_int_t rc = NGX_OK;
    ngx_http_subs_loc_conf_t *conf = NULL;
    conf = (ngx_http_subs_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_subs_filter_module);
    if (conf == NULL) {
        printnginxlog("p2s_urlquery_process_handler not conf\n");
        ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    
    
    get_raw_http_body(r);
    printnginxlog("----------------------http body data end-------------------\n");

    if (waiting_more_body) {
        waiting_more_body = 0;
        ngx_http_core_run_phases(r);
        printnginxlog("ngx_http_core_run_phases\n");
    } else {
        ngx_http_finalize_request(r, NGX_DONE);
        printnginxlog("ngx_http_finalize_request\n");
    }
    
    return;
}/*}}}*/

static ngx_int_t ngx_http_p2s_urlquery_handler(ngx_http_request_t *r) {/*{{{*/
    printnginxlog("ngx_http_p2s_urlquery_handler\n");

    if (r->method != NGX_HTTP_POST && r->method != NGX_HTTP_GET) {       //过滤非post请求
        printnginxlog("ngx_http_p2s_urlquery_handler not NGX_HTTP_POST\n");
        return NGX_DECLINED;
    }
    printnginxlog("ngx_http_p2s_urlquery_handler NGX_HTTP_POST\n");
    ngx_int_t rc = NGX_DONE;
    
    rc = ngx_http_read_client_request_body(r, p2s_urlquery_process_handler);    //处理request body
    if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        printnginxlog("ngx_http_p2s_urlquery_handler not NGX_DONE\n");
        return rc;
    }
    printnginxlog("ngx_http_p2s_urlquery_handler NGX_DONE rc=%d\n",(int)rc);
    if(rc == NGX_AGAIN){
        waiting_more_body = 1;
        return NGX_AGAIN;
    }
    
    
    return NGX_DECLINED;
}/*}}}*/

static ngx_int_t ngx_http_subs_filter_init(ngx_conf_t *cf) {

    printnginxlog("ngx_http_subs_filter_init\n");
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_subs_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_subs_body_filter;
    
    InitEncrypt();
    InitPostPolicy();


    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);
    //Location请求地址重写阶段
    //这个主要处理location block的rewrite。
    
    if (h == NULL) {
        printnginxlog("ngx_http_subs_filter_init NGX_ERROR\n");
        return NGX_ERROR;
    }

    *h = ngx_http_p2s_urlquery_handler;
    printnginxlog("ngx_http_subs_filter_init end\n");

    return NGX_OK;
}
