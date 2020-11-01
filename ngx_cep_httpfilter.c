#include "ngx_cep_httpfilter.h"
#include "policymanage.h"
#include <iconv.h>

typedef struct _KEY_HEADER{
    char key[32];
    char header[2];
}KEY_HEADER,*PKEY_HEADER;

int Encrypt_AES256(char * input, char * key);
int Decrypt_AES256(char * input, char * key,int *len);
int InitKeyHeader();
int GetKeyHeader(PKEY_HEADER pKeyHeader);
int SetKeyHeader(PKEY_HEADER pKeyHeader);

char key[]="11223344556677881122334455667788"; 


int printnginxlog(const char* format, ...) {
    char sprint_buf[10240];
    va_list args;
    int n;
    FILE *fa;

    fa = fopen("/etc/nginx.log", "a+");
    if (fa == NULL) {
        return 0;
    }
    memset(sprint_buf,0,sizeof(sprint_buf));

    va_start(args, format);
    n = vsprintf(sprint_buf, format, args);
    va_end(args);

    fprintf(fa, sprint_buf);
    fclose(fa);
    return 0;
}


void print_ngx_str_t(char * description, u_char * data, int len) {
    FILE *fa;
    if(data == NULL){
        return ;
    }
    fa = fopen("/etc/nginx.log", "a+");
    if (fa == NULL) {
        return;
    }
    fprintf(fa, "%s\t", description);
    //printf("%s\t", description);
    u_char * ptemp;
    for (ptemp = data; ptemp != (data + len); ptemp++) {
        fprintf(fa, "%c", *ptemp);
        //printf("%c", *ptemp);
    }
    fprintf(fa, "\n");
    //printf("\n");
    fclose(fa);
}

void print_start2end(char * description, u_char * start, u_char *end) {
    FILE *fa;
    fa = fopen("/etc/nginx.log", "a+");
    if (fa == NULL) {
        return;
    }
    fprintf(fa, "%s\t", description);
    printf("%s\t", description);
    u_char * ptemp;
    for (ptemp = start; ptemp != end; ptemp++) {
        fprintf(fa, "%c", *ptemp);
        printf("%c", *ptemp);
    }
    fprintf(fa, "\n");
    printf("\n");
    fclose(fa);
}

int InitEncrypt(){
    InitKeyHeader();
    return 1;
}

