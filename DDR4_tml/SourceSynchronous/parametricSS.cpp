#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
using namespace std;
using namespace V93kLimits;

class parametricSS: public testmethod::TestMethod {
protected:
    string ssClockPins;
    string ssDataPins;
    string ssSpec;
    double startOffset_ps;
    int linearSteps;
    double linearStepWidth_ps;
    double binaryResolution_ps;
    string limitSpecifier;

protected:
  virtual void initialize()
  {
      addParameter("SourceSyncClockPins",
                   "PinString",
                   &ssClockPins)
        .setComment("the source synchronous clock");
      addParameter("SourceSyncDataPins",
                   "PinString",
                   &ssDataPins)
        .setComment("the source synchronous data pins");
      addParameter("SourceSyncSpec",
                   "string",
                   &ssSpec)
        .setOptions("tDQSQ:tQH")
        .setComment("Specification to test");
      addParameter("startOffset_ps",
                   "double",
                   &startOffset_ps)
        .setComment("search start condition [ps]: this is an offset to defined receive edge (r1) timing definition");
      addParameter("linearStepWidth_ps",
                   "double",
                   &linearStepWidth_ps)
        .setComment("stepwidth of linear search. Rule of thumb: UI/4");
      addParameter("linearSteps",
                   "int",
                   &linearSteps)
        .setComment("number of steps for linear search. Rule of thumb: UI/linearStepWidth_ps");
      addParameter("binaryResolution_ps",
                   "double",
                   &binaryResolution_ps)
        .setDefault("10")
        .setComment("binary search resolution [ps]");
       addParameter("limitSpecifier",
                     "string",
                     &limitSpecifier)
          .setDefault("")
          .setComment("as specified in \"Test name\" column of Limit TestTable");


      //Add your initialization code here
  }

  virtual void run()
  {
    static int isOffline;
    static int debugMode;
    TIMING_OFFSET_SEARCH_TASK embTask;
    TASK_LIST controller;
    static LIMIT ssLimit;
    static double tCK_ps;
    static bool run_once = true;


    /* do a search for tDQSQ
     * DoubleStrobe setting
     * - will quarantee max tDQSCK jitter for source pins
     * - will quarantee tDQSQ and tQH for sink pins
     */

    ON_FIRST_INVOCATION_BEGIN();
        /*
         * read out testtable limits
         */
        TMLimits& limits = TMLimits::getInstance();
        if(run_once) {
        // testtable file is spefified in the testflow //    limits.load("TMLimits.csv");
            GET_SYSTEM_FLAG("offline",&isOffline);

            tCK_ps = Primary.getSpecification().getSpecValue("tCK")*1E+3;
            run_once=false;
        } //run_once
        GET_TESTFLOW_FLAG("debug_mode",&debugMode);
        ssLimit = limits.getLimitObj(limitSpecifier);

        /*
         * this is a linear-binary search
         * For best performance the linear search steps should be minimized.
         * Linear search parameters: numSteps , stepwidth
         * Known good setup is to specify max 4 steps per UI, i.e. 1.6Gb --> 625ps UI --> setup: 4 steps with 150ps stepwidth
         * Binary search parameters: resolution
         * For performance reasons resolution should not be specified smaller than 10ps
         */
        embTask.pin(ssClockPins).edgeType(TIMING_OFFSET_SEARCH::RECEIVE).searchType(TIMING_OFFSET_SEARCH::EYE).start(startOffset_ps/1E+12).numSteps(linearSteps).stepWidth(linearStepWidth_ps/1E+12).resolution(binaryResolution_ps/1E+12);
        embTask.pin(ssDataPins).edgeType(TIMING_OFFSET_SEARCH::RECEIVE).searchType(TIMING_OFFSET_SEARCH::EYE).start(startOffset_ps/1E+12).numSteps(linearSteps).stepWidth(linearStepWidth_ps/1E+12).resolution(binaryResolution_ps/1E+12);
        controller.add(embTask).setup();

        CONNECT();
        controller.execute();
      ON_FIRST_INVOCATION_END();

     // setup timingset for uploading embedded values; use current PRM
     TIMING_OFFSET_SEARCH::TimingSetSelector dest(TIMING_OFFSET_SEARCH::PRM,TIMING_OFFSET_SEARCH::PRM);
     embTask.pin(ssClockPins).uploadResult(dest,TIMING_OFFSET_SEARCH::RECEIVE);
     embTask.pin(ssDataPins).uploadResult(dest,TIMING_OFFSET_SEARCH::RECEIVE);

     if(! isOffline) { //online case
         double clockTransition, dataTransition, specLimit;
         double leftCK, rightCK, clockTransitionAverage;
         string specCompare;

         if(ssSpec == "tDQSQ"){
             clockTransition = 0;
             dataTransition = tCK_ps/2;

             if(embTask.pin(ssClockPins).TransitionOffsetFound() != TIMING_OFFSET_SEARCH::NOT_FOUND){
                 clockTransition = embTask.pin(ssClockPins).getLatestLBTransitionOffset()*1E+12;
             }
             if(embTask.pin(ssDataPins).TransitionOffsetFound() != TIMING_OFFSET_SEARCH::NOT_FOUND){
                 dataTransition = embTask.pin(ssDataPins).getLatestLBTransitionOffset()*1E+12;
             }
             ssLimit.getHigh(&specLimit);
             specCompare = "< ";
         }
         else if (ssSpec == "tQH"){
             clockTransitionAverage = 0;
             dataTransition = 0;

             if(embTask.pin(ssClockPins).TransitionOffsetFound() != TIMING_OFFSET_SEARCH::NOT_FOUND){
                 leftCK = embTask.pin(ssClockPins).getLatestLBTransitionOffset()*1E+12;
                 rightCK = embTask.pin(ssClockPins).getLatestRBTransitionOffset()*1E+12;
                 clockTransitionAverage = (625- (abs(leftCK)+rightCK))/2;
             }
             clockTransition = leftCK - clockTransitionAverage;

             if(embTask.pin(ssDataPins).TransitionOffsetFound() != TIMING_OFFSET_SEARCH::NOT_FOUND){
                 dataTransition = embTask.pin(ssDataPins).getLatestRBTransitionOffset()*1E+12;
             }
             ssLimit.getLow(&specLimit);
             specCompare = "> ";
         }
         else {
             cerr << "Unknown source synchronous spec parameter " << ssSpec << endl;
             TEST(false);
             return;
         }
         double result   = dataTransition - clockTransition;

         if(debugMode == 2) {
             cout << "\nSourceSynchronous Parametric Search Results\n";
             cout << "-------------------------------------------\n";
             cout << ssClockPins << "," << ssDataPins << " - " << limitSpecifier << " [ps]: "
                  << result << "\t( " << specCompare << specLimit << " )" << endl;
         }

         TESTSET().judgeAndLog_ParametricTest(ssDataPins,"limitSpecifier",ssLimit,result);
     }

    return;
  }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        //Add your code
        return;
    }
};

REGISTER_TESTMETHOD("parametricSS", parametricSS);
