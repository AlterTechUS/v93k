#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;

#include "../include/User.h"

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Initialize_Get_Vector_Number_By_Comment_Search: public testmethod::TestMethod {

    protected:
    string sInit_Label_PDA;
    string sInit_Label_Training;
    string sComment_MR0;
    string sComment_MR1;
    string sComment_MR2;
    string sComment_MR3;
    string sComment_MR4;
    string sComment_MR5;
    string sComment_MR6_VrefDQ_Enable;
    string sComment_MR6_VrefDQ_Set;
    string sComment_MR6_VrefDQ_Disable;
    string sComment_MR3_PDA_Enable;
    string sComment_MR3_PDA_Disable;
    string sComment_MR6_PDA_VrefDQ_Enable;
    string sComment_Prefix_MR6_PDA_VrefDQ_Set;
    string sComment_Prefix_MR6_PDA_VrefDQ_Disable;
    int    iReportFlag;
    
    bool   run_first;
    bool   test_pass;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sInit_Label_PDA",                            "string", &sInit_Label_PDA).setDefault("initialize_pda");
        addParameter("sInit_Label_Training",                       "string", &sInit_Label_Training).setDefault("initialize_training");
        addParameter("sComment_MR0",                               "string", &sComment_MR0).setDefault("INIT_MR0");
        addParameter("sComment_MR1",                               "string", &sComment_MR1).setDefault("INIT_MR1");
        addParameter("sComment_MR2",                               "string", &sComment_MR2).setDefault("INIT_MR2");
        addParameter("sComment_MR3",                               "string", &sComment_MR3).setDefault("INIT_MR3");
        addParameter("sComment_MR4",                               "string", &sComment_MR4).setDefault("INIT_MR4");
        addParameter("sComment_MR5",                               "string", &sComment_MR5).setDefault("INIT_MR5");
        addParameter("sComment_MR6_VrefDQ_Enable",                 "string", &sComment_MR6_VrefDQ_Enable).setDefault("INIT_MR6_VrefDQ_Enable");
        addParameter("sComment_MR6_VrefDQ_Set",                    "string", &sComment_MR6_VrefDQ_Set).setDefault("INIT_MR6_VrefDQ_Set");
        addParameter("sComment_MR6_VrefDQ_Disable",                "string", &sComment_MR6_VrefDQ_Disable).setDefault("INIT_MR6_VrefDQ_Disable");
        addParameter("sComment_MR3_PDA_Enable",                    "string", &sComment_MR3_PDA_Enable).setDefault("PDA_MR3_Enable");
        addParameter("sComment_MR3_PDA_Disable",                   "string", &sComment_MR3_PDA_Disable).setDefault("PDA_MR3_Disable");
        addParameter("sComment_MR6_PDA_VrefDQ_Enable",             "string", &sComment_MR6_PDA_VrefDQ_Enable).setDefault("PDA_MR6_VrefDQ_Enable");
        addParameter("sComment_Prefix_MR6_PDA_VrefDQ_Set",         "string", &sComment_Prefix_MR6_PDA_VrefDQ_Set).setDefault("PDA_MR6_VrefDQ_Set_");
        addParameter("sComment_Prefix_MR6_PDA_VrefDQ_Disable",     "string", &sComment_Prefix_MR6_PDA_VrefDQ_Disable).setDefault("PDA_MR6_VrefDQ_Disable_");
        addParameter("iReportFlag",                                "int",    &iReportFlag).setDefault("1").setOptions("0:1");

        run_first = true;
        test_pass = false;
    }

    virtual void run()
    {
        //Add your test code here.

        ON_FIRST_INVOCATION_BEGIN();
            
            if(run_first){

                InitLabel il;
                
                il.clean();
                il.reportflag = iReportFlag;
                
                il.set_initlabel_pda                             (sInit_Label_PDA);
                il.set_comment_pda_mr0                           (sComment_MR0);
                il.set_comment_pda_mr1                           (sComment_MR1);
                il.set_comment_pda_mr2                           (sComment_MR2);
                il.set_comment_pda_mr3                           (sComment_MR3);
                il.set_comment_pda_mr4                           (sComment_MR4);
                il.set_comment_pda_mr5                           (sComment_MR5);
                il.set_comment_pda_mr6_common_vrefdq_enable      (sComment_MR6_VrefDQ_Enable);
                il.set_comment_pda_mr6_common_vrefdq_set         (sComment_MR6_VrefDQ_Set);
                il.set_comment_pda_mr6_common_vrefdq_disable     (sComment_MR6_VrefDQ_Disable);
                il.set_comment_pda_mr3_pda_enable                (sComment_MR3_PDA_Enable);
                il.set_comment_pda_mr3_pda_disable               (sComment_MR3_PDA_Disable);
                il.set_comment_pda_mr6_pda_vrefdq_enable         (sComment_MR6_PDA_VrefDQ_Enable);
                il.set_comment_pda_prefix_mr6_pda_vrefdq_set     (sComment_Prefix_MR6_PDA_VrefDQ_Set);
                il.set_comment_pda_prefix_mr6_pda_vrefdq_disable (sComment_Prefix_MR6_PDA_VrefDQ_Disable);
                
                il.set_initlabel_training                        (sInit_Label_Training);
                il.set_comment_training_mr0                      (sComment_MR0);
                il.set_comment_training_mr1                      (sComment_MR1);
                il.set_comment_training_mr2                      (sComment_MR2);
                il.set_comment_training_mr3                      (sComment_MR3);
                il.set_comment_training_mr4                      (sComment_MR4);
                il.set_comment_training_mr5                      (sComment_MR5);
                il.set_comment_training_mr6_common_vrefdq_enable (sComment_MR6_VrefDQ_Enable);
                il.set_comment_training_mr6_common_vrefdq_set    (sComment_MR6_VrefDQ_Set);
                il.set_comment_training_mr6_common_vrefdq_disable(sComment_MR6_VrefDQ_Disable);
                
                int iPassCounter = il.extract_vecnum();

                run_first = false;
                if(iPassCounter == 123) test_pass = true;
                //cout << "iPassCounter = " << iPassCounter << endl;
            }
        
        ON_FIRST_INVOCATION_END();
        
        TESTSET().cont(true).judgeAndLog_ParametricTest("@", "", LIMIT(TM::GT,0.5,TM::LT,1.5), test_pass);
        
        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
        
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("Initialize.Get_Vector_Number_By_Comment_Search", Initialize_Get_Vector_Number_By_Comment_Search);

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Initialize_Edit_MRS_Vector: public testmethod::TestMethod {

    protected:
    string sType_Init_Label;

    int    iFlag_MR0_Edit;
    int    iFlag_MR1_Edit;
    int    iFlag_MR2_Edit;
    int    iFlag_MR3_Edit;
    int    iFlag_MR4_Edit;
    int    iFlag_MR5_Edit;
    int    iFlag_MR6_Edit;

    string sMR0_BurstLength;
    string sMR0_ReadBurstType;
    string sMR0_CASLatency;
    string sMR0_DLL_Reset;
    string sMR0_WriteRecovery;
    string sMR1_DLL_Enable;
    string sMR1_DriverImpedanceControl;
    string sMR1_AdditiveLatency;
    string sMR1_RTT_NOM;
    string sMR1_TDQS_Enable;
    string sMR2_CASWriteLatency;
    string sMR2_RTT_WR;
    string sMR2_WriteCRC_Enable;
    string sMR2_LP_ASR;
    string sMR3_GeardownMode;
//    string sMR3_PerDRAMAddressability_Enable;
    string sMR3_WriteCMDLatency;
    string sMR4_CALatency;
    string sMR4_SelfRefreshAbort_Enable;
    string sMR4_ReadPreamble;
    string sMR4_WritePreamble;
    string sMR5_CAParityLatency;
    string sMR5_RTT_PARK;
    string sMR5_DataMask_Enable;
    string sMR5_WriteDBI_Enable;
    string sMR5_ReadDBI_Enable;
    string sMR6_tCCD_L;
    string sMR6_VrefDQRange;
    string sMR6_VrefDQValue;
    string sMR6_PDA_VrefDQ_Enable;

    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sType_Init_Label",                  "string", &sType_Init_Label).setOptions("PDA:Training").setDefault("PDA");
        addParameter("MR0",                               "int",    &iFlag_MR0_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR0.BurstLength",                   "string", &sMR0_BurstLength).setOptions("BC8:BC4or8:BC4").setDefault("BC8");
        addParameter("MR0.ReadBurstType",                 "string", &sMR0_ReadBurstType).setOptions("Sequential:Interleave").setDefault("Sequential");
        addParameter("MR0.CASLatency",                    "string", &sMR0_CASLatency).setOptions("9:10:11:12:13:14:15:16:18:20:22:24").setDefault("9");
        addParameter("MR0.DLL_Reset",                     "string", &sMR0_DLL_Reset).setOptions("0:1").setDefault("0");
        addParameter("MR0.WriteRecovery",                 "string", &sMR0_WriteRecovery).setOptions("10:12:14:16:18:20:24").setDefault("10");
        addParameter("MR1",                               "int",    &iFlag_MR1_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR1.DLL_Enable",                    "string", &sMR1_DLL_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR1.DriverImpedanceControl",        "string", &sMR1_DriverImpedanceControl).setOptions("RZQ/7:RZQ/5").setDefault("RZQ/7");
        addParameter("MR1.AdditiveLatency",               "string", &sMR1_AdditiveLatency).setOptions("0:1:2").setDefault("0");
        addParameter("MR1.RTT_NOM",                       "string", &sMR1_RTT_NOM).setOptions("Disable:RZQ/4:RZQ/2:RZQ/6:RZQ/1:RZQ/5:RZQ/3:RZQ/7").setDefault("Disable");
        addParameter("MR1.TDQS_Enable",                   "string", &sMR1_TDQS_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR2",                               "int",    &iFlag_MR2_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR2.CASWriteLatency",               "string", &sMR2_CASWriteLatency).setOptions("9:10:11:12:14:16:18").setDefault("9");
        addParameter("MR2.RTT_WR",                        "string", &sMR2_RTT_WR).setOptions("Off:RZQ/2:RZQ/1:HiZ").setDefault("Off");
        addParameter("MR2.WriteCRC_Enable",               "string", &sMR2_WriteCRC_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR2.LP_ASR",                        "string", &sMR2_LP_ASR).setOptions("Manual_Normal:Manual_Reduced:Manual_Extended:ASR").setDefault("Manual_Normal");
        addParameter("MR3",                               "int",    &iFlag_MR3_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR3.GeardownMode",                  "string", &sMR3_GeardownMode).setOptions("1/2Rate:1/4Rate").setDefault("1/2Rate");
//        addParameter("MR3.PerDRAMAddressability_Enable", "int",    &iMR3_PerDRAMAddressability_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR3.WriteCMDLatency",               "string", &sMR3_WriteCMDLatency).setOptions("4:5:6").setDefault("4");
        addParameter("MR4",                               "int",    &iFlag_MR4_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR4.CALatency",                     "string", &sMR4_CALatency).setOptions("0:3:4:5:6:8").setDefault("0");
        addParameter("MR4.SelfRefreshAbort_Enable",       "string", &sMR4_SelfRefreshAbort_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR4.ReadPreamble",                  "string", &sMR4_ReadPreamble).setOptions("1:2").setDefault("1");
        addParameter("MR4.WritePreamble",                 "string", &sMR4_WritePreamble).setOptions("1:2").setDefault("1");
        addParameter("MR5",                               "int",    &iFlag_MR5_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR5.CAParityLatency",               "string", &sMR5_CAParityLatency).setOptions("0:4:5:6:8").setDefault("0");
        addParameter("MR5.RTT_PARK",                      "string", &sMR5_RTT_PARK).setOptions("0:1:2:3:4:5:6:7").setDefault("0");
        addParameter("MR5.DataMask_Enable",               "string", &sMR5_DataMask_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR5.WriteDBI_Enable",               "string", &sMR5_WriteDBI_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR5.ReadDBI_Enable",                "string", &sMR5_ReadDBI_Enable).setOptions("0:1").setDefault("0");
        addParameter("MR6",                               "int",    &iFlag_MR6_Edit).setOptions("0:1").setDefault("0");
        addParameter("MR6.tCCD_L",                        "string", &sMR6_tCCD_L).setOptions("4:5:6:7:8").setDefault("4");
        addParameter("MR6.VrefDQRange",                   "string", &sMR6_VrefDQRange).setOptions("Range1:Range2").setDefault("Range1");
        addParameter("MR6.VrefDQValue",                   "string", &sMR6_VrefDQValue).setOptions("Avg:0x18").setDefault("0x18");
        addParameter("MR6.PDA_VrefDQ_Enable",             "string", &sMR6_PDA_VrefDQ_Enable).setOptions("0:1").setDefault("1");
    }

    virtual void run()
    {
        //Add your test code here.

        //================================================================================
        // Edit MRS vectors
        //================================================================================
        edit_mrs_vectors(sType_Init_Label,
                         iFlag_MR0_Edit,
                         iFlag_MR1_Edit,
                         iFlag_MR2_Edit,
                         iFlag_MR3_Edit,
                         iFlag_MR4_Edit,
                         iFlag_MR5_Edit,
                         iFlag_MR6_Edit,
                         sMR0_BurstLength,
                         sMR0_ReadBurstType,
                         sMR0_CASLatency,
                         sMR0_DLL_Reset,
                         sMR0_WriteRecovery,
                         sMR1_DLL_Enable,
                         sMR1_DriverImpedanceControl,
                         sMR1_AdditiveLatency,
                         sMR1_RTT_NOM,
                         sMR1_TDQS_Enable,
                         sMR2_CASWriteLatency,
                         sMR2_RTT_WR,
                         sMR2_WriteCRC_Enable,
                         sMR2_LP_ASR,
                         sMR3_GeardownMode,
                         sMR3_WriteCMDLatency,
                         sMR4_CALatency,
                         sMR4_SelfRefreshAbort_Enable,
                         sMR4_ReadPreamble,
                         sMR4_WritePreamble,
                         sMR5_CAParityLatency,
                         sMR5_RTT_PARK,
                         sMR5_DataMask_Enable,
                         sMR5_WriteDBI_Enable,
                         sMR5_ReadDBI_Enable,
                         sMR6_tCCD_L,
                         sMR6_VrefDQRange,
                         sMR6_VrefDQValue,
                         sMR6_PDA_VrefDQ_Enable);

        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
        if(iFlag_MR0_Edit == 0){ getParameter("MR0.BurstLength").setVisible(false);             } else{ getParameter("MR0.BurstLength").setVisible(true);             }
        if(iFlag_MR0_Edit == 0){ getParameter("MR0.ReadBurstType").setVisible(false);           } else{ getParameter("MR0.ReadBurstType").setVisible(true);           }
        if(iFlag_MR0_Edit == 0){ getParameter("MR0.CASLatency").setVisible(false);              } else{ getParameter("MR0.CASLatency").setVisible(true);              }
        if(iFlag_MR0_Edit == 0){ getParameter("MR0.DLL_Reset").setVisible(false);               } else{ getParameter("MR0.DLL_Reset").setVisible(true);               }
        if(iFlag_MR0_Edit == 0){ getParameter("MR0.WriteRecovery").setVisible(false);           } else{ getParameter("MR0.WriteRecovery").setVisible(true);           }
        
        if(iFlag_MR1_Edit == 0){ getParameter("MR1.DLL_Enable").setVisible(false);              } else{ getParameter("MR1.DLL_Enable").setVisible(true);              }
        if(iFlag_MR1_Edit == 0){ getParameter("MR1.DriverImpedanceControl").setVisible(false);  } else{ getParameter("MR1.DriverImpedanceControl").setVisible(true);  }
        if(iFlag_MR1_Edit == 0){ getParameter("MR1.AdditiveLatency").setVisible(false);         } else{ getParameter("MR1.AdditiveLatency").setVisible(true);         }
        if(iFlag_MR1_Edit == 0){ getParameter("MR1.RTT_NOM").setVisible(false);                 } else{ getParameter("MR1.RTT_NOM").setVisible(true);                 }
        if(iFlag_MR1_Edit == 0){ getParameter("MR1.TDQS_Enable").setVisible(false);             } else{ getParameter("MR1.TDQS_Enable").setVisible(true);             }

        if(iFlag_MR2_Edit == 0){ getParameter("MR2.CASWriteLatency").setVisible(false);         } else{ getParameter("MR2.CASWriteLatency").setVisible(true);         }
        if(iFlag_MR2_Edit == 0){ getParameter("MR2.RTT_WR").setVisible(false);                  } else{ getParameter("MR2.RTT_WR").setVisible(true);                  }
        if(iFlag_MR2_Edit == 0){ getParameter("MR2.WriteCRC_Enable").setVisible(false);         } else{ getParameter("MR2.WriteCRC_Enable").setVisible(true);         }
        if(iFlag_MR2_Edit == 0){ getParameter("MR2.LP_ASR").setVisible(false);                  } else{ getParameter("MR2.LP_ASR").setVisible(true);                  }

        if(iFlag_MR3_Edit == 0){ getParameter("MR3.GeardownMode").setVisible(false);            } else{ getParameter("MR3.GeardownMode").setVisible(true);            }
        if(iFlag_MR3_Edit == 0){ getParameter("MR3.WriteCMDLatency").setVisible(false);         } else{ getParameter("MR3.WriteCMDLatency").setVisible(true);         }

        if(iFlag_MR4_Edit == 0){ getParameter("MR4.CALatency").setVisible(false);               } else{ getParameter("MR4.CALatency").setVisible(true);               }
        if(iFlag_MR4_Edit == 0){ getParameter("MR4.SelfRefreshAbort_Enable").setVisible(false); } else{ getParameter("MR4.SelfRefreshAbort_Enable").setVisible(true); }
        if(iFlag_MR4_Edit == 0){ getParameter("MR4.ReadPreamble").setVisible(false);            } else{ getParameter("MR4.ReadPreamble").setVisible(true);            }
        if(iFlag_MR4_Edit == 0){ getParameter("MR4.WritePreamble").setVisible(false);           } else{ getParameter("MR4.WritePreamble").setVisible(true);           }

        if(iFlag_MR5_Edit == 0){ getParameter("MR5.CAParityLatency").setVisible(false);         } else{ getParameter("MR5.CAParityLatency").setVisible(true);         }
        if(iFlag_MR5_Edit == 0){ getParameter("MR5.RTT_PARK").setVisible(false);                } else{ getParameter("MR5.RTT_PARK").setVisible(true);                }
        if(iFlag_MR5_Edit == 0){ getParameter("MR5.DataMask_Enable").setVisible(false);         } else{ getParameter("MR5.DataMask_Enable").setVisible(true);         }
        if(iFlag_MR5_Edit == 0){ getParameter("MR5.WriteDBI_Enable").setVisible(false);         } else{ getParameter("MR5.WriteDBI_Enable").setVisible(true);         }
        if(iFlag_MR5_Edit == 0){ getParameter("MR5.ReadDBI_Enable").setVisible(false);          } else{ getParameter("MR5.ReadDBI_Enable").setVisible(true);         }

        if(iFlag_MR6_Edit == 0){ getParameter("MR6.tCCD_L").setVisible(false);                  } else{ getParameter("MR6.tCCD_L").setVisible(true);                  }
        if(iFlag_MR6_Edit == 0){ getParameter("MR6.VrefDQRange").setVisible(false);             } else{ getParameter("MR6.VrefDQRange").setVisible(true);             }
        if(iFlag_MR6_Edit == 0){ getParameter("MR6.VrefDQValue").setVisible(false);             } else{ getParameter("MR6.VrefDQValue").setVisible(true);             }
        
        if((iFlag_MR6_Edit == 1)&&(sType_Init_Label == "PDA")){
            getParameter("MR6.PDA_VrefDQ_Enable").setVisible(true);
        } else{
            getParameter("MR6.PDA_VrefDQ_Enable").setVisible(false);
        }
        
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("Initialize.Edit_MRS_Vector", Initialize_Edit_MRS_Vector);


//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Initialize_Get_CurrentTime: public testmethod::TestMethod {

    protected:
    
    virtual void initialize()
    {
        //Add your initialization code here
    }

    virtual void run()
    {
        //Add your test code here.

        ON_FIRST_INVOCATION_BEGIN();
            
            //=======================================================================
            //     Get current time
            //=======================================================================
            char cNowTime[100];
            time_t timer = time(NULL);
            struct tm *tm;
            tm = localtime(&timer);
            strftime(cNowTime, sizeof(cNowTime), "%Y%m%d_%H%M%S", tm);

            SET_USER_STRING("CurrentTime", string(cNowTime));
            
            //=======================================================================
            //     Print current time
            //=======================================================================
            string sNowTime;
            GET_USER_STRING("CurrentTime", sNowTime);
            
            cout << "Current time is " << sNowTime << endl;
        
        ON_FIRST_INVOCATION_END();
        
        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
        
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("Initialize.Get_CurrentTime", Initialize_Get_CurrentTime);

