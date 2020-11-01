#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "policymanage.h"
#include "cJSON.h"


char *dbfile = "/etc/getway.db";
void ConnectSqlite(sqlite3 **db){
    
    int rc;
    rc = sqlite3_open ( dbfile, db );
    if ( rc !=SQLITE_OK ) {
        printf ( "open sqlite error\n" );
    }else{
        //printf ( "open sqlite success\n" );
    }
}
int ExecSql(sqlite3 *db,char *sql){
    
    char *errMsg;
    int rc;
    if(db == NULL){
        return 0;
    }
    rc=sqlite3_exec ( db,sql,NULL,NULL,&errMsg );
    if ( rc != SQLITE_OK ) {
            printf ( "error:%s\n",errMsg );
            return 0;
    }else{
        return 1;
    }
}
void CloseSqlite(sqlite3 *db){
    
    if(db != NULL){
        sqlite3_close(db);
        db = NULL;
    }
}

int ExecSqlite(char *sql){
    
    sqlite3 *db=NULL;
    int rc = 0;
    ConnectSqlite(&db);   
    rc = ExecSql(db,sql);
    CloseSqlite(db);
    return rc;
}



int showCount ( void *para,int n_column,char **column_value,char **column_name ) {
    //printf ( "一共%s条记录%s\n",column_value[0],column_name[0] );
    int* pcount = (int *)para;
    *pcount = atoi(column_value[0]);
    return 0;
}
int GetTableCount(char * sql){
    
    int count = 0;
    
    sqlite3 *db=NULL;
    int rc = 0;
    ConnectSqlite(&db);   
    
    char *errMsg;
    
    if(db == NULL){
        return 0;
    }
    rc=sqlite3_exec ( db,sql,showCount,&count,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    
    
    CloseSqlite(db);
    
    return count;
}

int UpdateSitePolicy(SitePolicy site,int type){
    
    char sql[4096] = {0};
    if(type == 0){
        sprintf(sql,"insert into SitePolicy (\"id\", \"proxyhost\", \"host\", \"key\", \"desc\") values (%d, '%s', '%s', '%s', '%s')",
                site.id,site.proxyhost,site.host,site.key,site.desc);
    }else{
        sprintf(sql,"update SitePolicy set \"proxyhost\"='%s', \"host\"='%s', \"desc\"='%s' where id=%d",
                site.proxyhost,site.host,site.desc,site.id);
    }
    printf("sql:%s\n",sql);
    return ExecSqlite(sql);
}

int ShowSitePolicyTableInfo ( void *para,int n_column,char **column_value,char **column_name ) {
    PSitePolicy psite = (PSitePolicy)para;
    int i;
    
    
    for ( i=0; i<n_column; i++ ) {
        if(column_value[i] == NULL)
            continue;
        if(strcmp(column_name[i],"id") == 0){
            psite->id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"host") == 0){
            strcpy(psite->host ,column_value[i]);
        }else if(strcmp(column_name[i],"proxyhost") == 0){
            strcpy(psite->proxyhost ,column_value[i]);
        }else if(strcmp(column_name[i],"key") == 0){
            strcpy(psite->key ,column_value[i]);
        }else if(strcmp(column_name[i],"desc") == 0){
            strcpy(psite->desc ,column_value[i]);
        }
    }
    
    return 0;
}
int GetSitePolicyByHost(PSitePolicy psite){
    char sql[4096] = {0};
    
    sprintf(sql,"select * from SitePolicy where host='%s' or id=%d",psite->host,psite->id);
    
    sqlite3 *db=NULL;
    int rc = 0;
    ConnectSqlite(&db);   
    
    char *errMsg;
    
    if(db == NULL){
        return 0;
    }
    rc=sqlite3_exec ( db,sql,ShowSitePolicyTableInfo,psite,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    
    
    CloseSqlite(db);
    
    return 0;
}

int DeleteField4Form(FormPolicy form){
    char sql[4096] = {0};
    sprintf(sql,"DELETE FROM FieldPolicy WHERE form_id=%d",form.id);
    return ExecSqlite(sql);
}
int DeleteSitePolicy(SitePolicy site){
    char sql[4096] = {0};
    int i;
    QueryFormPolicy(&site);
    for(i = 0 ; i < site.formPolicyLen ; i ++){
        memset(sql,0,sizeof(sql));
        DeleteField4Form(site.pFormPolicy[i]);
        DeleteFormPolicy(site.pFormPolicy[i]);
    }
    
    
    sprintf(sql,"DELETE FROM SitePolicy WHERE id=%d",site.id);
    return ExecSqlite(sql);
}

int DeleteFormPolicy(FormPolicy form){
    char sql[4096] = {0};
    sprintf(sql,"DELETE FROM FormPolicy WHERE id=%d",form.id);
    return ExecSqlite(sql);
}

int DeleteFieldPolicy(FieldPolicy field){
    char sql[4096] = {0};
    sprintf(sql,"DELETE FROM FieldPolicy WHERE id=%d",field.id);
    return ExecSqlite(sql);
}

int UpdateFormPolicy(FormPolicy form,int type){
    char sql[4096] = {0};
    if(type == 0){
        sprintf(sql,"insert into FormPolicy (\"id\", \"site_id\", \"url\", \"desc\") values (%d, %d, '%s', '%s')",
                form.id,form.site_id,form.url,form.desc);
    }else{
        sprintf(sql,"update FormPolicy set \"site_id\"=%d, \"url\"='%s', \"desc\"='%s' where id=%d",
                form.site_id,form.url,form.desc,form.id);
    }
    printf("sql:%s\n",sql);
    return ExecSqlite(sql);
}
 


int ShowFormPolicyTableInfo ( void *para,int n_column,char **column_value,char **column_name ) {
    FormPolicy *pform = (FormPolicy*)para;
    int i;
    
    for ( i=0; i<n_column; i++ ) {
        if(column_value[i] == NULL)
            continue;
        if(strcmp(column_name[i],"id") == 0){
            pform->id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"site_id") == 0){
            pform->site_id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"url") == 0){
            strcpy(pform->url ,column_value[i]);
        }else if(strcmp(column_name[i],"desc") == 0){
            strcpy(pform->desc ,column_value[i]);
        }
    }
    
    return 0;
}
int GetFormPolicy(PFormPolicy pform){
    char sql[4096] = {0};
    
    sprintf(sql,"select * from FormPolicy where site_id=%d and url='%s'",pform->site_id,pform->url);
    
    sqlite3 *db=NULL;
    int rc = 0;
    ConnectSqlite(&db);   
    
    char *errMsg;
    
    if(db == NULL){
        return 0;
    }
    rc=sqlite3_exec ( db,sql,ShowFormPolicyTableInfo,pform,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    
    
    CloseSqlite(db);
    
    return 0;
}

void GetFieldType(POST_TYPE type,char * strType){
    if(type == FORMJSON){
        strcpy(strType,"FORMJSON");
    }else if(type == POSTFORM){
        strcpy(strType,"POSTFORM");
    }else if(type == POSTFORMS){
        strcpy(strType,"POSTFORMS");
    }else if(type == POSTFORMSW){
        strcpy(strType,"POSTFORMSW");
    }else if(type == POSTURL){
        strcpy(strType,"POSTURL");
    }else if(type == POSTURLS){
        strcpy(strType,"POSTURLS");
    }else if(type == POSTURLSPLIT){
        strcpy(strType,"POSTURLSPLIT");
    }else if(type == POSTURLSW){
        strcpy(strType,"POSTURLSW");
    }else if(type == URLJSON){
        strcpy(strType,"URLJSON");
    }
}
POST_TYPE GetPOSTTYPE(char * strType){
    if(strcmp(strType ,"FORMJSON") == 0){
        return FORMJSON;
    }else if(strcmp(strType ,"POSTFORM") == 0){
        return POSTFORM;
    }else if(strcmp(strType ,"POSTFORMS") == 0){
        return POSTFORMS;
    }else if(strcmp(strType ,"POSTFORMSW") == 0){
        return POSTFORMSW;
    }else if(strcmp(strType ,"POSTURL") == 0){
        return POSTURL;
    }else if(strcmp(strType ,"POSTURLS") == 0){
        return POSTURLS;
    }else if(strcmp(strType ,"POSTURLSPLIT") == 0){
        return POSTURLSPLIT;
    }else if(strcmp(strType ,"POSTURLSW") == 0){
        return POSTURLSW;
    }else if(strcmp(strType ,"URLJSON") == 0){
        return URLJSON;
    }
}
int UpdateFieldPolicy(FieldPolicy field,int type){
    char sql[4096] = {0};
    GetFieldType(field.type,field.strType);
    if(type == 0){
        sprintf(sql,"insert into FieldPolicy (\"id\", \"form_id\", \"field_name\", \"type\", \"desc\") values (%d, %d, '%s', '%s', '%s')",
                field.id,field.form_id,field.field_name,field.strType,field.desc);
    }else{
        sprintf(sql,"update FieldPolicy set \"form_id\"=%d, \"field_name\"='%s', \"type\"='%s', \"desc\"='%s' where id=%d",
                field.form_id,field.field_name,field.strType,field.desc,field.id);
    }
    printf("sql:%s\n",sql);
    return ExecSqlite(sql);
}

int ShowFieldPolicyTableInfo ( void *para,int n_column,char **column_value,char **column_name ) {
    FieldPolicy *pField = (FieldPolicy*)para;
    int i;
    
    for ( i=0; i<n_column; i++ ) {
        if(column_value[i] == NULL)
            continue;
        if(strcmp(column_name[i],"id") == 0){
            pField->id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"form_id") == 0){
            pField->form_id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"field_name") == 0){
             strcpy(pField->field_name ,column_value[i]);
        }else if(strcmp(column_name[i],"type") == 0){
             strcpy(pField->strType ,column_value[i]);
             pField->type = GetPOSTTYPE(column_value[i]);
             if(strlen(pField->field_name) > 0 && (pField->type == POSTFORM || pField->type == POSTFORMS || pField->type == POSTFORMSW)){
                 if(pField->field_name[strlen(pField->field_name) - 1] != '\n' ){
                     strcat(pField->field_name,"\r\n\r\n");
                 }
             }
        }else if(strcmp(column_name[i],"desc") == 0){
             strcpy(pField->desc ,column_value[i]);
        
        }else if(strcmp(column_name[i],"submenu") == 0){
             strcpy(pField->submenu ,column_value[i]);
        
        }else if(strcmp(column_name[i],"menu") == 0){
             strcpy(pField->menu ,column_value[i]);
        }
    }
    
    return 0;
}
int GetFieldPolicy(FieldPolicy *pField){
    sqlite3 *db=NULL;
    char sql[4096] = {0};
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from FieldPolicy where form_id=%d and field_name='%s'",pField->form_id,pField->field_name);

    rc=sqlite3_exec ( db,sql,ShowFieldPolicyTableInfo,pField,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
}

int ShowFieldPolicyAllTableInfo ( void *para,int n_column,char **column_value,char **column_name ) {
    FormPolicy* pForm = (FormPolicy*)para;
    int i;
    
    for ( i=0; i<n_column; i++ ) {
        if(column_value[i] == NULL)
            continue;
        if(strcmp(column_name[i],"id") == 0){
            pForm->pFieldPolicy[pForm->index].id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"form_id") == 0){
            pForm->pFieldPolicy[pForm->index].form_id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"field_name") == 0){
             strcpy(pForm->pFieldPolicy[pForm->index].field_name ,column_value[i]);
        }else if(strcmp(column_name[i],"type") == 0){
             strcpy(pForm->pFieldPolicy[pForm->index].strType ,column_value[i]);
             pForm->pFieldPolicy[pForm->index].type = GetPOSTTYPE(column_value[i]);
             if(strlen(pForm->pFieldPolicy[pForm->index].field_name) > 0 && (pForm->pFieldPolicy[pForm->index].type == POSTFORM || pForm->pFieldPolicy[pForm->index].type == POSTFORMS || pForm->pFieldPolicy[pForm->index].type == POSTFORMSW)){
                 if(pForm->pFieldPolicy[pForm->index].field_name[strlen(pForm->pFieldPolicy[pForm->index].field_name) - 1] != '\n' ){
                     strcat(pForm->pFieldPolicy[pForm->index].field_name,"\r\n\r\n");
                 }
             }
        }else if(strcmp(column_name[i],"desc") == 0){
             strcpy(pForm->pFieldPolicy[pForm->index].desc ,column_value[i]);
        }else if(strcmp(column_name[i],"submenu") == 0){
             strcpy(pForm->pFieldPolicy[pForm->index].submenu ,column_value[i]);
        }else if(strcmp(column_name[i],"menu") == 0){
             strcpy(pForm->pFieldPolicy[pForm->index].menu ,column_value[i]);
        }
    }
    pForm->index ++;
    return 0;
}
void QueryFieldPolicy(FormPolicy *pForm){
    char sql[4096] = {0};
    sprintf(sql,"select count(*) from FieldPolicy where form_id=%d ",pForm->id);
    pForm->fieldPolicyLen = GetTableCount(sql);
    pForm->pFieldPolicy = (FieldPolicy *)malloc(sizeof(FieldPolicy) * pForm->fieldPolicyLen);
    memset(pForm->pFieldPolicy ,0 , sizeof(FieldPolicy) * pForm->fieldPolicyLen);
    
    
    sqlite3 *db=NULL;
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from FieldPolicy where form_id=%d ",pForm->id);

    rc=sqlite3_exec ( db,sql,ShowFieldPolicyAllTableInfo,pForm,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
    
    printf("        field count : %d \n",pForm->fieldPolicyLen);
    int i;
    for(i = 0 ; i < pForm->fieldPolicyLen ; i ++){
        printf("        field_name : %s  desc: %s  submenu: %s  menu: %s\n" ,pForm->pFieldPolicy[i].field_name,pForm->pFieldPolicy[i].desc,pForm->pFieldPolicy[i].submenu,pForm->pFieldPolicy[i].menu);
    }
}

void QueryFieldPolicyById(FieldPolicy *pField){
    char sql[4096] = {0};
    sqlite3 *db=NULL;
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from FieldPolicy where id=%d ",pField->id);

    rc=sqlite3_exec ( db,sql,ShowFieldPolicyTableInfo,pField,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
    
}

int ShowFormPolicyAllTableInfo ( void *para,int n_column,char **column_value,char **column_name ) {
    SitePolicy* psite = (SitePolicy*)para;
    int i;
    
    for ( i=0; i<n_column; i++ ) {
        if(column_value[i] == NULL)
            continue;
        if(strcmp(column_name[i],"id") == 0){
            psite->pFormPolicy[psite->index].id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"site_id") == 0){
            psite->pFormPolicy[psite->index].site_id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"url") == 0){
            strcpy(psite->pFormPolicy[psite->index].url ,column_value[i]);
        }else if(strcmp(column_name[i],"desc") == 0){
            strcpy(psite->pFormPolicy[psite->index].desc ,column_value[i]);
        }
    }
    psite->index ++;
    return 0;
}
void QueryFormPolicy(SitePolicy *psite){
    char sql[4096] = {0};
    sprintf(sql,"select count(*) from FormPolicy where site_id=%d ",psite->id);
    psite->formPolicyLen  = GetTableCount(sql);
    psite->pFormPolicy = (FormPolicy*)malloc(sizeof(FormPolicy) * psite->formPolicyLen);
    memset(psite->pFormPolicy,0,sizeof(FormPolicy) * psite->formPolicyLen);
    
    
    sqlite3 *db=NULL;
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from FormPolicy where site_id=%d",psite->id);

    rc=sqlite3_exec ( db,sql,ShowFormPolicyAllTableInfo,psite,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
    
    int i;
    for(i = 0 ; i < psite->formPolicyLen ; i ++){
        printf("    url : %s \n",psite->pFormPolicy[i].url);
        QueryFieldPolicy(&psite->pFormPolicy[i]);
    }
}

void QueryFormPolicyById(FormPolicy *pform){
    char sql[4096] = {0};
    sqlite3 *db=NULL;
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from FormPolicy where id=%d",pform->id);

    rc=sqlite3_exec ( db,sql,ShowFormPolicyTableInfo,pform,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
    
}

int ShowSitePolicyAllTableInfo ( void *para,int n_column,char **column_value,char **column_name ) {
    SitePolicys* psites = (SitePolicys*)para;
    int i;
    
    for ( i=0; i<n_column; i++ ) {
        if(column_value[i] == NULL)
            continue;
        if(strcmp(column_name[i],"id") == 0){
            psites->pSitePolicy[psites->index].id = atoi(column_value[i]);
        }else if(strcmp(column_name[i],"host") == 0){
            strcpy(psites->pSitePolicy[psites->index].host ,column_value[i]);
        }else if(strcmp(column_name[i],"proxyhost") == 0){
            strcpy(psites->pSitePolicy[psites->index].proxyhost ,column_value[i]);
        }else if(strcmp(column_name[i],"key") == 0){
            strcpy(psites->pSitePolicy[psites->index].key ,column_value[i]);
        }else if(strcmp(column_name[i],"desc") == 0){
            strcpy(psites->pSitePolicy[psites->index].desc ,column_value[i]);
        }
    }
    psites->index ++;
    return 0;
}
void GetSitePolicy(SitePolicys *sites){
    if(sites->sitePolicyLen > 0){
        int i,j;
        for(i = 0 ; i < sites->sitePolicyLen ; i ++){
            for(j = 0 ; j < sites->pSitePolicy[i].formPolicyLen ; j ++){
                    free(sites->pSitePolicy[i].pFormPolicy[j].pFieldPolicy);
                    sites->pSitePolicy[i].pFormPolicy[j].pFieldPolicy = NULL;
            }
            free(sites->pSitePolicy[i].pFormPolicy);
            sites->pSitePolicy[i].pFormPolicy = NULL;
        }
        free(sites->pSitePolicy);
        sites->pSitePolicy = NULL;
        sites->sitePolicyLen = 0;
        sites->index = 0;
    }
    char sql[4096] = {0};
    sprintf(sql,"select count(*) from %s ","SitePolicy");
    sites->sitePolicyLen  = GetTableCount(sql);
    sites->pSitePolicy = (PSitePolicy)malloc(sizeof(SitePolicy) * (sites->sitePolicyLen));
    memset(sites->pSitePolicy,0,sizeof(SitePolicy) * (sites->sitePolicyLen));
    
    sqlite3 *db=NULL;
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from SitePolicy");

    rc=sqlite3_exec ( db,sql,ShowSitePolicyAllTableInfo,sites,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
    
    int i;
    printf("site count : %d \n",sites->sitePolicyLen);
    for(i = 0 ; i < sites->sitePolicyLen ; i ++){
        printf("site host : %s   proxyhost: %s\n",sites->pSitePolicy[i].host,sites->pSitePolicy[i].proxyhost);
        QueryFormPolicy(&sites->pSitePolicy[i]);
    }
}
void GetSitesPolicy(SitePolicys *sites){
    
    char sql[4096] = {0};
    sprintf(sql,"select count(*) from %s ","SitePolicy");
    sites->sitePolicyLen  = GetTableCount(sql);
    sites->pSitePolicy = (PSitePolicy)malloc(sizeof(SitePolicy) * (sites->sitePolicyLen));
    memset(sites->pSitePolicy,0,sizeof(SitePolicy) * (sites->sitePolicyLen));
    
    sqlite3 *db=NULL;
    char *errMsg;
    int rc = 0;
    ConnectSqlite(&db);       
    if(db == NULL){
        return;
    }
    
    sprintf(sql,"select * from SitePolicy");

    rc=sqlite3_exec ( db,sql,ShowSitePolicyAllTableInfo,sites,&errMsg );
    if ( rc != SQLITE_OK ) {
        printf ( "error:%s\n",errMsg );
    }
    CloseSqlite(db);
    
}
char* concat(char *s1, char *s2)
{
    if(s1 == NULL){
        char *result = malloc(strlen(s2)+1);
        memset(result,0,strlen(s2)+1);
        strcpy(result, s2);
        return result;
    }else{
        char *result = malloc(strlen(s1)+strlen(s2)+1);
        memset(result,0,strlen(s1)+strlen(s2)+1);
        strcpy(result, s1);
        strcat(result, s2);
        free(s1);
        return result;
    }
}
char* concat2(char *s1, int i2)
{
    char s2[20] = {0};
    sprintf(s2,"%d",i2);
    return concat(s1,s2);
}

int replace_counts(unsigned char * data,unsigned char dest){
    int count = 0;
    int i = 0;
    for(i = 0 ; i < strlen(data) ; i ++){
        unsigned char c = *(data+i);
        
        if(c == dest){
            count += 1;
        }
    }
    return count ;
}
char * findstrchar(char * src, char dest, int len) {
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
        point = findstrchar(input_head, pSrc, inputLen-offset);
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


char* jsoncat(char *json, char *s1, char *s2){
    json = concat(json,s1);
    json = concat(json,":\"");
    
    int isnew = 0;
    int offset = replace_counts(s2,'\r');
    if(offset > 0){
        char* p_back = ( char*) malloc(strlen(s2)+offset+1);
        memset(p_back,0x0,strlen(s2)+offset+1);
        replace_char(s2, strlen(s2), p_back, '\r', "\\r");
        s2 = p_back;
        isnew = 1;
    }
    offset = replace_counts(s2,'\n');
    if(offset > 0){
        char* p_back = ( char*) malloc(strlen(s2)+offset+1);
        memset(p_back,0x0,strlen(s2)+offset+1);
        replace_char(s2, strlen(s2), p_back, '\n', "\\n");
        if(isnew == 1){
            free(s2);
        }
        s2 = p_back;
        isnew = 1;
    }
    offset = replace_counts(s2,'\"');
    if(offset > 0){
        char* p_back = ( char*) malloc(strlen(s2)+offset+1);
        memset(p_back,0x0,strlen(s2)+offset+1);
        replace_char(s2, strlen(s2), p_back, '\"', "\\\"");
        if(isnew == 1){
            free(s2);
        }
        s2 = p_back;
        isnew = 1;
    }
    json = concat(json,s2);
    if(isnew == 1){
            free(s2);
    }
    
    json = concat(json,"\"");
    return  json;
}
char* jsoncat2(char *json, char *s1, int s2){
    json = concat(json,s1);
    json = concat(json,":\"");
    json = concat2(json,s2);
    json = concat(json,"\"");
    return json;
}
char *  GetSitePolicyJson(char *buf){
    
    
    SitePolicys sites;
    memset(&sites,0,sizeof(SitePolicys));
   
    GetSitesPolicy(&sites);

    int i;
    char *json = NULL;
    json = concat(json, "{");
    json = concat(json, "\"sites\":[");
    for (i = 0; i < sites.sitePolicyLen; i++) {
        json = concat(json, "{");
        json = jsoncat2(json, "\"id\"", sites.pSitePolicy[i].id);
        json = concat(json, ",");
        json = jsoncat(json, "\"host\"", sites.pSitePolicy[i].host);
        json = concat(json, ",");
        json = jsoncat(json, "\"proxyhost\"", sites.pSitePolicy[i].proxyhost);
        json = concat(json, ",");
        json = jsoncat(json, "\"key\"", sites.pSitePolicy[i].key);
        json = concat(json, ",");
        json = jsoncat(json, "\"desc\"", sites.pSitePolicy[i].desc);
        json = concat(json, "}");
        if (i < sites.sitePolicyLen - 1) {
            json = concat(json, ",");
        }
    }
    json = concat(json, "]");


    json = concat(json, "}");

    printf("%s\n", json);
    return json;
}

char *  GetSiteJson(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.host) <= 0 && strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    
    SitePolicy site;
    memset(&site,0,sizeof(SitePolicy));
    strcpy(site.host,jsonbuf.host);
    if(strlen(jsonbuf.id)>0){
        site.id = atoi(jsonbuf.id);
    }
    GetSitePolicyByHost(&site);
    char *json = NULL;
    json = concat(json, "{");

    json = jsoncat2(json, "\"id\"", site.id);
    json = concat(json, ",");
    json = jsoncat(json, "\"host\"", site.host);
    json = concat(json, ",");
    json = jsoncat(json, "\"proxyhost\"", site.proxyhost);
    json = concat(json, ",");
    json = jsoncat(json, "\"key\"", site.key);
    json = concat(json, ",");
    json = jsoncat(json, "\"desc\"", site.desc);
    json = concat(json, "}");
    return json;
    
}
char *  GetFormJson(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FormPolicy form;
    memset(&form,0,sizeof(FormPolicy));
    form.id = atoi(jsonbuf.id);
    QueryFormPolicyById(&form);
    char *json = NULL;
    json = concat(json, "{");

    json = jsoncat2(json, "\"id\"", form.id);
    json = concat(json, ",");
    json = jsoncat2(json, "\"site_id\"", form.site_id);
    json = concat(json, ",");
    json = jsoncat(json, "\"url\"", form.url);
    json = concat(json, ",");
    json = jsoncat(json, "\"desc\"", form.desc);
    json = concat(json, "}");
    return json;
}

char *  GetFormsJson(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    SitePolicy site;
    memset(&site,0,sizeof(SitePolicy));
    site.id = atoi(jsonbuf.id);
    
    QueryFormPolicy(&site);
    
    int i;
    char *json = NULL;
    json = concat(json, "{\"forms\":[");
    
    for(i = 0 ; i < site.formPolicyLen ; i ++){
        json = concat(json, "{");
        json = jsoncat2(json, "\"id\"", site.pFormPolicy[i].id);
        json = concat(json, ",");
        json = jsoncat2(json, "\"site_id\"", site.pFormPolicy[i].site_id);
        json = concat(json, ",");
        json = jsoncat(json, "\"url\"", site.pFormPolicy[i].url);
        json = concat(json, ",");
        json = jsoncat(json, "\"desc\"", site.pFormPolicy[i].desc);
        json = concat(json, "}");
        
        if (i < site.formPolicyLen - 1) {
            json = concat(json, ",");
        }
    }
    json = concat(json, "]}");
    return json;
}

char *  GetFieldsJson(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FormPolicy form;
    memset(&form,0,sizeof(FormPolicy));
    form.id = atoi(jsonbuf.id);
    QueryFieldPolicy(&form);
    
    int i;        
    char *json = NULL;
    json = concat(json, "{\"fields\":[");

    for( i = 0 ; i < form.fieldPolicyLen ; i ++){
        json = concat(json, "{");
        json = jsoncat2(json, "\"id\"", form.pFieldPolicy[i].id);
        json = concat(json, ",");
        json = jsoncat2(json, "\"form_id\"", form.pFieldPolicy[i].form_id);
        json = concat(json, ",");
        json = jsoncat(json, "\"field_name\"", form.pFieldPolicy[i].field_name);
        json = concat(json, ",");
        json = jsoncat(json, "\"desc\"", form.pFieldPolicy[i].desc);
        json = concat(json, ",");
        json = jsoncat(json, "\"submenu\"", form.pFieldPolicy[i].submenu);
        json = concat(json, ",");
        json = jsoncat(json, "\"menu\"", form.pFieldPolicy[i].menu);
        json = concat(json, "}");
        
        
        if (i < form.fieldPolicyLen - 1) {
            json = concat(json, ",");
        }
    }
    
    json = concat(json, "]}");
    return json;
}

char *  GetFieldJson(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FieldPolicy field;
    memset(&field,0,sizeof(FieldPolicy));
    field.id = atoi(jsonbuf.id);
    QueryFieldPolicyById(&field);
    char *json = NULL;
    json = concat(json, "{");

    json = jsoncat2(json, "\"id\"", field.id);
    json = concat(json, ",");
    json = jsoncat2(json, "\"form_id\"", field.form_id);
    json = concat(json, ",");
    json = jsoncat(json, "\"field_name\"", field.field_name);
    json = concat(json, ",");
    json = jsoncat(json, "\"desc\"", field.desc);
    json = concat(json, ",");
    json = jsoncat(json, "\"submenu\"", field.submenu);
    json = concat(json, ",");
    json = jsoncat(json, "\"menu\"", field.menu);
    json = concat(json, "}");
    return json;
}
char * AddSite(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.host) <= 0 || strlen(jsonbuf.proxyhost) <= 0 || strlen(jsonbuf.key) <= 0 ){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    
    SitePolicy site;
    memset(&site,0,sizeof(SitePolicy));
    site.id = atoi(jsonbuf.id);
    strcpy(site.host,jsonbuf.host);
    strcpy(site.proxyhost,jsonbuf.proxyhost);
    strcpy(site.key,jsonbuf.key);
    strcpy(site.desc,jsonbuf.desc);
    
    if(UpdateSitePolicy(site,0) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    GetSitePolicyByHost(&site);
    char *json = NULL;
    json = concat(json, "{");

    json = jsoncat2(json, "\"id\"", site.id);
    json = concat(json, ",");
    json = jsoncat(json, "\"host\"", site.host);
    json = concat(json, ",");
    json = jsoncat(json, "\"proxyhost\"", site.proxyhost);
    json = concat(json, ",");
    json = jsoncat(json, "\"desc\"", site.desc);
    json = concat(json, "}");
    return json;
}
char * AddForm(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.site_id) <= 0 || strlen(jsonbuf.url) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FormPolicy form;
    memset(&form,0,sizeof(FormPolicy));
    form.id = atoi(jsonbuf.id);
    form.site_id = atoi(jsonbuf.site_id);
    strcpy(form.url,jsonbuf.url);
    strcpy(form.desc,jsonbuf.desc);
    
    if(UpdateFormPolicy(form,0) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    GetFormPolicy(&form);
    char *json = NULL;
    json = concat(json, "{");

    json = jsoncat2(json, "\"id\"", form.id);
    json = concat(json, ",");
    json = jsoncat2(json, "\"site_id\"", form.site_id);
    json = concat(json, ",");
    json = jsoncat(json, "\"url\"", form.url);
    json = concat(json, ",");
    json = jsoncat(json, "\"desc\"", form.desc);
    json = concat(json, "}");
    return json;
}
char * AddField(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.form_id) <= 0 || strlen(jsonbuf.field_name) <= 0 ){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FieldPolicy field;
    memset(&field,0,sizeof(FieldPolicy));
    field.id = atoi(jsonbuf.id);
    field.form_id = atoi(jsonbuf.form_id);
    strcpy(field.field_name,jsonbuf.field_name);
    strcpy(field.strType,jsonbuf.type);
    strcpy(field.desc,jsonbuf.desc);
    
    if(UpdateFieldPolicy(field,0) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    GetFieldPolicy(&field);
    
    char *json = NULL;
    json = concat(json, "{");

    json = jsoncat2(json, "\"id\"", field.id);
    json = concat(json, ",");
    json = jsoncat2(json, "\"form_id\"", field.form_id);
    json = concat(json, ",");
    json = jsoncat(json, "\"field_name\"", field.field_name);
    json = concat(json, ",");
    json = jsoncat(json, "\"desc\"", field.desc);
    json = concat(json, ",");
    json = jsoncat(json, "\"submenu\"", field.submenu);
    json = concat(json, ",");
    json = jsoncat(json, "\"menu\"", field.menu);
    json = concat(json, "}");
    return json;
}

char * ModifySite(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0 ||strlen(jsonbuf.host) <= 0 || strlen(jsonbuf.proxyhost) <= 0 ){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    
    SitePolicy site;
    memset(&site,0,sizeof(SitePolicy));
    site.id = atoi(jsonbuf.id);
    strcpy(site.host,jsonbuf.host);
    strcpy(site.proxyhost,jsonbuf.proxyhost);
    strcpy(site.key,jsonbuf.key);
    strcpy(site.desc,jsonbuf.desc);
    
    if(UpdateSitePolicy(site,1) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }else{
        char *json = NULL;
        json = concat(json,"{\"success\":\"true\"}");
        return json;
    }
    
}
char * ModifyForm(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0 ||strlen(jsonbuf.site_id) <= 0 || strlen(jsonbuf.url) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FormPolicy form;
    memset(&form,0,sizeof(FormPolicy));
    form.id = atoi(jsonbuf.id);
    form.site_id = atoi(jsonbuf.site_id);
    strcpy(form.url,jsonbuf.url);
    strcpy(form.desc,jsonbuf.desc);
    
    if(UpdateFormPolicy(form,1) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }else{
        char *json = NULL;
        json = concat(json,"{\"success\":\"true\"}");
        return json;
    }
    
}
char * ModifyField(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0 ||strlen(jsonbuf.form_id) <= 0 || strlen(jsonbuf.field_name) <= 0 ){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FieldPolicy field;
    memset(&field,0,sizeof(FieldPolicy));
    field.id = atoi(jsonbuf.id);
    field.form_id = atoi(jsonbuf.form_id);
    strcpy(field.field_name,jsonbuf.field_name);
    strcpy(field.strType,jsonbuf.type);
    strcpy(field.desc,jsonbuf.desc);
    
    if(UpdateFieldPolicy(field,1) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }else{
        char *json = NULL;
        json = concat(json,"{\"success\":\"true\"}");
        return json;
    }
}

char * DeleteSite(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    SitePolicy site;
    memset(&site,0,sizeof(SitePolicy));
    site.id = atoi(jsonbuf.id);
    if(DeleteSitePolicy(site) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }else{
        char *json = NULL;
        json = concat(json,"{\"success\":\"true\"}");
        return json;
    }
}
char * DeleteForm(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FormPolicy form;
    memset(&form,0,sizeof(FormPolicy));
    form.id = atoi(jsonbuf.id);
    if(DeleteFormPolicy(form) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }else{
        char *json = NULL;
        json = concat(json,"{\"success\":\"true\"}");
        return json;
    }
}
char * DeleteField(char *buf){
    OpreateBuf jsonbuf = GetJsonData(buf);
    if(strlen(jsonbuf.id) <= 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }
    FieldPolicy field;
    memset(&field,0,sizeof(FieldPolicy));
    field.id = atoi(jsonbuf.id);
    if(DeleteFieldPolicy(field) == 0){
        char *json = NULL;
        json = concat(json,"{\"success\":\"false\"}");
        return json;
    }else{
        char *json = NULL;
        json = concat(json,"{\"success\":\"true\"}");
        return json;
    }
}
OpreateBuf GetJsonData(char *data){
    OpreateBuf oData;
    memset(&oData,0,sizeof(oData));
    if(data == NULL){
        return oData;
    }
    cJSON * pJson = cJSON_Parse(data);
    if(NULL == pJson)                                                                                         
    {
      return oData;
    }

    
    
    
    cJSON * pSubSub = cJSON_GetObjectItem(pJson, "id");
    if(NULL != pSubSub)
    {
        printf("id : %s\n", pSubSub->valuestring);
        strcpy(oData.id,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "site_id");
    if(NULL != pSubSub)
    {
        printf("site_id : %s\n", pSubSub->valuestring);
        strcpy(oData.site_id,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "form_id");
    if(NULL != pSubSub)
    {
        printf("form_id : %s\n", pSubSub->valuestring);
        strcpy(oData.form_id,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "host");
    if(NULL != pSubSub)
    {
        printf("host : %s\n", pSubSub->valuestring);
        strcpy(oData.host,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "proxyhost");
    if(NULL != pSubSub)
    {
        printf("proxyhost : %s\n", pSubSub->valuestring);
        strcpy(oData.proxyhost,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "key");
    if(NULL != pSubSub)
    {
        printf("key : %s\n", pSubSub->valuestring);
        strcpy(oData.key,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "desc");
    if(NULL != pSubSub)
    {
        printf("desc : %s\n", pSubSub->valuestring);
        strcpy(oData.desc,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "url");
    if(NULL != pSubSub)
    {
        printf("url : %s\n", pSubSub->valuestring);
        strcpy(oData.url,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "field_name");
    if(NULL != pSubSub)
    {
        printf("field_name : %s\n", pSubSub->valuestring);
        strcpy(oData.field_name,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "type");
    if(NULL != pSubSub)
    {
        printf("type : %s\n", pSubSub->valuestring);
        strcpy(oData.type,pSubSub->valuestring);
    }
    pSubSub = cJSON_GetObjectItem(pJson, "submenu");
    if(NULL != pSubSub)
    {
        printf("submenu : %s\n", pSubSub->valuestring);
        strcpy(oData.submenu,pSubSub->valuestring);
    }
    
    pSubSub = cJSON_GetObjectItem(pJson, "menu");
    if(NULL != pSubSub)
    {
        printf("menu : %s\n", pSubSub->valuestring);
        strcpy(oData.menu,pSubSub->valuestring);
    }
    

    cJSON_Delete(pJson);
    return oData;
}

SitePolicy xtoolPolicy[20];
int serverCount = 1;

int InitXToolsPolicy(){
    
    memset(xtoolPolicy,0,sizeof(xtoolPolicy));
    strcpy(xtoolPolicy[0].host,"t2.xtcrm.com");
    strcpy(xtoolPolicy[0].proxyhost,"example.com");
    strcpy(xtoolPolicy[0].key,"11223344556677881122334455667788");
    strcpy(xtoolPolicy[0].desc,"xtools");
    
    xtoolPolicy[0].formPolicyLen = 16;
    xtoolPolicy[0].pFormPolicy = (FormPolicy*)malloc(sizeof(FormPolicy)*xtoolPolicy[0].formPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy,0,sizeof(FormPolicy)*xtoolPolicy[0].formPolicyLen);
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].url,"/system/editok.xt");
    xtoolPolicy[0].pFormPolicy[0].fieldPolicyLen = 236;
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[0].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[0].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[0].field_name,"name=\"dt_postit_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[0].type = POSTFORM;   
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[1].field_name,"name=\"dt_telnote_note\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[1].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[2].field_name,"name=\"dt_bulletin_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[2].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[3].field_name,"name=\"dt_bulletin_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[3].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[4].field_name,"name=\"dt_customer_cu_name_ori\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[4].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[5].field_name,"name=\"dt_customer_cu_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[5].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[6].field_name,"name=\"dt_customer_m_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[6].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[7].field_name,"name=\"dt_customer_hotmemo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[7].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[8].field_name,"name=\"dt_customer_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[8].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[9].field_name,"name=\"dt_customer_info\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[9].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[10].field_name,"name=\"dt_customer_pst\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[10].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[11].field_name,"name=\"dt_customer_tel\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[11].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[12].field_name,"name=\"dt_customer_fax\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[12].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[13].field_name,"name=\"dt_customer_city\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[13].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[14].field_name,"name=\"dt_customer_web\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[14].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[15].field_name,"name=\"dt_customer_district\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[15].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[16].field_name,"name=\"dt_customer_address\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[16].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[17].field_name,"name=\"dt_customer_cu_remark\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[17].type = POSTFORM;
    
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[18].field_name,"name=\"dt_cuview_name_ori\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[18].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[19].field_name,"name=\"dt_cuview_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[19].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[20].field_name,"name=\"dt_cuview_mphone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[20].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[21].field_name,"name=\"dt_cuview_hotmemo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[21].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[22].field_name,"name=\"dt_cuview_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[22].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[23].field_name,"name=\"dt_cuview_phone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[23].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[24].field_name,"name=\"dt_cuview_email\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[24].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[25].field_name,"name=\"dt_cuview_h_phone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[25].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[26].field_name,"name=\"dt_cuview_msn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[26].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[27].field_name,"name=\"dt_cuview_fax\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[27].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[28].field_name,"name=\"dt_cuview_ww\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[28].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[29].field_name,"name=\"dt_cuview_h_pst\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[29].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[30].field_name,"name=\"dt_cuview_skype\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[30].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[31].field_name,"name=\"dt_cuview_h_addr\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[31].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[32].field_name,"name=\"dt_cuview_pst\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[32].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[33].field_name,"name=\"dt_cuview_department\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[33].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[34].field_name,"name=\"dt_cuview_preside\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[34].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[35].field_name,"name=\"dt_cuview_city\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[35].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[36].field_name,"name=\"dt_cuview_appellation\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[36].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[37].field_name,"name=\"dt_cuview_district\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[37].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[38].field_name,"name=\"dt_cuview_headship\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[38].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[39].field_name,"name=\"dt_cuview_address\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[39].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[40].field_name,"name=\"dt_cuview_web\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[40].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[41].field_name,"name=\"dt_cuview_befontof\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[41].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[42].field_name,"name=\"dt_cuview_cr_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[42].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[43].field_name,"name=\"dt_cuview_remark\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[43].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[44].field_name,"name=\"dt_cu_care_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[44].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[45].field_name,"name=\"dt_cu_care_description\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[45].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[46].field_name,"name=\"dt_cu_care_feed_back\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[46].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[47].field_name,"name=\"dt_cu_care_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[47].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[48].field_name,"name=\"dt_costdetail_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[48].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[49].field_name,"name=\"dt_costdetail_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[49].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[50].field_name,"name=\"dt_action_title\"\r\n\r\n");          //日程任务->日程
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[50].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[51].field_name,"name=\"dt_action_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[51].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[52].field_name,"name=\"dt_cupool_name\"\r\n\r\n");          //日程任务->批量客户任务
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[52].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[53].field_name,"name=\"dt_cupool_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[53].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[54].field_name,"name=\"dt_contact_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[54].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[55].field_name,"name=\"dt_contact_preside\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[55].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[56].field_name,"name=\"dt_contact_appellation\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[56].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[57].field_name,"name=\"dt_contact_department\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[57].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[58].field_name,"name=\"dt_contact_headship\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[58].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[59].field_name,"name=\"dt_contact_phone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[59].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[60].field_name,"name=\"dt_contact_email\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[60].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[61].field_name,"name=\"dt_contact_mphone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[61].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[62].field_name,"name=\"dt_contact_msn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[62].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[63].field_name,"name=\"dt_contact_h_phone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[63].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[64].field_name,"name=\"dt_contact_skype\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[64].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[65].field_name,"name=\"dt_contact_fax\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[65].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[66].field_name,"name=\"dt_contact_ww\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[66].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[67].field_name,"name=\"dt_contact_h_pst\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[67].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[68].field_name,"name=\"dt_contact_h_addr\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[68].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[69].field_name,"name=\"dt_contact_befontof\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[69].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[70].field_name,"name=\"dt_contact_cr_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[70].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[71].field_name,"name=\"dt_contact_remark\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[71].type = POSTFORM;


    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[72].field_name,"name=\"dt_memday_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[72].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[73].field_name,"name=\"dt_opport_oppname\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[73].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[74].field_name,"name=\"dt_opport_provider\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[74].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[75].field_name,"name=\"dt_opport_cu_require\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[75].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[76].field_name,"name=\"dt_opport_pnote\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[76].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[77].field_name,"name=\"dt_price_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[77].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[78].field_name,"name=\"dt_price_number\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[78].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[79].field_name,"name=\"dt_price_s_contact\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[79].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[80].field_name,"name=\"dt_price_pgoods\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[80].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[81].field_name,"name=\"dt_price_pgath\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[81].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[82].field_name,"name=\"dt_price_ptrans\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[82].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[83].field_name,"name=\"dt_price_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[83].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[84].field_name,"name=\"dt_demand_topic\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[84].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[85].field_name,"name=\"dt_demand_provider\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[85].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[86].field_name,"name=\"dt_demand_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[86].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[87].field_name,"name=\"dt_solution_topic\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[87].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[88].field_name,"name=\"dt_solution_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[88].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[89].field_name,"name=\"dt_solution_feedback\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[89].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[90].field_name,"name=\"dt_opponent_company\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[90].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[91].field_name,"name=\"dt_opponent_prosol\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[91].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[92].field_name,"name=\"dt_opponent_pred\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[92].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[93].field_name,"name=\"dt_opponent_infer\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[93].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[94].field_name,"name=\"dt_opponent_mane\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[94].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[95].field_name,"name=\"dt_opponent_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[95].type = POSTFORM;


    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[96].field_name,"name=\"dt_contract_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[96].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[97].field_name,"name=\"dt_contract_number\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[97].type = POSTFORMS;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[98].field_name,"name=\"dt_contract_sum_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[98].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[99].field_name,"name=\"dt_contract_deli_place\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[99].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[100].field_name,"name=\"dt_contract_pro_ser\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[100].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[101].field_name,"name=\"dt_contract_cu_sub\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[101].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[102].field_name,"name=\"dt_contract_contract\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[102].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[103].field_name,"name=\"dt_contract_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[103].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[104].field_name,"name=\"dt_contract_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[104].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[105].field_name,"name=\"dt_contract_tel\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[105].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[106].field_name,"name=\"dt_contract_mphone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[106].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[107].field_name,"name=\"dt_contract_addr\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[107].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[108].field_name,"name=\"dt_contract_pst\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[108].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[109].field_name,"name=\"dt_salesticket_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[109].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[110].field_name,"name=\"dt_salesticket_number\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[110].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[111].field_name,"name=\"dt_salesticket_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[111].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[112].field_name,"name=\"dt_salesticket_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[112].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[113].field_name,"name=\"dt_salesticket_billsn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[113].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[114].field_name,"name=\"dt_salesticket_send_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[114].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[115].field_name,"name=\"dt_salesticket_send_tel\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[115].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[116].field_name,"name=\"dt_salesticket_send_mphone\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[116].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[117].field_name,"name=\"dt_salesticket_send_addr\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[117].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[118].field_name,"name=\"dt_salesticket_send_pst\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[118].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[119].field_name,"name=\"dt_salesticket_sendcomp\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[119].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[120].field_name,"name=\"dt_salesticket_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[120].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[121].field_name,"name=\"dt_product_pro_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[121].type = POSTFORMS;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[122].field_name,"name=\"dt_product_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[122].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[123].field_name,"name=\"dt_product_model\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[123].type = POSTFORMS;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[124].field_name,"name=\"dt_product_str_ps_1\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[124].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[125].field_name,"name=\"dt_product_str_ps_2\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[125].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[126].field_name,"name=\"dt_product_str_ps_3\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[126].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[127].field_name,"name=\"dt_product_list_remark\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[127].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[128].field_name,"name=\"dt_product_discount\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[128].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[129].field_name,"name=\"dt_product_intro\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[129].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[130].field_name,"name=\"dt_product_parameter\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[130].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[131].field_name,"name=\"dt_product_faq\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[131].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[132].field_name,"name=\"dt_product_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[132].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[133].field_name,"name=\"dt_goods_model\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[133].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[134].field_name,"name=\"dt_goods_request\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[134].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[135].field_name,"name=\"dt_goods_send_condit\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[135].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[136].field_name,"name=\"dt_goods_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[136].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[137].field_name,"name=\"dt_deli_note_cu_who\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[137].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[138].field_name,"name=\"dt_deli_note_plan\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[138].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[139].field_name,"name=\"dt_gathering_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[139].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[140].field_name,"name=\"dt_gathering_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[140].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[141].field_name,"name=\"dt_gathering_note_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[141].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[142].field_name,"name=\"dt_gathering_note_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[142].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[143].field_name,"name=\"dt_bill_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[143].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[144].field_name,"name=\"dt_bill_billsn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[144].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[145].field_name,"name=\"dt_bill_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[145].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[146].field_name,"name=\"dt_project_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[146].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[147].field_name,"name=\"dt_project_pnote\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[147].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[148].field_name,"name=\"dt_project_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[148].type = POSTFORM;


    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[149].field_name,"name=\"dt_prjitem_note\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[149].type = POSTFORM;


    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[150].field_name,"&dt_costdetail_money_memo=");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[150].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[151].field_name,"&dt_costdetail_memo=");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[151].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[152].field_name,"name=\"dt_sn_status_sn_num\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[152].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[153].field_name,"name=\"dt_honor_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[153].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[154].field_name,"name=\"dt_honor_description\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[154].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[155].field_name,"name=\"dt_honor_remark\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[155].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[156].field_name,"name=\"location0\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[156].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[157].field_name,"name=\"location1\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[157].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[158].field_name,"name=\"location2\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[158].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[159].field_name,"name=\"dt_location_product_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[159].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[160].field_name,"name=\"dt_libstock_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[160].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[161].field_name,"name=\"dt_libstock_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[161].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[162].field_name,"name=\"dt_libout_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[162].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[163].field_name,"name=\"dt_libout_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[163].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[164].field_name,"name=\"dt_libmov_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[164].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[165].field_name,"name=\"dt_libmov_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[165].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[166].field_name,"name=\"dt_libpack_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[166].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[167].field_name,"name=\"dt_libpack_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[167].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[168].field_name,"name=\"dt_libin_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[168].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[169].field_name,"name=\"dt_libin_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[169].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[170].field_name,"name=\"dt_purchase_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[170].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[171].field_name,"name=\"dt_purchase_cu_sub\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[171].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[172].field_name,"name=\"dt_purchase_number\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[172].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[173].field_name,"name=\"dt_purchase_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[173].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[174].field_name,"name=\"dt_pay_plan_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[174].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[175].field_name,"name=\"dt_pay_plan_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[175].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[176].field_name,"name=\"dt_pay_note_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[176].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[177].field_name,"name=\"dt_pay_note_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[177].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[178].field_name,"name=\"dt_pay_bill_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[178].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[179].field_name,"name=\"dt_pay_bill_billsn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[179].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[180].field_name,"name=\"dt_pay_bill_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[180].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[181].field_name,"name=\"dt_purreturn_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[181].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[182].field_name,"name=\"dt_purreturn_return_no\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[182].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[183].field_name,"name=\"dt_purreturn_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[183].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[184].field_name,"name=\"dt_market_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[184].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[185].field_name,"name=\"dt_market_place\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[185].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[186].field_name,"name=\"dt_market_plan\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[186].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[187].field_name,"name=\"dt_market_execute\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[187].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[188].field_name,"name=\"dt_market_sum_up\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[188].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[189].field_name,"name=\"dt_market_evaluate\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[189].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[190].field_name,"name=\"dt_market_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[190].type = POSTFORM;


    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[191].field_name,"name=\"dt_market_child_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[191].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[192].field_name,"name=\"dt_market_child_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[192].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[193].field_name,"name=\"dt_market_child_backmemo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[193].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[194].field_name,"name=\"dt_ad_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[194].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[195].field_name,"name=\"dt_ad_media\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[195].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[196].field_name,"name=\"dt_ad_timetable\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[196].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[197].field_name,"name=\"dt_ad_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[197].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[198].field_name,"name=\"dt_ad_ad_agent\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[198].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[199].field_name,"name=\"dt_ad_linkman\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[199].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[200].field_name,"name=\"dt_ad_contact\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[200].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[201].field_name,"name=\"dt_ad_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[201].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[202].field_name,"name=\"dt_sheet_sheet_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[202].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[203].field_name,"name=\"dt_sheet_depict\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[203].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[204].field_name,"name=\"dt_sheet_printery\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[204].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[205].field_name,"name=\"dt_sheet_linkman\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[205].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[206].field_name,"name=\"dt_sheet_contact\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[206].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[207].field_name,"name=\"dt_sheet_draw_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[207].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[208].field_name,"name=\"dt_gift_name\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[208].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[209].field_name,"name=\"dt_gift_depict\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[209].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[210].field_name,"name=\"dt_gift_manufacturer\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[210].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[211].field_name,"name=\"dt_gift_contact\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[211].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[212].field_name,"name=\"dt_gift_linkman\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[212].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[213].field_name,"name=\"dt_gift_draw_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[213].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[214].field_name,"name=\"dt_qa_qq\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[214].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[215].field_name,"name=\"dt_qa_aa\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[215].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[216].field_name,"name=\"dt_qa_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[216].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[217].field_name,"name=\"dt_lodge_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[217].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[218].field_name,"name=\"dt_lodge_1st_takein\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[218].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[219].field_name,"name=\"dt_lodge_description\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[219].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[220].field_name,"name=\"dt_lodge_solve\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[220].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[221].field_name,"name=\"dt_lodge_feed_back\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[221].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[222].field_name,"name=\"dt_lodge_checkback\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[222].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[223].field_name,"name=\"dt_lodge_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[223].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[224].field_name,"name=\"dt_cu_service_subject\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[224].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[225].field_name,"name=\"dt_cu_service_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[225].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[226].field_name,"name=\"dt_cu_service_feedback\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[226].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[227].field_name,"name=\"dt_cu_service_memo\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[227].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[228].field_name,"name=\"dt_repairinfo_p_sn\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[228].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[229].field_name,"name=\"dt_repairinfo_p_info\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[229].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[230].field_name,"name=\"dt_repairinfo_p_notice\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[230].type = POSTFORM;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[231].field_name,"name=\"REPAIR_L*\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[231].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[232].field_name,"name=\"REPAIR_R*\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[232].type = POSTFORMSW;

    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[232].field_name,"name=\"dt_km_title\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[233].type = POSTFORM;
    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[234].field_name,"name=\"dt_km_content\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[234].type = POSTFORM;


    strcpy(xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[235].field_name,"name=\"dt_contact_weixin\"\r\n\r\n");
    xtoolPolicy[0].pFormPolicy[0].pFieldPolicy[235].type = POSTFORM;









































    
    strcpy(xtoolPolicy[0].pFormPolicy[1].url,"/system/qssub.xt");
    xtoolPolicy[0].pFormPolicy[1].fieldPolicyLen = 72;
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[1].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[1].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[0].field_name,"&T1=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[1].field_name,"&FS_title_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[2].field_name,"&FS_content_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[2].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[3].field_name,"&FS_note_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[3].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[4].field_name,"&T2=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[4].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[5].field_name,"&FS_subject_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[5].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[6].field_name,"&FS_cu_name_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[6].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[7].field_name,"&FS_m_name_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[7].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[8].field_name,"&FS_sn_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[8].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[9].field_name,"&FS_tel_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[9].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[10].field_name,"&FS_fax_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[10].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[11].field_name,"&FS_web_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[11].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[12].field_name,"&FS_city_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[12].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[13].field_name,"&FS_district_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[13].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[14].field_name,"&FS_pst_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[14].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[15].field_name,"&FS_address_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[15].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[16].field_name,"&FS_cu_remark_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[16].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[17].field_name,"&FS_money_memo_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[17].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[18].field_name,"&FS_name_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[18].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[19].field_name,"&FS_headship_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[19].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[20].field_name,"&FS_appellation_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[20].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[21].field_name,"&FS_department_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[21].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[22].field_name,"&FS_preside_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[22].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[23].field_name,"&FS_befontof_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[23].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[24].field_name,"&FS_phone_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[24].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[26].field_name,"&FS_mphone_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[26].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[27].field_name,"&FS_email_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[27].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[28].field_name,"&FS_msn_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[28].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[29].field_name,"&FS_h_phone_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[29].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[30].field_name,"&FS_h_addr_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[30].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[31].field_name,"&FS_cr_sn_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[31].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[32].field_name,"&FS_ww_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[32].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[33].field_name,"&FS_skype_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[33].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[34].field_name,"&FS_weixin_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[34].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[35].field_name,"&FS_oppname_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[35].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[36].field_name,"&FS_number_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[36].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[37].field_name,"&FS_topic_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[37].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[38].field_name,"&FS_provider_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[38].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[39].field_name,"&FS_company_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[39].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[40].field_name,"&FS_prosol_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[40].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[41].field_name,"&FS_pro_ser_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[41].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[42].field_name,"&dt_contract_cu_sub=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[42].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[43].field_name,"&FS_deli_place_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[43].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[44].field_name,"&FS_memo_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[44].type = POSTURL;
            
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[45].field_name,"&FS_p_sn_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[45].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[46].field_name,"&FS_p_notice_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[46].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[47].field_name,"&FS_p_info_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[47].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[48].field_name,"&FS_c_info_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[48].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[49].field_name,"&FS_s_memo_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[49].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[50].field_name,"&FS_r_memo_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[50].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[51].field_name,"&FS_o_cuname_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[51].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[52].field_name,"&FS_o_memo_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[52].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[53].field_name,"&FS_request_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[53].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[54].field_name,"&FS_plan_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[54].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[55].field_name,"&FS_cu_who_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[55].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[56].field_name,"&FS_sendcomp_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[56].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[57].field_name,"&FS_billsn_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[57].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[58].field_name,"&FS_pro_name_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[58].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[59].field_name,"&FS_model_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[59].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[60].field_name,"&FS_manufacturer_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[60].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[61].field_name,"&FS_approval_num_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[61].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[62].field_name,"&location*=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[62].type = POSTURLSPLIT;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[63].field_name,"&FS_return_no_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[63].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[64].field_name,"&FS_place_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[64].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[65].field_name,"&FS_media_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[65].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[66].field_name,"&FS_timetable_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[66].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[67].field_name,"&FS_ad_agent_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[67].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[68].field_name,"&FS_sheet_name_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[68].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[69].field_name,"&FS_printery_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[69].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[70].field_name,"&FS_linkman_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[70].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[71].field_name,"&FS_1st_takein_key=");
    xtoolPolicy[0].pFormPolicy[1].pFieldPolicy[71].type = POSTURL;
    
































    
    strcpy(xtoolPolicy[0].pFormPolicy[2].url,"/sys/dayrep/wdayok.xt");
    xtoolPolicy[0].pFormPolicy[2].fieldPolicyLen = 3;
    xtoolPolicy[0].pFormPolicy[2].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[2].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[2].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[2].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[2].pFieldPolicy[0].field_name,"&zj=");
    xtoolPolicy[0].pFormPolicy[2].pFieldPolicy[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[2].pFieldPolicy[1].field_name,"&jh=");
    xtoolPolicy[0].pFormPolicy[2].pFieldPolicy[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[2].pFieldPolicy[2].field_name,"pz=");
    xtoolPolicy[0].pFormPolicy[2].pFieldPolicy[2].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[3].url,"/sys/weekrpt/weekrpteditok.xt");
    xtoolPolicy[0].pFormPolicy[3].fieldPolicyLen = 3;
    xtoolPolicy[0].pFormPolicy[3].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[3].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[3].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[3].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[3].pFieldPolicy[0].field_name,"zj=");
    xtoolPolicy[0].pFormPolicy[3].pFieldPolicy[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[3].pFieldPolicy[1].field_name,"&jh=");
    xtoolPolicy[0].pFormPolicy[3].pFieldPolicy[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[3].pFieldPolicy[2].field_name,"pz=");
    xtoolPolicy[0].pFormPolicy[3].pFieldPolicy[2].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[4].url,"/sys/monthrpt/monthrpteditok.xt");
    xtoolPolicy[0].pFormPolicy[4].fieldPolicyLen = 3;
    xtoolPolicy[0].pFormPolicy[4].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[4].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[4].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[4].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[4].pFieldPolicy[0].field_name,"&zj=");
    xtoolPolicy[0].pFormPolicy[4].pFieldPolicy[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[4].pFieldPolicy[1].field_name,"&jh=");
    xtoolPolicy[0].pFormPolicy[4].pFieldPolicy[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[4].pFieldPolicy[2].field_name,"&pz=");
    xtoolPolicy[0].pFormPolicy[4].pFieldPolicy[2].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[5].url,"/sys/csstree/editfolderok.xt");
    xtoolPolicy[0].pFormPolicy[5].fieldPolicyLen = 2;
    xtoolPolicy[0].pFormPolicy[5].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[5].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[5].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[5].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[5].pFieldPolicy[0].field_name,"&nname=");
    xtoolPolicy[0].pFormPolicy[5].pFieldPolicy[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[5].pFieldPolicy[1].field_name,"&gname=");
    xtoolPolicy[0].pFormPolicy[5].pFieldPolicy[1].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[6].url,"/sys/editprice/goodseditok.xt");
    xtoolPolicy[0].pFormPolicy[6].fieldPolicyLen = 2;
    xtoolPolicy[0].pFormPolicy[6].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[6].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[6].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[6].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[6].pFieldPolicy[0].field_name,"&dt_goods_memo_*=");
    xtoolPolicy[0].pFormPolicy[6].pFieldPolicy[0].type = POSTURLS;
    strcpy(xtoolPolicy[0].pFormPolicy[6].pFieldPolicy[1].field_name,"dt_goods_memo_1%22%2C%22value%22%3A%22");
    xtoolPolicy[0].pFormPolicy[6].pFieldPolicy[1].type = URLJSON;

    strcpy(xtoolPolicy[0].pFormPolicy[7].url,"/xcrm/honor/editok.xt");
    xtoolPolicy[0].pFormPolicy[7].fieldPolicyLen = 2;
    xtoolPolicy[0].pFormPolicy[7].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[7].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[7].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[7].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[7].pFieldPolicy[0].field_name,"&title%5B%5D=");
    xtoolPolicy[0].pFormPolicy[7].pFieldPolicy[0].type = POSTURLS;
    strcpy(xtoolPolicy[0].pFormPolicy[7].pFieldPolicy[1].field_name,"&comment%5B%5D=");
    xtoolPolicy[0].pFormPolicy[7].pFieldPolicy[1].type = POSTURLS;

    strcpy(xtoolPolicy[0].pFormPolicy[8].url,"/system/oc.xt?*");
    xtoolPolicy[0].pFormPolicy[8].fieldPolicyLen = 1;
    xtoolPolicy[0].pFormPolicy[8].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[8].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[8].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[8].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[8].pFieldPolicy[0].field_name,"&i1=");
    xtoolPolicy[0].pFormPolicy[8].pFieldPolicy[0].type = POSTURL;

    
    strcpy(xtoolPolicy[0].pFormPolicy[9].url,"/sys/packsetup/index.xt");
    xtoolPolicy[0].pFormPolicy[9].fieldPolicyLen = 2;
    xtoolPolicy[0].pFormPolicy[9].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[9].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[9].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[9].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[9].pFieldPolicy[0].field_name,"name=");
    xtoolPolicy[0].pFormPolicy[9].pFieldPolicy[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pFormPolicy[9].pFieldPolicy[1].field_name,"search_name=");
    xtoolPolicy[0].pFormPolicy[9].pFieldPolicy[1].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pFormPolicy[10].url,"/system/basepopok.xt");
    xtoolPolicy[0].pFormPolicy[10].fieldPolicyLen = 1;
    xtoolPolicy[0].pFormPolicy[10].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[10].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[10].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[10].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[10].pFieldPolicy[0].field_name,"&name=");
    xtoolPolicy[0].pFormPolicy[10].pFieldPolicy[0].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[11].url,"/sys/editprice/puritemeditok.xt");
    xtoolPolicy[0].pFormPolicy[11].fieldPolicyLen = 1;
    xtoolPolicy[0].pFormPolicy[11].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[11].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[11].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[11].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[11].pFieldPolicy[0].field_name,"&dt_puritem_memo_*=");
    xtoolPolicy[0].pFormPolicy[11].pFieldPolicy[0].type = POSTURLSW;

    strcpy(xtoolPolicy[0].pFormPolicy[12].url,"/websetup/svc/index.xt");
    xtoolPolicy[0].pFormPolicy[12].fieldPolicyLen = 1;
    xtoolPolicy[0].pFormPolicy[12].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[12].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[12].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[12].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[12].pFieldPolicy[0].field_name,"name=");
    xtoolPolicy[0].pFormPolicy[12].pFieldPolicy[0].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[13].url,"/websetup/svc/setup.xt");
    xtoolPolicy[0].pFormPolicy[13].fieldPolicyLen = 5;
    xtoolPolicy[0].pFormPolicy[13].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[13].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[13].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[13].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[0].field_name,"&LA*=");
    xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[0].type = POSTURLSW;
    strcpy(xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[1].field_name,"&RA*=");
    xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[1].type = POSTURLSW;
    strcpy(xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[2].field_name,"&LC*=");
    xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[2].type = POSTURLSPLIT;
    strcpy(xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[3].field_name,"&RC*=");
    xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[3].type = POSTURLSPLIT;
    strcpy(xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[4].field_name,"&name=");
    xtoolPolicy[0].pFormPolicy[13].pFieldPolicy[4].type = POSTURL;

    strcpy(xtoolPolicy[0].pFormPolicy[14].url,"/setup/nltsetup.xt");
    xtoolPolicy[0].pFormPolicy[14].fieldPolicyLen = 1;
    xtoolPolicy[0].pFormPolicy[14].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[14].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[14].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[14].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[14].pFieldPolicy[0].field_name,"&N*=");
    xtoolPolicy[0].pFormPolicy[14].pFieldPolicy[0].type = POSTURLSPLIT;

    strcpy(xtoolPolicy[0].pFormPolicy[15].url,"/setup/setupok.xt");
    xtoolPolicy[0].pFormPolicy[15].fieldPolicyLen = 1;
    xtoolPolicy[0].pFormPolicy[15].pFieldPolicy = (FieldPolicy*)malloc(sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[15].fieldPolicyLen);
    memset(xtoolPolicy[0].pFormPolicy[15].pFieldPolicy,0,sizeof(FieldPolicy)*xtoolPolicy[0].pFormPolicy[15].fieldPolicyLen);    
    strcpy(xtoolPolicy[0].pFormPolicy[15].pFieldPolicy[0].field_name,"name_*=");
    xtoolPolicy[0].pFormPolicy[15].pFieldPolicy[0].type = POSTURLSPLIT;
    
    
    int i = 0,j= 0,k=0;
    for(i = 0 ; i < serverCount ; i ++){
        UpdateSitePolicy(xtoolPolicy[i],0);
        GetSitePolicyByHost(&xtoolPolicy[i]);
        for(j = 0 ; j < xtoolPolicy[i].formPolicyLen ; j ++){
            xtoolPolicy[i].pFormPolicy[j].site_id = xtoolPolicy[i].id;
            UpdateFormPolicy(xtoolPolicy[i].pFormPolicy[j],0);
            GetFormPolicy(&xtoolPolicy[i].pFormPolicy[j]);
            for(k = 0 ; k < xtoolPolicy[i].pFormPolicy[j].fieldPolicyLen ; k ++){
                xtoolPolicy[i].pFormPolicy[j].pFieldPolicy[k].form_id = xtoolPolicy[i].pFormPolicy[j].id;
                UpdateFieldPolicy(xtoolPolicy[i].pFormPolicy[j].pFieldPolicy[k],0);
            }
        }
    }
    return 0;
}