int findEncodeHead(u_char * src, u_char * dest, int len) {
    u_char *pSrc = src;
    u_char *pDest = dest;
    if (src == NULL) {
        return 0;
    }
    int i = 0;
    for (i = 0; i < len; i++) {
        if (pSrc[i] != pDest[i]) {
            return 0;
        }
    }
    return 1;
}
char * findstrstr(u_char * src, char * dest, int len) {
    u_char *pSrc = src;
    u_char *pDest = (u_char*)dest;
    if (src == NULL) {
        return NULL;
    }
    int i = 0,j=0;
    int dlen = strlen((char*)dest);
    for (i = 0; i < len; i++) {
        int isfind = 1;
        for(j = 0 ; j < dlen ; j ++){
            if (pSrc[i+j] != pDest[j]) {
                isfind = 0;
            }
        }
        if(isfind == 1){
            return (char*)src + i;
        }
    }
    return NULL;
}
char * findstrchar(u_char * src, u_char dest, int len) {
    if (src == NULL) {
        return NULL;
    }
    int i = 0;
    for (i = 0; i < len; i++) {
        
        if(*(src+i) == dest){
            return (char*)src + i;
        }
    }
    return NULL;
}
u_char startCode = 0;
int encodetype = 0;
int endCode = 0;
int replace_ngx_str_t(ngx_http_request_t *r, ngx_http_subs_ctx_t *ctx, u_char * data, int len,int *real_len,ngx_chain_t  *next) {
    u_char szEncodeHead[3] = {0};
    u_char szEncodeHead1[3] = {0};
    u_char szEncodeHead2[8] = {0};
    u_char * ptemp=NULL;
    int find = 0;
    int i = 0;
    int curlen = 0;
    u_char *findp = NULL;
    int havepacket = 0;
    char url[10240] = {0};
    int end_len = 0;
    int findStartHead = 0;
    int headLen = 2;
    int headtype = 0;
    
    
    
    szEncodeHead[0] = 0xff;
    szEncodeHead[1] = 0xfe;
    szEncodeHead[2] = 0;
    
    szEncodeHead1[0] = '[';
    szEncodeHead1[1] = 0x1f;
    szEncodeHead1[2] = 0;
    
    strcpy((char*)szEncodeHead2,"[\\u001f");
    
    if(endCode != 0){
        if(*data == szEncodeHead[1] || *data == szEncodeHead1[1]){
            findStartHead = 1;
        }
    }else{
    if(next != NULL){
        if(*(data+len-1) == szEncodeHead[0] && *(next->buf->start) == szEncodeHead[1]){
            endCode = 1;
        }else if(*(data+len-1) == szEncodeHead1[0] && *(next->buf->start) == szEncodeHead1[1]){
            endCode = 2;
        }
        
        if(endCode != 0){
            len --;
            *real_len = *real_len -1;
        }
    }
    }

    if (ctx == NULL) {
        return 0;
    }
    
    
    if(data == NULL){
        return 0;
    }
    if (len <= (int) strlen((char*) szEncodeHead)) {
        return 0;
    }
    memcpy(url, r->unparsed_uri.data, r->unparsed_uri.len);
    printnginxlog("replace_ngx_str_t url = %s\n", url);
    printnginxlog("replace_ngx_str_t find encode heand %s\n", szEncodeHead);
    
    
    if(ctx->lastPacketLen > 0){
        findp = data;
    }else if(findStartHead == 1){
        findp = data + 1;
        endCode = 0;
        find = 1;
        ptemp = data; 
    }else{
    for (ptemp = data; ptemp != (data + len - strlen((char*) szEncodeHead)); ptemp++) {
        if (findEncodeHead(ptemp, szEncodeHead, strlen((char*) szEncodeHead)) == 1) {
            printnginxlog("is find encode heand\n");
            find = 1;
            headtype = 0;
            findp = ptemp+strlen((char*) szEncodeHead);
            encodetype = 0;
            int count=1;
            while(count <= curlen){
                if(*(ptemp -count) == '\'' || *(ptemp -count) == '\"' || *(ptemp -count) == '>' || *(ptemp -count) == '('|| *(ptemp -count) == '='|| *(ptemp -count) == ' '){
                    startCode = *(ptemp -count);
                    break;
                }
                count ++;
            }
            
            
            break;
        }else if (findEncodeHead(ptemp, szEncodeHead1, strlen((char*) szEncodeHead1)) == 1) {
            printnginxlog("is find encode1 heand\n");
            find = 1;
            headtype = 1;
            findp = ptemp+strlen((char*) szEncodeHead1);
            encodetype = 1;
            int count=1;
            while(count <= curlen){
                if(*(ptemp -count) == '\'' || *(ptemp -count) == '\"' || *(ptemp -count) == '>' || *(ptemp -count) == '('|| *(ptemp -count) == '='|| *(ptemp -count) == ' '){
                    startCode = *(ptemp -count);
                    break;
                }
                count ++;
            }
            
            
            break;
        }else if (findEncodeHead(ptemp, szEncodeHead2, strlen((char*) szEncodeHead2)) == 1) {
            printnginxlog("is find encode2 heand\n");
            find = 1;
            headtype = 2;
            findp = ptemp+strlen((char*) szEncodeHead2);
            headLen = strlen((char*) szEncodeHead2);
            encodetype = 1;
            int count=1;
            while(count <= curlen){
                if(*(ptemp -count) == '\'' || *(ptemp -count) == '\"' || *(ptemp -count) == '>' || *(ptemp -count) == '('|| *(ptemp -count) == '='|| *(ptemp -count) == ' '){
                    startCode = *(ptemp -count);
                    break;
                }
                count ++;
            }
            
            
            break;
        }
        
        curlen++;
    }
    }
    int data_len = 0;
    if (find == 1 || ctx->lastPacketLen > 0) {
        
        end_len = len - getmemlen((char*)data,(char*)findp);
        char *endbuf = NULL;
        if(startCode == '\''){
                endbuf = findstrstr(findp,"\'",end_len);
        }else if(startCode == '\"'){
                endbuf = findstrstr(findp,"\"",end_len);
        }else if(startCode == '>'){
                endbuf = findstrstr(findp,"<",end_len);
         }else if(startCode == '('){
                endbuf = findstrstr(findp,")",end_len);
         }else if(startCode == '='){
                endbuf = findstrstr(findp,"&",end_len);
         }else if(startCode == ' '){
                endbuf = findstrstr(findp,"<",end_len);
         }else if(findStartHead == 1){
             endbuf = (char*)data+len;
         }
        
        if(endbuf == NULL){
            havepacket = 1;
            endbuf = (char*)findp + end_len;
            if(findstrstr(findp,(char*)szEncodeHead,end_len) != NULL || findstrstr(findp,(char*)szEncodeHead1,end_len) != NULL){
                havepacket = 0;
            }
        }
        
            endbuf = mmstrstr((char*)findp, endbuf,(char*) szEncodeHead);
            endbuf = mmstrstr((char*)findp, endbuf,(char*) szEncodeHead1);
            
            
            endbuf = mmstrstr((char*)findp, endbuf,"<");
            endbuf = mmstrstr((char*)findp, endbuf," ");
            endbuf = mmstrstr((char*)findp, endbuf,"\n");
            endbuf = mmstrstr((char*)findp, endbuf,"\t\n");
            endbuf = mmstrstr((char*)findp, endbuf,"&nbsp;");
            endbuf = mmstrstr((char*)findp, endbuf,"(");
            endbuf = mmstrstr((char*)findp, endbuf,"/");
            endbuf = mmstrstr((char*)findp, endbuf,"[");
            endbuf = mmstrstr((char*)findp, endbuf,":");
            endbuf = mmstrstr((char*)findp, endbuf,"-");
            endbuf = mmstrstr((char*)findp, endbuf,"\241\274");//〖
            endbuf = mmstrstr((char*)findp, endbuf,"\241\275");//〖
            endbuf = mmstrstr((char*)findp, endbuf,"]");
            
            char end_flag[2] = {0};
            int find_flag = 0;
            end_flag[0] = 0xfd;
            end_flag[1] = 0;
            if(mmstrstr((char*)findp, endbuf,end_flag) != endbuf){
                endbuf = mmstrstr((char*)findp, endbuf,end_flag) ;
                find_flag = 1;
            }
            
            
        
        
        if(havepacket == 0){
            data_len = getmemlen((char*)findp, endbuf);
            if(data_len >= 3){
                if(*(endbuf-1) == '.' && *(endbuf-2) == '.' && *(endbuf-3) == '.'){
                    endbuf = endbuf -3;
                    data_len = data_len -3;
                }else if(*(endbuf-1) == '>' && *(endbuf-2) == '/' && *(endbuf-3) == '\''){
                    endbuf = endbuf -3;
                    data_len = data_len -3;
                }
            }
        }else{
            data_len = getmemlen((char*)findp, endbuf);
        }
        end_len = len - getmemlen((char*)data,(char*)endbuf);
        
        if(ctx->lastPacketLen <= 0){
            printnginxlog("curlen:%d, data_len:%d, len:%d\n", curlen, data_len, len);
            if (curlen + data_len > len) {
                return 0;
            }
            unsigned char * encode_data = (unsigned char*) ngx_palloc(r->pool, data_len + 1);
            memset(encode_data, 0, data_len + 1);
            unsigned char * p = encode_data;
            memcpy(p, (char*)findp, data_len);

            
            p = encode_data;
            int outData_len = data_len;
            ReplaceHtmlCode((char*)p,&outData_len);
            /*printnginxlog("Encode content :");
            for (i = 0; i < outData_len; i++) {
                printnginxlog("%02X ", *p);
                p++;
            }*/
            printnginxlog("\n");
            p = encode_data;
            
            if(encodetype == 1 ){
                ReplaceBase64Code((char*)p,&outData_len);
            }
            
            Decrypt_AES256((char*)p,key,&outData_len);

            if(havepacket == 1){
                ctx->lastPacketLen = data_len;
                ctx->lastDePacketLen = outData_len;
                ctx->lastPacket = (unsigned char*) ngx_palloc(r->pool, data_len + 1);
                memset(ctx->lastPacket, 0, data_len + 1);
                memcpy(ctx->lastPacket, (char*)findp, data_len);
            }

            printnginxlog("encode_data:%s\n", encode_data);
            printnginxlog("headtype:%d\n", headtype);
            if(headtype == 2){
                /*int jsonlen = outData_len ;
                
                unsigned char * json_data = (unsigned char*) ngx_palloc(r->pool, jsonlen + 1);
                memset(json_data, 0, jsonlen + 1);
                
                unsigned char *pi1    =  encode_data;
                unsigned char **pi2    =  &pi1;
                unsigned char *po1    =  json_data;
                unsigned char **po2    =  &po1;
                size_t           ilen        = outData_len;
                size_t           olen    = jsonlen;

                iconv_t conveter;

               if((conveter = iconv_open("gbk", "utf-8")) == (iconv_t)-1)
               {

                   printnginxlog("iconv open fail \n");
                   return 1;
               }
               else
               {
                   printnginxlog("iconv open ok \n");    
               }

               iconv(conveter,(char**)pi2,&ilen,(char**)po2,&olen);

                printnginxlog("json_data:%s\n", json_data);
                ngx_pfree(r->pool, encode_data);
                encode_data = json_data;
                outData_len = strlen((char*)json_data);*/
            }
            memcpy((char*)ptemp,(char*)encode_data,outData_len);
            //ptemp += outData_len;
            data_len +=(headLen+find_flag);
            if(findStartHead == 1){
                data_len --;
            }
            int offset = (data_len-outData_len);
            *real_len -= offset;
            memcpy((char*)ptemp+outData_len,(char*)ptemp+data_len,end_len);
            ptemp += outData_len;
            if(havepacket == 1){
                return 0;
            }
        }else{
            curlen = 0;
            ptemp = data;
            printnginxlog("curlen:%d, data_len:%d, len:%d\n", curlen, data_len, len);
            if (curlen + data_len > len) {
                return 0;
            }
            unsigned char * encode_data = (unsigned char*) ngx_palloc(r->pool, ctx->lastPacketLen + data_len + 1);
            memset(encode_data, 0, ctx->lastPacketLen + data_len + 1);
            unsigned char * p = encode_data;
            memcpy(p, (char*)ctx->lastPacket, ctx->lastPacketLen);
            memcpy(p+ctx->lastPacketLen, (char*)findp, data_len);

            
            p = encode_data;
            int outData_len = ctx->lastPacketLen+data_len;
            ReplaceHtmlCode((char*)p,&outData_len);
            
            printnginxlog("Encode content :");
            for (i = 0; i < ctx->lastDePacketLen; i++) {
                printnginxlog("%02X ", *p);
                p++;
            }
            printnginxlog("\n");
            p = encode_data;
            if(encodetype == 1 ){
                ReplaceBase64Code((char*)p,&outData_len);
            }
            Decrypt_AES256((char*)p,key,&outData_len);

            encode_data+=ctx->lastDePacketLen;
            
            printnginxlog("encode_data:%s\n", encode_data);
            memcpy((char*)ptemp,(char*)encode_data,outData_len-ctx->lastDePacketLen);
            ptemp += outData_len-ctx->lastDePacketLen;
            int offset = (data_len-(outData_len-ctx->lastDePacketLen));
            *real_len -= offset;
            memcpy((char*)ptemp,(char*)ptemp+offset+find_flag,len-curlen-data_len-find_flag);
            
            ctx->lastPacketLen = 0;
            ctx->lastDePacketLen = 0;
            ngx_pfree(r->pool, ctx->lastPacket);
            ctx->lastPacket = NULL;
        }
        
    }else{
        return 0;
    }

    
    replace_ngx_str_t(r, ctx, data, *real_len,real_len,next);
    
    return 0;
}



