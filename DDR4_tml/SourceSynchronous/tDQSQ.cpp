#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"

#include "../include/FW.h"

namespace tDQSQ_namespace{
using namespace V93kLimits;

    //===========================================================================
    // Get spec value by SVLR FWcommand
    //===========================================================================
    double get_spec_val (const string& sType, const string& sSpecName){

        double dVal;

        if(sType == "TIM") fwout << "SVLR? TIM,PRM,,\"" << sSpecName << "\"" << endl;
        else               fwout << "SVLR? LEV,PRM,,\"" << sSpecName << "\"" << endl;

        dVal = atof(fwresult[0][4].c_str());

        return dVal;
    }

    //===========================================================================
    // Set spec value by SVLR FWcommand
    //===========================================================================
    void set_spec_val (const string& sType, const string& sSpecName, const double dVal){

        if(sType == "TIM") fwout << "SVLR TIM,PRM,,\"" << sSpecName << "\"," << dVal << endl;
        else               fwout << "SVLR LEV,PRM,,\"" << sSpecName << "\"," << dVal << endl;

        fwout << "PSFC?" << endl;
        string sRetPSFC = fwresult[0][0];

        FLUSH(TM::APRM);

        if(sRetPSFC == "ALL") fwout << "PSFC " << sRetPSFC << endl;
        else                  fwout << "PSFC \"" << sRetPSFC << "\"" << endl;

        return;
    }
}
using namespace tDQSQ_namespace;

