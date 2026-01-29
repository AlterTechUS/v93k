#include "testmethod.hpp"

//for testmethod API interfaces
#include "mapi.hpp"
using namespace std;

#include "../include/FW.h"
#include "JitterUtility.hpp"

class CycleCycleJitter: public testmethod::TestMethod {
protected:
    string pins;
    int ppNoise_ps;
     
protected:
  virtual void initialize()
  {
    //Add your initialization code here
       addParameter("pins",
                    "PinString",
                    &pins,
                    testmethod::TM_PARAMETER_INPUT)
      .setDefault("CK,CKn")
      .setComment("pins that get jitter injected");
       addParameter("ppNoise_ps",
                    "int",
                    &ppNoise_ps,
                    testmethod::TM_PARAMETER_INPUT)
      .setDefault("0")
      .setComment("max. jitter amplitude [ps]");
  }

  virtual void run()
  {

    string prm = Primary.getSpecification().getName();
    cout << " prm= " <<  prm << endl;
    fwout << "CLKR? EXACT,\"" << prm << "\",(PortAC)" << endl;
    double eclk = atof(fwresult[0][2].c_str()) ;
    cout << "eclk = " << eclk << endl;

    if ( eclk < 1.0 && ppNoise_ps > 0 || eclk >=1.0 && ppNoise_ps > 100 ){
        cout << "eclk out of range" << endl;
        TEST("","",TM::Fail,TRUE); return; 
    }

    //FW_TASK("CLKR? EXACT,\"FCT_1600_AAA\"\n",sAns);
    //FW_TASK("CLKR? EXACT,\"FCT_1066_AAA\"\n",sAns);
    //    cout << "sonodadesu = " << sAns << endl;

      ON_FIRST_INVOCATION_BEGIN();
          JITTER_INJECTION_SET ccJitter("tJITcc");
          
          ccJitter.pin(pins)
              .setMode(JITTER_INJECTION::RESTART)
              // dummy shape for random noise insertion
              // --> setup period in order to fill-up all 512 samples
              .setWaveFormSinus(ppNoise_ps / 1E+12, (512*JitterUtility::getJitterSampleInsertionPeriod_ps()) / 1E+12)
              .setPhase(0)
              .setNoise(ppNoise_ps / 1E+12); //srand()

          // EXECUTE
          // activate jitter item  -  FTST?  -  disable jitter injection
          ccJitter.setAsPrimary();
          CONNECT();
          FUNCTIONAL_TEST();
           
          cerr << "\n\nInjected tJITcc: " << ppNoise_ps << "[ps]" << endl;
          // print-out for debugMode=2
          //JitterDebug::printJitterSampleInsertionPeriod();
          JitterDebug::printJitterWaveform("tJITcc");
       
          JITTER_INJECTION_SET::deselectGlobally();
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
REGISTER_TESTMETHOD("JitterInjection.CycleCycleJitter", CycleCycleJitter);
