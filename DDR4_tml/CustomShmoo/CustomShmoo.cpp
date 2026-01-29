#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;

#include "../include/User.h"

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class CustomShmoo_Two_ResultPin: public testmethod::TestMethod {

    protected:
    string  sX_Type;
    string  sX_Spec;
    double  dX_Start;
    double  dX_Stop;
    int     iX_NumOfStep;
    string  sY_Type;
    string  sY_Spec;
    double  dY_Start;
    double  dY_Stop;
    int     iY_NumOfStep;
    string  sResultPin_A;
    string  sResultPin_B;
    int     iFlag_DisconnectConnect;
    int     iFlag_FailStop;
//==    int     iFlag_TrainingPerY;
//==    string  sTrainingConfigurationFile;
    
    map<int, double> dX_Org;
    map<int, double> dY_Org;
    map<int, map<string, map<int, map<int, int> > > > iResult;
    
    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("sX_Type",                    "string",    &sX_Type).setDefault("TIM").setOptions("TIM:LEV:PERIOD");
        addParameter("sX_Spec",                    "string",    &sX_Spec);
        addParameter("dX_Start",                   "double",    &dX_Start);
        addParameter("dX_Stop",                    "double",    &dX_Stop);
        addParameter("iX_NumOfStep",               "int",       &iX_NumOfStep);
        addParameter("sY_Type",                    "string",    &sY_Type).setDefault("TIM").setOptions("TIM:LEV:PERIOD");
        addParameter("sY_Spec",                    "string",    &sY_Spec);
        addParameter("dY_Start",                   "double",    &dY_Start);
        addParameter("dY_Stop",                    "double",    &dY_Stop);
        addParameter("iY_NumOfStep",               "int",       &iY_NumOfStep);
        addParameter("sResultPin_A",               "PinString", &sResultPin_A);
        addParameter("sResultPin_B",               "PinString", &sResultPin_B);
        addParameter("iFlag_DisconnectConnect",    "int",       &iFlag_DisconnectConnect).setDefault("0").setOptions("0:1");
        addParameter("iFlag_FailStop",             "int",       &iFlag_FailStop).setDefault("0").setOptions("0:1");

//==        addParameter("iFlag_TrainingPerY",         "int",       &iFlag_TrainingPerY).setDefault("0").setOptions("0:1");
//==        addParameter("sTrainingConfigurationFile", "string",    &sTrainingConfigurationFile).setComment("e.g. testflow/training_1600K.cfg");
    }

    virtual void run()
    {
        //Add your test code here.

        //=======================================================================
        //     Modify below depending on your preference
        //=======================================================================
        string sPPcha = ".";
        string sFFcha = "F";
        string sPFcha = "1";
        int    iX_DecimalPlace = 3;
        int    iY_DecimalPlace = 3;
        int    iX_setw         = 6;
        int    iY_setw         = 6;

        SEARCH_FUNC_TASK T_CA_searchtask;
        SEARCH_FUNC_TASK T_VthDQ_searchtask;
        SEARCH_FUNC_TASK T_WriteLeveling_searchtask;
        
        ON_FIRST_INVOCATION_BEGIN();

            iResult.clear();
            
            double dX_Step = ((dX_Stop-dX_Start)/iX_NumOfStep);
            double dY_Step = ((dY_Stop-dY_Start)/iY_NumOfStep);
            vector<double> dX_Array;
            vector<double> dY_Array;
            
            //=======================================================================
            //     Get original spec value
            //=======================================================================
            dX_Org.clear();
            dY_Org.clear();
            FOR_EACH_SITE_BEGIN_WITH_QUERY();
                int iSite = CURRENT_SITE_NUMBER();
                if(sX_Type == "LEV"){
                                                    dX_Org[iSite] = Primary.getLevelSpec()    .getSpecValue(sX_Spec);
                } else {
                    if(Primary.isTimingPortBased()) dX_Org[iSite] = Primary.getSpecification().getSpecValue(sX_Spec);
                    else                            dX_Org[iSite] = Primary.getTimingSpec()   .getSpecValue(sX_Spec);
                }
                //cout << "site" << iSite << ", dX_Org=" << dX_Org[iSite] << endl;
                if(sY_Type == "LEV"){
                                                    dY_Org[iSite] = Primary.getLevelSpec()    .getSpecValue(sY_Spec);
                } else {
                    if(Primary.isTimingPortBased()) dY_Org[iSite] = Primary.getSpecification().getSpecValue(sY_Spec);
                    else                            dY_Org[iSite] = Primary.getTimingSpec()   .getSpecValue(sY_Spec);
                }
                //cout << "site" << iSite << ", dY_Org=" << dY_Org[iSite] << endl;
            FOR_EACH_SITE_END();

            //=======================================================================
            //     Get number of sites under test
            //=======================================================================
            int iNumOfSite = 0;
            FOR_EACH_SITE_BEGIN();
                iNumOfSite++;
            FOR_EACH_SITE_END();
            
            //=======================================================================
            //     Execute shmoo
            //=======================================================================
            CONNECT();
            
            for(int y=0; y<(iY_NumOfStep+1); y++){
                //===== Set y-axis parameter =====
                double dY = dY_Start + dY_Step * y;
                dY_Array.push_back(dY);
                
                if(sY_Type == "LEV"){
                                                    Primary.getLevelSpec()    .change(sY_Spec, dY);
                } else {
                    if(Primary.isTimingPortBased()) Primary.getSpecification().change(sY_Spec, dY);
                    else                            Primary.getTimingSpec()   .change(sY_Spec, dY);
                }
                FLUSH(TM::APRM);
                

                for(int x=0; x<(iX_NumOfStep+1); x++){
                    //===== Set x-axis parameter =====
                    double dX = dX_Start + dX_Step * x;
                    if(y == 0) dX_Array.push_back(dX);
                    
                    if(sX_Type == "LEV"){
                                                        Primary.getLevelSpec()    .change(sX_Spec, dX);
                    } else {
                        if(Primary.isTimingPortBased()) Primary.getSpecification().change(sX_Spec, dX);
                        else                            Primary.getTimingSpec()   .change(sX_Spec, dX);
                    }
                    //FLUSH(TM::APRM);

                    if(iFlag_DisconnectConnect){
                        DISCONNECT();
                        CONNECT();
                    }
                    
                    if(iFlag_FailStop){
                        if(iNumOfSite == 1){
                            fwout << "SQGB SE," << endl;
                        } else{
                            cout << "ERROR: FailStop is NOT valid. Please change \"iFlag_FailStop\" to 0 in case of multi-site testing." << endl;
                        }
                    }

                    int iFlagError = 0;
                    try{
                        FUNCTIONAL_TEST();
                    }
                    catch(Error & error){
                        iFlagError = 1;
                        string sErrorLabel = Primary.getLabel();
                        cout << "ERROR: Error occurred when running CustomShmoo! " << sX_Spec << "=" << dX << ", " << sY_Spec << "=" << dY << ", label=" << sErrorLabel << endl;
                    }
                    
                    if((iFlag_FailStop)&&(iNumOfSite == 1)){
                        fwout << "SQGB ACQF,0" << endl;
                    }

                    if(iFlagError == 1){
                        FOR_EACH_SITE_BEGIN();
                            int iSite = CURRENT_SITE_NUMBER();
                            iResult[iSite]["ResultPin_A"][y][x] = 0;
                            iResult[iSite]["ResultPin_B"][y][x] = 0;
                        FOR_EACH_SITE_END();
                    } else{
                        FOR_EACH_SITE_BEGIN();
                            int iSite = CURRENT_SITE_NUMBER();
                            iResult[iSite]["ResultPin_A"][y][x] = GET_FUNCTIONAL_RESULT(sResultPin_A);
                            iResult[iSite]["ResultPin_B"][y][x] = GET_FUNCTIONAL_RESULT(sResultPin_B);
                        FOR_EACH_SITE_END();
                   }
                    cout << iResult[CURRENT_SITE_NUMBER()]["ResultPin_A"][y][x] << flush;
                }
                cout << endl;
            }

            //=======================================================================
            //     Restore spec value for X-axis
            //=======================================================================
            if(sX_Type == "PERIOD"){
                if(Primary.isTimingPortBased()) Primary.getSpecification().change(sX_Spec, dX_Org[CURRENT_SITE_NUMBER()]);
                else                            Primary.getTimingSpec()   .change(sX_Spec, dX_Org[CURRENT_SITE_NUMBER()]);
            } else{
                FOR_EACH_SITE_BEGIN();
                    int iSite = CURRENT_SITE_NUMBER();
                    if(sX_Type == "LEV"){
                                                        Primary.getLevelSpec().change(sX_Spec, dX_Org[iSite]);
                    } else {
                        if(Primary.isTimingPortBased()) Primary.getSpecification().change(sX_Spec, dX_Org[iSite]);
                        else                            Primary.getTimingSpec()   .change(sX_Spec, dX_Org[iSite]);
                    }
                    FLUSH(TM::APRM);
                FOR_EACH_SITE_END();
            }
            
            //=======================================================================
            //     Restore spec value for Y-axis
            //=======================================================================
            if(sY_Type == "PERIOD"){
                if(Primary.isTimingPortBased()) Primary.getSpecification().change(sY_Spec, dY_Org[CURRENT_SITE_NUMBER()]);
                else                            Primary.getTimingSpec()   .change(sY_Spec, dY_Org[CURRENT_SITE_NUMBER()]);
            } else{
                FOR_EACH_SITE_BEGIN();
                    int iSite = CURRENT_SITE_NUMBER();
                    if(sY_Type == "LEV"){
                                                        Primary.getLevelSpec()    .change(sY_Spec, dY_Org[iSite]);
                    } else {
                        if(Primary.isTimingPortBased()) Primary.getSpecification().change(sY_Spec, dY_Org[iSite]);
                        else                            Primary.getTimingSpec()   .change(sY_Spec, dY_Org[iSite]);
                    }
                    FLUSH(TM::APRM);
                FOR_EACH_SITE_END();
            }

            //=======================================================================
            //     Store printed shmoo header into stringstream
            //=======================================================================
            stringstream ssAllLog;
            string sX_Start = double2str_decimalplace(dX_Start, iX_DecimalPlace, iX_setw);
            string sX_Stop  = double2str_decimalplace(dX_Stop,  iX_DecimalPlace, iX_setw);
            string sX_Step  = double2str_decimalplace(dX_Step,  iX_DecimalPlace, iX_setw);
            string sY_Start = double2str_decimalplace(dY_Start, iY_DecimalPlace, iY_setw);
            string sY_Stop  = double2str_decimalplace(dY_Stop,  iY_DecimalPlace, iY_setw);
            string sY_Step  = double2str_decimalplace(dY_Step,  iY_DecimalPlace, iY_setw);

            ssAllLog << "...................................................................................." << endl;
            ssAllLog << left << "sResultPin_A: " << sResultPin_A << endl;
            ssAllLog << left << "sResultPin_B: " << sResultPin_B << endl;
            ssAllLog << left << sPPcha << ": " << "sResultPin_A=PASS, sResultPin_B=PASS" << endl;
            ssAllLog << left << sFFcha << ": " << "sResultPin_A=FAIL, sResultPin_B=FAIL" << endl;
            ssAllLog << left << sPFcha << ": " << "sResultPin_A=PASS, sResultPin_B=FAIL/sResultPin_A=FAIL, sResultPin_B=PASS" << endl;
            ssAllLog << endl;
            ssAllLog << "X-axis: " << left << setw(32) << sX_Spec << "[start:" << sX_Start << ", stop:" << sX_Stop << ", step:" << sX_Step << "]" << endl;
            ssAllLog << "Y-axis: " << left << setw(32) << sY_Spec << "[start:" << sY_Start << ", stop:" << sY_Stop << ", step:" << sY_Step << "]" << endl;
            ssAllLog << "...................................................................................." << endl;
            ssAllLog << endl;

            //=======================================================================
            //     Store printed x-axis values into stringstream
            //=======================================================================
            stringstream ssX_Header;
            char cXval[dX_Array.size()][10];
            for(int x=0; x<(int)dX_Array.size(); x++){
                string sXval = double2str_decimalplace(dX_Array[x], iX_DecimalPlace, iX_setw);
                sprintf(cXval[x],"%s",sXval.c_str());
            }
            for(int m=0; m<(int)strlen(cXval[0]); m++){
                ssX_Header << setw(iY_setw+1) << "" << flush;
                for(int x=0; x<(int)dX_Array.size(); x++){
                    ssX_Header << cXval[x][m] << flush;
                }
                ssX_Header << endl;
            }

            ssX_Header << setw(iY_setw) << "" << "+" << flush;
            for(int x=0; x<(int)dX_Array.size(); x++){
                ssX_Header << "-" << flush;
            }

            //=======================================================================
            //     Store printed shmoo results into stringstream
            //=======================================================================
            FOR_EACH_SITE_BEGIN();
                
                int iSite = CURRENT_SITE_NUMBER();

                ssAllLog << "Site: " << iSite << endl;
                ssAllLog << ssX_Header.str() << endl;

                for(int y=(int)dY_Array.size()-1; y>= 0; y--){
                    string sYval = double2str_decimalplace(dY_Array[y], iY_DecimalPlace, iY_setw);
                    ssAllLog << sYval << "|" << flush;

                    for(int x=0; x<(int)dX_Array.size(); x++){
                        if     ((iResult[iSite]["ResultPin_A"][y][x]  == 1)&&(iResult[iSite]["ResultPin_B"][y][x] == 1))  ssAllLog << sPPcha << flush;
                        else if((iResult[iSite]["ResultPin_A"][y][x]  == 0)&&(iResult[iSite]["ResultPin_B"][y][x] == 0))  ssAllLog << sFFcha << flush;
                        else                                                                                              ssAllLog << sPFcha << flush;
                    }
                    
                    int    iPassInc      = 0;
                    int    iMaxNumOfPass = 0;
                    string sPassFail;
                    string sMatchStr;

                    for(int x=0; x<(int)dX_Array.size(); x++){
                        if((iResult[iSite]["ResultPin_A"][y][x] == 1)&&(iResult[iSite]["ResultPin_B"][y][x] == 1)){
                            iPassInc++;
                            sPassFail += "1";
                        } else{
                            iPassInc = 0;
                            sPassFail += "0";
                        }

                        if(iPassInc > iMaxNumOfPass) iMaxNumOfPass = iPassInc;
                    }
                    for(int i=0; i<iMaxNumOfPass; i++){
                        sMatchStr += "1";
                    }

                    int iPos = sPassFail.find(sMatchStr, 0);

                    int iLeftIndex  = iPos;
                    int iRightIndex = iPos + sMatchStr.length() - 1;

                    if(iPos == 0)                                     iLeftIndex  = -1;
                    if(iPos+iMaxNumOfPass == int(sPassFail.length())) iRightIndex = -1;

                    string sLeftBoundary;
                    if(iLeftIndex != -1){
                        sLeftBoundary = double2str_decimalplace((dX_Start + iLeftIndex*dX_Step), iX_DecimalPlace, iX_setw);
                    } else{
                        sLeftBoundary = "---";
                    }
                    
                    string sRightBoundary;
                    if(iRightIndex != -1){
                        sRightBoundary = double2str_decimalplace((dX_Start + iRightIndex*dX_Step), iX_DecimalPlace, iX_setw);
                    } else{
                        sRightBoundary = "---";
                    }
                    ssAllLog << " " << right << setw(iX_setw) << sLeftBoundary << " " << right << setw(iX_setw) << sRightBoundary << left << endl;
                }
                ssAllLog << endl;

            FOR_EACH_SITE_END();
            
            //=======================================================================
            //     Output shmoo results into report_UI
            //=======================================================================
            cout << ssAllLog.str() << endl;
            
            //=======================================================================
            //     Output shmoo results into file
            //=======================================================================
            char cNowTime[100];
            time_t timer = time(NULL);
            struct tm *tm;
            tm = localtime(&timer);
            strftime(cNowTime, sizeof(cNowTime), "%Y%m%d_%H%M%S", tm);

            char cPath[300];
            GetDevPath(cPath);
            string sDevDir = cPath;
            string sTestsuiteName;
            GET_TESTSUITE_NAME(sTestsuiteName);
            string sFileName = sDevDir + "/shmoo/" + sTestsuiteName + "_" + string(cNowTime) + ".txt";

            ofstream fout( sFileName.c_str(), ios::app );
            fout << ssAllLog.str() << endl;
            fout.close();

        ON_FIRST_INVOCATION_END();
        
        //================================================================================
        // Pass/Fail judge
        //================================================================================
        //int iSite = CURRENT_SITE_NUMBER();
        //TEST("@",sTestsuiteName,LIMIT(TM::GT,0.5,TM::LT,1.5),iResult, TRUE);

        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
//==        if(iFlag_TrainingPerY == 0){
//==            getParameter("sTrainingConfigurationFile").setVisible(false);
//==        } else{
//==            getParameter("sTrainingConfigurationFile").setVisible(true);
//==        }
        
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("CustomShmoo.Two_ResultPin", CustomShmoo_Two_ResultPin);
/*
//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class debug_fail_stop: public testmethod::TestMethod {

    protected:

    map<int, int> iResultPerSite;
    
    virtual void initialize()
    {
        //Add your initialization code here
    }

    virtual void run()
    {
        //Add your test code here.

        static string sTestsuiteName;
        
        ON_FIRST_INVOCATION_BEGIN();
            
            
            CONNECT();
            
            START_TEST();
            WAIT_TEST_DONE(TM::ABORT_ON_FAIL_ANY_PIN, "@");
            
        ON_FIRST_INVOCATION_END();
        
        fwout << "CMPS? YPQF,(@)" << endl;
        cout << "result ==== " << fwresult[0] << endl;
        
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
REGISTER_TESTMETHOD("debug.fail_stop", debug_fail_stop);
*/
