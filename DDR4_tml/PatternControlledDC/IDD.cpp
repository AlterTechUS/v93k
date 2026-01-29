#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
using namespace V93kLimits;

//###=##########################################################################################################################
//##############################################################################################################################
//##############################################################################################################################
class IDD: public testmethod::TestMethod {
    protected:
    string iddPins;
    string limitSpecifier;

    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("measurePins",    "PinString", &iddPins,        testmethod::TM_PARAMETER_INPUT).setDefault("VDD");
        addParameter("limitSpecifier", "string",    &limitSpecifier, testmethod::TM_PARAMETER_INPUT).setDefault("").setComment("as specified in \"Test name\" column of Limit TestTable");
    }

    virtual void run()
    {
        static int isOffline;
        static int debugMode;
        static LIMIT limitIDD;
        static double limitLow, limitHigh;
        static bool run_once = true;

        DC_RESULT_ACCESSOR allResult;

        // DC_EVENTS need to be specified in the corresponding vector label --> MTL!
        ON_FIRST_INVOCATION_BEGIN();

            //================================================================================
            // Read out testtable limits
            //================================================================================
            TMLimits& limits = TMLimits::getInstance();
            if(run_once) {
            // testtable file is spefified in the testflow //   limits.load("TMLimits.csv");
                GET_SYSTEM_FLAG("offline",&isOffline);
                run_once=false;
            } //run_once
            GET_TESTFLOW_FLAG("debug_mode",&debugMode);
            limitIDD = limits.getLimitObj(limitSpecifier);
            limitIDD.getLow(&limitLow);
            limitIDD.getHigh(&limitHigh);

            CONNECT();
            FUNCTIONAL_TEST();
            
            //================================================================================
            // Upload results and put to datalog
            //================================================================================
            const TM::RESULT_RANGE range(allResult.uploadResult(iddPins, TM::RESULT_INDEX));
            
        ON_FIRST_INVOCATION_END();

        STRING_VECTOR pinList = PinUtility.getDigitalPinNamesFromPinList(iddPins,TM::ALL_DIGITAL);

        if(! isOffline) { //online case
            // for each pin
            for(unsigned int i=0; i<pinList.size(); i++){

                // only one result point available --> refer to number of DC_EVENT in MTL program file
                const RESULT_POINT first(allResult.getFirstResultPoint(pinList[i]));
                DOUBLE iddValue = allResult.getValue(pinList[i],first)*1000; //[mA]

                if(debugMode == 2) {
                    cout << "DPS-Pin: " <<  pinList[i] << " - " << limitSpecifier << " [mA]\t\t"
                         << "( " << limitLow << " <\t" << iddValue << "\t< " << limitHigh << " )" << endl;
                }

                TESTSET().cont(true).judgeAndLog_ParametricTest(pinList[i],"",limitIDD,iddValue);
            } // foreach pin
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
REGISTER_TESTMETHOD("PatternControlledDC.IDD", IDD);

//###=##########################################################################################################################
//##############################################################################################################################
//##############################################################################################################################
class IDD_profiling: public testmethod::TestMethod {
    protected:
    string iddPins;
    double range_mA;
    double interval_ms;
    int    oversampling;

    virtual void initialize()
    {
        //Add your initialization code here
        addParameter("measurePins",  "PinString", &iddPins,      testmethod::TM_PARAMETER_INPUT).setDefault("VDD");
        addParameter("range_mA",     "double",    &range_mA,     testmethod::TM_PARAMETER_INPUT);
        addParameter("interval_ms",  "double",    &interval_ms,  testmethod::TM_PARAMETER_INPUT);
        addParameter("oversampling", "int",       &oversampling, testmethod::TM_PARAMETER_INPUT);
    }

    virtual void run()
    {
        static int isOffline;
        static int debugMode;

        ON_FIRST_INVOCATION_BEGIN();

            GET_SYSTEM_FLAG("offline",&isOffline);
            GET_TESTFLOW_FLAG("debug_mode",&debugMode);
            
            CONNECT();
            
            DC_PROFILING_ON(iddPins, TM::CURRENT, range_mA mA, interval_ms ms, oversampling, TM::RESTORE_RANGE);
            
            FUNCTIONAL_TEST();
            
            DC_PROFILING_OFF(iddPins);
            
        ON_FIRST_INVOCATION_END();

        STRING_VECTOR pinList = PinUtility.getDigitalPinNamesFromPinList(iddPins,TM::ALL_DIGITAL);

        DC_RESULT_ACCESSOR ra;
        ra.uploadResult(iddPins, TM::RESULT_INDEX);
        
        if(! isOffline) {
            for(unsigned int i=0; i<pinList.size(); i++){

                ARRAY_D dCurrentProfile = ra.getValues(pinList[i]);
                cout << "Current profiling values on pin " << pinList[i] << " : " << dCurrentProfile << endl;

                //TEST(TRUE);
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
REGISTER_TESTMETHOD("PatternControlledDC.IDD_profiling", IDD_profiling);
