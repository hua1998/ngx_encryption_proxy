/* 
 * File:   policymanage.h
 * Author: root
 *
 * Created on December 22, 2014, 11:59 PM
 */

#ifndef POLICYMANAGE_H
#define	POLICYMANAGE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
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
typedef struct _FieldPolicy{
    int id;
    int form_id;
    char field_name[512];
    char desc[512];
    char strType[512];
    POST_TYPE type;
    char submenu[512];
    char menu[512];
}FieldPolicy,*PFieldPolicy;

typedef struct _FormPolicy{
    int id;
    int site_id;
    char url[1024];
    char desc[1024];
    FieldPolicy *pFieldPolicy;
    int fieldPolicyLen;
    int index;
}FormPolicy,*PFormPolicy;

typedef struct _SitePolicy{
    int id;
    char host[128];
    char proxyhost[128];
    char key[512];
    char desc[512];
    FormPolicy *pFormPolicy;
    int formPolicyLen;
    int index;
}SitePolicy,*PSitePolicy;

typedef struct _SitePolicys{
    SitePolicy *pSitePolicy;
    int sitePolicyLen;
    int index;
}SitePolicys,*PSitePolicys;

typedef struct _OpreateBuf{
    char id[32];
    char site_id[32];
    char form_id[32];
    char host[128];
    char proxyhost[128];
    char key[512];
    char desc[512];
    char url[1024];
    char field_name[512];
    char type[32];
    char menu[512];
    char submenu[512];
}OpreateBuf,*POpreateBuf;
typedef struct _OpreateData{
    char action[32];
    char table[64];
    OpreateBuf buf;
}OpreateData,*POpreateData;

int UpdateSitePolicy(SitePolicy site,int type);

    
int InitXToolsPolicy();
void GetSitePolicy(SitePolicys *sites);
char *  GetSitePolicyJson(char  *buf);
char *  GetSiteJson(char *buf);
char *  GetFormJson(char *buf);
char *  GetFieldJson(char *buf);
char *  GetFormsJson(char *buf);
char *  GetFieldsJson(char *buf);

char * AddSite(char *buf);
char * AddForm(char *buf);
char * AddField(char *buf);

char * ModifySite(char *buf);
char * ModifyForm(char *buf);
char * ModifyField(char *buf);

char * DeleteSite(char *buf);
char * DeleteForm(char *buf);
char * DeleteField(char *buf);

OpreateBuf GetJsonData(char *data);

#ifdef	__cplusplus
}
#endif

#endif	/* POLICYMANAGE_H */

