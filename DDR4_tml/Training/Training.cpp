#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
using namespace V93kLimits;

#include "../include/User.h"

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_Timing_CA: public testmethod::TestMethod {

    protected:
    string sSetupPin;
    string sResultPin;
    string sSpecOffset;
    string sSpecSweep;
    double dLinStart;
    double dLinStop;
    double dLinStep;
    double dBinStep;
    int    iReportFlag;

    map<int, double> dTrainingResult;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sSetupPin",   "PinString", &sSetupPin);
        addParameter("sResultPin",  "PinString", &sResultPin);
        addParameter("sSpecSweep",  "string",    &sSpecSweep);
        addParameter("sSpecOffset", "string",    &sSpecOffset);
        addParameter("dLinStart",   "double",    &dLinStart);
        addParameter("dLinStop",    "double",    &dLinStop);
        addParameter("dLinStep",    "double",    &dLinStep);
        addParameter("dBinStep",    "double",    &dBinStep);
        addParameter("iReportFlag", "int",       &iReportFlag);
        addLimit("limit_training_ca");
    }

    virtual void run()
    {
        //Add your test code here.

        static string sTestsuiteName;
        static LIMIT limitCA;
        static double limitLow, limitHigh;
        SEARCH_FUNC_TASK searchtask;

        ON_FIRST_INVOCATION_BEGIN();
            
            GET_TESTSUITE_NAME(sTestsuiteName);
            
            DISCONNECT();
            CONNECT();

            //================================================================================
            // Run Traiing
            //================================================================================
            training_timing_ca(sSetupPin,
                               sResultPin,
                               sSpecOffset,
                               sSpecSweep,
                               dLinStart,
                               dLinStop,
                               dLinStep,
                               dBinStep,
                               iReportFlag,
                               searchtask,
                               dTrainingResult);

            TMLimits& limits = TMLimits::getInstance();
            limitCA = limits.getLimitObj("limit_training_ca");
            limitCA.getLow(&limitLow);
            limitCA.getHigh(&limitHigh);


        ON_FIRST_INVOCATION_END();

        //================================================================================
        // Pass/Fail judge
        //================================================================================
       int iSite = CURRENT_SITE_NUMBER();
       TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitCA,dTrainingResult[iSite]);

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
REGISTER_TESTMETHOD("Training.Timing_CA", Training_Timing_CA);

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_Timing_WriteLeveling: public testmethod::TestMethod {

    protected:
    string sSetupPin;
    string sResultPin;
    string sSpecOffset;
    string sSpecSweep;
    double dLinStart;
    double dLinStop;
    double dLinStep;
    double dBinStep;
    int    iReportFlag;

    map<int, map<int, double> > dTrainingResult;

    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sSetupPin",   "PinString", &sSetupPin);
        addParameter("sResultPin",  "PinString", &sResultPin);
        addParameter("sSpecSweep",  "string",    &sSpecSweep);
        addParameter("sSpecOffset", "string",    &sSpecOffset);
        addParameter("dLinStart",   "double",    &dLinStart);
        addParameter("dLinStop",    "double",    &dLinStop);
        addParameter("dLinStep",    "double",    &dLinStep);
        addParameter("dBinStep",    "double",    &dBinStep);
        addParameter("iReportFlag", "int",       &iReportFlag);
        addLimit("limit_training_writeleveling");
    }

    virtual void run()
    {
        //Add your test code here.

        static string sTestsuiteName;
        static LIMIT limitWL;
        static double limitLow, limitHigh;
        SEARCH_FUNC_TASK searchtask;
        
        ON_FIRST_INVOCATION_BEGIN();
            
            GET_TESTSUITE_NAME(sTestsuiteName);
            
            DISCONNECT();
            CONNECT();

            //================================================================================
            // Run Traiing
            //================================================================================
            training_timing_writeleveling(sSetupPin,
                                          sResultPin,
                                          sSpecOffset,
                                          sSpecSweep,
                                          dLinStart,
                                          dLinStop,
                                          dLinStep,
                                          dBinStep,
                                          iReportFlag,
                                          searchtask,
                                          dTrainingResult);

            TMLimits& limits = TMLimits::getInstance();
            limitWL = limits.getLimitObj("limit_training_writeleveling");
            limitWL.getLow(&limitLow);
            limitWL.getHigh(&limitHigh);

        ON_FIRST_INVOCATION_END();
        
        //================================================================================
        // Pass/Fail judge
        //================================================================================
        int iSite = CURRENT_SITE_NUMBER();
        if(dTrainingResult[iSite].size() == 0){
            TEST("@",sTestsuiteName,"limit_training_writeleveling", -9999, TRUE);
        } else{
            for(int k=0; k<(int)dTrainingResult[iSite].size(); k++){
                TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitWL,dTrainingResult[iSite][k]);
            }
        }

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
REGISTER_TESTMETHOD("Training.Timing_WriteLeveling", Training_Timing_WriteLeveling);

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_Timing_ReadWrite: public testmethod::TestMethod {

    protected:
    string sMode;
    string sSetupPin;
    string sSpecOffset;
    double dLinStart_ns;
    double dLinStop_ns;
    double dLinStep_ns;
    double dBinStep_ns;
    int    iReportFlag;

    map<int, map<int, double> > dTrainingResult;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sMode",        "string",    &sMode).setDefault("READ").setOptions("READ:WRITE");
        addParameter("sSetupPin",    "PinString", &sSetupPin);
        addParameter("sSpecOffset",  "string",    &sSpecOffset);
        addParameter("dLinStart_ns", "double",    &dLinStart_ns);
        addParameter("dLinStop_ns",  "double",    &dLinStop_ns);
        addParameter("dLinStep_ns",  "double",    &dLinStep_ns);
        addParameter("dBinStep_ns",  "double",    &dBinStep_ns);
        addParameter("iReportFlag",  "int",       &iReportFlag);
        addLimit("limit_training_readwrite");
    }

    virtual void run()
    {
        //Add your test code here.

        static string sTestsuiteName;
        static LIMIT limitRW;
        static double limitLow, limitHigh;
        
        ON_FIRST_INVOCATION_BEGIN();
            
            GET_TESTSUITE_NAME(sTestsuiteName);
            
            DISCONNECT();
            CONNECT();

            //================================================================================
            // Run Traiing
            //================================================================================
            training_timing_readwrite(sMode,
                                      sSetupPin,
                                      sSpecOffset,
                                      dLinStart_ns,
                                      dLinStop_ns,
                                      dLinStep_ns,
                                      dBinStep_ns,
                                      iReportFlag,
                                      dTrainingResult);

            TMLimits& limits = TMLimits::getInstance();
            limitRW = limits.getLimitObj("limit_training_readwrite");
            limitRW.getLow(&limitLow);
            limitRW.getHigh(&limitHigh);

        ON_FIRST_INVOCATION_END();

        //================================================================================
        // Pass/Fail judge
        //================================================================================
        int iSite = CURRENT_SITE_NUMBER();
        if(dTrainingResult[iSite].size() == 0){
            TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitRW,-9999);
        } else{
            for(int k=0; k<(int)dTrainingResult[iSite].size(); k++){
                TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitRW,dTrainingResult[iSite][k]);
            }
        }

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
REGISTER_TESTMETHOD("Training.Timing_ReadWrite", Training_Timing_ReadWrite);

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_Level_VthDQ: public testmethod::TestMethod {

    protected:
    string sSetupPin;
    string sSpecOffset;
    string sSpecSweep;
    double dLinStart_Low;
    double dLinStop_Low;
    double dLinStart_High;
    double dLinStop_High;
    double dLinStep;
    double dBinStep;
    int    iReportFlag;

    map<int, double> dTrainingResult;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sSetupPin",      "PinString", &sSetupPin);
        addParameter("sSpecSweep",     "string",    &sSpecSweep);
        addParameter("sSpecOffset",    "string",    &sSpecOffset);
        addParameter("dLinStart_Low",  "double",    &dLinStart_Low);
        addParameter("dLinStop_Low",   "double",    &dLinStop_Low);
        addParameter("dLinStart_High", "double",    &dLinStart_High);
        addParameter("dLinStop_High",  "double",    &dLinStop_High);
        addParameter("dLinStep",       "double",    &dLinStep);
        addParameter("dBinStep",       "double",    &dBinStep);
        addParameter("iReportFlag",    "int",       &iReportFlag);
        addLimit("limit_training_vthdq");
    }

    virtual void run()
    {
        //Add your test code here.

        static string sTestsuiteName;
        static LIMIT limitVth;
        static double limitLow, limitHigh;
        SEARCH_FUNC_TASK searchtask;
        
        ON_FIRST_INVOCATION_BEGIN();
            
            GET_TESTSUITE_NAME(sTestsuiteName);
            
            DISCONNECT();
            CONNECT();

            //================================================================================
            // Run Traiing
            //================================================================================
            training_level_vthdq(sSetupPin,
                                 sSpecOffset,
                                 sSpecSweep,
                                 dLinStart_Low,
                                 dLinStop_Low,
                                 dLinStart_High,
                                 dLinStop_High,
                                 dLinStep,
                                 dBinStep,
                                 iReportFlag,
                                 searchtask,
                                 dTrainingResult);

            TMLimits& limits = TMLimits::getInstance();
            limitVth = limits.getLimitObj("limit_training_vthdq");
            limitVth.getLow(&limitLow);
            limitVth.getHigh(&limitHigh);

            ON_FIRST_INVOCATION_END();

        //================================================================================
        // Pass/Fail judge
        //================================================================================
        int iSite = CURRENT_SITE_NUMBER();
        TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitVth,dTrainingResult[iSite]);

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
REGISTER_TESTMETHOD("Training.Level_VthDQ", Training_Level_VthDQ);

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_OpCode_VrefDQ: public testmethod::TestMethod {

    protected:

    map<int, int> iTrainingResult;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addLimit("limit_training_vrefdq");
    }

    virtual void run()
    {
        //Add your test code here.

        static string sTestsuiteName;
        static LIMIT limitVrefDQ;
        static double limitLow, limitHigh;
        
        ON_FIRST_INVOCATION_BEGIN();
            
            GET_TESTSUITE_NAME(sTestsuiteName);
            
            DISCONNECT();
            CONNECT();

            //================================================================================
            // Run Traiing
            //================================================================================
            training_level_vrefdq(iTrainingResult);

            TMLimits& limits = TMLimits::getInstance();
            limitVrefDQ = limits.getLimitObj("limit_training_vrefdq");
            limitVrefDQ.getLow(&limitLow);
            limitVrefDQ.getHigh(&limitHigh);

            ON_FIRST_INVOCATION_END();

        //================================================================================
        // Pass/Fail judge
        //================================================================================
        int iSite = CURRENT_SITE_NUMBER();
        TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitVrefDQ,iTrainingResult[iSite]);

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
REGISTER_TESTMETHOD("Training.OpCode_VrefDQ", Training_OpCode_VrefDQ);

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_Spec_Value_Copy: public testmethod::TestMethod {

    protected:
    string sTimingSpecification;
    string sTimingSpecVariable;
    string sLevelEqnSpecSet;
    string sLevelSpecVariable;

    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sTimingSpecification", "string", &sTimingSpecification).setComment("e.g. specification_1:specification_2:specification_3");
        addParameter("sTimingSpecVariable",  "string", &sTimingSpecVariable) .setComment("e.g. variable_1:variable_2:variable_3");
        addParameter("sLevelEqnSpecSet",     "string", &sLevelEqnSpecSet)    .setComment("<eqn>,<spec>:<eqn>,<spec>:... e.g. 3,1:3,2:4,1");
        addParameter("sLevelSpecVariable",   "string", &sLevelSpecVariable)  .setComment("e.g. variable_1:variable_2:variable_3");
    }

    virtual void run()
    {
        //Add your test code here.

        ON_FIRST_INVOCATION_BEGIN();

            FOR_EACH_SITE_BEGIN();
                
                //================================================================================
                // Copy timing spec variable
                //================================================================================
               if(sTimingSpecification != ""){
                   vector<string> sTimingSpecificationArray;
                   tokenize(sTimingSpecification,sTimingSpecificationArray,":");

                   vector<string> sTimingSpecVariableArray;
                   tokenize(sTimingSpecVariable,sTimingSpecVariableArray,":");

                   vector<double> dPrimaryTimingSpecValue(sTimingSpecVariableArray.size());
                   for(int i=0; i<(int)sTimingSpecVariableArray.size(); i++){
                       dPrimaryTimingSpecValue[i] = Primary.getSpecification().getSpecValue(sTimingSpecVariableArray[i]);
                   }

                   for(int j=0; j<(int)sTimingSpecificationArray.size(); j++){

                       for(int i=0; i<(int)sTimingSpecVariableArray.size(); i++){
                           SPECIFICATION tim_spec(sTimingSpecificationArray[j]);
                           tim_spec.change(sTimingSpecVariableArray[i], dPrimaryTimingSpecValue[i]);
                           FLUSH(TM::APRM);
                           cout << "Message: Copy timing spec " << left << setw(32) << sTimingSpecificationArray[j] << setw(32) << sTimingSpecVariableArray[i] << setw(10) << dPrimaryTimingSpecValue[i] << endl;
                       }
                   }
               }

                //================================================================================
                // Copy level spec variable
                //================================================================================
               if(sLevelEqnSpecSet != ""){
                   vector<string> sLevelEqnSpecSetArray;
                   tokenize(sLevelEqnSpecSet,sLevelEqnSpecSetArray,":");

                   vector<string> sLevelSpecVariableArray;
                   tokenize(sLevelSpecVariable,sLevelSpecVariableArray,":");

                   vector<double> dPrimaryLevelSpecValue(sLevelSpecVariableArray.size());
                   for(int i=0; i<(int)sLevelSpecVariableArray.size(); i++){
                       dPrimaryLevelSpecValue[i] = Primary.getLevelSpec().getSpecValue(sLevelSpecVariableArray[i]);
                   }

                   for(int j=0; j<(int)sLevelEqnSpecSetArray.size(); j++){

                       vector<string> sTmpArray;
                       tokenize(sLevelEqnSpecSetArray[j],sTmpArray,",");
                       if(sTmpArray.size() == 2){
                           int iLevelEqnSet  = atoi(sTmpArray[0].c_str());
                           int iLevelSpecSet = atoi(sTmpArray[1].c_str());

                           for(int i=0; i<(int)sLevelSpecVariableArray.size(); i++){
                               LEVEL_SPEC lev_spec(iLevelEqnSet, iLevelSpecSet);
                               lev_spec.change(sLevelSpecVariableArray[i], dPrimaryLevelSpecValue[i]);
                               FLUSH(TM::APRM);
                               cout << "Message: Copy level spec " << left << setw(3) << iLevelEqnSet << setw(3) << iLevelSpecSet << setw(32) << sLevelSpecVariableArray[i] << setw(10) << dPrimaryLevelSpecValue[i] << endl;
                           }
                       } else{
                           cout << "ERROR: Please check testmethod paramter \"sLevelEqnSpec\"!" << endl;
                       }
                   }
               }
           
            FOR_EACH_SITE_END();
            
        ON_FIRST_INVOCATION_END();

        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
        if(sTimingSpecification == ""){ getParameter("sTimingSpecVariable").setVisible(false); } else{ getParameter("sTimingSpecVariable").setVisible(true); }
        if(sLevelEqnSpecSet == "")    { getParameter("sLevelSpecVariable") .setVisible(false); } else{ getParameter("sLevelSpecVariable") .setVisible(true); }
        
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("Training.Spec_Value_Copy", Training_Spec_Value_Copy);


//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class Training_All: public testmethod::TestMethod {

    protected:
    string sTrainingConfigurationFile;

    map<int, int> iTrainingResult;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sTrainingConfigurationFile", "string", &sTrainingConfigurationFile).setComment("e.g. testflow/training_1600K.cfg");
    }

    virtual void run()
    {

        //Add your test code here.

        static string sTestsuiteName;
        SEARCH_FUNC_TASK T_CA_searchtask;
        SEARCH_FUNC_TASK T_VthDQ_searchtask;
        SEARCH_FUNC_TASK T_WriteLeveling_searchtask;
        
        ON_FIRST_INVOCATION_BEGIN();
            
            GET_TESTSUITE_NAME(sTestsuiteName);
            
            DISCONNECT();
            CONNECT();

            //================================================================================
            // Run Traiing
            //================================================================================
            training_all(sTrainingConfigurationFile, T_CA_searchtask, T_VthDQ_searchtask, T_WriteLeveling_searchtask, iTrainingResult);

        ON_FIRST_INVOCATION_END();

        //================================================================================
        // Pass/Fail judge
        //================================================================================
        int iSite = CURRENT_SITE_NUMBER();
        TEST("@",sTestsuiteName,LIMIT(TM::GT,0.5,TM::LT,1.5),iTrainingResult[iSite], TRUE);

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
REGISTER_TESTMETHOD("Training.All", Training_All);