int getmemlen(char *start, char *end) {
    int len = 0;
    char * p = start;
    while (p != end) {
        len++;
        p++;
    }
    return len;
}
char * mmstrstr(char *start, char *end, char *substr){
    int len = strlen(substr);
    int i = 0 ,j = 0;
    for(i = 0 ; start + i != end ; i ++){
        int find = 1;
        for(j = 0 ; j < len ; j ++){
            if(*(start+i+j) != *(substr+j)){
                find = 0;
            }
        }
        if(find == 1){
            return start + i;
        }
    }
    return end;
}
char* memstrcpy(char *start, char *end) {
    char * dest = NULL;
    int len = getmemlen(start, end);
    printnginxlog("memstrcpy len:%d\n", len);

    dest = (char*) malloc(len + 1);
    memset(dest, 0, len + 1);

    char *p = dest;
    while (start != end) {
        *p = *start;
        p++;
        start++;
    }
    printnginxlog("memstrcpy dest:%s len:%d\n", dest, (int) strlen(dest));
    return dest;
}
int hexchar2int(unsigned char c){
     if (c >= '0' && c <= '9')
         return c - '0';
     else if (c >= 'a' && c <= 'f')
         return c - 'a' + 10;
     else if (c >= 'A' && c <= 'F')
         return c - 'A' + 10;
     else
        return -1;
}
void replaceURLcode(unsigned char *  data, int* len){
    
    unsigned char * outData = (unsigned char *)malloc(*len);
    memset(outData,0,*len);
    int i=0,j=0;
    for(i = 0 ; i < *len ; i ++){
        if(*(data+i) != '%'){
            *(outData+j) = *(data + i);
            j ++;
        }else{
            
            int c = 0;
            c = hexchar2int(*(data+i+1))*0x10 + hexchar2int(*(data+i+2));
            *(outData+j) = (unsigned char)c;
            j ++;
            i += 2;
        }
    }
    memset(data,0,*len);
    memcpy(data,outData,j);
    *len = j;
    free(outData);
    
}
char * findWildString(char *src,char *dest,int *plen){
    int slen = strlen(src);
    int dlen = strlen(dest);
    int find = 0;
    int i,j;
    char *p = NULL;
    *plen = 0;
    
    for(i = 0 ,j = 0; i < slen; i ++){
        if(j >= dlen ) break;
        if(*(src+i) == *(dest+j)){
            find = 1;
            j++;
            *plen += 1;
            if(p == NULL){
                p = src + i;
            }
        }else if(find == 1 && *(dest+j) == '*'){
            if(*(src+i) == *(dest+j+1)){
                j += 2;
            }
            *plen += 1;
        }else{
            find = 0;
            j = 0;
            p = NULL;
            *plen = 0;
        }
        
    }
    
    
    return p;
}
void SetEncodeHead(char *enCodeHead,int type){
    if(type == 1){
        *(enCodeHead) = 0xff;
        *(enCodeHead+1) = 0xfe;
        *(enCodeHead+2) = 0;
    }else if(type == 2){
        *(enCodeHead) = 0xff;
        *(enCodeHead+1) = 0;
        *(enCodeHead+2) = 0;
    }
}
char * findsplit(char *pData ){
    if (strstr(pData, "\r\n") != NULL) {
            printnginxlog("find 0D0A\n");
            char * split = memstrcpy(pData, strstr(pData, "\r\n"));
            printnginxlog("split:%s\n", split);
            return split;
    } else {
            printnginxlog("not find 0D0A\n");
            return NULL;
    }
}

void printEnData(unsigned char * pEnData, int endata_len) {
    int i = 0;
    unsigned char * pE = pEnData;
    printnginxlog("Encode content :");
    for (i = 0; i < endata_len; i++) {
        printnginxlog("%02X ", *pE);
        pE++;
    }
    printnginxlog("\n");
    printnginxlog("Encode content string :%s\n", pEnData);
}
unsigned char *EncryptData(ngx_pool_t *pool,char *p,int data_len,char* szEncodeHead,int* endata_len,int isurl){
    *endata_len = strlen(szEncodeHead) + data_len;

    printnginxlog("data_len:%d,endata_len:%d\n", data_len, *endata_len);

    unsigned char * pEnData = (unsigned char *) ngx_palloc(pool, *endata_len + 1);
    memset(pEnData, 0, (*endata_len + 1));
    unsigned char *pE = pEnData;
    strcpy((char *) pE, szEncodeHead);
    pE += strlen(szEncodeHead);
    memcpy(pE, p, data_len);

    if(isurl == 1){
        replaceURLcode(pEnData,endata_len);
    }
    Encrypt_AES256((char *) pE, key);
    printEnData(pEnData,*endata_len);
    int count = replace_counts(pEnData, *endata_len);
    if (count > 0) {
        *endata_len += count;
        unsigned char * pnewEnData = (unsigned char *) ngx_palloc(pool, *endata_len + 1);
        memset(pnewEnData, 0, (*endata_len + 1));
        memcpy(pnewEnData, pEnData, *endata_len - count);
        ngx_pfree(pool, pEnData);
        pEnData = pnewEnData;
        replace_post(pEnData, *endata_len);
    }
    return pEnData;
}
unsigned char *EncryptDataBase64(ngx_pool_t *pool,char *p,int data_len,char* szEncodeHead,int* endata_len,int i){
    *endata_len = strlen(szEncodeHead) + data_len;

    printnginxlog("data_len:%d,endata_len:%d\n", data_len, *endata_len);

    unsigned char * pEnData = (unsigned char *) ngx_palloc(pool, *endata_len + 1);
    memset(pEnData, 0, (*endata_len + 1));
    unsigned char *pE = pEnData;
    strcpy((char *) pE, szEncodeHead);
    pE += strlen(szEncodeHead);
    memcpy(pE, p, data_len);

    replaceURLcode(pEnData,endata_len);
    if(i){
        int jsonlen = *endata_len -strlen(szEncodeHead);

        unsigned char * json_data = (unsigned char*) ngx_palloc(pool, jsonlen + 1);
        memset(json_data, 0, jsonlen + 1);

        unsigned char *pi1    =  pEnData+strlen(szEncodeHead);
        unsigned char **pi2    =  &pi1;
        unsigned char *po1    =  json_data;
        unsigned char **po2    =  &po1;
        size_t           ilen        = *endata_len;
        size_t           olen    = jsonlen;

        iconv_t conveter;

       if((conveter = iconv_open("gbk", "utf-8")) == (iconv_t)-1)
       {

           printnginxlog("iconv open fail \n");
           
       }
       else
       {
           printnginxlog("iconv open ok \n");    
       

            iconv(conveter,(char**)pi2,&ilen,(char**)po2,&olen);

            printnginxlog("json_data:%s\n", json_data);
            jsonlen = strlen((char*)json_data);
            memcpy(pEnData+strlen(szEncodeHead),json_data,jsonlen);
            *endata_len = jsonlen + strlen(szEncodeHead);
            ngx_pfree(pool, json_data);
            
        }
    }
    Encrypt_AES256((char *) pE, key);
    printEnData(pEnData,*endata_len);
    
    if (1) {

        unsigned char * pnewEnData = (unsigned char *) ngx_palloc(pool, *endata_len + *endata_len + 1);
        memset(pnewEnData, 0, (*endata_len + *endata_len + 1));
        pE = pEnData;

        *pnewEnData = '[';
        *(pnewEnData + 1) = 0x1f;
        pE++;
        int i = 0;
        for (i = 1; i < *endata_len; i++) {
            sprintf((char*) (pnewEnData + 2 * i), "%02X", *pE);
            pE++;
        }

        ngx_pfree(pool, pEnData);
        pEnData = pnewEnData;

        *endata_len += *endata_len;
    }
    return pEnData;
}

