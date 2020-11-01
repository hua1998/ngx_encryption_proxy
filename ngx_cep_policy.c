#include "ngx_cep_policy.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//SERVER_POLICY xtoolPolicy[20];
//int serverCount = 1;
SitePolicys sites;

int InitPostPolicy(){
    
    GetSitePolicy(&sites);
    
    /*memset(xtoolPolicy,0,sizeof(xtoolPolicy));
    strcpy(xtoolPolicy[0].server,"example.com");
    
    xtoolPolicy[0].postPolicyLen = 16;
    xtoolPolicy[0].pPostPolicy = (POST_POLICY*)malloc(sizeof(POST_POLICY)*xtoolPolicy[0].postPolicyLen);
    memset(xtoolPolicy[0].pPostPolicy,0,sizeof(POST_POLICY)*xtoolPolicy[0].postPolicyLen);
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].url,"/system/editok.xt");
    xtoolPolicy[0].pPostPolicy[0].filterLen = 236;
    xtoolPolicy[0].pPostPolicy[0].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[0].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[0].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[0].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[0].name,"name=\"dt_postit_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[0].type = POSTFORM;   
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[1].name,"name=\"dt_telnote_note\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[1].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[2].name,"name=\"dt_bulletin_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[2].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[3].name,"name=\"dt_bulletin_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[3].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[4].name,"name=\"dt_customer_cu_name_ori\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[4].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[5].name,"name=\"dt_customer_cu_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[5].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[6].name,"name=\"dt_customer_m_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[6].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[7].name,"name=\"dt_customer_hotmemo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[7].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[8].name,"name=\"dt_customer_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[8].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[9].name,"name=\"dt_customer_info\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[9].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[10].name,"name=\"dt_customer_pst\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[10].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[11].name,"name=\"dt_customer_tel\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[11].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[12].name,"name=\"dt_customer_fax\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[12].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[13].name,"name=\"dt_customer_city\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[13].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[14].name,"name=\"dt_customer_web\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[14].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[15].name,"name=\"dt_customer_district\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[15].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[16].name,"name=\"dt_customer_address\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[16].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[17].name,"name=\"dt_customer_cu_remark\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[17].type = POSTFORM;
    
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[18].name,"name=\"dt_cuview_name_ori\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[18].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[19].name,"name=\"dt_cuview_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[19].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[20].name,"name=\"dt_cuview_mphone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[20].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[21].name,"name=\"dt_cuview_hotmemo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[21].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[22].name,"name=\"dt_cuview_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[22].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[23].name,"name=\"dt_cuview_phone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[23].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[24].name,"name=\"dt_cuview_email\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[24].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[25].name,"name=\"dt_cuview_h_phone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[25].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[26].name,"name=\"dt_cuview_msn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[26].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[27].name,"name=\"dt_cuview_fax\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[27].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[28].name,"name=\"dt_cuview_ww\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[28].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[29].name,"name=\"dt_cuview_h_pst\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[29].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[30].name,"name=\"dt_cuview_skype\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[30].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[31].name,"name=\"dt_cuview_h_addr\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[31].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[32].name,"name=\"dt_cuview_pst\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[32].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[33].name,"name=\"dt_cuview_department\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[33].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[34].name,"name=\"dt_cuview_preside\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[34].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[35].name,"name=\"dt_cuview_city\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[35].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[36].name,"name=\"dt_cuview_appellation\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[36].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[37].name,"name=\"dt_cuview_district\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[37].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[38].name,"name=\"dt_cuview_headship\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[38].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[39].name,"name=\"dt_cuview_address\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[39].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[40].name,"name=\"dt_cuview_web\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[40].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[41].name,"name=\"dt_cuview_befontof\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[41].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[42].name,"name=\"dt_cuview_cr_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[42].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[43].name,"name=\"dt_cuview_remark\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[43].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[44].name,"name=\"dt_cu_care_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[44].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[45].name,"name=\"dt_cu_care_description\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[45].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[46].name,"name=\"dt_cu_care_feed_back\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[46].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[47].name,"name=\"dt_cu_care_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[47].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[48].name,"name=\"dt_costdetail_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[48].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[49].name,"name=\"dt_costdetail_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[49].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[50].name,"name=\"dt_action_title\"\r\n\r\n");          //日程任务->日程
    xtoolPolicy[0].pPostPolicy[0].pFilterName[50].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[51].name,"name=\"dt_action_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[51].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[52].name,"name=\"dt_cupool_name\"\r\n\r\n");          //日程任务->批量客户任务
    xtoolPolicy[0].pPostPolicy[0].pFilterName[52].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[53].name,"name=\"dt_cupool_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[53].type = POSTFORM;
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[54].name,"name=\"dt_contact_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[54].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[55].name,"name=\"dt_contact_preside\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[55].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[56].name,"name=\"dt_contact_appellation\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[56].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[57].name,"name=\"dt_contact_department\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[57].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[58].name,"name=\"dt_contact_headship\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[58].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[59].name,"name=\"dt_contact_phone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[59].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[60].name,"name=\"dt_contact_email\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[60].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[61].name,"name=\"dt_contact_mphone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[61].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[62].name,"name=\"dt_contact_msn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[62].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[63].name,"name=\"dt_contact_h_phone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[63].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[64].name,"name=\"dt_contact_skype\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[64].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[65].name,"name=\"dt_contact_fax\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[65].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[66].name,"name=\"dt_contact_ww\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[66].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[67].name,"name=\"dt_contact_h_pst\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[67].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[68].name,"name=\"dt_contact_h_addr\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[68].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[69].name,"name=\"dt_contact_befontof\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[69].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[70].name,"name=\"dt_contact_cr_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[70].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[71].name,"name=\"dt_contact_remark\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[71].type = POSTFORM;


    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[72].name,"name=\"dt_memday_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[72].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[73].name,"name=\"dt_opport_oppname\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[73].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[74].name,"name=\"dt_opport_provider\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[74].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[75].name,"name=\"dt_opport_cu_require\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[75].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[76].name,"name=\"dt_opport_pnote\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[76].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[77].name,"name=\"dt_price_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[77].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[78].name,"name=\"dt_price_number\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[78].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[79].name,"name=\"dt_price_s_contact\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[79].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[80].name,"name=\"dt_price_pgoods\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[80].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[81].name,"name=\"dt_price_pgath\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[81].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[82].name,"name=\"dt_price_ptrans\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[82].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[83].name,"name=\"dt_price_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[83].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[84].name,"name=\"dt_demand_topic\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[84].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[85].name,"name=\"dt_demand_provider\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[85].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[86].name,"name=\"dt_demand_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[86].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[87].name,"name=\"dt_solution_topic\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[87].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[88].name,"name=\"dt_solution_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[88].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[89].name,"name=\"dt_solution_feedback\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[89].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[90].name,"name=\"dt_opponent_company\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[90].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[91].name,"name=\"dt_opponent_prosol\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[91].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[92].name,"name=\"dt_opponent_pred\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[92].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[93].name,"name=\"dt_opponent_infer\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[93].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[94].name,"name=\"dt_opponent_mane\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[94].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[95].name,"name=\"dt_opponent_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[95].type = POSTFORM;


    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[96].name,"name=\"dt_contract_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[96].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[97].name,"name=\"dt_contract_number\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[97].type = POSTFORMS;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[98].name,"name=\"dt_contract_sum_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[98].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[99].name,"name=\"dt_contract_deli_place\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[99].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[100].name,"name=\"dt_contract_pro_ser\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[100].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[101].name,"name=\"dt_contract_cu_sub\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[101].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[102].name,"name=\"dt_contract_contract\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[102].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[103].name,"name=\"dt_contract_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[103].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[104].name,"name=\"dt_contract_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[104].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[105].name,"name=\"dt_contract_tel\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[105].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[106].name,"name=\"dt_contract_mphone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[106].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[107].name,"name=\"dt_contract_addr\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[107].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[108].name,"name=\"dt_contract_pst\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[108].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[109].name,"name=\"dt_salesticket_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[109].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[110].name,"name=\"dt_salesticket_number\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[110].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[111].name,"name=\"dt_salesticket_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[111].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[112].name,"name=\"dt_salesticket_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[112].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[113].name,"name=\"dt_salesticket_billsn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[113].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[114].name,"name=\"dt_salesticket_send_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[114].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[115].name,"name=\"dt_salesticket_send_tel\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[115].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[116].name,"name=\"dt_salesticket_send_mphone\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[116].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[117].name,"name=\"dt_salesticket_send_addr\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[117].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[118].name,"name=\"dt_salesticket_send_pst\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[118].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[119].name,"name=\"dt_salesticket_sendcomp\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[119].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[120].name,"name=\"dt_salesticket_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[120].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[121].name,"name=\"dt_product_pro_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[121].type = POSTFORMS;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[122].name,"name=\"dt_product_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[122].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[123].name,"name=\"dt_product_model\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[123].type = POSTFORMS;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[124].name,"name=\"dt_product_str_ps_1\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[124].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[125].name,"name=\"dt_product_str_ps_2\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[125].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[126].name,"name=\"dt_product_str_ps_3\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[126].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[127].name,"name=\"dt_product_list_remark\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[127].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[128].name,"name=\"dt_product_discount\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[128].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[129].name,"name=\"dt_product_intro\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[129].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[130].name,"name=\"dt_product_parameter\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[130].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[131].name,"name=\"dt_product_faq\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[131].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[132].name,"name=\"dt_product_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[132].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[133].name,"name=\"dt_goods_model\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[133].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[134].name,"name=\"dt_goods_request\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[134].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[135].name,"name=\"dt_goods_send_condit\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[135].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[136].name,"name=\"dt_goods_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[136].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[137].name,"name=\"dt_deli_note_cu_who\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[137].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[138].name,"name=\"dt_deli_note_plan\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[138].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[139].name,"name=\"dt_gathering_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[139].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[140].name,"name=\"dt_gathering_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[140].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[141].name,"name=\"dt_gathering_note_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[141].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[142].name,"name=\"dt_gathering_note_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[142].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[143].name,"name=\"dt_bill_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[143].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[144].name,"name=\"dt_bill_billsn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[144].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[145].name,"name=\"dt_bill_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[145].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[146].name,"name=\"dt_project_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[146].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[147].name,"name=\"dt_project_pnote\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[147].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[148].name,"name=\"dt_project_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[148].type = POSTFORM;


    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[149].name,"name=\"dt_prjitem_note\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[149].type = POSTFORM;


    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[150].name,"&dt_costdetail_money_memo=");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[150].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[151].name,"&dt_costdetail_memo=");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[151].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[152].name,"name=\"dt_sn_status_sn_num\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[152].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[153].name,"name=\"dt_honor_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[153].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[154].name,"name=\"dt_honor_description\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[154].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[155].name,"name=\"dt_honor_remark\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[155].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[156].name,"name=\"location0\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[156].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[157].name,"name=\"location1\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[157].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[158].name,"name=\"location2\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[158].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[159].name,"name=\"dt_location_product_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[159].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[160].name,"name=\"dt_libstock_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[160].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[161].name,"name=\"dt_libstock_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[161].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[162].name,"name=\"dt_libout_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[162].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[163].name,"name=\"dt_libout_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[163].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[164].name,"name=\"dt_libmov_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[164].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[165].name,"name=\"dt_libmov_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[165].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[166].name,"name=\"dt_libpack_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[166].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[167].name,"name=\"dt_libpack_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[167].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[168].name,"name=\"dt_libin_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[168].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[169].name,"name=\"dt_libin_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[169].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[170].name,"name=\"dt_purchase_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[170].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[171].name,"name=\"dt_purchase_cu_sub\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[171].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[172].name,"name=\"dt_purchase_number\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[172].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[173].name,"name=\"dt_purchase_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[173].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[174].name,"name=\"dt_pay_plan_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[174].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[175].name,"name=\"dt_pay_plan_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[175].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[176].name,"name=\"dt_pay_note_money_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[176].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[177].name,"name=\"dt_pay_note_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[177].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[178].name,"name=\"dt_pay_bill_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[178].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[179].name,"name=\"dt_pay_bill_billsn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[179].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[180].name,"name=\"dt_pay_bill_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[180].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[181].name,"name=\"dt_purreturn_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[181].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[182].name,"name=\"dt_purreturn_return_no\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[182].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[183].name,"name=\"dt_purreturn_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[183].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[184].name,"name=\"dt_market_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[184].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[185].name,"name=\"dt_market_place\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[185].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[186].name,"name=\"dt_market_plan\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[186].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[187].name,"name=\"dt_market_execute\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[187].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[188].name,"name=\"dt_market_sum_up\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[188].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[189].name,"name=\"dt_market_evaluate\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[189].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[190].name,"name=\"dt_market_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[190].type = POSTFORM;


    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[191].name,"name=\"dt_market_child_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[191].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[192].name,"name=\"dt_market_child_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[192].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[193].name,"name=\"dt_market_child_backmemo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[193].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[194].name,"name=\"dt_ad_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[194].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[195].name,"name=\"dt_ad_media\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[195].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[196].name,"name=\"dt_ad_timetable\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[196].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[197].name,"name=\"dt_ad_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[197].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[198].name,"name=\"dt_ad_ad_agent\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[198].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[199].name,"name=\"dt_ad_linkman\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[199].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[200].name,"name=\"dt_ad_contact\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[200].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[201].name,"name=\"dt_ad_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[201].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[202].name,"name=\"dt_sheet_sheet_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[202].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[203].name,"name=\"dt_sheet_depict\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[203].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[204].name,"name=\"dt_sheet_printery\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[204].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[205].name,"name=\"dt_sheet_linkman\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[205].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[206].name,"name=\"dt_sheet_contact\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[206].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[207].name,"name=\"dt_sheet_draw_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[207].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[208].name,"name=\"dt_gift_name\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[208].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[209].name,"name=\"dt_gift_depict\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[209].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[210].name,"name=\"dt_gift_manufacturer\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[210].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[211].name,"name=\"dt_gift_contact\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[211].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[212].name,"name=\"dt_gift_linkman\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[212].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[213].name,"name=\"dt_gift_draw_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[213].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[214].name,"name=\"dt_qa_qq\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[214].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[215].name,"name=\"dt_qa_aa\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[215].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[216].name,"name=\"dt_qa_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[216].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[217].name,"name=\"dt_lodge_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[217].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[218].name,"name=\"dt_lodge_1st_takein\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[218].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[219].name,"name=\"dt_lodge_description\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[219].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[220].name,"name=\"dt_lodge_solve\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[220].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[221].name,"name=\"dt_lodge_feed_back\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[221].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[222].name,"name=\"dt_lodge_checkback\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[222].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[223].name,"name=\"dt_lodge_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[223].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[224].name,"name=\"dt_cu_service_subject\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[224].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[225].name,"name=\"dt_cu_service_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[225].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[226].name,"name=\"dt_cu_service_feedback\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[226].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[227].name,"name=\"dt_cu_service_memo\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[227].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[228].name,"name=\"dt_repairinfo_p_sn\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[228].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[229].name,"name=\"dt_repairinfo_p_info\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[229].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[230].name,"name=\"dt_repairinfo_p_notice\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[230].type = POSTFORM;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[231].name,"name=\"REPAIR_L*\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[231].type = POSTFORMSW;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[232].name,"name=\"REPAIR_R*\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[232].type = POSTFORMSW;

    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[232].name,"name=\"dt_km_title\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[233].type = POSTFORM;
    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[234].name,"name=\"dt_km_content\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[234].type = POSTFORM;


    strcpy(xtoolPolicy[0].pPostPolicy[0].pFilterName[235].name,"name=\"dt_contact_weixin\"\r\n\r\n");
    xtoolPolicy[0].pPostPolicy[0].pFilterName[235].type = POSTFORM;









































    
    strcpy(xtoolPolicy[0].pPostPolicy[1].url,"/system/qssub.xt");
    xtoolPolicy[0].pPostPolicy[1].filterLen = 72;
    xtoolPolicy[0].pPostPolicy[1].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[1].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[1].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[1].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[0].name,"&T1=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[1].name,"&FS_title_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[2].name,"&FS_content_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[2].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[3].name,"&FS_note_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[3].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[4].name,"&T2=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[4].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[5].name,"&FS_subject_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[5].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[6].name,"&FS_cu_name_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[6].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[7].name,"&FS_m_name_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[7].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[8].name,"&FS_sn_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[8].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[9].name,"&FS_tel_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[9].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[10].name,"&FS_fax_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[10].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[11].name,"&FS_web_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[11].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[12].name,"&FS_city_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[12].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[13].name,"&FS_district_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[13].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[14].name,"&FS_pst_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[14].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[15].name,"&FS_address_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[15].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[16].name,"&FS_cu_remark_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[16].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[17].name,"&FS_money_memo_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[17].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[18].name,"&FS_name_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[18].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[19].name,"&FS_headship_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[19].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[20].name,"&FS_appellation_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[20].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[21].name,"&FS_department_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[21].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[22].name,"&FS_preside_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[22].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[23].name,"&FS_befontof_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[23].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[24].name,"&FS_phone_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[24].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[26].name,"&FS_mphone_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[26].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[27].name,"&FS_email_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[27].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[28].name,"&FS_msn_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[28].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[29].name,"&FS_h_phone_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[29].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[30].name,"&FS_h_addr_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[30].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[31].name,"&FS_cr_sn_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[31].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[32].name,"&FS_ww_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[32].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[33].name,"&FS_skype_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[33].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[34].name,"&FS_weixin_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[34].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[35].name,"&FS_oppname_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[35].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[36].name,"&FS_number_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[36].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[37].name,"&FS_topic_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[37].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[38].name,"&FS_provider_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[38].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[39].name,"&FS_company_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[39].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[40].name,"&FS_prosol_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[40].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[41].name,"&FS_pro_ser_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[41].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[42].name,"&dt_contract_cu_sub=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[42].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[43].name,"&FS_deli_place_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[43].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[44].name,"&FS_memo_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[44].type = POSTURL;
            
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[45].name,"&FS_p_sn_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[45].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[46].name,"&FS_p_notice_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[46].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[47].name,"&FS_p_info_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[47].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[48].name,"&FS_c_info_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[48].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[49].name,"&FS_s_memo_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[49].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[50].name,"&FS_r_memo_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[50].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[51].name,"&FS_o_cuname_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[51].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[52].name,"&FS_o_memo_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[52].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[53].name,"&FS_request_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[53].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[54].name,"&FS_plan_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[54].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[55].name,"&FS_cu_who_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[55].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[56].name,"&FS_sendcomp_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[56].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[57].name,"&FS_billsn_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[57].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[58].name,"&FS_pro_name_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[58].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[59].name,"&FS_model_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[59].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[60].name,"&FS_manufacturer_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[60].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[61].name,"&FS_approval_num_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[61].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[62].name,"&location*=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[62].type = POSTURLSPLIT;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[63].name,"&FS_return_no_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[63].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[64].name,"&FS_place_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[64].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[65].name,"&FS_media_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[65].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[66].name,"&FS_timetable_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[66].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[67].name,"&FS_ad_agent_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[67].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[68].name,"&FS_sheet_name_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[68].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[69].name,"&FS_printery_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[69].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[70].name,"&FS_linkman_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[70].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[1].pFilterName[71].name,"&FS_1st_takein_key=");
    xtoolPolicy[0].pPostPolicy[1].pFilterName[71].type = POSTURL;
    
































    
    strcpy(xtoolPolicy[0].pPostPolicy[2].url,"/sys/dayrep/wdayok.xt");
    xtoolPolicy[0].pPostPolicy[2].filterLen = 3;
    xtoolPolicy[0].pPostPolicy[2].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[2].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[2].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[2].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[2].pFilterName[0].name,"&zj=");
    xtoolPolicy[0].pPostPolicy[2].pFilterName[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[2].pFilterName[1].name,"&jh=");
    xtoolPolicy[0].pPostPolicy[2].pFilterName[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[2].pFilterName[2].name,"pz=");
    xtoolPolicy[0].pPostPolicy[2].pFilterName[2].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[3].url,"/sys/weekrpt/weekrpteditok.xt");
    xtoolPolicy[0].pPostPolicy[3].filterLen = 3;
    xtoolPolicy[0].pPostPolicy[3].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[3].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[3].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[3].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[3].pFilterName[0].name,"zj=");
    xtoolPolicy[0].pPostPolicy[3].pFilterName[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[3].pFilterName[1].name,"&jh=");
    xtoolPolicy[0].pPostPolicy[3].pFilterName[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[3].pFilterName[2].name,"pz=");
    xtoolPolicy[0].pPostPolicy[3].pFilterName[2].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[4].url,"/sys/monthrpt/monthrpteditok.xt");
    xtoolPolicy[0].pPostPolicy[4].filterLen = 3;
    xtoolPolicy[0].pPostPolicy[4].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[4].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[4].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[4].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[4].pFilterName[0].name,"&zj=");
    xtoolPolicy[0].pPostPolicy[4].pFilterName[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[4].pFilterName[1].name,"&jh=");
    xtoolPolicy[0].pPostPolicy[4].pFilterName[1].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[4].pFilterName[2].name,"&pz=");
    xtoolPolicy[0].pPostPolicy[4].pFilterName[2].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[5].url,"/sys/csstree/editfolderok.xt");
    xtoolPolicy[0].pPostPolicy[5].filterLen = 2;
    xtoolPolicy[0].pPostPolicy[5].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[5].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[5].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[5].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[5].pFilterName[0].name,"&nname=");
    xtoolPolicy[0].pPostPolicy[5].pFilterName[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[5].pFilterName[1].name,"&gname=");
    xtoolPolicy[0].pPostPolicy[5].pFilterName[1].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[6].url,"/sys/editprice/goodseditok.xt");
    xtoolPolicy[0].pPostPolicy[6].filterLen = 2;
    xtoolPolicy[0].pPostPolicy[6].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[6].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[6].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[6].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[6].pFilterName[0].name,"&dt_goods_memo_*=");
    xtoolPolicy[0].pPostPolicy[6].pFilterName[0].type = POSTURLS;
    strcpy(xtoolPolicy[0].pPostPolicy[6].pFilterName[1].name,"dt_goods_memo_1%22%2C%22value%22%3A%22");
    xtoolPolicy[0].pPostPolicy[6].pFilterName[1].type = URLJSON;

    strcpy(xtoolPolicy[0].pPostPolicy[7].url,"/xcrm/honor/editok.xt");
    xtoolPolicy[0].pPostPolicy[7].filterLen = 2;
    xtoolPolicy[0].pPostPolicy[7].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[7].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[7].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[7].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[7].pFilterName[0].name,"&title%5B%5D=");
    xtoolPolicy[0].pPostPolicy[7].pFilterName[0].type = POSTURLS;
    strcpy(xtoolPolicy[0].pPostPolicy[7].pFilterName[1].name,"&comment%5B%5D=");
    xtoolPolicy[0].pPostPolicy[7].pFilterName[1].type = POSTURLS;

    strcpy(xtoolPolicy[0].pPostPolicy[8].url,"/system/oc.xt?*");
    xtoolPolicy[0].pPostPolicy[8].filterLen = 1;
    xtoolPolicy[0].pPostPolicy[8].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[8].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[8].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[8].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[8].pFilterName[0].name,"&i1=");
    xtoolPolicy[0].pPostPolicy[8].pFilterName[0].type = POSTURL;

    
    strcpy(xtoolPolicy[0].pPostPolicy[9].url,"/sys/packsetup/index.xt");
    xtoolPolicy[0].pPostPolicy[9].filterLen = 2;
    xtoolPolicy[0].pPostPolicy[9].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[9].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[9].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[9].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[9].pFilterName[0].name,"name=");
    xtoolPolicy[0].pPostPolicy[9].pFilterName[0].type = POSTURL;
    strcpy(xtoolPolicy[0].pPostPolicy[9].pFilterName[1].name,"search_name=");
    xtoolPolicy[0].pPostPolicy[9].pFilterName[1].type = POSTURL;
    
    strcpy(xtoolPolicy[0].pPostPolicy[10].url,"/system/basepopok.xt");
    xtoolPolicy[0].pPostPolicy[10].filterLen = 1;
    xtoolPolicy[0].pPostPolicy[10].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[10].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[10].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[10].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[10].pFilterName[0].name,"&name=");
    xtoolPolicy[0].pPostPolicy[10].pFilterName[0].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[11].url,"/sys/editprice/puritemeditok.xt");
    xtoolPolicy[0].pPostPolicy[11].filterLen = 1;
    xtoolPolicy[0].pPostPolicy[11].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[11].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[11].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[11].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[11].pFilterName[0].name,"&dt_puritem_memo_*=");
    xtoolPolicy[0].pPostPolicy[11].pFilterName[0].type = POSTURLSW;

    strcpy(xtoolPolicy[0].pPostPolicy[12].url,"/websetup/svc/index.xt");
    xtoolPolicy[0].pPostPolicy[12].filterLen = 1;
    xtoolPolicy[0].pPostPolicy[12].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[12].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[12].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[12].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[12].pFilterName[0].name,"name=");
    xtoolPolicy[0].pPostPolicy[12].pFilterName[0].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[13].url,"/websetup/svc/setup.xt");
    xtoolPolicy[0].pPostPolicy[13].filterLen = 5;
    xtoolPolicy[0].pPostPolicy[13].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[13].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[13].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[13].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[13].pFilterName[0].name,"&LA*=");
    xtoolPolicy[0].pPostPolicy[13].pFilterName[0].type = POSTURLSW;
    strcpy(xtoolPolicy[0].pPostPolicy[13].pFilterName[1].name,"&RA*=");
    xtoolPolicy[0].pPostPolicy[13].pFilterName[1].type = POSTURLSW;
    strcpy(xtoolPolicy[0].pPostPolicy[13].pFilterName[2].name,"&LC*=");
    xtoolPolicy[0].pPostPolicy[13].pFilterName[2].type = POSTURLSPLIT;
    strcpy(xtoolPolicy[0].pPostPolicy[13].pFilterName[3].name,"&RC*=");
    xtoolPolicy[0].pPostPolicy[13].pFilterName[3].type = POSTURLSPLIT;
    strcpy(xtoolPolicy[0].pPostPolicy[13].pFilterName[4].name,"&name=");
    xtoolPolicy[0].pPostPolicy[13].pFilterName[4].type = POSTURL;

    strcpy(xtoolPolicy[0].pPostPolicy[14].url,"/setup/nltsetup.xt");
    xtoolPolicy[0].pPostPolicy[14].filterLen = 1;
    xtoolPolicy[0].pPostPolicy[14].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[14].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[14].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[14].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[14].pFilterName[0].name,"&N*=");
    xtoolPolicy[0].pPostPolicy[14].pFilterName[0].type = POSTURLSPLIT;

    strcpy(xtoolPolicy[0].pPostPolicy[15].url,"/setup/setupok.xt");
    xtoolPolicy[0].pPostPolicy[15].filterLen = 1;
    xtoolPolicy[0].pPostPolicy[15].pFilterName = (FILTER_NAME*)malloc(sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[15].filterLen);
    memset(xtoolPolicy[0].pPostPolicy[15].pFilterName,0,sizeof(FILTER_NAME)*xtoolPolicy[0].pPostPolicy[15].filterLen);    
    strcpy(xtoolPolicy[0].pPostPolicy[15].pFilterName[0].name,"name_*=");
    xtoolPolicy[0].pPostPolicy[15].pFilterName[0].type = POSTURLSPLIT;
    */
    return 0;
}

FormPolicy* GetPostPolicy(char * server, char * url){
    int i = 0, j = 0, k = 0 ;
    for(i = 0 ; i < sites.sitePolicyLen ; i ++){
        if(strcmp(server,sites.pSitePolicy[i].proxyhost) == 0){
            for(j = 0 ; j < sites.pSitePolicy[i].formPolicyLen ; j ++){
                if(strcmp(url, sites.pSitePolicy[i].pFormPolicy[j].url) == 0){
                    return &sites.pSitePolicy[i].pFormPolicy[j];
                }else if(strstr(sites.pSitePolicy[i].pFormPolicy[j].url,"*") != NULL){
                    
                    for(k = 0 ; k < (int)strlen(url) ; k ++){
                        if(url[k] == sites.pSitePolicy[i].pFormPolicy[j].url[k]){
                            continue;
                        }else if(sites.pSitePolicy[i].pFormPolicy[j].url[k] == '*'){
                            return &sites.pSitePolicy[i].pFormPolicy[j];
                        }else{
                            break;
                        }
                    }
                }
            }
        }
    }
    return NULL;
}