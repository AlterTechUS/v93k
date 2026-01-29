#include "testmethod.hpp"
#include "mapi.hpp"
using namespace std;

#ifndef ___User_h___
#define ___User_h___

#include "FW.h"

extern string int2str( const int n );
extern string double2str( const double n );
extern string float2str( const double n );
extern string double2str_decimalplace( const double n, const int iDecimalPlace, const int iSetw);
extern int str2int_bin_dec_hex( string str);
extern int get_vecnum_comment_search(const string& sLabel, const string& sWord);
extern void replace_str(string& str, const string sb, const string sa);
extern void parity_computation(const int  iACT, const int  iBank, int& iCode);
extern void vec_label_edit_A0_A17(const string& sLabel, const vector<int>& iVecNumArray, const vector<string>& sDataArray, const int iWF0, const int iWF1);
extern void vec_label_edit_A0_A5(const string& sLabel, const vector<int>& iVecNumArray, const vector<string>& sDataArray, const int iWF0, const int iWF1);
extern void vec_label_edit_DQ0(const string& sLabel, const vector<int>& iVecNumArray, const vector<int>& iPhyWvfIndexArray);
extern void tokenize(const string& source, vector<string>& dest, const string& delimiter);

extern void edit_mrs_vectors(const string& sType_Init_Label, const int& iFlag_MR0_Edit, const int& iFlag_MR1_Edit, const int& iFlag_MR2_Edit, const int& iFlag_MR3_Edit, const int& iFlag_MR4_Edit, const int& iFlag_MR5_Edit, const int& iFlag_MR6_Edit, const string& sMR0_BurstLength, const string& sMR0_ReadBurstType, const string& sMR0_CASLatency, const string& sMR0_DLL_Reset, const string& sMR0_WriteRecovery, const string& sMR1_DLL_Enable, const string& sMR1_DriverImpedanceControl, const string& sMR1_AdditiveLatency, const string& sMR1_RTT_NOM, const string& sMR1_TDQS_Enable, const string& sMR2_CASWriteLatency, const string& sMR2_RTT_WR, const string& sMR2_WriteCRC_Enable, const string& sMR2_LP_ASR, const string& sMR3_GeardownMode, const string& sMR3_WriteCMDLatency, const string& sMR4_CALatency, const string& sMR4_SelfRefreshAbort_Enable, const string& sMR4_ReadPreamble, const string& sMR4_WritePreamble, const string& sMR5_CAParityLatency, const string& sMR5_RTT_PARK, const string& sMR5_DataMask_Enable, const string& sMR5_WriteDBI_Enable, const string& sMR5_ReadDBI_Enable, const string& sMR6_tCCD_L, const string& sMR6_VrefDQRange, const string& sMR6_VrefDQValue, const string& sMR6_PDA_VrefDQ_Enable);

extern void training_timing_ca(const string& sSetupPin, const string& sResultPin, const string& sSpecOffset, const string& sSpecSweep, const double& dLinStart, const double& dLinStop, const double& dLinStep, const double& dBinStep, const int& iReportFlag, SEARCH_FUNC_TASK& searchtask, map<int, double>& dTrainingResult);
extern void training_timing_writeleveling(const string& sSetupPin, const string& sResultPin, const string& sSpecOffset, const string& sSpecSweep, const double& dLinStart, const double& dLinStop, const double& dLinStep, const double& dBinStep, const int& iReportFlag, SEARCH_FUNC_TASK& searchtask, map<int, map<int, double> >& dTrainingResult);
extern void training_timing_readwrite(const string& sMode, const string& sSetupPin, const string& sSpecOffset, const double& dLinStart_ns, const double& dLinStop_ns, const double& dLinStep_ns, const double& dBinStep_ns, const int& iReportFlag, map<int, map<int, double> >& dTrainingResult);
extern void training_level_vthdq(const string& sSetupPin, const string& sSpecOffset, const string& sSpecSweep, const double& dLinStart_Low, const double& dLinStop_Low, const double& dLinStart_High, const double& dLinStop_High, const double& dLinStep, const double& dBinStep, const int& iReportFlag, SEARCH_FUNC_TASK& searchtask, map<int, double>& dTrainingResult);
extern void training_level_vrefdq(map<int, int>& iTrainingResult);
extern void training_all(const string& sTrainingConfigurationFile, SEARCH_FUNC_TASK& T_CA_searchtask, SEARCH_FUNC_TASK& T_VthDQ_searchtask, SEARCH_FUNC_TASK& T_WriteLeveling_searchtask, map<int, int>& iResultPerSite);