unsigned char *EncryptDataBase642(ngx_pool_t *pool,char *p,int data_len,char* szEncodeHead,int* endata_len){
    *endata_len = strlen(szEncodeHead) + data_len;

    printnginxlog("data_len:%d,endata_len:%d\n", data_len, *endata_len);

    unsigned char * pEnData = (unsigned char *) ngx_palloc(pool, *endata_len + 1);
    memset(pEnData, 0, (*endata_len + 1));
    unsigned char *pE = pEnData;
    strcpy((char *) pE, szEncodeHead);
    pE += strlen(szEncodeHead);
    memcpy(pE, p, data_len);

    Encrypt_AES256((char *) pE, key);
    printEnData(pEnData,*endata_len);
    
    if (1) {

        unsigned char * pnewEnData = (unsigned char *) ngx_palloc(pool, *endata_len + *endata_len + 1);
        memset(pnewEnData, 0, (*endata_len + *endata_len + 1));
        pE = pEnData;

        *pnewEnData = '[';
        *(pnewEnData + 1) = 0x1f;
        pE++;
        int i = 0;
        for (i = 1; i < *endata_len; i++) {
            sprintf((char*) (pnewEnData + 2 * i), "%02X", *pE);
            pE++;
        }

        ngx_pfree(pool, pEnData);
        pEnData = pnewEnData;

        *endata_len += *endata_len;
    }
    return pEnData;
}

uint8_t* replaceEnBuf(ngx_pool_t *pool,uint8_t* data_buf, size_t *body_len,int offset,int data_len ,unsigned char * pEnData,int endata_len){
    
        
        int newlen = *body_len +(endata_len-data_len);
        uint8_t* newdata_buf = (uint8_t*) ngx_palloc(pool, newlen + 1);
        memset(newdata_buf, 0, newlen + 1);
        memcpy(newdata_buf, data_buf, offset );
        
        char* p = (char*) newdata_buf + offset;
        char* p_end = (char*)data_buf + offset + data_len;
        int endlen = *body_len - offset - data_len;

        printnginxlog("start replace buf\n");
        
        memcpy(p, pEnData, endata_len);
        p += endata_len;
        memcpy(p, p_end,endlen);
        ngx_pfree(pool, pEnData);
        *body_len = newlen;
        
        return newdata_buf;
}

