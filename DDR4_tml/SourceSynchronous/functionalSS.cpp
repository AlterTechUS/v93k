#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;

class functionalSS: public testmethod::TestMethod {

protected:
    string ssClockPins;
    string ssDataPins;
    int rtsaEnable;

    virtual void initialize()
    {
      addParameter("SourceSyncClockPins",
                   "PinString",
                   &ssClockPins)
        .setDefault("DQS")
        .setComment("the source synchronous clock");
      addParameter("SourceSyncDataPins",
                   "PinString",
                   &ssDataPins)
        .setDefault("DQ_Bus")
        .setComment("the source synchronous data pins");
      addParameter("enable RTSA",
                   "int",
                   &rtsaEnable)
        .setOptions("1:0")
        .setComment("define usage of RTSA ");
    }

     virtual void run()
     {
         ON_FIRST_INVOCATION_BEGIN();

             const string trackPins = ssClockPins + "," + ssDataPins;

             DISCONNECT();
             TRANSITION_TEST_SET tracker("Tracker");
             if(rtsaEnable ==1) {
                 tracker.pin(trackPins).trackingMode(TRANSITION_TEST::ACTIVE);
             }
             else {
                 tracker.pin(trackPins).trackingMode(TRANSITION_TEST::FREEZE_ALWAYS);
             }
             tracker.pin(trackPins).firstInitialSyncMode(false).subsequentInitialSyncMode(false);

              //RTSA required initial sync offset: 16UI shift to right
              const double rtsaShift_ns = Primary.getSpecification().getSpecValue("tCK")*8;
               const string syncSpec = "rtsa_initial_sync@PortAC";
              Primary.getSpecification().change(syncSpec,rtsaShift_ns);
              FLUSH();

              CLOCKFORWARDING_SET clockForward("DDR3");
              // setup source-sync domain incl. clocking information
              clockForward.source(ssClockPins).sink(ssDataPins);


             tracker.setAsPrimary();
             clockForward.setAsPrimary();

             CONNECT();
             FUNCTIONAL_TEST();

             TRANSITION_TEST_SET::setDefaultAsPrimary();
             Primary.getSpecification().restore();
             FLUSH();

         ON_FIRST_INVOCATION_END();

         TESTSET().judgeAndLog_FunctionalTest();

         return;
     }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
      //Add your code
      return;
    }
};
REGISTER_TESTMETHOD("functionalSS", functionalSS);
