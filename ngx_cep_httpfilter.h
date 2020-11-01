/* 
 * File:   ngx_cep_httpfilter.h
 * Author: Administrator
 *
 * Created on 2014年10月10日, 上午10:47
 */

#ifndef NGX_CEP_HTTPFILTER_H
#define	NGX_CEP_HTTPFILTER_H

#include <ngx_http.h>
#include <nginx.h>
#include "ngx_http_subs_filter_module.h"
#include "ngx_cep_policy.h"

 
#define CHAIN_BUFFER_SIZE 4096 
    

char * findstrstr(u_char * src, char * dest, int len);
int findEncodeHead(u_char * src, u_char * dest, int len);
int printnginxlog(const char* format, ...) ;
int replace_ngx_str_t(ngx_http_request_t *r, ngx_http_subs_ctx_t *ctx, u_char * data, int len,int *real_len,ngx_chain_t  *next);
void print_ngx_str_t(char * description, u_char * data, int len);
void print_start2end(char * description, u_char * start, u_char *end);
int getmemlen(char *start, char *end);
char * mmstrstr(char *start, char *end, char *substr);
char* memstrcpy(char *start, char *end);
uint8_t* replaceBuf(ngx_http_request_t* r, uint8_t* data_buf, size_t *body_len,FieldPolicy filterName,int iconv);
char * replaceGetBuf(ngx_http_request_t* r, char* data_buf, int *body_len,FieldPolicy filterName);
ngx_chain_t * createchain(unsigned char * str, int len, ngx_pool_t *pool);
int InitEncrypt();
char * ReplaceToHtmlCode(char * pdata, int * plen);
int replace_counts(unsigned char * data, int len);
int replace_post(unsigned char * data, int len);
int ReplaceHtmlCode(char *input,int *plen);
void ReplaceBase64Code(char *input,int *plen);
void malloc_ngx_str_t(ngx_http_request_t* r,ngx_str_t *src , char * dest);
#endif	/* NGX_CEP_HTTPFILTER_H */