int getdatalen(char* split, char* p) {
    char *p_end = strstr((char*) p, split);
    if (p_end == NULL) {
        return 0;
    }
    p_end -= 2; // \r\n
    printnginxlog("find content end\n");
    int data_len = getmemlen(p, p_end);
    return data_len;
}
uint8_t* replaceBuf(ngx_http_request_t* r, uint8_t* data_buf, size_t *body_len,FieldPolicy filterName,int iconv) {
    char szEncodeHead[3] = {0};
    char *szConnect = filterName.field_name;
    char * split = NULL;
    char *pData = (char*) data_buf;
    printnginxlog("replaceBuf----------------------------------\n");
    
    SetEncodeHead(szEncodeHead,1);
    
    
    
    if(filterName.type == POSTFORM||filterName.type == POSTFORMS||filterName.type == POSTFORMSW){
        if(filterName.type == POSTFORMSW){
                SetEncodeHead(szEncodeHead,2);
        }
        split = findsplit(pData);
        if (split == NULL) {
            return data_buf;
        }
        
        int replaceCount = 0;
        uint8_t* srcData =  data_buf;
        int connectlen = 0;
        char *p = findWildString((char*) srcData, szConnect,&connectlen);
        
        
        while (p != NULL) {
            printnginxlog("find content : %s\n", szConnect);
            int offset = getmemlen((char*) srcData, p) + connectlen;
            
            p += connectlen;
            int data_len = getdatalen(split,p);
            if(data_len<=0){
                free(split);
                return srcData;
            }
            if(data_len > 2){
                if(
                        (*((uint8_t*)p) == 0xff && *((uint8_t*)p+1) == 0xfe) ||
                        (*((uint8_t*)p) == '[' && *((uint8_t*)p+1) == 0x1f) 
                  ){
                    printnginxlog("it's EncryptData=====================================n");
                    return srcData;
                }

            }
            if(strcmp(filterName.field_name,"name=\"dt_cuview_web\"\r\n\r\n") == 0||strcmp(filterName.field_name,"name=\"dt_customer_web\"\r\n\r\n") == 0){
                if(*(p) == 'h' &&
                   *(p+1) == 't' &&
                   *(p+2) == 't' &&
                   *(p+3) == 'p' &&
                   *(p+4) == ':' &&
                   *(p+5) == '/' &&
                   *(p+6) == '/'){
                    p += 7;
                    offset+=7;
                    data_len -= 7;
                    if(data_len<=0){
                        free(split);
                        return srcData;
                    }
                }
            }
            int endata_len = 0;
            unsigned char * pEnData = NULL;
            if(filterName.type == POSTFORMSW){
                pEnData = EncryptDataBase64(r->pool,p,data_len,szEncodeHead,&endata_len,0);
            }else{
                pEnData = EncryptData(r->pool,p,data_len,szEncodeHead,&endata_len,0);
            }
            if(filterName.type == POSTFORMS || filterName.type == POSTFORMSW){
            
                unsigned char * pnewEnData = (unsigned char *) ngx_palloc(r->pool, endata_len + 2);
                memset(pnewEnData, 0, (endata_len + 2));
                memcpy(pnewEnData, pEnData, endata_len);
                *(pnewEnData+endata_len) = 0xfd;
                ngx_pfree(r->pool, pEnData);
                pEnData = pnewEnData;
                endata_len ++;
            }
            uint8_t* newdata_buf = replaceEnBuf(r->pool,srcData,body_len,offset,data_len,pEnData,endata_len);

            if (replaceCount > 0) {
                ngx_pfree(r->pool, srcData);
            }
            srcData = newdata_buf;
            if(filterName.type != POSTFORMSW){
                break;
            }
            p = (char*)srcData + offset + endata_len;
            p = findWildString((char*) p, szConnect, &connectlen);
            replaceCount++;

        }
        free(split);
        return srcData;
    }else if(filterName.type == POSTURLS ||filterName.type == POSTURLSW || filterName.type == POSTURL){
        
        if(filterName.type == POSTURLS||filterName.type == POSTURLSW){
            SetEncodeHead(szEncodeHead,2);
        }
        
       
        
        
        int replaceCount = 0;
        uint8_t* srcData =  data_buf;
        int connectlen = 0;
        char *p = findWildString((char*) srcData, szConnect,&connectlen);
        
        
        while (p != NULL) {
            printnginxlog("find content : %s\n", szConnect);
            int offset = getmemlen((char*) srcData, p) + connectlen;
            
            p += connectlen;
            char *p_end = strstr((char*) p, "&");
            if(p_end == NULL){
                    p_end = (char*)srcData + *body_len;
            }
            int data_len = getmemlen(p, p_end);
            if(data_len<=0){
                return srcData;
            }
            if(data_len > 2){
                if(
                        (*((uint8_t*)p) == 0xff && *((uint8_t*)p+1) == 0xfe) ||
                        (*((uint8_t*)p) == '[' && *((uint8_t*)p+1) == 0x1f) 
                  ){
                    printnginxlog("it's EncryptData=====================================n");
                    return srcData;
                }

            }
            int endata_len = 0;
            unsigned char * pEnData = NULL;
            
            if(filterName.type == POSTURLS||filterName.type == POSTURLSW){
                pEnData = EncryptDataBase64(r->pool,p,data_len,szEncodeHead,&endata_len,iconv);
            }else if(filterName.type == POSTURL){
                pEnData = EncryptData(r->pool,p,data_len,szEncodeHead,&endata_len,1);
            }
            uint8_t* newdata_buf = replaceEnBuf(r->pool,srcData,body_len,offset,data_len,pEnData,endata_len);

            if (replaceCount > 0) {
                ngx_pfree(r->pool, srcData);
            }
            srcData = newdata_buf;
            if(filterName.type == POSTURL){
                break;
            }
            p = (char*)srcData + offset + endata_len;
            p = findWildString((char*) p, szConnect, &connectlen);
            replaceCount++;

        }
        return srcData;
        
    
    }else if(filterName.type == POSTURLSPLIT){
        
        int replaceCount = 0;
        uint8_t* srcData =  data_buf;
        int connectlen = 0;
        char *p = findWildString((char*) srcData, szConnect,&connectlen);
        SetEncodeHead(szEncodeHead,2);
        
        while (p != NULL) {
            printnginxlog("find content : %s\n", szConnect);
            int offset = getmemlen((char*) srcData, p) + connectlen;
            
            p += connectlen;
            char *p_end = strstr((char*) p, "&");
            if(p_end == NULL){
                    p_end = (char*)srcData + *body_len;
            }
            int data_len = getmemlen(p, p_end);
            if(data_len<=0){
                return srcData;
            }
            if(data_len > 2){
                if(
                        (*((uint8_t*)p) == 0xff && *((uint8_t*)p+1) == 0xfe) ||
                        (*((uint8_t*)p) == '[' && *((uint8_t*)p+1) == 0x1f) 
                  ){
                    printnginxlog("it's EncryptData=====================================n");
                    return srcData;
                }

            }
            
            
            int endata_len = data_len;
            unsigned char * pEnData = NULL;
            
            pEnData = (unsigned char *) ngx_palloc(r->pool, endata_len+1);
            memset(pEnData,0,endata_len+1);
            memcpy(pEnData,srcData+offset,endata_len);
            
            //unsigned char *pEn = pEnData;
            int split_offset = 0;
            while(1){
                int split_len = 0;
                int isendsplit = 0;
                if(strstr((char*)pEnData+split_offset,"%2C") != NULL){
                    split_len = getmemlen((char*)pEnData+split_offset, strstr((char*)pEnData+split_offset,"%2C"));
                }else{
                    split_len = strlen((char*)pEnData+split_offset);
                    isendsplit = 1;
                }
                int ensplit_len = 0;
                unsigned char * pEnSplitData = EncryptDataBase64(r->pool,(char*)pEnData+split_offset,split_len,szEncodeHead,&ensplit_len,0);
                if(1){
            
                    unsigned char * pnewEnData = (unsigned char *) ngx_palloc(r->pool, ensplit_len + 2);
                    memset(pnewEnData, 0, (ensplit_len + 2));
                    memcpy(pnewEnData, pEnSplitData, ensplit_len);
                    *(pnewEnData+ensplit_len) = 0xfd;
                    ngx_pfree(r->pool, pEnSplitData);
                    pEnSplitData = pnewEnData;
                    ensplit_len ++;
                }
                
                unsigned char * pEnNewSplitData = (unsigned char *) ngx_palloc(r->pool, endata_len + (ensplit_len-split_len) + 1 );
                memset(pEnNewSplitData,0,endata_len + (ensplit_len-split_len) + 1 );
                memcpy(pEnNewSplitData,pEnData,split_offset);
                
                memcpy(pEnNewSplitData+split_offset,pEnSplitData,ensplit_len);
                memcpy(pEnNewSplitData+split_offset+ensplit_len,pEnData+split_offset+split_len,endata_len-(split_offset+split_len));
                
                endata_len = endata_len + (ensplit_len-split_len);
                ngx_pfree(r->pool, pEnData);
                pEnData = pEnNewSplitData;
                if(isendsplit == 1){
                    break;
                }
                split_offset += ensplit_len + strlen("%2C");
            }
            
            
            
            uint8_t* newdata_buf = replaceEnBuf(r->pool,srcData,body_len,offset,data_len,pEnData,endata_len);

            if (replaceCount > 0) {
                ngx_pfree(r->pool, srcData);
            }
            srcData = newdata_buf;
            if(filterName.type == POSTURL){
                break;
            }
            p = (char*)srcData + offset + endata_len;
            p = findWildString((char*) p, szConnect, &connectlen);
            replaceCount++;

        }
        return srcData;
    }else if(filterName.type == URLJSON){
        
        int replaceCount = 0;
        uint8_t* srcData =  data_buf;
        int connectlen = 0;
        char *p = findWildString((char*) srcData, szConnect,&connectlen);
        
        
        while (p != NULL) {
            printnginxlog("find content : %s\n", szConnect);
            int offset = getmemlen((char*) srcData, p) + connectlen;
            
            p += connectlen;
            char *p_end = strstr((char*) p, "%22%7D");
            if(p_end == NULL){
                    p_end = (char*)srcData + *body_len;
            }
            int data_len = getmemlen(p, p_end);
            if(data_len<=0){
                return srcData;
            }
            if(data_len > 2){
                if(
                        (*((uint8_t*)p) == 0xff && *((uint8_t*)p+1) == 0xfe) ||
                        (*((uint8_t*)p) == '[' && *(p+1) == 0x1f) 
                  ){
                    printnginxlog("it's EncryptData=====================================n");
                    return srcData;
                }

            }
            int endata_len = 0;
            unsigned char * pEnData = NULL;
            
            pEnData = EncryptDataBase642(r->pool,p,data_len,szEncodeHead,&endata_len);
            
            uint8_t* newdata_buf = replaceEnBuf(r->pool,srcData,body_len,offset,data_len,pEnData,endata_len);

            if (replaceCount > 0) {
                ngx_pfree(r->pool, srcData);
            }
            srcData = newdata_buf;
            
            p = (char*)srcData + offset + endata_len;
            p = findWildString((char*) p, szConnect, &connectlen);
            replaceCount++;

        }
        return srcData;
        
    }
    return data_buf;
}

