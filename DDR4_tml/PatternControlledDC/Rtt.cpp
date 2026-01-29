#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
using namespace V93kLimits;

class Rtt: public testmethod::TestMethod {
    protected:
    string measPins;
    double vForceH_mV;
    double vForceL_mV;
    string subLabel;
    string limitSpecifier;

    virtual void initialize()
    {
        addParameter("measurementPins",     "PinString", &measPins,       testmethod::TM_PARAMETER_INPUT).setDefault("DQ_Bus");
        addParameter("forceVoltageHigh_mV", "double",    &vForceH_mV,     testmethod::TM_PARAMETER_INPUT).setDefault("1200");
        addParameter("forceVoltageLow_mV",  "double",    &vForceL_mV,     testmethod::TM_PARAMETER_INPUT).setDefault("300");
        addParameter("GenericDCSubLabel",   "string",    &subLabel,       testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("generic name of DC-SubRoutine to be used, i.e. DC_MEAS_RTT120");
        addParameter("limitSpecifier",      "string",    &limitSpecifier, testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("as Specified in \"Test name\" column of Limit TestTable");
    }

    virtual void run()
    {
        static int isOffline;
        static int debugMode;
        PPMU_MEASURE meas;
        PATTERN_CONTROLLER ctrl;
        static LIMIT limitRTT;
        static double limitLow, limitHigh;
        static double vForceH_V, vForceL_V;
        static bool run_once = true;

        DC_RESULT_ACCESSOR pmuEvent;
        
        ON_FIRST_INVOCATION_BEGIN();

            //================================================================================
            // Read out testtable limits
            //================================================================================
            TMLimits& limits = TMLimits::getInstance();
            if(run_once) {
            // testtable file is spefified in the testflow //    limits.load("TMLimits.csv");
                GET_SYSTEM_FLAG("offline",&isOffline);

                vForceH_V = vForceH_mV / 1E+3;
                vForceL_V = vForceL_mV / 1E+3;
                run_once=false;
            } // run_once
            GET_TESTFLOW_FLAG("debug_mode",&debugMode);
            limitRTT = limits.getLimitObj(limitSpecifier);
            limitRTT.getLow(&limitLow);
            limitRTT.getHigh(&limitHigh);

            //================================================================================
            // DC setup
            //================================================================================
            PPMU_SETTING setting_RTTpu, setting_RTTpd;
//            setting_RTTpu.pin(measPins).vForce(vForceH_V).iRange(vForceH_V/limitLow).min(-(vForceH_V/limitLow)).max(vForceH_V/limitLow);
//            setting_RTTpd.pin(measPins).vForce(vForceL_V).iRange(vForceL_V/limitLow).min(-(vForceL_V/limitLow)).max(vForceL_V/limitLow);
            setting_RTTpu.pin(measPins).vForce(vForceH_V).iRange(0.02).min(-(vForceH_V/limitLow)).max(vForceH_V/limitLow);
            setting_RTTpd.pin(measPins).vForce(vForceL_V).iRange(0.02).min(-(vForceL_V/limitLow)).max(vForceL_V/limitLow);

            cout  << "limitLow  = " << limitLow <<endl;
            cout  << "vForceL_V/limitLow  = " << vForceL_V/limitLow <<endl;
            cout  << "vForceH_V/limitLow  = " << vForceH_V/limitLow <<endl;

            PPMU_RELAY closeRelay, openRelay;
            closeRelay.pin(measPins).status("PPMU_ON");
//            closeRelay.pin(measPins).status("ACPM_ON");
            closeRelay.wait(0.001); // 1ms
            openRelay.pin(measPins).status("AC_ON");

            meas.pin(measPins).measurementType(TM::VMUM);

            //================================================================================
            // Add to Label & execute
            //================================================================================
            ctrl.label(subLabel+"_PU").add(setting_RTTpu).add(closeRelay).add(meas).add(openRelay);
            ctrl.label(subLabel+"_PD").add(setting_RTTpd).add(closeRelay).add(meas).add(openRelay);

            CONNECT();
            ctrl.execMode(TM::DCTEST_AUTO).execute();

            //================================================================================
            // Upload results and put to datalog
            //================================================================================
            pmuEvent.uploadResult(measPins, TM::RESULT_INDEX);

        ON_FIRST_INVOCATION_END();

        STRING_VECTOR pinList = PinUtility.getDigitalPinNamesFromPinList(measPins,TM::ALL_DIGITAL);

        if(! isOffline) { //online case
            for(unsigned int i=0; i<pinList.size(); i++){

                // use result accessor member functions getFirstResultPoint() and getLastResultPoint()
                // to obtain iterators (RESUL_POINT objects) to the first and last uploaded result per pin
                const RESULT_POINT first(pmuEvent.getFirstResultPoint(pinList[i]));
                const RESULT_POINT end  (pmuEvent.getLastResultPoint(pinList[i]));
                double iOutpu_A;
                double iOutpd_A;

                for ( RESULT_POINT p = first; p <= end; ++p) {
                    DOUBLE measuredValue = pmuEvent.getValue(pinList[i],p);

                    RESULT_POINT::IndexType resultIndex = p.getIndex();
                    if(resultIndex == 0) {
                        //pull-down, refer to pattern
                        iOutpd_A = measuredValue;
                    }
                    else if(resultIndex == 1) {
                        // pull-up, refer to pattern
                        iOutpu_A = measuredValue;
                    }
                    else {
                        cerr << "Unknown Rtt Measurement" << endl;
                        TEST(false);
                        return;
                    }
                }// result point loop

                double rtt = (vForceH_V - vForceL_V)/(iOutpu_A - iOutpd_A);
                if(debugMode == 2) {
                    cout <<  pinList[i] << " - " << limitSpecifier << " - nominal [Ohm]\t\t"
                         << "( " << limitLow << " <\t" << rtt << "\t< " << limitHigh << " )" << "\tvForceH_V=" << vForceH_V << "\tvForceL_V=" << vForceL_V << "\tiOutpu_A=" << iOutpu_A << "\tiOutpd_A=" << iOutpd_A << endl;
                }
                TESTSET().cont(true).judgeAndLog_ParametricTest(pinList[i],"",limitRTT,rtt);
            } // pin loop
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
REGISTER_TESTMETHOD("PatternControlledDC.Rtt", Rtt);
