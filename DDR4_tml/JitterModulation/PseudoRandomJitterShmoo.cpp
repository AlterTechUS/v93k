#include "testmethod.hpp"

//for testmethod API interfaces
#include "mapi.hpp"
using namespace std;

#include "JitterUtility.hpp"
#include "../include/FW.h"

class PseudoRandomJitterShmoo: public testmethod::TestMethod {
protected:
    string pins;
    int  period_ns;
    int  ppJitter_ps;
    int  stepsize_ps;

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
    addParameter("period_ns",
                 "int",
                 &period_ns,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("100")
      .setComment("jitter sine waveform period [ps]");
    addParameter("ppJitter_ps",
                 "int",
                 &ppJitter_ps,
                 testmethod::TM_PARAMETER_INPUT)
     .setDefault("100")
     .setComment("max. jitter amplitude [ps]");
    addParameter("stepsize_ps",
                 "int",
                 &stepsize_ps,
                 testmethod::TM_PARAMETER_INPUT)
      .setDefault("10")
      .setComment("increase of ppJitter [ps]");
  }

  virtual void run()
  {
        JITTER_INJECTION_SET shmoo("pseudoRandom");

        // use predefined sine shape with amplitude variation (amplitude == max jitter)
        shmoo.pin(pins)
            .setMode(JITTER_INJECTION::RESTART)
            .setPhase(0)
            .setNoise(0);
     
        // EXECUTE
        // activate jitter item  -  FTST?  -  disable jitter injection
        shmoo.setAsPrimary();
        CONNECT();
        cerr << "\n\n*** sinusoidal jitter injection: pp-range = "<< ppJitter_ps << "ps\tstepsize = " << stepsize_ps <<"ps ***" << endl;
             
        string prm = Primary.getSpecification().getName();
        cout << " prm= " <<  prm << endl;
        fwout << "CLKR? EXACT,\"" << prm << "\",(PortAC)" << endl;
        double eclk = atof(fwresult[0][2].c_str()) ;
        cout << "eclk = " << eclk << endl;
    
        if ( eclk < 1.0 && ppJitter_ps > 0 || eclk >=1.0 && ppJitter_ps > 100 ){
            cout << "eclk out of range" << endl;
            TEST("","",TM::Fail,TRUE); return;
        }



        bool allPassed = true;
        for(int maxJitter_ps=0; maxJitter_ps<=ppJitter_ps; maxJitter_ps+=stepsize_ps) {
            shmoo.pin(pins).setWaveFormSinus(maxJitter_ps / 1E+12, period_ns / 1E+9);

           cout << "sakai " << maxJitter_ps/1E+12<< " " << period_ns / 1E+9 << endl;
            
            FUNCTIONAL_TEST();
            string result = "FAIL";
            if(GET_FUNCTIONAL_RESULT()){ 
                result = "PASS"; 
            } else { 
                result = "FAIL";
                allPassed=false;
            };
            cerr << "\n\n - injected ppJitter: " << maxJitter_ps << " [ps]   :   TestResult: " << result << endl;

            // print-out for debugMode=2
            //JitterDebug::printJitterSampleInsertionPeriod();
            JitterDebug::printJitterWaveform("pseudoRandom");

        }
        
        JITTER_INJECTION_SET::deselectGlobally();
        
        if(allPassed){
            TEST(true);
        } else {
            TEST(false);
        }
        return;
  }

  virtual void postParameterChange(const string& parameterIdentifier)
  {
    //Add your code
    return;
  }
};
REGISTER_TESTMETHOD("JitterInjection.PseudoRandomJitterShmoo", PseudoRandomJitterShmoo);