char * replaceGetBuf(ngx_http_request_t* r, char* data_buf, int *body_len,FieldPolicy filterName){
    char szEncodeHead[3] = {0};
    char *szConnect = filterName.field_name;
    
    int i = 0;
    printnginxlog("replaceBuf----------------------------------\n");
    
    szEncodeHead[0] = 0xff;
    szEncodeHead[1] = 0xfe;
    szEncodeHead[2] = 0;
    
    if(filterName.type == POSTURL){
        char *p = strstr((char*) data_buf, szConnect);
        if (p != NULL) {
            printnginxlog("find content : %s\n", szConnect);
            *body_len += strlen(szConnect);
            p += strlen(szConnect);
            char *p_end = strstr((char*) p, "&");
            if(p_end == NULL){
                p_end = (char*)data_buf + *body_len;
            }
            int data_len = getmemlen(p, p_end);
            
            int endata_len = strlen(szEncodeHead) + data_len;

            printnginxlog("data_len:%d,endata_len:%d\n", data_len, endata_len);

            unsigned char * pEnData = (unsigned char *) ngx_palloc(r->pool, endata_len + 1);
            memset(pEnData, 0, sizeof (endata_len + 1));
            unsigned char *pE = pEnData;
            strcpy((char *) pE, szEncodeHead);
            pE += strlen(szEncodeHead);

            memcpy(pE,p,data_len);

            replaceURLcode(pEnData,&endata_len);
            Encrypt_AES256((char *)pE, key);

            pE = pEnData;
            printnginxlog("Encode content :");
            for (i = 0; i < endata_len; i++) {
                printnginxlog("%02X ", *pE);
                pE++;
            }
            printnginxlog("\n");
            printnginxlog("Encode content string :%s\n", pEnData);
            int count = replace_counts(pEnData,endata_len);
            if(count > 0){
                    endata_len += count;
                    unsigned char * pnewEnData = (unsigned char *) ngx_palloc(r->pool, endata_len + 1);
                    memset(pnewEnData, 0,  (endata_len + 1));
                    memcpy(pnewEnData, pEnData, endata_len - count);
                    ngx_pfree(r->pool, pEnData);
                    pEnData = pnewEnData;
                    replace_post(pEnData,endata_len);
                    *body_len += count;
            }
            char* newdata_buf = (char*) ngx_palloc(r->pool, *body_len+1);
            memset(newdata_buf, 0, *body_len+1);
            memcpy(newdata_buf, data_buf, *body_len);
            p = strstr((char*) newdata_buf, szConnect);
            if (p != NULL) {
                printnginxlog("start replace buf\n");
                p += strlen(szConnect);
                memcpy(p, pEnData, endata_len);
                p += endata_len;
                strcpy(p, p_end);

                //memcpy(data_buf ,newdata_buf, *body_len);
                //ngx_pfree(r->pool, newdata_buf);
                ngx_pfree(r->pool, pEnData);
                //printnginxlog("replace buf:%s\n", data_buf);
                //printnginxlog("-------------------------------end\n");
                    
                    
               return newdata_buf;
            }
        }
    }
    
    return data_buf;
}

ngx_chain_t * createchain(unsigned char * str, int len, ngx_pool_t *pool) {//由于要发送的数据太多，我要开辟一个chain buffer链来发送数据。
    int num, i;
    if (len % CHAIN_BUFFER_SIZE == 0) {
        num = len/CHAIN_BUFFER_SIZE;
    } else {
        num = len/CHAIN_BUFFER_SIZE + 1;
    }

    ngx_chain_t * out;
    out = (ngx_chain_t *) ngx_pcalloc(pool, num * sizeof (ngx_chain_t));
    ngx_buf_t * b;
    b = (ngx_buf_t *) ngx_pcalloc(pool, num * sizeof (ngx_buf_t));
    for (i = 0; i < num; i++) {
        out[i].buf = &b[i];
        if (i == num - 1) {
            out[i].next = NULL;
            b[i].pos = str + i * CHAIN_BUFFER_SIZE;
            b[i].last = str + len;
            b[i].memory = 1;
            b[i].last_buf = 1;

        } else {
            out[i].next = &out[i + 1];
            b[i].pos = str + i * CHAIN_BUFFER_SIZE;
            b[i].last = str + (i + 1) * CHAIN_BUFFER_SIZE;
            b[i].memory = 1;
            b[i].last_buf = 0;
        }
    }
    return out;
}

int strings_replace(char *pInput, int inputLen, char *pOutput, char *pSrc, char *pDst) {
    char* point = NULL;
    char* input_head = pInput;
    char* output_head = pOutput;
    int lens = 0;
    int offset = 0;
    int src_lens = strlen(pSrc);
    int dst_lens = strlen(pDst);
    int count = 0;
    
    while (1) {
        point = findstrstr((u_char *)input_head, pSrc, inputLen-offset);
        if (point != NULL) {
            lens = (int) (point - input_head);
            count ++;
        } else {
            int len = (int) (inputLen - offset);
            memcpy(output_head, input_head, len);
            break;
        }
        
        
        memcpy(output_head, input_head, lens);
        input_head += lens;
        offset += lens;
        output_head += lens;
       
        memcpy(output_head, pDst, dst_lens);
        input_head += src_lens;
        offset += src_lens;
        output_head += dst_lens;
        
    }
    return count;
}
char * ReplaceToHtmlCode(char * pdata, int * plen){
    int i = 0;
    int newlen = *plen;
    char quot[] = "&quot;";
    char br[] = "%3cbr%20%2f3e%0a";
    char gt[] = "%26gt%3b";
    char lt[] = "%26lt%3b";
    char dy[] = "%26%23039%3b";
    char amp[] = "%26amp%3b";
    
    for(i = 0 ; i < *plen ; i ++){
        if(*(pdata+i) == '\"'){ //&quot; -> "
            newlen += strlen(quot)-1;
        }else if(*(pdata+i) == '\n'){ //<br /> -> \n
            newlen += strlen(br) - 1;
        }else if(*(pdata+i) == '>'){ //&gt; -> >
            newlen += strlen(gt)-1;
        }else if(*(pdata+i) == '<'){ //&lt; -> <
            newlen += strlen(lt)-1;
        }else if(*(pdata+i) == '\''){ //&#039; -> '
            newlen += strlen(dy)-1;
        }else if(*(pdata+i) == '&'){ //&amp; -> &
            newlen += strlen(amp)-1;
        }
    }
    
    char *pnewdata = (char *)malloc(newlen);
    memset(pnewdata,0,newlen);
    
    char *polddata = (char *)malloc(newlen);
    memset(polddata,0,newlen);
    memcpy(polddata,pdata,*plen);
    
    int count = strings_replace(polddata, *plen, pnewdata, "\"", quot);
    *plen += (strlen(quot)-1)*count;
    memcpy(polddata,pnewdata,*plen);
    
    count = strings_replace(polddata, *plen, pnewdata, "\n", br);
    *plen += (strlen(br)-1)*count;
    memcpy(polddata,pnewdata,*plen);
    
    count = strings_replace(polddata, *plen, pnewdata, ">", gt);
    *plen += (strlen(gt)-1)*count;
    memcpy(polddata,pnewdata,*plen);
    
    count = strings_replace(polddata, *plen, pnewdata, "<", lt);
    *plen += (strlen(lt)-1)*count;
    memcpy(polddata,pnewdata,*plen);
    
    count = strings_replace(polddata, *plen, pnewdata, "\'", dy);
    *plen += (strlen(dy)-1)*count;
    memcpy(polddata,pnewdata,*plen);
    
    //count = strings_replace(polddata, *plen, pnewdata, "&", amp);
    //*plen += (strlen(amp)-1)*count;
    //memcpy(polddata,pnewdata,*plen);
    
    free(pdata);
    free(polddata);
    return pnewdata;
}