//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class tDQSQ: public testmethod::TestMethod {

    protected:
    string sSourcePin;
    string sSinkPin;
    string sSpecVariable;
    double dSweepStart;
    double dSweepStop;
    double dSweepStep;
    int    iCycleStart;
    int    iCycleLength;
    string sMode;
    int    iEdgesPerCycle;
    string sReportMode;  
    string sOutput;  
    string sFileDirectory;  

    string sSourcePinName;
    string sSinkPinName;
    vector<string> sMeasPinList;
    vector<string> sAllPinList;
    map<int, map<string, int   > > iDeltaMin;    // [Site][Pin]
    map<int, map<string, int   > > iDeltaMax;    // [Site][Pin]
    map<int, map<string, double> > dDeltaMean;   // [Site][Pin]
    map<int, map<string, double> > dDeltaRms;    // [Site][Pin]
        
    virtual void initialize()
    {
        addParameter("sSourcePin",     "string", &sSourcePin);
        addParameter("sSinkPin",       "string", &sSinkPin);
        addParameter("sSpecVariable",  "string", &sSpecVariable);
        addParameter("dSweepStart",    "double", &dSweepStart);
        addParameter("dSweepStop",     "double", &dSweepStop);
        addParameter("dSweepStep",     "double", &dSweepStep);
        addParameter("iCycleStart",    "int",    &iCycleStart);
        addParameter("iCycleLength",   "int",    &iCycleLength);
        addParameter("sMode",          "string", &sMode).setDefault("CYCL").setOptions("CYCL:EDGE");
        addParameter("iEdgesPerCycle", "int",    &iEdgesPerCycle).setDefault("1").setOptions("1:2:3:4:6:8");
        addParameter("sReportMode",    "string", &sReportMode).setDefault("All").setOptions("All:ValueSummary:BitSummaryOnly");
        addParameter("sOutput",        "string", &sOutput).setDefault("ReportUI").setOptions("ReportUI:File:Both");
        addParameter("sFileDirectory", "string", &sFileDirectory).setDefault("./shmoo");
        addLimit("limit_tDQSQ");
    }

    virtual void run()
    {
        //Add your test code here.
        static LIMIT limitTDQSQ;
        static double limitLow, limitHigh;

        sSourcePinName = "Source";
        sSinkPinName   = "Sink";

        TM::CYCLE_MODE CycleMode;
        if(sMode == "CYCL") CycleMode = TM::TESTER_CYCLES;
        else                CycleMode = TM::XMODE_CYCLES;
        ERROR_MAP_TEST emap(CycleMode);

        ON_FIRST_INVOCATION_BEGIN();

            iDeltaMin.clear(); 
            iDeltaMax.clear(); 
            dDeltaMean.clear();
            dDeltaRms.clear(); 

            CONNECT();

            //=======================================================================================================
            // Convert from cycle# to bit# with x-mode
            //=======================================================================================================
            if(sMode == "CYCL") iEdgesPerCycle = 1;
            int iBitStart  = iCycleStart  * iEdgesPerCycle;
            int iBitLength = iCycleLength * iEdgesPerCycle;

            //=======================================================================================================
            // Define log redirections
            //=======================================================================================================
            int iFlagReportUI;
            int iFlagExportFile;
            if     (sOutput == "ReportUI"){ iFlagReportUI = 1; iFlagExportFile = 0; }
            else if(sOutput == "File")    { iFlagReportUI = 0; iFlagExportFile = 1; }
            else                          { iFlagReportUI = 1; iFlagExportFile = 1; }

            //=======================================================================================================
            // Pin processing
            //=======================================================================================================
            string sMeasPin = sSourcePin + "," + sSinkPin;
            string sAllPin  = sMeasPin + "," + sSourcePinName + "," + sSinkPinName;

            sMeasPinList = PinUtility.getDigitalPinNamesFromPinList(sMeasPin,TM::ALL_DIGITAL,false,false,PIN_UTILITY::DEFINITION_ORDER);
            int iNumOfMeasPin = sMeasPinList.size();
            sAllPinList = sMeasPinList;
            sAllPinList.push_back(sSourcePinName);
            sAllPinList.push_back(sSinkPinName);
            int iNumOfAllPin = sAllPinList.size();

            //=======================================================================================================
            // Define ERROR_MAP mode
            //=======================================================================================================
//            emap.pin(sMeasPin).setLocation(TM::TP).setRecordingMode(TM::PF).setStartCycle(iBitStart).setMaxCycles(iBitLength);
//            emap.pin(sMeasPin).setLocation(TM::RAM).setRecordingMode(TM::PF).setStartCycle(iBitStart).setMaxCycles(iBitLength);
            emap.pin(sMeasPin).setLocation(TM::RAM).setRecordingMode(TM::PF);

            double dOrgSpecVal = get_spec_val("TIM",sSpecVariable);

            //=======================================================================================================
            // Initialize & resize map arrays for pass/fail results
            //=======================================================================================================
            int iNumOfSweep = 0;
            for (double dSweep = dSweepStart; dSweep < dSweepStop+dSweepStep/2; dSweep += dSweepStep){
                iNumOfSweep++;
            }
                                        ARRAY_BOOL      bTmp1(iBitLength+1);         //                          [Bit], +1 for BitLength summary
                                 vector<ARRAY_BOOL>     bTmp2(iNumOfSweep, bTmp1);   //                   [Sweep][Bit]
                     map<string, vector<ARRAY_BOOL> >   bTmp3;                       //              [Pin][Sweep][Bit]
            map<int, map<string, vector<ARRAY_BOOL> > > bResult;                     // bResult[Site][Pin][Sweep][Bit]

            for(int j=0; j<iNumOfAllPin; j++){
                bTmp3.insert(map<string, vector<ARRAY_BOOL> >::value_type(sAllPinList[j], bTmp2));
            }

            FOR_EACH_SITE_BEGIN();
                bResult.insert(map<int, map<string, vector<ARRAY_BOOL> > >::value_type(CURRENT_SITE_NUMBER(), bTmp3));
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // Sweep strobe edges
            //=======================================================================================================
            int iSweepCurr = 0;
            for (double dSweep = dSweepStart; dSweep < dSweepStop+dSweepStep/2; dSweep += dSweepStep){
                set_spec_val("TIM",sSpecVariable,dSweep);

                FUNCTIONAL_TEST();

                FOR_EACH_SITE_BEGIN();
                    int iSite = CURRENT_SITE_NUMBER();
                    //=======================================================================================================
                    // Get pass/fail results in vector cycle range
                    // Execution time with API is slow. If you want higher x-put, you should use FW command like ERMP/EMAP.
                    //=======================================================================================================
                    for(int j=0; j<iNumOfMeasPin; j++){
                        emap.pin(sMeasPinList[j]).getPassFailResultsInRange(iBitStart, iBitLength, bResult[iSite][sMeasPinList[j]][iSweepCurr]);
                    }
                FOR_EACH_SITE_END();
                iSweepCurr++;
            }
            set_spec_val("TIM",sSpecVariable,dOrgSpecVal);

            //=======================================================================================================
            // Manipulation of pass/fail results for offline debug
            //=======================================================================================================
            INT iOffline;
            GET_SYSTEM_FLAG("offline", &iOffline );
            if(iOffline){
                FOR_EACH_SITE_BEGIN();
                    int iSite = CURRENT_SITE_NUMBER();

                    for(int j=0; j<iNumOfMeasPin; j++){
                        for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                            bool *ptr = &bResult[iSite][sMeasPinList[j]][iSweep][0];
                            for(int iBit=0; iBit<iBitLength; iBit++){
                                if((iSweep>=(0                )&&(iSweep<(20+(4-(j+iBit)%7))))) ptr[iBit] = 0;
                                if((iSweep>=(21+(4-(j+iBit)%7))&&(iSweep<(22+(4-(j+iBit)%7))))) ptr[iBit] = 0;
                                if((iSweep>=(40+(3-(j+iBit)%5))&&(iSweep<(41+(3-(j+iBit)%5))))) ptr[iBit] = 0;
                                if((iSweep>=(42+(3-(j+iBit)%5))&&(iSweep<(iNumOfSweep+j-2))))   ptr[iBit] = 0;
                                //if(iBit==5) ptr[iBit] = 0;
                                if         (iBit==0)                                    ptr[iBit] = 1;
                                if         (iBit==1)                                    ptr[iBit] = 1;
                                if         (iBit==2)                                    ptr[iBit] = 1;
                                if         (iBit==9)                                    ptr[iBit] = 1;
                                if((j==0)&&(iBit==10))                                  ptr[iBit] = 1;
                                if((j==1)&&(iBit==10))                                  ptr[iBit] = 1;
                                //if((j==0)&&(iBit==21))                                  ptr[iBit] = 0;
                                //if((j==1)&&(iBit==21))                                  ptr[iBit] = 1;
                                //if((j==0)&&(iBit==22))                                  ptr[iBit] = 1;
                                //if((j==1)&&(iBit==22))                                  ptr[iBit] = 0;
                                //if((j==0)&&(iBit==23))                                  ptr[iBit] = 0;
                                //if((j==1)&&(iBit==23))                                  ptr[iBit] = 0;
                                //if((j==2)&&(iBit==24))                                  ptr[iBit] = 0;
                                //if((j==2)&&(iBit==25))                                  ptr[iBit] = 1;
                                //if(iBit==26)                                            ptr[iBit] = 0;
                            }
                        }
                    }
                FOR_EACH_SITE_END();
            }

            //=======================================================================================================
            // Overlay pass/fail results among source pins
            //=======================================================================================================
            vector<string> sSourcePinList;
            sSourcePinList = PinUtility.getDigitalPinNamesFromPinList(sSourcePin,TM::ALL_DIGITAL,false,false,PIN_UTILITY::DEFINITION_ORDER);
            int iNumOfSourcePin = sSourcePinList.size();

            FOR_EACH_SITE_BEGIN();
                int iSite = CURRENT_SITE_NUMBER();

                for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                    bool *ptrSourceResult = &bResult[iSite][sSourcePinName][iSweep][0];
                    for(int iBit=0; iBit<iBitLength; iBit++){
                        ptrSourceResult[iBit] = 1;
                    }
                }

                for(int j=0; j<iNumOfSourcePin; j++){
                    for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                        bool *ptrResult = &bResult[iSite][sSourcePinList[j]][iSweep][0];
                        bool *ptrSourceResult = &bResult[iSite][sSourcePinName][iSweep][0];
                        for(int iBit=0; iBit<iBitLength; iBit++){
                            ptrSourceResult[iBit] = ptrSourceResult[iBit] & ptrResult[iBit];
                        }
                    }
                }
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // Overlay pass/fail results among sink pins
            //=======================================================================================================
            vector<string> sSinkPinList;
            sSinkPinList = PinUtility.getDigitalPinNamesFromPinList(sSinkPin,TM::ALL_DIGITAL,false,false,PIN_UTILITY::DEFINITION_ORDER);
            int iNumOfSinkPin = sSinkPinList.size();

            FOR_EACH_SITE_BEGIN();
                int iSite = CURRENT_SITE_NUMBER();

                for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                    bool *ptrSinkResult = &bResult[iSite][sSinkPinName][iSweep][0];
                    for(int iBit=0; iBit<iBitLength; iBit++){
                        ptrSinkResult[iBit] = 1;
                    }
                }

                for(int j=0; j<iNumOfSinkPin; j++){
                    for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                        bool *ptrResult = &bResult[iSite][sSinkPinList[j]][iSweep][0];
                        bool *ptrSinkResult = &bResult[iSite][sSinkPinName][iSweep][0];
                        for(int iBit=0; iBit<iBitLength; iBit++){
                            ptrSinkResult[iBit] = ptrSinkResult[iBit] & ptrResult[iBit];
                        }
                    }
                }
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // Overlay pass/fail results over bits length 
            //=======================================================================================================
             FOR_EACH_SITE_BEGIN();
                int iSite = CURRENT_SITE_NUMBER();

                for(int j=0; j<iNumOfAllPin; j++){
                    for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                        bool *ptrResult = &bResult[iSite][sAllPinList[j]][iSweep][0];
                        bool *ptrSummaryResult = &bResult[iSite][sAllPinList[j]][iSweep][iBitLength]; // iBit length summay
                        *ptrSummaryResult = 1; // iBit length summary initialization
                        for(int iBit=0; iBit<iBitLength; iBit++){
                            *ptrSummaryResult = *ptrSummaryResult & ptrResult[iBit];
                        }
                    }
                }
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // Initialize & resize map array for left, right boundaries and delta between source pin and target pin
            //=======================================================================================================
                                 vector<int>     iTmp1(iBitLength+1, sizeof(int));  //            [Bit], +1 for BitLength summary                       
                     map<string, vector<int> >   iTmp2;                             //       [Pin][Bit]
            map<int, map<string, vector<int> > > iLeft;                             // [Site][Pin][Bit]
            map<int, map<string, vector<int> > > iRight;                            // [Site][Pin][Bit]
            map<int, map<string, vector<int> > > iDelta;                            // [Site][Pin][Bit]

            for(int j=0; j<iNumOfAllPin; j++){
                iTmp2.insert(map<string, vector<int> >::value_type(sAllPinList[j], iTmp1));
            }

            FOR_EACH_SITE_BEGIN();
                iLeft.insert (map<int, map<string, vector<int> > >::value_type(CURRENT_SITE_NUMBER(), iTmp2));
                iRight.insert(map<int, map<string, vector<int> > >::value_type(CURRENT_SITE_NUMBER(), iTmp2));
                iDelta.insert(map<int, map<string, vector<int> > >::value_type(CURRENT_SITE_NUMBER(), iTmp2));
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // Calculate left & right boundaries per cycle (Biggest pass region determines boundaries)
            //=======================================================================================================
            FOR_EACH_SITE_BEGIN();
                int iSite = CURRENT_SITE_NUMBER();

                //=============================================================
                // For left & right boundaries
                //=============================================================
                for(int j=0; j<iNumOfAllPin; j++){
                    int *ptrLeft  = &iLeft[iSite][sAllPinList[j]][0];
                    int *ptrRight = &iRight[iSite][sAllPinList[j]][0];
                    for(int iBit=0; iBit<iBitLength+1; iBit++){
                        int iLeftTmp      = -1;
                        int iLeftCur      = 0;
                        int iRightTmp     = -1;
                        int iRightCur     = 0;
                        int iPassCountTmp = 0;
                        int iPassCountCur = 0;
                        for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                            if(bResult[iSite][sAllPinList[j]][iSweep][iBit]){
                                if(iPassCountCur == 0){
                                    iLeftCur = iSweep;
                                }
                                iPassCountCur++;
                                if(iSweep == iNumOfSweep-1){
                                    iRightCur = iSweep;
                                    if(iPassCountTmp < iPassCountCur){
                                        iPassCountTmp = iPassCountCur;
                                        iLeftTmp      = iLeftCur;
                                        iRightTmp     = iRightCur;
                                    }
                                    iPassCountCur = 0;
                                }
                            } else{
                                if(iPassCountCur){
                                    iRightCur = iSweep-1;
                                    if(iPassCountTmp < iPassCountCur){
                                        iPassCountTmp = iPassCountCur;
                                        iLeftTmp      = iLeftCur;
                                        iRightTmp     = iRightCur;
                                    }
                                    iPassCountCur = 0;
                                }
                            }
                        }
                        if     (iLeftTmp == -1)             ptrLeft[iBit]  = -9999;     // Failure
                        else if(iLeftTmp ==  0)             ptrLeft[iBit]  =  9999;     // NA
                        else                                ptrLeft[iBit]  = iLeftTmp;  // Normal

                        if     (iRightTmp == -1)            ptrRight[iBit] = -9999;     // Failure
                        else if(iRightTmp == iNumOfSweep-1) ptrRight[iBit] =  9999;     // NA
                        else                                ptrRight[iBit] = iRightTmp; // Normal
                    }
                }

                //=============================================================
                // For delta between source pin and target pin on left boundary
                //=============================================================
                int *ptrSourceLeft  = &iLeft[iSite][sSourcePinName][0];
                for(int j=0; j<iNumOfAllPin; j++){
                    int *ptrLeft  = &iLeft[iSite][sAllPinList[j]][0];
                    int *ptrDelta = &iDelta[iSite][sAllPinList[j]][0];
                    for(int iBit=0; iBit<iBitLength+1; iBit++){
                        if     (ptrSourceLeft[iBit] == -9999) ptrDelta[iBit] = -9999;                               // Failure
                        else if(ptrSourceLeft[iBit] ==  9999) ptrDelta[iBit] =  9999;                               // NA
                        else if(ptrLeft[iBit]       == -9999) ptrDelta[iBit] = -9999;                               // Failure
                        else if(ptrLeft[iBit]       ==  9999) ptrDelta[iBit] =  9999;                               // NA
                        else                                  ptrDelta[iBit] = ptrLeft[iBit] - ptrSourceLeft[iBit]; // Normal
                    }
                }
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // Calculate max, min, mean and rms through cycles
            //=======================================================================================================
            map<int, map<string, int   > > iLeftMin;     // [Site][Pin]
            map<int, map<string, int   > > iLeftMax;     // [Site][Pin]
            map<int, map<string, double> > dLeftMean;    // [Site][Pin]
            map<int, map<string, double> > dLeftRms;     // [Site][Pin]
            map<int, map<string, int   > > iRightMin;    // [Site][Pin]
            map<int, map<string, int   > > iRightMax;    // [Site][Pin]
            map<int, map<string, double> > dRightMean;   // [Site][Pin]
            map<int, map<string, double> > dRightRms;    // [Site][Pin]

            FOR_EACH_SITE_BEGIN();
                int iSite = CURRENT_SITE_NUMBER();

                for(int j=0; j<iNumOfAllPin; j++){
                    //=============================================================
                    // Extract meaningful data
                    //=============================================================
                    int iCountLeft  = 0;
                    int iCountRight = 0;
                    int iCountDelta = 0;
                    ARRAY_I iMeaningfulLeft(iBitLength+1);
                    ARRAY_I iMeaningfulRight(iBitLength+1);
                    ARRAY_I iMeaningfulDelta(iBitLength+1);
                    int *ptrLeft  = &iLeft[iSite][sAllPinList[j]][0];
                    int *ptrRight = &iRight[iSite][sAllPinList[j]][0];
                    int *ptrDelta = &iDelta[iSite][sAllPinList[j]][0];

                    for(int iBit=0; iBit<iBitLength+1; iBit++){
                        if(ptrLeft[iBit] != 9999){
                            iMeaningfulLeft[iCountLeft] = ptrLeft[iBit];
                            iCountLeft++;
                        }

                        if(ptrRight[iBit] != 9999){
                            iMeaningfulRight[iCountRight] = ptrRight[iBit];
                            iCountRight++;
                        }

                        if(ptrDelta[iBit] != 9999){
                            iMeaningfulDelta[iCountDelta] = ptrDelta[iBit];
                            iCountDelta++;
                        }
                    }
                    //=============================================================
                    // Calculate max, min, mean and rms from meaningful data
                    //=============================================================
                    int iMinIndex;
                    int iMaxIndex;
                    if(iCountLeft > 1){
                        DSP_MINMAX(iMeaningfulLeft,&iLeftMin[iSite][sAllPinList[j]],&iLeftMax[iSite][sAllPinList[j]],&iMinIndex,&iMaxIndex,iCountLeft);
                        DSP_MEAN(iMeaningfulLeft,&dLeftMean[iSite][sAllPinList[j]],&dLeftRms[iSite][sAllPinList[j]],iCountLeft);
                    } else if(iCountLeft == 1){
                        iLeftMin[iSite][sAllPinList[j]]  = iMeaningfulLeft[0];
                        iLeftMax[iSite][sAllPinList[j]]  = iMeaningfulLeft[0];
                        dLeftMean[iSite][sAllPinList[j]] = iMeaningfulLeft[0];
                        dLeftRms[iSite][sAllPinList[j]]  = 0;
                    } else{
                        iLeftMin[iSite][sAllPinList[j]]  = -9999;
                        iLeftMax[iSite][sAllPinList[j]]  = -9999;
                        dLeftMean[iSite][sAllPinList[j]] = -9999;
                        dLeftRms[iSite][sAllPinList[j]]  = -9999;
                    }

                    if(iCountRight > 1){
                        DSP_MINMAX(iMeaningfulRight,&iRightMin[iSite][sAllPinList[j]],&iRightMax[iSite][sAllPinList[j]],&iMinIndex,&iMaxIndex,iCountRight);
                        DSP_MEAN(iMeaningfulRight,&dRightMean[iSite][sAllPinList[j]],&dRightRms[iSite][sAllPinList[j]],iCountRight);
                    } else if(iCountRight == 1){
                        iRightMin[iSite][sAllPinList[j]]  = iMeaningfulRight[0];
                        iRightMax[iSite][sAllPinList[j]]  = iMeaningfulRight[0];
                        dRightMean[iSite][sAllPinList[j]] = iMeaningfulRight[0];
                        dRightRms[iSite][sAllPinList[j]]  = 0;
                    } else{
                        iRightMin[iSite][sAllPinList[j]]  = -9999;
                        iRightMax[iSite][sAllPinList[j]]  = -9999;
                        dRightMean[iSite][sAllPinList[j]] = -9999;
                        dRightRms[iSite][sAllPinList[j]]  = -9999;
                    }

                    if(iCountDelta > 1){
                        DSP_MINMAX(iMeaningfulDelta,&iDeltaMin[iSite][sAllPinList[j]],&iDeltaMax[iSite][sAllPinList[j]],&iMinIndex,&iMaxIndex,iCountDelta);
                        DSP_MEAN(iMeaningfulDelta,&dDeltaMean[iSite][sAllPinList[j]],&dDeltaRms[iSite][sAllPinList[j]],iCountDelta);
                    } else if(iCountDelta == 1){
                        iDeltaMin[iSite][sAllPinList[j]]  = iMeaningfulDelta[0];
                        iDeltaMax[iSite][sAllPinList[j]]  = iMeaningfulDelta[0];
                        dDeltaMean[iSite][sAllPinList[j]] = iMeaningfulDelta[0];
                        dDeltaRms[iSite][sAllPinList[j]]  = 0;
                    } else{
                        iDeltaMin[iSite][sAllPinList[j]]  = -9999;
                        iDeltaMax[iSite][sAllPinList[j]]  = -9999;
                        dDeltaMean[iSite][sAllPinList[j]] = -9999;
                        dDeltaRms[iSite][sAllPinList[j]]  = -9999;
                    }
                }
            FOR_EACH_SITE_END();

            //=======================================================================================================
            // File open
            //=======================================================================================================
            ofstream fout;
            if(iFlagExportFile){
                string sFilePath;
                char cDevPath[1000];
                GetDevPath(cDevPath);
                string sSuiteName;
                GET_TESTSUITE_NAME(sSuiteName);

                char   cNowTime[100];
                time_t timer = time(NULL);
                struct tm *tm;
                tm = localtime(&timer);
                strftime(cNowTime,sizeof(cNowTime),"%Y%m%d_%H%M%S",tm);

                sFilePath = string(cDevPath) + "/" + sFileDirectory + "/" + sSuiteName + "_" + string(cNowTime) + ".log";
                fout.open(sFilePath.c_str(), ios::app);
                if(!fout){
                    cout << "ERROR: File " << sFilePath << " cannot open.\n";
                    iFlagExportFile = 0;
                } else{
                    cout << "File: " << sFilePath << " is generated." << endl;
                }
            }

            //=======================================================================================================
            // Output results into reportUI and/or file
            //=======================================================================================================
            FOR_EACH_SITE_BEGIN();
                int iSite = CURRENT_SITE_NUMBER();
                stringstream sout;

                if ((sReportMode == "All") || (sReportMode == "ValueSummary")) {
                     //=============================================================
                     // Output pin name
                     //=============================================================
                     sout << endl;
                     sout << "Site" << left << setw(6) << iSite;
                     for(int j=0; j<iNumOfAllPin; j++){
                         sout << left << setw(iNumOfSweep) << sAllPinList[j];
                         sout << " (" << right << fixed << setw(7) << "lead";
                         sout << " ," << right << fixed << setw(7) << "trail";
                         sout << " ," << right << fixed << setw(7) << "delta";
                         sout << ")  ";
                     }
                     sout << "Summary";
                     if(iFlagReportUI)   cout << sout.str() << endl;
                     if(iFlagExportFile) fout << sout.str() << endl;
                     sout.str("");
                }

                if ((sReportMode == "All")) {   
                    //=============================================================
                    // Output per pin results for each cycle
                    //=============================================================
                    for(int iBit=0; iBit<iBitLength; iBit++){
                        sout << left << setw(5) << iCycleStart + int(iBit/iEdgesPerCycle) << ":" << setw(2) << iBit%iEdgesPerCycle + 1 << "  ";

                        for(int j=0; j<iNumOfAllPin; j++){
                            for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                               if(bResult[iSite][sAllPinList[j]][iSweep][iBit]) sout << "."; 
                               else                                             sout << "F";
                            }

                            int iLeftIndex = iLeft[iSite][sAllPinList[j]][iBit];
                            if     (iLeftIndex == -9999)   sout << " (" << right << fixed << setw(7) << setprecision(0) << -9999;
                            else if(iLeftIndex ==  9999)   sout << " (" << right << fixed << setw(7) << "----";
                            else                           sout << " (" << right << fixed << setw(7) << setprecision(3) << iLeftIndex  * dSweepStep + dSweepStart;

                            int iRightIndex = iRight[iSite][sAllPinList[j]][iBit];
                            if     (iRightIndex == -9999)  sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                            else if(iRightIndex ==  9999)  sout << " ," << right << fixed << setw(7) << "----";
                            else                           sout << " ," << right << fixed << setw(7) << setprecision(3) << iRightIndex  * dSweepStep + dSweepStart;

                            int iDeltaIndex = iDelta[iSite][sAllPinList[j]][iBit];
                            if     (iDeltaIndex == -9999)  sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                            else if(iDeltaIndex ==  9999)  sout << " ," << right << fixed << setw(7) << "----";
                            else                           sout << " ," << right << fixed << setw(7) << setprecision(3) << iDeltaIndex  * dSweepStep;

                            sout << ")  ";
                        }

                        {
                            //=============================================================
                            // Summary
                            //=============================================================
                            for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                               if     (( bResult[iSite][sSourcePinName][iSweep][iBit])&&( bResult[iSite][sSinkPinName][iSweep][iBit])) sout << "."; 
                               else if(( bResult[iSite][sSourcePinName][iSweep][iBit])&&(!bResult[iSite][sSinkPinName][iSweep][iBit])) sout << ">"; 
                               else if((!bResult[iSite][sSourcePinName][iSweep][iBit])&&( bResult[iSite][sSinkPinName][iSweep][iBit])) sout << "<"; 
                               else if((!bResult[iSite][sSourcePinName][iSweep][iBit])&&(!bResult[iSite][sSinkPinName][iSweep][iBit])) sout << "F"; 
                            }
                        }

                        if(iFlagReportUI) cout << sout.str() << endl;
                        if(iFlagExportFile) fout << sout.str() << endl;
                        sout.str("");
                    }

                    sout << endl;
                }

                if ((sReportMode == "All") ||(sReportMode == "ValueSummary") || (sReportMode == "BitSummaryOnly")) { 
                    //=============================================================
                    // bitLength summary output
                    //=============================================================
                    for(int iBit=iBitLength; iBit<iBitLength+1; iBit++){
                        sout << left << setw(9) << "summary" ;

                        for(int j=0; j<iNumOfAllPin; j++){
                              for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                                  if(bResult[iSite][sAllPinList[j]][iSweep][iBit]) sout << "."; 
                                  else          sout << "F";
                              }

                              int iLeftIndex = iLeft[iSite][sAllPinList[j]][iBit];
                              if     (iLeftIndex == -9999)   sout << " (" << right << fixed << setw(7) << setprecision(0) << -9999;
                              else if(iLeftIndex ==  9999)   sout << " (" << right << fixed << setw(7) << "----";
                              else                           sout << " (" << right << fixed << setw(7) << setprecision(3) << iLeftIndex  * dSweepStep + dSweepStart;

                              int iRightIndex = iRight[iSite][sAllPinList[j]][iBit];
                              if     (iRightIndex == -9999)  sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                              else if(iRightIndex ==  9999)  sout << " ," << right << fixed << setw(7) << "----";
                              else                           sout << " ," << right << fixed << setw(7) << setprecision(3) << iRightIndex  * dSweepStep + dSweepStart;

                              int iDeltaIndex = iDelta[iSite][sAllPinList[j]][iBit];
                              if     (iDeltaIndex == -9999)  sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                              else if(iDeltaIndex ==  9999)  sout << " ," << right << fixed << setw(7) << "----";
                              else                           sout << " ," << right << fixed << setw(7) << setprecision(3) << iDeltaIndex  * dSweepStep;

                              sout << ")  ";
                        }

                        {
                            //=============================================================
                            // Summary
                            //=============================================================
                            for(int iSweep=0; iSweep<iNumOfSweep; iSweep++){
                                if     (( bResult[iSite][sSourcePinName][iSweep][iBit])&&( bResult[iSite][sSinkPinName][iSweep][iBit])) sout << "."; 
                                else if(( bResult[iSite][sSourcePinName][iSweep][iBit])&&(!bResult[iSite][sSinkPinName][iSweep][iBit])) sout << ">"; 
                                else if((!bResult[iSite][sSourcePinName][iSweep][iBit])&&( bResult[iSite][sSinkPinName][iSweep][iBit])) sout << "<"; 
                                else if((!bResult[iSite][sSourcePinName][iSweep][iBit])&&(!bResult[iSite][sSinkPinName][iSweep][iBit])) sout << "F"; 
                            }
                        }

                        if(iFlagReportUI) cout << sout.str();
                        if(iFlagExportFile) fout << sout.str();
                        sout.str("");
                    }
                }          

                if ((sReportMode == "All") ||(sReportMode == "ValueSummary") ) {
                    sout << endl;
                    sout << endl;
                    //=============================================================
                    // Output per pin Min through cycles
                    //=============================================================
                    sout << left << setw(9) << "Min";
                    for(int j=0; j<iNumOfAllPin; j++){
                        sout << left << setw(iNumOfSweep) << ""; 
                        if(iLeftMin[iSite][sAllPinList[j]] != -9999)  sout << " (" << right << fixed << setw(7) << setprecision(3) << iLeftMin[iSite][sAllPinList[j]]  * dSweepStep + dSweepStart;
                        else                                          sout << " (" << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iRightMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << iRightMin[iSite][sAllPinList[j]] * dSweepStep + dSweepStart;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iDeltaMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << iDeltaMin[iSite][sAllPinList[j]] * dSweepStep;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                        sout << ")  ";
                    }
                    sout << endl;

                    //=============================================================
                    // Output per pin Max through cycles
                    //=============================================================
                    sout << left << setw(9) << "Max";
                    for(int j=0; j<iNumOfAllPin; j++){
                        sout << left << setw(iNumOfSweep) << ""; 
                        if(iLeftMin[iSite][sAllPinList[j]] != -9999)  sout << " (" << right << fixed << setw(7) << setprecision(3) << iLeftMax[iSite][sAllPinList[j]]  * dSweepStep + dSweepStart;
                        else                                          sout << " (" << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iRightMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << iRightMax[iSite][sAllPinList[j]] * dSweepStep + dSweepStart;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iDeltaMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << iDeltaMax[iSite][sAllPinList[j]] * dSweepStep;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                        sout << ")  ";
                    }
                    sout << endl;

                    //=============================================================
                    // Output per pin Mean through cycles
                    //=============================================================
                    sout << left << setw(9) << "Mean";
                    for(int j=0; j<iNumOfAllPin; j++){
                        sout << left << setw(iNumOfSweep) << ""; 
                        if(iLeftMin[iSite][sAllPinList[j]] != -9999)  sout << " (" << right << fixed << setw(7) << setprecision(3) << dLeftMean[iSite][sAllPinList[j]]  * dSweepStep + dSweepStart;
                        else                                          sout << " (" << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iRightMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << dRightMean[iSite][sAllPinList[j]] * dSweepStep + dSweepStart;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iDeltaMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << dDeltaMean[iSite][sAllPinList[j]] * dSweepStep;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                        sout << ")  ";
                    }
                    sout << endl;

                    //=============================================================
                    // Output per pin Rms through cycles
                    //=============================================================
                    sout << left << setw(9) << "Rms";
                    for(int j=0; j<iNumOfAllPin; j++){
                        sout << left << setw(iNumOfSweep) << ""; 
                        if(iLeftMin[iSite][sAllPinList[j]] != -9999)  sout << " (" << right << fixed << setw(7) << setprecision(3) << dLeftRms[iSite][sAllPinList[j]]  * dSweepStep;
                        else                                          sout << " (" << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iRightMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << dRightRms[iSite][sAllPinList[j]] * dSweepStep;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;

                        if(iDeltaMin[iSite][sAllPinList[j]] != -9999) sout << " ," << right << fixed << setw(7) << setprecision(3) << dDeltaRms[iSite][sAllPinList[j]] * dSweepStep;
                        else                                          sout << " ," << right << fixed << setw(7) << setprecision(0) << -9999;
                        sout << ")  ";
                    }
                    sout << endl;
                }

                if(iFlagReportUI)   cout << sout.str() << endl;
                if(iFlagExportFile) fout << sout.str() << endl;
                sout.str("");
                //sout << setprecision(6) << resetiosflags(ios::floatfield);
            FOR_EACH_SITE_END();

            if(iFlagExportFile) fout.close();

            TMLimits& limits = TMLimits::getInstance();
            limitTDQSQ = limits.getLimitObj("limit_tDQSQ");
            limitTDQSQ.getLow(&limitLow);
            limitTDQSQ.getHigh(&limitHigh);

        ON_FIRST_INVOCATION_END();
    
        if(iDeltaMin[CURRENT_SITE_NUMBER()][sSinkPinName] == -9999){
            TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitTDQSQ,-9999);
        } else{
            TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitTDQSQ,(iDeltaMin[CURRENT_SITE_NUMBER()][sSinkPinName] * dSweepStep));
            TESTSET().cont(true).judgeAndLog_ParametricTest("@","",limitTDQSQ,(iDeltaMax[CURRENT_SITE_NUMBER()][sSinkPinName] * dSweepStep));
        }

        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
        if(sOutput == "ReportUI"){
            getParameter("sFileDirectory").setVisible(false);
        } else{
            getParameter("sFileDirectory").setVisible(true);
        }

        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("tDQSQ", tDQSQ);
