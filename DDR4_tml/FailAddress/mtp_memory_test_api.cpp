//for testmethod framework interfaces
#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"

#include "MtpTest.hpp"

namespace mtp_memory_test_api_namespace{
}
using namespace mtp_memory_test_api_namespace;


//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class mtp_memory_test_api_fail_address_ordered_by_addr: public testmethod::TestMethod {

    protected:
    string sNotation;
    
    virtual void initialize()
    {
        addParameter("sNotation", "string", &sNotation).setDefault("dec").setOptions("dec:hex");
    }
    
    virtual void run()
    {

        int iNumOfRetrievedFail = 1024;
        
        MTP_MEMORY_TEST memoryTest;

        ON_FIRST_INVOCATION_BEGIN();

            //==================================================================
            // Setup MTP_MEMORY_TEST API parameters
            //==================================================================
            memoryTest.enableAddressBasedResults(true).setImmediateBitmapToFile(true);
            memoryTest.enableFailBitCount(true);
            memoryTest.setNumberOfFailCyclesToAcquirePerPin(iNumOfRetrievedFail);
            memoryTest.setMaxFailingAddressesToReconstructPerPin(iNumOfRetrievedFail);
            //memoryTest.setFBCLimit(MTP::FBC_DEVICE,100).setFBCLimit(MTP::FBC_DBIT,10).setFBCLimit(MTP::FBC_BANK,10);
            
            //memoryTest.setBitmapMode(MTP::ASCII_ENCODING);
            memoryTest.setBitmapMode(MTP::BINARY_ENCODING);
            
            memoryTest.setFailListStyle(MTP::PASS_FAIL_ONLY);
            //memoryTest.setFailListStyle(MTP::FAIL_FOR_0_1);
            
            //memoryTest.setFailAddressListOrdering(MTP::FAIL_CYCLE);
            memoryTest.setFailAddressListOrdering(MTP::PHYSICAL_ADDRESS);

            //==================================================================
            // Execute memory pattern
            //==================================================================
            CONNECT();
            memoryTest.execute();

        ON_FIRST_INVOCATION_END();

        vector<string> sPatternArray;
        int iNumOfPattern = 0;
        iNumOfPattern = memoryTest.getMemoryPatternNamesList(sPatternArray);

        for(int pat_loop=0; pat_loop<iNumOfPattern; pat_loop++){
            
            cout << "Memory Pattern Name: " << sPatternArray[pat_loop] << endl;
            MTP_MEMORY_PATTERN& mmp = memoryTest.getMemoryPattern(sPatternArray[pat_loop]); 

            //==================================================================
            // Log fail address into report window
            //==================================================================
            for(int bit_loop=0; bit_loop<8; bit_loop++){
                ARRAY_ADDRESSES addressList;
                LONG nr_addresses;

                nr_addresses = mmp.getFailingAddressList(bit_loop,iNumOfRetrievedFail,addressList);

                for(int addr_loop=0; addr_loop<nr_addresses; addr_loop++){
                    MsgStream msg;
                    if(sNotation == "dec"){
                        msg << "Site"   << left << setw(10) << dec << CURRENT_SITE_NUMBER() 
                            << " x="    << left << setw(10) << dec << addressList[addr_loop].x
                            << " y="    << left << setw(10) << dec << addressList[addr_loop].y
                            << " z="    << left << setw(10) << dec << addressList[addr_loop].z
                            << " d="    << left << setw(10) << dec << addressList[addr_loop].d
                            << " expD=" << left << setw(10) << dec << addressList[addr_loop].expD << endl;
                    } else{
                        msg << "Site"   << left << setw(10) << dec << CURRENT_SITE_NUMBER() 
                            << " x=0x"  << left << setw(10) << hex << addressList[addr_loop].x
                            << " y=0x"  << left << setw(10) << hex << addressList[addr_loop].y
                            << " z=0x"  << left << setw(10) << hex << addressList[addr_loop].z
                            << " d="    << left << setw(10) << dec << addressList[addr_loop].d
                            << " expD=" << left << setw(10) << dec << addressList[addr_loop].expD << endl;
                    }
                    cout << msg.c_str();
                    //PUT_DATALOG(msg.c_str());
                }
            }

            //==================================================================
            // Log fail bit count into report window
            //==================================================================
            LONG FBC = mmp.getFailBitCount(MTP::FBC_DEVICE,0);
            cout << "Site" << CURRENT_SITE_NUMBER() << " FailBitCount=" << FBC << endl;

            //==================================================================
            // Define .bdf file name and Generate .bfd file for BitMapViewer
            //==================================================================
            if(FBC > 0){ 
                char   cNowTime[100];
                time_t timer = time(NULL);
                struct tm *tm;
                tm = localtime(&timer);
                strftime(cNowTime,sizeof(cNowTime),"%Y-%m-%d_%H-%M-%S",tm);

                char cDevDir[1000];
                GetDevPath(cDevDir);

                stringstream strSite;
                strSite << CURRENT_SITE_NUMBER();
                string sCurrentPattern = sPatternArray[pat_loop];
                string sFileName = string(cNowTime) + "_" + sCurrentPattern + "_Site" + strSite.str().c_str();
                string sFilePath = string(cDevDir) + "/mts/bitmap/bitmap_data/";

                mmp.setBitMapFilePath(sFilePath);
                mmp.addBitMapFileUserComments("API Bitmap");
                mmp.setBitMapFileName(sFileName);
                mmp.generateBitmapFile();
                cout << "Message: " << sFileName << ".bfd file was generated under " << sFilePath << "." << endl;
            }

        }
        
        TEST(memoryTest); 
        
        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("mtp_memory_test_api.fail_address_ordered_by_addr", mtp_memory_test_api_fail_address_ordered_by_addr);


//###-########################################################################################################
//############################################################################################################
//############################################################################################################
class mtp_memory_test_api_fail_address_ordered_by_failcycle: public testmethod::TestMethod {

    protected:
    string sNotation;
    
    virtual void initialize()
    {
        addParameter("sNotation", "string", &sNotation).setDefault("dec").setOptions("dec:hex");
    }
    
    virtual void run()
    {

        int iNumOfRetrievedFail = 1024;
        
        MTP_MEMORY_TEST memoryTest;

        ON_FIRST_INVOCATION_BEGIN();

            //==================================================================
            // Setup MTP_MEMORY_TEST API parameters
            //==================================================================
            memoryTest.enableAddressBasedResults(true).setImmediateBitmapToFile(true);
            memoryTest.enableFailBitCount(true);
            memoryTest.setNumberOfFailCyclesToAcquirePerPin(iNumOfRetrievedFail);
            memoryTest.setMaxFailingAddressesToReconstructPerPin(iNumOfRetrievedFail);
            //memoryTest.setFBCLimit(MTP::FBC_DEVICE,100).setFBCLimit(MTP::FBC_DBIT,10).setFBCLimit(MTP::FBC_BANK,10);
            
            //memoryTest.setBitmapMode(MTP::ASCII_ENCODING);
            memoryTest.setBitmapMode(MTP::BINARY_ENCODING);
            
            memoryTest.setFailListStyle(MTP::PASS_FAIL_ONLY);
            //memoryTest.setFailListStyle(MTP::FAIL_FOR_0_1);
            
            memoryTest.setFailAddressListOrdering(MTP::FAIL_CYCLE);
            //memoryTest.setFailAddressListOrdering(MTP::PHYSICAL_ADDRESS);

            //==================================================================
            // Execute memory pattern
            //==================================================================
            CONNECT();
            memoryTest.execute();

        ON_FIRST_INVOCATION_END();

        vector<string> sPatternArray;
        int iNumOfPattern = 0;
        iNumOfPattern = memoryTest.getMemoryPatternNamesList(sPatternArray);

        for(int pat_loop=0; pat_loop<iNumOfPattern; pat_loop++){
            
            cout << "Memory Pattern Name: " << sPatternArray[pat_loop] << endl;
            MTP_MEMORY_PATTERN& mmp = memoryTest.getMemoryPattern(sPatternArray[pat_loop]); 

            //==================================================================
            // Log fail address into report window
            //==================================================================
            DeviceCycleToAddrWordMap cycleToAddrWordMap;
            long lNumOfAddrWord = mmp.getFailingAddressList(iNumOfRetrievedFail, cycleToAddrWordMap);
            for (DeviceCycleToAddrWordMapIt wordIt = cycleToAddrWordMap.begin(); wordIt != cycleToAddrWordMap.end(); wordIt++){

                MsgStream msg;
                    if(sNotation == "dec"){
                        msg << "Site"       << left << setw(10) << dec << CURRENT_SITE_NUMBER()
                            << " cycle="    << left << setw(10) << dec << wordIt->first
                            << " x="        << left << setw(10) << dec << wordIt->second.x
                            << " y="        << left << setw(10) << dec << wordIt->second.y
                            << " z="        << left << setw(10) << dec << wordIt->second.z
                            << " dataword=" << left << setw(10) << dec << wordIt->second.dataWord
                            << " expword="  << left << setw(10) << dec << wordIt->second.expWord << endl;
                    } else{
                        msg << "Site"       << left << setw(10) << dec << CURRENT_SITE_NUMBER()
                            << " cycle="    << left << setw(10) << dec << wordIt->first
                            << " x=0x"      << left << setw(10) << hex << wordIt->second.x
                            << " y=0x"      << left << setw(10) << hex << wordIt->second.y
                            << " z=0x"      << left << setw(10) << hex << wordIt->second.z
                            << " dataword=" << left << setw(10) << dec << wordIt->second.dataWord
                            << " expword="  << left << setw(10) << dec << wordIt->second.expWord << endl;
                    }
                    cout << msg.c_str();
                    //PUT_DATALOG(msg.c_str());
            }

            cout << "Address word count is " << lNumOfAddrWord << endl;
  
            //==================================================================
            // Log fail bit count into report window
            //==================================================================
            LONG FBC = mmp.getFailBitCount(MTP::FBC_DEVICE,0);
            cout << "Site" << CURRENT_SITE_NUMBER() << " FailBitCount=" << FBC << endl;

            //==================================================================
            // Define .bdf file name and Generate .bfd file for BitMapViewer
            //==================================================================
            if(FBC > 0){ 
                char   cNowTime[100];
                time_t timer = time(NULL);
                struct tm *tm;
                tm = localtime(&timer);
                strftime(cNowTime,sizeof(cNowTime),"%Y-%m-%d_%H-%M-%S",tm);

                char cDevDir[1000];
                GetDevPath(cDevDir);

                stringstream strSite;
                strSite << CURRENT_SITE_NUMBER();
                string sCurrentPattern = sPatternArray[pat_loop];
                string sFileName = string(cNowTime) + "_" + sCurrentPattern + "_Site" + strSite.str().c_str();
                string sFilePath = string(cDevDir) + "/mts/bitmap/bitmap_data/";

                mmp.setBitMapFilePath(sFilePath);
                mmp.addBitMapFileUserComments("API Bitmap");
                mmp.setBitMapFileName(sFileName);
                mmp.generateBitmapFile();
                cout << "Message: " << sFileName << ".bfd file was generated under " << sFilePath << "." << endl;
            }

        }

        TEST(memoryTest); 
        
        return;
    }

    virtual void postParameterChange(const string& parameterIdentifier)
    {
        return;
    }

    virtual const string getComment() const
    {
        string comment = " please add your comment for this method.";
        return comment;
    }
};
REGISTER_TESTMETHOD("mtp_memory_test_api.fail_address_ordered_by_failcycle", mtp_memory_test_api_fail_address_ordered_by_failcycle);
