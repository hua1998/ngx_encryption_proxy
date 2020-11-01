/* 
 * File:   ngx_cep_policy.h
 * Author: maozhenghua
 *
 * Created on 2014年10月22日, 下午4:55
 */

#ifndef NGX_CEP_POLICY_H
#define	NGX_CEP_POLICY_H

#include "policymanage.h"
/*
typedef enum _POST_TYPE{
    POSTFORM,
    POSTFORMS,
    POSTFORMSW,
    POSTURL,
    POSTURLS,
    POSTURLSW,
    POSTURLSPLIT,
    FORMJSON,
    URLJSON
}POST_TYPE;
typedef struct _FILTER_NAME{
    char name[256];
    POST_TYPE type;
}FILTER_NAME;

typedef struct _POST_POLICY{
    char url[1024];
    FILTER_NAME *pFilterName;
    int filterLen;
}POST_POLICY;

typedef struct _SERVER_POLICY{
    char server[256];
    POST_POLICY *pPostPolicy;
    int postPolicyLen;
}SERVER_POLICY;
*/


int InitPostPolicy();
FormPolicy* GetPostPolicy(char * server, char * url);

#endif	/* NGX_CEP_POLICY_H */

