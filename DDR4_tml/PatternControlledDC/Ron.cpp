#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
using namespace V93kLimits;


class Ron: public testmethod::TestMethod {
    protected:
    string measPins;
    string subLabel;
    string limitSpecifier;

    virtual void initialize()
    {
        addParameter("measurementPins", "PinString", &measPins, testmethod::TM_PARAMETER_INPUT).setDefault("DQ_Bus");
        addParameter("DCSubLabel",      "string",    &subLabel,testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("name of DC-SubRoutine to be used");
        addParameter("limitSpecifier",  "string",    &limitSpecifier,testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("as specified in \"Test name\" column of Limit TestTable");
    }

    virtual void run()
    {
        static int isOffline;
        static int debugMode;
        PPMU_MEASURE meas;
        PATTERN_CONTROLLER ctrl;
        static LIMIT limitRON;
        static double limitLow, limitHigh;
        static double vForce_V;
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

                // use VREF as force voltage
                vForce_V = (Primary.getLevelSpec().getSpecValue("VDD")) / 2;
                run_once=false;
            } // run_once

            GET_TESTFLOW_FLAG("debug_mode",&debugMode);

            limitRON = limits.getLimitObj(limitSpecifier);

            limitRON.getLow(&limitLow);

            limitRON.getHigh(&limitHigh);

            //================================================================================
            // DC setup
            //================================================================================
            PPMU_SETTING setting_RON;
            //setting_RON.pin(measPins).vForce(vForce_V).iRange(vForce_V/limitLow).min(-(vForce_V/limitLow)).max(vForce_V/limitLow);
            setting_RON.pin(measPins).vForce(vForce_V).iRange(0.06).min(-0.06).max(0.06);

            PPMU_RELAY closeRelay, openRelay;
            closeRelay.pin(measPins).status("PPMU_ON");
            closeRelay.wait(0.001); // 1ms
            //closeRelay.wait(5.001); // 1ms
            openRelay.pin(measPins).status("AC_ON");

            meas.pin(measPins).measurementType(TM::VMUM);

            //================================================================================
            // Add to Label & execute
            //================================================================================
            ctrl.label(subLabel).add(setting_RON).add(closeRelay).add(meas).add(openRelay);

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
                        cerr << "Unknown Ron Measurement" << endl;
                        TEST(false);
                        return;
                    }
                }// result point loop


                double ron_pd = vForce_V / abs(iOutpd_A);
                double ron_pu = vForce_V / abs(iOutpu_A);

                if(debugMode == 2 ) {
                    cout <<  pinList[i] << " - " << limitSpecifier << " - pull-down [Ohm]\t\t"
                         << "( " << limitLow << " <\t" << ron_pd << "\t< " << limitHigh << " )\tvForce_V= " << vForce_V << "\tiOutPd_A = " << iOutpd_A << endl;
                    cout <<  pinList[i] << " - " << limitSpecifier << " - pull-up   [Ohm]\t\t"
                         << "( " << limitLow << " <\t" << ron_pu << "\t< " << limitHigh << " )\tvForce_V= " << vForce_V << "\tiOutPu_A = " << iOutpu_A << endl;
                }
                TESTSET().cont(true).judgeAndLog_ParametricTest(pinList[i],"",limitRON,ron_pd);
                TESTSET().cont(true).judgeAndLog_ParametricTest(pinList[i],"",limitRON,ron_pu);
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
REGISTER_TESTMETHOD("PatternControlledDC.Ron", Ron);
