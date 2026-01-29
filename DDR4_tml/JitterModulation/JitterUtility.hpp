#ifndef _JITTERUTILITY_HPP_
#define _JITTERUTILITY_HPP_

using namespace std;


class JitterUtility
{
public:
	static double getJitterSampleInsertionPeriod_ps()
	{
		/*
		 *  single port
		 *
		int prmEQ = Primary.getTimingSpec().getEquation();
		int prmSpec = Primary.getTimingSpec().getSpec();
		stringstream eclkQuery;
		eclkQuery << "CLKR? EXACT," << prmEQ*100 + prmSpec;
		*/

		/*
		 * multi-port
		 */
		string prm = Primary.getSpecification().getName();
		stringstream eclkQuery;
		eclkQuery << "CLKR? EXACT,\"" << prm << "\",(PortAC)";

		string answer;
		BEGIN_EXTENSION();
			FW_TASK(eclkQuery.str(),answer);
		END_EXTENSION();

		/* answer:  CLKR spec,ref_period,ECLK */
		string::size_type eclkStart = answer.find_first_of(",",answer.find_first_of(",",0)+1)+1;
		string::size_type eclkStop = answer.find_last_of(",",answer.length());
		string eclk = answer.substr(eclkStart,(eclkStop-eclkStart));
		return (2 * atof(eclk.data()))*1000;
	};
};


class JitterDebug
{
public: 
	static void printJitterSampleInsertionPeriod()
	{
		int debugMode;
		GET_TESTFLOW_FLAG("debug_mode",&debugMode);
		if(debugMode == 2) {
			double jitterSamplePeriod = JitterUtility::getJitterSampleInsertionPeriod_ps();
			double period_res = jitterSamplePeriod/2;

			cerr << "DEBUG: period resolution [ps]:             " << period_res << endl;
			cerr << "DEBUG: JitterSample insertion period [ps]: " << jitterSamplePeriod << endl;
		}
	};

	static void printJitterWaveform(string jitterSet)
	{
		int debugMode;
		GET_TESTFLOW_FLAG("debug_mode",&debugMode);
		if(debugMode == 2) {
			double jitterSamplePeriod = JitterUtility::getJitterSampleInsertionPeriod_ps();
			double period_res = jitterSamplePeriod/2;

			stringstream task;
			string answer;
			task << "jswf? \"" << jitterSet << "\"," << period_res <<  ",(@)";
			BEGIN_EXTENSION();
		  	  FW_TASK(task.str() ,answer);
		  	END_EXTENSION();

		  	cerr << "DEBUG: programmed jitter waveform        : " <<  endl;
		  	cerr << answer << endl;
		 }
	};
};

#endif /*_JITTERUTILITY_HPP_*/
