#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
using namespace V93kLimits;


//###=##########################################################################################################################
//##############################################################################################################################
//##############################################################################################################################
class StaticDC_IFVM: public testmethod::TestMethod {
    protected:
    string measPins;
    string limitSpecifier;

    virtual void initialize()
    {
        addParameter("measurementPins", "PinString", &measPins, testmethod::TM_PARAMETER_INPUT).setDefault("DQ_Bus");
        addParameter("limitSpecifier",  "string",    &limitSpecifier,testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("as specified in \"Test name\" column of Limit TestTable");
    }

    virtual void run()
    {
        static int debugMode;
        PPMU_MEASURE meas;
        static LIMIT limitIFVM;
        static double limitLow, limitHigh;
        static bool run_once = true;

        ON_FIRST_INVOCATION_BEGIN();
            
            //================================================================================
            // Read out testtable limits
            //================================================================================
            TMLimits& limits = TMLimits::getInstance();
            if(run_once) {
            // testtable file is spefified in the testflow //    limits.load("TMLimits.csv");
                run_once=false;
            } // run_once

            GET_TESTFLOW_FLAG("debug_mode",&debugMode);

            limitIFVM = limits.getLimitObj(limitSpecifier);

            limitIFVM.getLow(&limitLow);

            limitIFVM.getHigh(&limitHigh);

            //================================================================================
            // DC setup
            //================================================================================
            PPMU_SETTING setting;
            PPMU_RELAY   closeRelay, openRelay;
            PPMU_CLAMP   enableClamp, disableClamp;
            TASK_LIST    task;
            
            setting.pin(measPins).iForce(100 uA).iRange(100 uA).min(-1).max(1.2);
            enableClamp.pin(measPins).status("CLAMP_ON").low(0 V).high(1.5 V);
            closeRelay.pin(measPins).status("PPMU_ON");
            closeRelay.wait(1 ms);
            openRelay.pin(measPins).status("AC_ON");
            disableClamp.pin(measPins).status("CLAMP_OFF");

            meas.pin(measPins).execMode("PVAL");
            task.add(setting).add(enableClamp).add(closeRelay).add(meas).add(openRelay).add(disableClamp);

            //================================================================================
            // Execute
            //================================================================================
            CONNECT();
            EXECUTE_TEST();
            task.execute();

        ON_FIRST_INVOCATION_END();

        STRING_VECTOR pinList = PinUtility.getDigitalPinNamesFromPinList(measPins,TM::ALL_DIGITAL);

        for(unsigned int i=0; i<pinList.size(); i++){

            double result = meas.getValue(pinList[i]);

            if(debugMode == 2 ) {
                cout <<  pinList[i] << " - " << limitSpecifier << "\t\t"
                     << "( " << limitLow << " <\t" << result << "\t< " << limitHigh << " )" << endl;
            }
            TESTSET().cont(true).judgeAndLog_ParametricTest(pinList[i],"",limitIFVM,result);
        } // pin loop

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
REGISTER_TESTMETHOD("StaticDC.IFVM", StaticDC_IFVM);

//###=##########################################################################################################################
//##############################################################################################################################
//##############################################################################################################################
class StaticDC_IDD: public testmethod::TestMethod {
    protected:
    string measPins;
    string limitSpecifier;
    double settling_ms;

    virtual void initialize()
    {
        addParameter("measurementPins", "PinString", &measPins, testmethod::TM_PARAMETER_INPUT).setDefault("VDD,VDDQ");
        addParameter("limitSpecifier",  "string",    &limitSpecifier,testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("as specified in \"Test name\" column of Limit TestTable");
        addParameter("settling_ms",     "double",    &settling_ms,testmethod::TM_PARAMETER_INPUT);
    }

    virtual void run()
    {
        static int debugMode;
        DPS_TASK dps_task;
        static LIMIT limitIDD;
        static double limitLow, limitHigh;
        static bool run_once = true;

        ON_FIRST_INVOCATION_BEGIN();
            
            //================================================================================
            // Read out testtable limits
            //================================================================================
            TMLimits& limits = TMLimits::getInstance();
            if(run_once) {
            // testtable file is spefified in the testflow //    limits.load("TMLimits.csv");
                run_once=false;
            } // run_once

            GET_TESTFLOW_FLAG("debug_mode",&debugMode);

            limitIDD = limits.getLimitObj(limitSpecifier);
            limitIDD.getLow(&limitLow);
            limitIDD.getHigh(&limitHigh);

            //================================================================================
            // DC setup
            //================================================================================
            dps_task.pin(measPins).limits(limitIDD);
            dps_task.trigMode(TM::INTERNAL).execMode("PVAL").wait(settling_ms * 0.001).samples(16);

            //================================================================================
            // Execute
            //================================================================================
            CONNECT();
            START_TEST();
            dps_task.execute();
            ABORT_TEST();

        ON_FIRST_INVOCATION_END();

        STRING_VECTOR pinList = PinUtility.getDigitalPinNamesFromPinList(measPins,TM::DC_PIN);

        for(unsigned int i=0; i<pinList.size(); i++){

            double result = dps_task.getValue(pinList[i]);

            if(debugMode == 2 ) {
                cout <<  pinList[i] << " - " << limitSpecifier << "\t\t"
                     << "( " << limitLow << " <\t" << result << "\t< " << limitHigh << " )" << endl;
            }
            TESTSET().cont(true).judgeAndLog_ParametricTest(pinList[i],"",limitIDD,result);
        } // pin loop

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
REGISTER_TESTMETHOD("StaticDC.IDD", StaticDC_IDD);