int replace_counts(unsigned char * data, int len){
    int count = 0;
    int i = 0;
    //char sz0x20[] = "$20;";space
    //char sz0x0a[] = "$0a;";\n
    //char sz0x0d[] = "$0d;";\r
    //char sz0x3c[] = "$3c;";<
    for(i = 0 ; i < len ; i ++){
        unsigned char c = *(data+i);
        
        if(c == 0x20){
            count += 3;
        }else if(c == 0x0a){
            count += 3;
        }else if(c == 0x0d){
            count += 3;
        }else if(c == '<'){
            count += 3;
        }else if(c == '>'){
            count += 3;
        }else if(c == '&'){
            count += 3;
        }else if(c == '\"'){
            count += 3;
        }else if(c == '\''){
            count += 3;
        }else if(c == '%'){
            count += 3;
        }else if(c == '+'){
            count += 3;
        }else if(c == '('){
            count += 3;
        }else if(c == '/'){
            count += 3;
        }else if(c == '['){
            count += 3;
        }else if(c == ':'){
            count += 3;
        }else if(c == '-'){
            count += 3;
        }else if(c == ']'){
            count += 3;
        }else if(c == 0x00){
            count += 3;
        }else if(c == 0xfd){
            count += 3;
        }else if(c == 0xff && i > 0){
            count += 3;
        }else if(c == '\\'){
            count += 3;
        }
    }
    return count ;
}

int replace_str(char *pInput, int inputLen, char *pOutput, char *pSrc, char *pDst) {
    char* point = NULL;
    char* input_head = pInput;
    char* output_head = pOutput;
    int lens = 0;
    int offset = 0;
    int src_lens = strlen(pSrc);
    int dst_lens = strlen(pDst);
    int count = 0;
    while (1) {
        point = findstrstr((u_char*)input_head, pSrc, inputLen-offset);
        if (point != NULL) {
            lens = (int) (point - input_head);
            count ++;
        } else {
            int len = (int) (inputLen - offset);
            memcpy(output_head, input_head, len);
            break;
        }
        
        memcpy(output_head, input_head, lens);
        input_head += lens;
        offset += lens;
        output_head += lens;
       
        memcpy(output_head, pDst, dst_lens);
        input_head += src_lens;
        offset += src_lens;
        output_head += dst_lens;
        
        
    }
    return count;
}

int replace_char(char *pInput, int inputLen, char *pOutput, char pSrc, char *pDst) {
    char* point = NULL;
    char* input_head = pInput;
    char* output_head = pOutput;
    int lens = 0;
    int offset = 0;
    int src_lens = 1;
    int dst_lens = strlen(pDst);
    int count = 0;
    while (1) {
        point = findstrchar((u_char*)input_head, (u_char)pSrc, inputLen-offset);
        if (point != NULL) {
            lens = (int) (point - input_head);
            count ++;
        } else {
            int len = (int) (inputLen - offset);
            memcpy(output_head, input_head, len);
            break;
        }
        
        memcpy(output_head, input_head, lens);
        input_head += lens;
        offset += lens;
        output_head += lens;
       
        memcpy(output_head, pDst, dst_lens);
        input_head += src_lens;
        offset += src_lens;
        output_head += dst_lens;
        
        
    }
    return count;
}

int replace_strs(unsigned char *pStrings, int strlen,char *pNeedReplace, char* pNew){
    if(findstrstr((u_char*)pStrings,pNeedReplace,strlen) != NULL){
        int offset = replace_counts(pStrings,strlen);
        char* p_back = ( char*) malloc(strlen+offset);
        memset(p_back,0x0,strlen+offset);
        
        int count = replace_str((char*)pStrings, strlen, (char*)p_back, pNeedReplace, pNew);
        memset(pStrings,0x0,strlen);
        memcpy(pStrings,p_back,strlen);
        free(p_back);
        return count;
    }
    return 0;
}
int replace_chars(unsigned char *pStrings, int strlen,char pNeedReplace, char* pNew){
    if(findstrchar((u_char*)pStrings,(u_char)pNeedReplace,strlen) != NULL){
        int offset = replace_counts(pStrings,strlen);
        char* p_back = ( char*) malloc(strlen+offset);
        memset(p_back,0x0,strlen+offset);
        
        int count = replace_char((char*)pStrings, strlen, (char*)p_back, pNeedReplace, pNew);
        memset(pStrings,0x0,strlen);
        memcpy(pStrings,p_back,strlen);
        free(p_back);
        return count;
    }
    return 0;
}

int replace_post(unsigned char * data, int len){
    char sz0x20[] = "$20;";
    char sz0x0a[] = "$0a;";
    char sz0x0d[] = "$0d;";
    char sz0x3c[] = "$3c;";
    char sz0x26[] = "$26;";
    char sz0x22[] = "$22;";
    char sz0x3e[] = "$3e;";
    char sz0x27[] = "$27;";
    char sz0x25[] = "$25;";
    char sz0x2b[] = "$2b;";
    char sz0x28[] = "$28;";
    char sz0x2f[] = "$2f;";
    char sz0x5b[] = "$5b;";
    char sz0x3a[] = "$3a;";
    char sz0x2d[] = "$2d;";
    char sz0x5d[] = "$5d;";
    char sz0x00[] = "$00;";
    char sz0xfd[] = "$fd;";
    char sz0xff[] = "$ff;";
    char sz0x5c[] = "$5c;";
    
    replace_strs(data,len," ",sz0x20);
    replace_strs(data,len,"\n",sz0x0a);
    replace_strs(data,len,"\r",sz0x0d);
    replace_strs(data,len,"<",sz0x3c);
    replace_strs(data,len,"&",sz0x26);
    replace_strs(data,len,"\"",sz0x22);
    replace_strs(data,len,">",sz0x3e);
    replace_strs(data,len,"\'",sz0x27);
    replace_strs(data,len,"%",sz0x25);
    replace_strs(data,len,"+",sz0x2b);
    replace_strs(data,len,"(",sz0x28);
    replace_strs(data,len,"/",sz0x2f);
    replace_strs(data,len,"[",sz0x5b);
    replace_strs(data,len,":",sz0x3a);
    replace_strs(data,len,"-",sz0x2d);
    replace_strs(data,len,"]",sz0x5d);
    
    replace_chars(data,len,0x00,sz0x00);
    replace_chars(data,len,0xfd,sz0xfd);
    replace_chars(data+2,len-2,0xff,sz0xff);
    replace_strs(data,len,"\\",sz0x5c);
    
    
    return 0;
}

int strings_replace1(char *pInput, int inputLen, char *pOutput, char *pSrc, char *pDst) {
    char* point = NULL;
    char* input_head = pInput;
    char* output_head = pOutput;
    int lens = 0;
    int offset = 0;
    int src_lens = strlen(pSrc);
    int dst_lens = strlen(pDst);
    int count = 0;
    while (1) {
        point = findstrstr((u_char*)input_head, pSrc, inputLen-offset);
        if (point != NULL) {
            lens = (int) (point - input_head);
            count ++;
        } else {
            int len = (int) (inputLen - offset);
            memcpy(output_head, input_head, len);
            break;
        }
        
        memcpy(output_head, input_head, lens);
        input_head += lens;
        offset += lens;
        output_head += lens;
       
        memcpy(output_head, pDst, dst_lens);
        input_head += src_lens;
        offset += src_lens;
        output_head += dst_lens;
        
        
    }
    return count;
}