class InitLabel
{

    private:
    
    static string initlabel_pda;
    static string comment_pda_mr0;
    static string comment_pda_mr1;
    static string comment_pda_mr2;
    static string comment_pda_mr3;
    static string comment_pda_mr4;
    static string comment_pda_mr5;
    static string comment_pda_mr6_common_vrefdq_enable;
    static string comment_pda_mr6_common_vrefdq_set;
    static string comment_pda_mr6_common_vrefdq_disable;
    static string comment_pda_mr3_pda_enable;
    static string comment_pda_mr3_pda_disable;
    static string comment_pda_mr6_pda_vrefdq_enable;
    static string comment_pda_prefix_mr6_pda_vrefdq_set;
    static string comment_pda_prefix_mr6_pda_vrefdq_disable;

    static string initlabel_training;
    static string comment_training_mr0;
    static string comment_training_mr1;
    static string comment_training_mr2;
    static string comment_training_mr3;
    static string comment_training_mr4;
    static string comment_training_mr5;
    static string comment_training_mr6_common_vrefdq_enable;
    static string comment_training_mr6_common_vrefdq_set;
    static string comment_training_mr6_common_vrefdq_disable;
    
    static map<string, int> vecnum_pda;      // vecnum_pda[comment]
    static map<string, int> vecnum_training; // vecnum_training[comment]
    
    public:
    
    InitLabel(){
        reportflag = 0;
    };
    
    ~InitLabel(){
    };

    int  reportflag;
    
    void clean();
    int  extract_vecnum();
    
    void set_initlabel_pda(const string& sLabel);
    void set_comment_pda_mr0(const string& sComment);
    void set_comment_pda_mr1(const string& sComment);
    void set_comment_pda_mr2(const string& sComment);
    void set_comment_pda_mr3(const string& sComment);
    void set_comment_pda_mr4(const string& sComment);
    void set_comment_pda_mr5(const string& sComment);
    void set_comment_pda_mr6_common_vrefdq_enable(const string& sComment);
    void set_comment_pda_mr6_common_vrefdq_set(const string& sComment);
    void set_comment_pda_mr6_common_vrefdq_disable(const string& sComment);
    void set_comment_pda_mr3_pda_enable(const string& sComment);
    void set_comment_pda_mr3_pda_disable(const string& sComment);
    void set_comment_pda_mr6_pda_vrefdq_enable(const string& sComment);
    void set_comment_pda_prefix_mr6_pda_vrefdq_set(const string& sComment);
    void set_comment_pda_prefix_mr6_pda_vrefdq_disable(const string& sComment);
    
    void set_initlabel_training(const string& sLabel);
    void set_comment_training_mr0(const string& sComment);
    void set_comment_training_mr1(const string& sComment);
    void set_comment_training_mr2(const string& sComment);
    void set_comment_training_mr3(const string& sComment);
    void set_comment_training_mr4(const string& sComment);
    void set_comment_training_mr5(const string& sComment);
    void set_comment_training_mr6_common_vrefdq_enable(const string& sComment);
    void set_comment_training_mr6_common_vrefdq_set(const string& sComment);
    void set_comment_training_mr6_common_vrefdq_disable(const string& sComment);
    
    string get_initlabel_pda();
    int get_vecnum_pda_mr0();
    int get_vecnum_pda_mr1();
    int get_vecnum_pda_mr2();
    int get_vecnum_pda_mr3();
    int get_vecnum_pda_mr4();
    int get_vecnum_pda_mr5();
    int get_vecnum_pda_mr6_common_vrefdq_enable();
    int get_vecnum_pda_mr6_common_vrefdq_set();
    int get_vecnum_pda_mr6_common_vrefdq_disable();
    int get_vecnum_pda_mr3_pda_enable();
    int get_vecnum_pda_mr3_pda_disable();
    int get_vecnum_pda_mr6_pda_vrefdq_enable();
    int get_vecnum_pda_mr6_pda_vrefdq_set(const int iCode);
    int get_vecnum_pda_mr6_pda_vrefdq_disable(const int iCode);

    string get_initlabel_training();
    int get_vecnum_training_mr0();
    int get_vecnum_training_mr1();
    int get_vecnum_training_mr2();
    int get_vecnum_training_mr3();
    int get_vecnum_training_mr4();
    int get_vecnum_training_mr5();
    int get_vecnum_training_mr6_common_vrefdq_enable();
    int get_vecnum_training_mr6_common_vrefdq_set();
    int get_vecnum_training_mr6_common_vrefdq_disable();

};

#endif