int strings_replace2(char *pInput, int inputLen, char *pOutput, char *pSrc, char pDst) {
    char* point = NULL;
    char* input_head = pInput;
    char* output_head = pOutput;
    int lens = 0;
    int offset = 0;
    int src_lens = strlen(pSrc);
    int dst_lens = 1;
    int count = 0;
    while (1) {
        point = findstrstr((u_char*)input_head, pSrc, inputLen-offset);
        if (point != NULL) {
            lens = (int) (point - input_head);
            count ++;
        } else {
            int len = (int) (inputLen - offset);
            memcpy(output_head, input_head, len);
            break;
        }
        
        memcpy(output_head, input_head, lens);
        input_head += lens;
        offset += lens;
        output_head += lens;
       
        *output_head= pDst;
        input_head += src_lens;
        offset += src_lens;
        output_head += dst_lens;
        
        
    }
    return count;
}
int replacestring(char *pStrings, int strlen,char *pNeedReplace, char* pNew){
    if(findstrstr((u_char*)pStrings,pNeedReplace,strlen) != NULL){
        char* p_back = (char*) malloc(strlen);
        memset(p_back,0x20,strlen);
        
        int count = strings_replace1(pStrings, strlen, p_back, pNeedReplace, pNew);
        memset(pStrings,0x0,strlen);
        memcpy(pStrings,p_back,strlen);
        free(p_back);
        return count;
    }
    return 0;
}

int replacechar(char *pStrings, int strlen,char *pNeedReplace, char pNew){
    if(findstrstr((u_char*)pStrings,pNeedReplace,strlen) != NULL){
        char* p_back = (char*) malloc(strlen);
        memset(p_back,0x20,strlen);
        
        int count = strings_replace2(pStrings, strlen, p_back, pNeedReplace, pNew);
        memset(pStrings,0x0,strlen);
        memcpy(pStrings,p_back,strlen);
        free(p_back);
        return count;
    }
    return 0;
}
void ReplaceBase64Code(char *input,int *plen){
    int strlen = *plen / 2;
    int i = 0;
    
    char* p_back = (char*) malloc(strlen + 1);
    memset(p_back,0,strlen + 1);
    
    for(i = 0 ; i < strlen ; i ++){
        int c = 0;
        c = hexchar2int(*(input+2*i))*0x10 + hexchar2int(*(input+2*i+1));
        *(p_back+i) = c;
    }
    memset(input,0,*plen);
    memcpy(input,p_back,strlen);
    free(p_back);
    *plen = strlen;
}
int ReplaceHtmlCode(char *input,int *plen){
    int len = *plen;
    if(findstrstr((u_char*)input,"&quot;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find &quot;\r\n");
        int count = replacestring(input,len,"&quot;","\"");
        len = len - count * (strlen("&quot;")-1);
    }
    if(findstrstr((u_char*)input,"<br />",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find <br />\r\n");
        int count = replacestring(input,len,"<br />","");
        len = len - count * strlen("<br />");
    }
    if(findstrstr((u_char*)input,"&gt;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find &gt;\r\n");
        int count = replacestring(input,len,"&gt;",">");
        len = len - count * (strlen("&gt;")-1);
    }
    if(findstrstr((u_char*)input,"&lt;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find &lt;\r\n");
        int count = replacestring(input,len,"&lt;","<");
        len = len - count * (strlen("&lt;")-1);
    }
    if(findstrstr((u_char*)input,"&#039;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find &#039;\r\n");
        int count = replacestring(input,len,"&#039;","'");
        len = len - count * (strlen("&#039;")-1);
    }
    if(findstrstr((u_char*)input,"&amp;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find &amp;\r\n");
        int count = replacestring(input,len,"&amp;","&");
        len = len - count * (strlen("&amp;")-1);
    }
    if(findstrstr((u_char*)input,"$20;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $20;\r\n");
        int count = replacestring(input,len,"$20;"," ");
        len = len - count * (strlen("$20;")-1);
    }
    if(findstrstr((u_char*)input,"$0a;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $0a;\r\n");
        int count = replacestring(input,len,"$0a;","\n");
        len = len - count * (strlen("$0a;")-1);
    }
    if(findstrstr((u_char*)input,"$0d;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $0d;\r\n");
        int count = replacestring(input,len,"$0d;","\r");
        len = len - count * (strlen("$0d;")-1);
    }
    if(findstrstr((u_char*)input,"$3c;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $3c;\r\n");
        int count = replacestring(input,len,"$3c;","<");
        len = len - count * (strlen("$3c;")-1);
    }
    if(findstrstr((u_char*)input,"$26;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $26;\r\n");
        int count = replacestring(input,len,"$26;","&");
        len = len - count * (strlen("$26;")-1);
    }
    if(findstrstr((u_char*)input,"$22;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $22;\r\n");
        int count = replacestring(input,len,"$22;","\"");
        len = len - count * (strlen("$22;")-1);
    }
    if(findstrstr((u_char*)input,"$3e;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $3e;\r\n");
        int count = replacestring(input,len,"$3e;",">");
        len = len - count * (strlen("$3e;")-1);
    }
    if(findstrstr((u_char*)input,"$27;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $27;\r\n");
        int count = replacestring(input,len,"$27;","\'");
        len = len - count * (strlen("$27;")-1);
    }
    if(findstrstr((u_char*)input,"$25;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $25;\r\n");
        int count = replacestring(input,len,"$25;","%");
        len = len - count * (strlen("$25;")-1);
    }
    if(findstrstr((u_char*)input,"$2b;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $2b;\r\n");
        int count = replacestring(input,len,"$2b;","+");
        len = len - count * (strlen("$2b;")-1);
    }
    if(findstrstr((u_char*)input,"$28;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $28;\r\n");
        int count = replacestring(input,len,"$28;","(");
        len = len - count * (strlen("$28;")-1);
    }
    if(findstrstr((u_char*)input,"$2f;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $2f;\r\n");
        int count = replacestring(input,len,"$2f;","/");
        len = len - count * (strlen("$2f;")-1);
    }
    if(findstrstr((u_char*)input,"$5b;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $5b;\r\n");
        int count = replacestring(input,len,"$5b;","[");
        len = len - count * (strlen("$5b;")-1);
    }
    if(findstrstr((u_char*)input,"$3a;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $3a;\r\n");
        int count = replacestring(input,len,"$3a;",":");
        len = len - count * (strlen("$3a;")-1);
    }
    if(findstrstr((u_char*)input,"$2d;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $2d;\r\n");
        int count = replacestring(input,len,"$2d;","-");
        len = len - count * (strlen("$2d;")-1);
    }
    if(findstrstr((u_char*)input,"$5d;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $5d;\r\n");
        int count = replacestring(input,len,"$5d;","]");
        len = len - count * (strlen("$5d;")-1);
    }
    if(findstrstr((u_char*)input,"$00;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $00;\r\n");
        int count = replacechar(input,len,"$00;",0x00);
        len = len - count * (strlen("$5d;")-1);
    }
    if(findstrstr((u_char*)input,"$fd;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $fd;\r\n");
        int count = replacechar(input,len,"$fd;",0xfd);
        len = len - count * (strlen("$fd;")-1);
    }
    if(findstrstr((u_char*)input,"$ff;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $ff;\r\n");
        int count = replacechar(input,len,"$ff;",0xff);
        len = len - count * (strlen("$ff;")-1);
    }
    if(findstrstr((u_char*)input,"$5c;",len) != NULL){
        printnginxlog("ReplaceHtmlCode:find $5c;\r\n");
        int count = replacestring(input,len,"$5c;","\\");
        len = len - count * (strlen("$5c;")-1);
    }
    *plen = len;
    
    return 0;
}


void malloc_ngx_str_t(ngx_http_request_t* r,ngx_str_t *src , char * dest){
    char * newdest = (char*) ngx_palloc(r->pool, strlen(dest)+1);
    memset(newdest, 0, strlen(dest)+1);
    memcpy(newdest,dest,strlen(dest));
    
    
    src->len = strlen(dest);
    src->data = (u_char*)newdest;
}

