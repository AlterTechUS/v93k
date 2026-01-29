#include "User.h"

namespace User_namesace{
    int iDebug = 0;
}
using namespace User_namesace;

//====================================================================================================================//
//===== Convert number (int, double, float,...) to string ============================================================//
//====================================================================================================================//
string int2str( const int n ){
    stringstream str;
    str << n;
    return str.str();
}
string double2str( const double n ){
    stringstream str;
    str << n;
    return str.str();
}
string float2str( const double n ){
    stringstream str;
    str << n;
    return str.str();
}

string double2str_decimalplace( const double n, const int iDecimalPlace, const int iSetw){
    stringstream str;
    str << right << setw(iSetw) << fixed << setprecision(iDecimalPlace) << n;
    return str.str();
}

int str2int_bin_dec_hex( string str){
    int n;
    if     (str.find("0x") != string::npos) { replace_str(str,"0x",""); n = strtol(str.c_str(), NULL, 16); }
    else if(str.find("0b") != string::npos) { replace_str(str,"0b",""); n = strtol(str.c_str(), NULL, 2);  }
    else                                    {                           n = atoi(str.c_str());             }
    return n;
}

//====================================================================================================================//
//===== Replace string  ==============================================================================================//
//====================================================================================================================//
void replace_str(string& str, const string sb, const string sa){
    string::size_type n, nb = 0;
    
    while((n = str.find(sb,nb)) != string::npos){
        str.replace(n,sb.size(),sa);
        nb = n + sa.size();
    }
}

//====================================================================================================================//
//===== Tokenize full template specialization (string) ===============================================================//
//====================================================================================================================//
void tokenize(const string& source, vector<string>& dest, const string& delimiter) {
    dest.clear();
    string::size_type lastPos = source.find_first_not_of(delimiter, 0);     //find first non delim char
    string::size_type     pos = source.find_first_of(delimiter,lastPos);    //find the next delim after token
    
    while (string::npos != pos || string::npos != lastPos)  {               //search for additional tokens
        dest.push_back(source.substr(lastPos, pos - lastPos));              //found token, save in vector
        lastPos = source.find_first_not_of(delimiter, pos);                 //find another non delim char
        pos = source.find_first_of(delimiter,lastPos);                      //find the next delim after token
    }
}

//====================================================================================================================//
//===== Extract all vector# based on "comment" only for one main label ===============================================//
//====================================================================================================================//
int get_vecnum_comment_search(const string& sLabel, const string& sWord){
    
    int iVecNum = -1;
    string            sReturnWord;
    string::size_type index;
    vector<FWString>  FWansTCMT;

    fwout << "TSTL \"" << sLabel << "\"," << endl;
    fwout << "TCMT?  0,100000000" << endl;
    FWansTCMT = fwresult;

    int iSizeOfFWansTCMT = FWansTCMT.size();
    for(int i=0; i<iSizeOfFWansTCMT; i++){
        sReturnWord = FWansTCMT[i][1];
        index = sReturnWord.find(sWord);
        if(index != string::npos){
            iVecNum = atoi(FWansTCMT[i][0].c_str());
            break;
        }
    }
    
    fwout << "TSTL \"\"," << endl;

    if(iVecNum == -1) cout << "ERROR: "   << left << setw(32) << sWord << " couldn't be found in " << setw(16) << sLabel << "!" << endl;
    if(iDebug)        cout << "Message: " << left << setw(32) << sWord << " was found in "         << setw(24) << sLabel << "at vectorNum=" << right << setw(10) << iVecNum << left << endl;

    return iVecNum;
}

//====================================================================================================================//
//===== Edit vector for MRS Operation Code ===========================================================================//
//====================================================================================================================//
void parity_computation(const int  iAct,        // ACTn
                        const int  iBank,       // BG, BA
                              int& iCode)       // PARITY, A17, A16, A13-A0
{
    
    int iMask;
    int iBank0, iBank1, iBank2, iBank3;
    int iAdd0, iAdd1, iAdd2, iAdd3, iAdd4, iAdd5, iAdd6, iAdd7, iAdd8, iAdd9, iAdd10, iAdd11, iAdd12, iAdd13, iAdd14, iAdd15, iAdd16, iAdd17;
    int iParity;
    
    iMask  = 0x1;
    iBank0 = (iBank & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iBank1 = (iBank & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iBank2 = (iBank & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iBank3 = (iBank & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    
    iMask  = 0x1;
    iAdd0  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd1  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd2  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd3  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd4  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd5  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd6  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd7  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd8  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd9  = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd10 = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd11 = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd12 = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd13 = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    iAdd14 =                         0x0;
    iAdd15 =                         0x0;
    iAdd16 =                         0x0;
    iAdd17 = (iCode & iMask) ? 0x1 : 0x0; iMask = iMask<<1;
    
    iParity = (iAct ^ iBank0 ^ iBank1 ^ iBank2 ^ iBank3 ^ iAdd0 ^ iAdd1 ^ iAdd2 ^ iAdd3 ^ iAdd4 ^ iAdd5 ^ iAdd6 ^ iAdd7 ^ iAdd8 ^ iAdd9 ^ iAdd10 ^ iAdd11 ^ iAdd12 ^ iAdd13 ^ iAdd14 ^ iAdd15 ^ iAdd16 ^ iAdd17);
    
    iCode = iCode | ((0x1&iParity)<<15);
    
    if(iDebug){
        cout << setw(16) << "code"
             << setw(6)  << "Act"
             << setw(6)  << "Bank3" << setw(6) << "Bank2" << setw(6) << "Bank1" << setw(6) << "Bank0"
             << setw(6)  << "Add17" << setw(6)<< "Add16" << setw(6)<< "Add15" << setw(6)<< "Add14" << setw(6)<< "Add13" << setw(6)<< "Add12" << setw(6)<< "Add11" << setw(6)<< "Add10" << setw(6)<< "Add9" << setw(6)<< "Add8" << setw(6)<< "Add7" << setw(6)<< "Add6" << setw(6)<< "Add5" << setw(6)<< "Add4" << setw(6)<< "Add3" << setw(6)<< "Add2" << setw(6)<< "Add1" << setw(6)<< "Add0"
             << setw(6)  << "Parity"
             << endl; 

        cout << setw(16) << hex << iCode
             << setw(6)  << iAct
             << setw(6)  << iBank3 << setw(6) << iBank2 << setw(6) << iBank1 << setw(6) << iBank0
             << setw(6)  << iAdd17 << setw(6)<< iAdd16 << setw(6)<< iAdd15 << setw(6)<< iAdd14 << setw(6)<< iAdd13 << setw(6)<< iAdd12 << setw(6)<< iAdd11 << setw(6)<< iAdd10 << setw(6)<< iAdd9 << setw(6)<< iAdd8 << setw(6)<< iAdd7 << setw(6)<< iAdd6 << setw(6)<< iAdd5 << setw(6)<< iAdd4 << setw(6)<< iAdd3 << setw(6)<< iAdd2 << setw(6)<< iAdd1 << setw(6)<< iAdd0
             << setw(6)  << iParity
             << dec << endl; 
    }
    
    return;
}

//====================================================================================================================//
//===== Edit vector for MRS Operation Code ===========================================================================//
//====================================================================================================================//
void vec_label_edit_A0_A17(const string& sLabel,
                           const vector<int>&    iVecNumArray,
                           const vector<string>& sDataArray,
                           const int iWF0,
                           const int iWF1)
{

    int iMask;
    int iSizeOfVecNumArray = iVecNumArray.size();
    
    if(iSizeOfVecNumArray != 0){
        
        VEC_LABEL_EDIT myLabel_00(sLabel, "A00");
        VEC_LABEL_EDIT myLabel_01(sLabel, "A01");
        VEC_LABEL_EDIT myLabel_02(sLabel, "A02");
        VEC_LABEL_EDIT myLabel_03(sLabel, "A03");
        VEC_LABEL_EDIT myLabel_04(sLabel, "A04");
        VEC_LABEL_EDIT myLabel_05(sLabel, "A05");
        VEC_LABEL_EDIT myLabel_06(sLabel, "A06");
        VEC_LABEL_EDIT myLabel_07(sLabel, "A07");
        VEC_LABEL_EDIT myLabel_08(sLabel, "A08");
        VEC_LABEL_EDIT myLabel_09(sLabel, "A09");
        VEC_LABEL_EDIT myLabel_10(sLabel, "A10");
        VEC_LABEL_EDIT myLabel_11(sLabel, "A11");
        VEC_LABEL_EDIT myLabel_12(sLabel, "A12");
        VEC_LABEL_EDIT myLabel_13(sLabel, "A13");
        VEC_LABEL_EDIT myLabel_17(sLabel, "A17");
        VEC_LABEL_EDIT myLabel_Pa(sLabel, "PARITY");

        VECTOR_DATA myVectorData_00[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_01[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_02[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_03[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_04[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_05[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_06[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_07[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_08[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_09[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_10[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_11[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_12[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_13[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_17[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_Pa[iSizeOfVecNumArray];

        for(int i=0; i<iSizeOfVecNumArray; i++){
            int iData;
            
            if(iDebug) cout << "Message: Vector edit for A0-A17 with data=" << left << setw(8) << sDataArray[i] << "at vectorNum=" << right << setw(8) << iVecNumArray[i] << " in " << left << sLabel << endl;
            
            string sData_dec = sDataArray[i];
            iData = str2int_bin_dec_hex(sData_dec);
            
            iMask = 0x01;
            myVectorData_00[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_00[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_01[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_01[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_02[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_02[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_03[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_03[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_04[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_04[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_05[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_05[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_06[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_06[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_07[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_07[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_08[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_08[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_09[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_09[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_10[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_10[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_11[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_11[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_12[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_12[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_13[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_13[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_17[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_17[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_Pa[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_Pa[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;

        }

        myLabel_00.downloadUserVectors(myVectorData_00, iSizeOfVecNumArray);
        myLabel_01.downloadUserVectors(myVectorData_01, iSizeOfVecNumArray);
        myLabel_02.downloadUserVectors(myVectorData_02, iSizeOfVecNumArray);
        myLabel_03.downloadUserVectors(myVectorData_03, iSizeOfVecNumArray);
        myLabel_04.downloadUserVectors(myVectorData_04, iSizeOfVecNumArray);
        myLabel_05.downloadUserVectors(myVectorData_05, iSizeOfVecNumArray);
        myLabel_06.downloadUserVectors(myVectorData_06, iSizeOfVecNumArray);
        myLabel_07.downloadUserVectors(myVectorData_07, iSizeOfVecNumArray);
        myLabel_08.downloadUserVectors(myVectorData_08, iSizeOfVecNumArray);
        myLabel_09.downloadUserVectors(myVectorData_09, iSizeOfVecNumArray);
        myLabel_10.downloadUserVectors(myVectorData_10, iSizeOfVecNumArray);
        myLabel_11.downloadUserVectors(myVectorData_11, iSizeOfVecNumArray);
        myLabel_12.downloadUserVectors(myVectorData_12, iSizeOfVecNumArray);
        myLabel_13.downloadUserVectors(myVectorData_13, iSizeOfVecNumArray);
        myLabel_17.downloadUserVectors(myVectorData_17, iSizeOfVecNumArray);
        myLabel_Pa.downloadUserVectors(myVectorData_Pa, iSizeOfVecNumArray);
    }
    
    return;
}

//====================================================================================================================//
//===== Edit vector for VrefDQ Code ===========================================================================//
//====================================================================================================================//
void vec_label_edit_A0_A5(const string& sLabel,
                          const vector<int>&    iVecNumArray,
                          const vector<string>& sDataArray,
                          const int iWF0,
                          const int iWF1)
{

    int iMask;
    int iSizeOfVecNumArray = iVecNumArray.size();
    
    if(iSizeOfVecNumArray != 0){
        
        VEC_LABEL_EDIT myLabel_00(sLabel, "A00");
        VEC_LABEL_EDIT myLabel_01(sLabel, "A01");
        VEC_LABEL_EDIT myLabel_02(sLabel, "A02");
        VEC_LABEL_EDIT myLabel_03(sLabel, "A03");
        VEC_LABEL_EDIT myLabel_04(sLabel, "A04");
        VEC_LABEL_EDIT myLabel_05(sLabel, "A05");

        VECTOR_DATA myVectorData_00[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_01[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_02[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_03[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_04[iSizeOfVecNumArray];
        VECTOR_DATA myVectorData_05[iSizeOfVecNumArray];

        for(int i=0; i<iSizeOfVecNumArray; i++){
            int iData;
            
            if(iDebug) cout << "Message: Vector edit for A0-A5 with data=" << left << setw(8) << sDataArray[i] << "at vectorNum=" << right << setw(8) << iVecNumArray[i] << " in " << left << sLabel << endl;
            
            string sData_dec = sDataArray[i];
            iData = str2int_bin_dec_hex(sData_dec);
            
            iMask = 0x01;
            myVectorData_00[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_00[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_01[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_01[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_02[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_02[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_03[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_03[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_04[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_04[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;
            myVectorData_05[i].phyWvfIndex = (iData & iMask)? iWF1: iWF0;  myVectorData_05[i].vectorNum = iVecNumArray[i]; iMask = iMask<<1;

        }

        myLabel_00.downloadUserVectors(myVectorData_00, iSizeOfVecNumArray);
        myLabel_01.downloadUserVectors(myVectorData_01, iSizeOfVecNumArray);
        myLabel_02.downloadUserVectors(myVectorData_02, iSizeOfVecNumArray);
        myLabel_03.downloadUserVectors(myVectorData_03, iSizeOfVecNumArray);
        myLabel_04.downloadUserVectors(myVectorData_04, iSizeOfVecNumArray);
        myLabel_05.downloadUserVectors(myVectorData_05, iSizeOfVecNumArray);
    }
    
    return;
}

//====================================================================================================================//
//===== Edit vector for VrefDQ Code Per DRAM =========================================================================//
//====================================================================================================================//
void vec_label_edit_DQ0(const string& sLabel,
                        const vector<int>& iVecNumArray,
                        const vector<int>& iPhyWvfIndexArray)
{

    int iSizeOfVecNumArray = iVecNumArray.size();
    
    if(iSizeOfVecNumArray != 0){
        
        VEC_LABEL_EDIT myLabel(sLabel, "DQ00");
        VECTOR_DATA myVectorData[iSizeOfVecNumArray];

        for(int i=0; i<iSizeOfVecNumArray; i++){
            if(iDebug) cout << "Message: Vector edit for DQ00 with PhyWvfIndex=" << left << setw(8) << iPhyWvfIndexArray[i] << "at vectorNum=" << right << setw(8) << iVecNumArray[i] << " in " << left << sLabel << endl;
            myVectorData[i].phyWvfIndex = iPhyWvfIndexArray[i];
            myVectorData[i].vectorNum   = iVecNumArray[i];
        }
        
        myLabel.downloadUserVectors(myVectorData, iSizeOfVecNumArray);
    }
    
    return;
}

//====================================================================================================================//
//===== Edit MRS Vectors =============================================================================================//
//====================================================================================================================//
void edit_mrs_vectors(const string& sType_Init_Label,
                      const int&    iFlag_MR0_Edit,
                      const int&    iFlag_MR1_Edit,
                      const int&    iFlag_MR2_Edit,
                      const int&    iFlag_MR3_Edit,
                      const int&    iFlag_MR4_Edit,
                      const int&    iFlag_MR5_Edit,
                      const int&    iFlag_MR6_Edit,
                      const string& sMR0_BurstLength,
                      const string& sMR0_ReadBurstType,
                      const string& sMR0_CASLatency,
                      const string& sMR0_DLL_Reset,
                      const string& sMR0_WriteRecovery,
                      const string& sMR1_DLL_Enable,
                      const string& sMR1_DriverImpedanceControl,
                      const string& sMR1_AdditiveLatency,
                      const string& sMR1_RTT_NOM,
                      const string& sMR1_TDQS_Enable,
                      const string& sMR2_CASWriteLatency,
                      const string& sMR2_RTT_WR,
                      const string& sMR2_WriteCRC_Enable,
                      const string& sMR2_LP_ASR,
                      const string& sMR3_GeardownMode,
                      const string& sMR3_WriteCMDLatency,
                      const string& sMR4_CALatency,
                      const string& sMR4_SelfRefreshAbort_Enable,
                      const string& sMR4_ReadPreamble,
                      const string& sMR4_WritePreamble,
                      const string& sMR5_CAParityLatency,
                      const string& sMR5_RTT_PARK,
                      const string& sMR5_DataMask_Enable,
                      const string& sMR5_WriteDBI_Enable,
                      const string& sMR5_ReadDBI_Enable,
                      const string& sMR6_tCCD_L,
                      const string& sMR6_VrefDQRange,
                      const string& sMR6_VrefDQValue,
                      const string& sMR6_PDA_VrefDQ_Enable)
{


    InitLabel il;

    //================================================================================
    // Physical Waveform Index for logical 0/1
    //================================================================================
    int    iWF0_Address = 0x0;    // 0 "0000"  "0000"  "...."   0000
    int    iWF1_Address = 0xf;    // f "1111"  "0000"  "...."   1111
    int    iWF0_DQ0     = 0x0;    // 0 "0000"  "0000"  "...."   0000
    int    iWF1_DQ0     = 0xf;    // f "1111"  "0000"  "...."   1111

    //================================================================================
    // MRS Operation Mode
    //================================================================================
    int iMR0_Code = 0x00000000;
    int iMR1_Code = 0x00000000;
    int iMR2_Code = 0x00000000;
    int iMR3_Code = 0x00000000;
    int iMR4_Code = 0x00000000;
    int iMR5_Code = 0x00000000;
    int iMR6_Code = 0x00000000;

    if ( sMR0_BurstLength == "BC8" )                { iMR0_Code = iMR0_Code | 0x00000000; }  // BC8 (Fixed)
    if ( sMR0_BurstLength == "BC4or8" )             { iMR0_Code = iMR0_Code | 0x00000001; }  // BC4 or 8 (on the fly)
    if ( sMR0_BurstLength == "BC4" )                { iMR0_Code = iMR0_Code | 0x00000002; }  // BC4 (Fixed)

    if ( sMR0_ReadBurstType == "Sequential" )       { iMR0_Code = iMR0_Code | 0x00000000; }  // Sequential
    if ( sMR0_ReadBurstType == "Interleave" )       { iMR0_Code = iMR0_Code | 0x00000008; }  // Interleave

    if ( sMR0_CASLatency ==  "9" )                  { iMR0_Code = iMR0_Code | 0x00000000; }  //
    if ( sMR0_CASLatency == "10" )                  { iMR0_Code = iMR0_Code | 0x00000004; }  //
    if ( sMR0_CASLatency == "11" )                  { iMR0_Code = iMR0_Code | 0x00000010; }  //
    if ( sMR0_CASLatency == "12" )                  { iMR0_Code = iMR0_Code | 0x00000014; }  //
    if ( sMR0_CASLatency == "13" )                  { iMR0_Code = iMR0_Code | 0x00000020; }  //
    if ( sMR0_CASLatency == "14" )                  { iMR0_Code = iMR0_Code | 0x00000024; }  //
    if ( sMR0_CASLatency == "15" )                  { iMR0_Code = iMR0_Code | 0x00000030; }  //
    if ( sMR0_CASLatency == "16" )                  { iMR0_Code = iMR0_Code | 0x00000034; }  //
    if ( sMR0_CASLatency == "18" )                  { iMR0_Code = iMR0_Code | 0x00000040; }  //
    if ( sMR0_CASLatency == "20" )                  { iMR0_Code = iMR0_Code | 0x00000044; }  //
    if ( sMR0_CASLatency == "22" )                  { iMR0_Code = iMR0_Code | 0x00000050; }  //
    if ( sMR0_CASLatency == "24" )                  { iMR0_Code = iMR0_Code | 0x00000054; }  //

    if ( sMR0_DLL_Reset == "1" )                    { iMR0_Code = iMR0_Code | 0x00000100; }  // DLL Reset

    if ( sMR0_WriteRecovery == "10" )               { iMR0_Code = iMR0_Code | 0x00000000; }  //
    if ( sMR0_WriteRecovery == "12" )               { iMR0_Code = iMR0_Code | 0x00000200; }  //
    if ( sMR0_WriteRecovery == "14" )               { iMR0_Code = iMR0_Code | 0x00000400; }  //
    if ( sMR0_WriteRecovery == "16" )               { iMR0_Code = iMR0_Code | 0x00000600; }  //
    if ( sMR0_WriteRecovery == "18" )               { iMR0_Code = iMR0_Code | 0x00000800; }  //
    if ( sMR0_WriteRecovery == "20" )               { iMR0_Code = iMR0_Code | 0x00000A00; }  //
    if ( sMR0_WriteRecovery == "24" )               { iMR0_Code = iMR0_Code | 0x00000C00; }  //

    if ( sMR1_DLL_Enable == "1" )                   { iMR1_Code = iMR1_Code | 0x00000001; }  // DLL Enable

    if ( sMR1_DriverImpedanceControl == "RZQ/7" )   { iMR1_Code = iMR1_Code | 0x00000000; }  // RZQ/7
    if ( sMR1_DriverImpedanceControl == "RZQ/5" )   { iMR1_Code = iMR1_Code | 0x00000002; }  // RZQ/5

    if ( sMR1_AdditiveLatency == "0" )              { iMR1_Code = iMR1_Code | 0x00000000; }  // Disable
    if ( sMR1_AdditiveLatency == "1" )              { iMR1_Code = iMR1_Code | 0x00000008; }  // CL-1
    if ( sMR1_AdditiveLatency == "2" )              { iMR1_Code = iMR1_Code | 0x00000010; }  // CL-2

    if ( sMR1_RTT_NOM == "Disable" )                { iMR1_Code = iMR1_Code | 0x00000000; }  // sMR1_RTT_NOM Disable
    if ( sMR1_RTT_NOM == "RZQ/4" )                  { iMR1_Code = iMR1_Code | 0x00000100; }  // RZQ/4
    if ( sMR1_RTT_NOM == "RZQ/2" )                  { iMR1_Code = iMR1_Code | 0x00000200; }  // RZQ/2
    if ( sMR1_RTT_NOM == "RZQ/6" )                  { iMR1_Code = iMR1_Code | 0x00000300; }  // RZQ/6
    if ( sMR1_RTT_NOM == "RZQ/1" )                  { iMR1_Code = iMR1_Code | 0x00000400; }  // RZQ/1
    if ( sMR1_RTT_NOM == "RZQ/5" )                  { iMR1_Code = iMR1_Code | 0x00000500; }  // RZQ/5
    if ( sMR1_RTT_NOM == "RZQ/3" )                  { iMR1_Code = iMR1_Code | 0x00000600; }  // RZQ/3
    if ( sMR1_RTT_NOM == "RZQ/7" )                  { iMR1_Code = iMR1_Code | 0x00000700; }  // RZQ/7

    if ( sMR1_TDQS_Enable == "1" )                  { iMR1_Code = iMR1_Code | 0x00000800; }  // Enable

    if ( sMR2_CASWriteLatency == "9"  )             { iMR2_Code = iMR2_Code | 0x00000000; }  //
    if ( sMR2_CASWriteLatency == "10" )             { iMR2_Code = iMR2_Code | 0x00000008; }  //
    if ( sMR2_CASWriteLatency == "11" )             { iMR2_Code = iMR2_Code | 0x00000010; }  //
    if ( sMR2_CASWriteLatency == "12" )             { iMR2_Code = iMR2_Code | 0x00000018; }  //
    if ( sMR2_CASWriteLatency == "14" )             { iMR2_Code = iMR2_Code | 0x00000020; }  //
    if ( sMR2_CASWriteLatency == "16" )             { iMR2_Code = iMR2_Code | 0x00000028; }  //
    if ( sMR2_CASWriteLatency == "18" )             { iMR2_Code = iMR2_Code | 0x00000030; }  //

    if ( sMR2_RTT_WR == "Off" )                     { iMR2_Code = iMR2_Code | 0x00000000; }  // 
    if ( sMR2_RTT_WR == "RZQ/2" )                   { iMR2_Code = iMR2_Code | 0x00000200; }  // 
    if ( sMR2_RTT_WR == "RZQ/1" )                   { iMR2_Code = iMR2_Code | 0x00000400; }  // 
    if ( sMR2_RTT_WR == "HiZ" )                     { iMR2_Code = iMR2_Code | 0x00000600; }  // 

    if ( sMR2_WriteCRC_Enable == "1" )              { iMR2_Code = iMR2_Code | 0x00001000; }  // Enable

    if ( sMR2_LP_ASR == "Manual_Normal"   )         { iMR2_Code = iMR2_Code | 0x00000000; }  // Manual_Normal
    if ( sMR2_LP_ASR == "Manual_Reduced"  )         { iMR2_Code = iMR2_Code | 0x00000040; }  // Manual_Reduced
    if ( sMR2_LP_ASR == "Manual_Extended" )         { iMR2_Code = iMR2_Code | 0x00000080; }  // Manual_Extended
    if ( sMR2_LP_ASR == "ASR" )                     { iMR2_Code = iMR2_Code | 0x000000C0; }  // ASR

    if ( sMR3_GeardownMode == "1/2Rate" )           { iMR3_Code = iMR3_Code | 0x00000000; }  //
    if ( sMR3_GeardownMode == "1/4Rate" )           { iMR3_Code = iMR3_Code | 0x00000008; }  //

    string sMR3_PerDRAMAddressability_Enable = "0";
    if ( sMR3_PerDRAMAddressability_Enable == "1" ) { iMR3_Code = iMR3_Code | 0x00000010; }  // Enable

    if ( sMR3_WriteCMDLatency == "4" )              { iMR3_Code = iMR3_Code | 0x00000000; }  //
    if ( sMR3_WriteCMDLatency == "5" )              { iMR3_Code = iMR3_Code | 0x00000200; }  //
    if ( sMR3_WriteCMDLatency == "6" )              { iMR3_Code = iMR3_Code | 0x00000400; }  //

    if ( sMR4_CALatency == "0" )                    { iMR4_Code = iMR4_Code | 0x00000000; }  // Disable
    if ( sMR4_CALatency == "3" )                    { iMR4_Code = iMR4_Code | 0x00000040; }  //
    if ( sMR4_CALatency == "4" )                    { iMR4_Code = iMR4_Code | 0x00000080; }  //
    if ( sMR4_CALatency == "5" )                    { iMR4_Code = iMR4_Code | 0x000000C0; }  //
    if ( sMR4_CALatency == "6" )                    { iMR4_Code = iMR4_Code | 0x00000100; }  //
    if ( sMR4_CALatency == "8" )                    { iMR4_Code = iMR4_Code | 0x00000140; }  //

    if ( sMR4_SelfRefreshAbort_Enable == "1" )      { iMR4_Code = iMR4_Code | 0x00000200; }  // Enable

    if ( sMR4_ReadPreamble == "1" )                 { iMR4_Code = iMR4_Code | 0x00000000; }  // 1 nCK
    if ( sMR4_ReadPreamble == "2" )                 { iMR4_Code = iMR4_Code | 0x00000800; }  // 2 nCK

    if ( sMR4_WritePreamble == "1" )                { iMR4_Code = iMR4_Code | 0x00000000; }  // 1 nCK
    if ( sMR4_WritePreamble == "2" )                { iMR4_Code = iMR4_Code | 0x00001000; }  // 2 nCK

    if ( sMR5_CAParityLatency == "0" )              { iMR5_Code = iMR5_Code | 0x00000000; }  // Disable
    if ( sMR5_CAParityLatency == "4" )              { iMR5_Code = iMR5_Code | 0x00000001; }  //
    if ( sMR5_CAParityLatency == "5" )              { iMR5_Code = iMR5_Code | 0x00000002; }  // 
    if ( sMR5_CAParityLatency == "6" )              { iMR5_Code = iMR5_Code | 0x00000003; }  // 
    if ( sMR5_CAParityLatency == "8" )              { iMR5_Code = iMR5_Code | 0x00000004; }  //

    if ( sMR5_RTT_PARK == "0" )                     { iMR5_Code = iMR5_Code | 0x00000000; }  // RTT_PARK Disable
    if ( sMR5_RTT_PARK == "1" )                     { iMR5_Code = iMR5_Code | 0x00000040; }  // RZQ/4
    if ( sMR5_RTT_PARK == "2" )                     { iMR5_Code = iMR5_Code | 0x00000080; }  // RZQ/2
    if ( sMR5_RTT_PARK == "3" )                     { iMR5_Code = iMR5_Code | 0x000000C0; }  // RZQ/6
    if ( sMR5_RTT_PARK == "4" )                     { iMR5_Code = iMR5_Code | 0x00000100; }  // RZQ/1
    if ( sMR5_RTT_PARK == "5" )                     { iMR5_Code = iMR5_Code | 0x00000140; }  // RZQ/5
    if ( sMR5_RTT_PARK == "6" )                     { iMR5_Code = iMR5_Code | 0x00000180; }  // RZQ/3
    if ( sMR5_RTT_PARK == "7" )                     { iMR5_Code = iMR5_Code | 0x000001C0; }  // RZQ/7

    if ( sMR5_DataMask_Enable == "1" )              { iMR5_Code = iMR5_Code | 0x00000400; }  // Enable

    if ( sMR5_WriteDBI_Enable == "1" )              { iMR5_Code = iMR5_Code | 0x00000800; }  // Enable

    if ( sMR5_ReadDBI_Enable == "1" )               { iMR5_Code = iMR5_Code | 0x00001000; }  // Enable

    if ( sMR6_tCCD_L == "4"  )                      { iMR6_Code = iMR6_Code | 0x00000000; }  //
    if ( sMR6_tCCD_L == "5"  )                      { iMR6_Code = iMR6_Code | 0x00000400; }  // 1600,1866
    if ( sMR6_tCCD_L == "6"  )                      { iMR6_Code = iMR6_Code | 0x00000800; }  // 2133,2400
    if ( sMR6_tCCD_L == "7"  )                      { iMR6_Code = iMR6_Code | 0x00000C00; }  //
    if ( sMR6_tCCD_L == "8"  )                      { iMR6_Code = iMR6_Code | 0x00001000; }  //

    if ( sMR6_VrefDQRange == "Range1" )             { iMR6_Code = iMR6_Code | 0x00000000; }  // VrefDQ Training Range1 
    if ( sMR6_VrefDQRange == "Range2" )             { iMR6_Code = iMR6_Code | 0x00000040; }  // VrefDQ Training Range2 

    if(sMR6_VrefDQValue == "Avg"){
        int iVrefDQ_Value_Avg;
        string sVrefDQ_Value_Avg;
        GET_USER_STRING("VrefDQ_Value_Avg", sVrefDQ_Value_Avg);                              // VrefDQ Training Average Value among duts
        iVrefDQ_Value_Avg = str2int_bin_dec_hex(sVrefDQ_Value_Avg);
        iMR6_Code = iMR6_Code | (iVrefDQ_Value_Avg & 0x3F);
    } else{
        int iMR6_VrefDQValue;
        iMR6_VrefDQValue = str2int_bin_dec_hex(sMR6_VrefDQValue);
        iMR6_Code = iMR6_Code | (iMR6_VrefDQValue  & 0x3F);                                  // VrefDQ Training Value specified via TM param
    }

    int iMR6_VrefDQ_Enable_Code          = iMR6_Code               | 0x00000080;             // VrefDQ Training Enable
    int iMR6_VrefDQ_Set_Code             = iMR6_VrefDQ_Enable_Code;                          // VrefDQ Training NewValue set       
    int iMR6_VrefDQ_Disable_Code         = iMR6_VrefDQ_Set_Code    - 0x00000080;             // VrefDQ Training Disable
    int iMR6_VrefDQ_Set_PDA_Code_Org     = iMR6_VrefDQ_Set_Code;
    int iMR6_VrefDQ_Disable_PDA_Code_Org = iMR6_VrefDQ_Disable_Code;

    int iMR3_PDA_Enable_Code             = iMR3_Code               | 0x00000010;             // PDA Addressability Enable for VrefDQ
    int iMR3_PDA_Disable_Code            = iMR3_PDA_Enable_Code    - 0x00000010;             // PerDRAM Addressability Disable for VrefDQ

    //================================================================================
    // Store Edit Vector# for MRS Setting & MRS Code into Array
    //================================================================================
    if(sType_Init_Label == "PDA"){

        vector<int>    iVecNumArray;
        vector<string> sDataArray;

        if(iFlag_MR0_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr0());                       parity_computation(1, 0, iMR0_Code);                sDataArray.push_back(int2str(iMR0_Code));                }
        if(iFlag_MR1_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr1());                       parity_computation(1, 1, iMR1_Code);                sDataArray.push_back(int2str(iMR1_Code));                }
        if(iFlag_MR2_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr2());                       parity_computation(1, 2, iMR2_Code);                sDataArray.push_back(int2str(iMR2_Code));                }
        if(iFlag_MR3_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr3());                       parity_computation(1, 3, iMR3_Code);                sDataArray.push_back(int2str(iMR3_Code));                }
        if(iFlag_MR4_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr4());                       parity_computation(1, 4, iMR4_Code);                sDataArray.push_back(int2str(iMR4_Code));                }
        if(iFlag_MR5_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr5());                       parity_computation(1, 5, iMR5_Code);                sDataArray.push_back(int2str(iMR5_Code));                }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr6_common_vrefdq_enable());  parity_computation(1, 6, iMR6_VrefDQ_Enable_Code);  sDataArray.push_back(int2str(iMR6_VrefDQ_Enable_Code));  }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr6_common_vrefdq_set());     parity_computation(1, 6, iMR6_VrefDQ_Set_Code);     sDataArray.push_back(int2str(iMR6_VrefDQ_Set_Code));     }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr6_common_vrefdq_disable()); parity_computation(1, 6, iMR6_VrefDQ_Disable_Code); sDataArray.push_back(int2str(iMR6_VrefDQ_Disable_Code)); }

        if(iFlag_MR3_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr3_pda_enable());            parity_computation(1, 3, iMR3_PDA_Enable_Code);     sDataArray.push_back(int2str(iMR3_PDA_Enable_Code));    }
        if(iFlag_MR3_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr3_pda_disable());           parity_computation(1, 3, iMR3_PDA_Disable_Code);    sDataArray.push_back(int2str(iMR3_PDA_Disable_Code));   }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_enable());     parity_computation(1, 6, iMR6_VrefDQ_Enable_Code);  sDataArray.push_back(int2str(iMR6_VrefDQ_Enable_Code)); }

        if(iFlag_MR6_Edit == 1){ 
            for(int i=0x00; i<0x33; i++){
                iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_set(i));
                int iMR6_VrefDQ_Set_PDA_Code = iMR6_VrefDQ_Set_PDA_Code_Org;
                iMR6_VrefDQ_Set_PDA_Code = iMR6_VrefDQ_Set_PDA_Code & 0xFFFC0; // 1111 1111 1111 1100 0000
                iMR6_VrefDQ_Set_PDA_Code = iMR6_VrefDQ_Set_PDA_Code | i;
                parity_computation(1, 6, iMR6_VrefDQ_Set_PDA_Code);
                sDataArray.push_back(int2str(iMR6_VrefDQ_Set_PDA_Code));

                iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_disable(i));
                int iMR6_VrefDQ_Disable_PDA_Code = iMR6_VrefDQ_Disable_PDA_Code_Org;
                iMR6_VrefDQ_Disable_PDA_Code = iMR6_VrefDQ_Disable_PDA_Code & 0xFFFC0; // 1111 1111 1111 1100 0000
                iMR6_VrefDQ_Disable_PDA_Code = iMR6_VrefDQ_Disable_PDA_Code | i;
                parity_computation(1, 6, iMR6_VrefDQ_Disable_PDA_Code);
                sDataArray.push_back(int2str(iMR6_VrefDQ_Disable_PDA_Code));
            }
        }

        //================================================================================
        // Edit Vectors of Address Pins based on Specified MRS Operation Mode
        //================================================================================
        vec_label_edit_A0_A17(il.get_initlabel_pda(), iVecNumArray, sDataArray, iWF0_Address, iWF1_Address);

    } else{

        vector<int>    iVecNumArray;
        vector<string> sDataArray;

        if(iFlag_MR0_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr0());                       parity_computation(1, 0, iMR0_Code);                sDataArray.push_back(int2str(iMR0_Code));                }
        if(iFlag_MR1_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr1());                       parity_computation(1, 1, iMR1_Code);                sDataArray.push_back(int2str(iMR1_Code));                }
        if(iFlag_MR2_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr2());                       parity_computation(1, 2, iMR2_Code);                sDataArray.push_back(int2str(iMR2_Code));                }
        if(iFlag_MR3_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr3());                       parity_computation(1, 3, iMR3_Code);                sDataArray.push_back(int2str(iMR3_Code));                }
        if(iFlag_MR4_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr4());                       parity_computation(1, 4, iMR4_Code);                sDataArray.push_back(int2str(iMR4_Code));                }
        if(iFlag_MR5_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr5());                       parity_computation(1, 5, iMR5_Code);                sDataArray.push_back(int2str(iMR5_Code));                }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr6_common_vrefdq_enable());  parity_computation(1, 6, iMR6_VrefDQ_Enable_Code);  sDataArray.push_back(int2str(iMR6_VrefDQ_Enable_Code));  }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr6_common_vrefdq_set());     parity_computation(1, 6, iMR6_VrefDQ_Set_Code);     sDataArray.push_back(int2str(iMR6_VrefDQ_Set_Code));     }
        if(iFlag_MR6_Edit == 1){ iVecNumArray.push_back(il.get_vecnum_training_mr6_common_vrefdq_disable()); parity_computation(1, 6, iMR6_VrefDQ_Disable_Code); sDataArray.push_back(int2str(iMR6_VrefDQ_Disable_Code)); }

        //================================================================================
        // Edit Vectors of Address Pins based on Specified MRS Operation Mode
        //================================================================================
        vec_label_edit_A0_A17(il.get_initlabel_training(), iVecNumArray, sDataArray, iWF0_Address, iWF1_Address);

    }

    //================================================================================
    // Per DRAM Addressability to control VrefDQ per DUT
    //================================================================================
    if(sType_Init_Label == "PDA"){

        if(iFlag_MR6_Edit == 1){

            vector<int>    iVecNumArray;

            //================================================================================
            // Extract Vector# for VrefDQ Per DRAM
            //================================================================================
            iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_enable());
            for(int i=0x00; i<0x33; i++){
                iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_set(i));
                iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_set(i) + 1);
                iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_disable(i));
                iVecNumArray.push_back(il.get_vecnum_pda_mr6_pda_vrefdq_disable(i) + 1);
            }

            if(sMR6_PDA_VrefDQ_Enable == "1"){
                //================================================================================
                // Edit Vectors of DQ0 for VrefDQ Per DRAM based on Testflow Variables
                //================================================================================
                FOR_EACH_SITE_BEGIN();
                    vector<int> iPhyWvfIndexArray;

                    int    iVrefDQ_Value;
                    string sVrefDQ_Value;
                    GET_USER_STRING("VrefDQ_Value", sVrefDQ_Value);

                    iVrefDQ_Value = str2int_bin_dec_hex(sVrefDQ_Value);

                    iPhyWvfIndexArray.push_back(iWF0_DQ0);
                    for(int i=0x00; i<0x33; i++){
                        if(i == iVrefDQ_Value){ iPhyWvfIndexArray.push_back(iWF0_DQ0); iPhyWvfIndexArray.push_back(iWF0_DQ0); iPhyWvfIndexArray.push_back(iWF0_DQ0); iPhyWvfIndexArray.push_back(iWF0_DQ0); }
                        else                  { iPhyWvfIndexArray.push_back(iWF1_DQ0); iPhyWvfIndexArray.push_back(iWF1_DQ0); iPhyWvfIndexArray.push_back(iWF1_DQ0); iPhyWvfIndexArray.push_back(iWF1_DQ0); }
                    }

                    vec_label_edit_DQ0(il.get_initlabel_pda(), iVecNumArray, iPhyWvfIndexArray);

                FOR_EACH_SITE_END();

            } else{
                //================================================================================
                // Edit Vectors of DQ0 for VrefDQ Per DRAM with always high
                //================================================================================
                {
                    vector<int> iPhyWvfIndexArray;

                    iPhyWvfIndexArray.push_back(iWF0_DQ0);
                    for(int i=0x00; i<0x33; i++){
                        iPhyWvfIndexArray.push_back(iWF1_DQ0); iPhyWvfIndexArray.push_back(iWF1_DQ0); iPhyWvfIndexArray.push_back(iWF1_DQ0); iPhyWvfIndexArray.push_back(iWF1_DQ0);
                    }

                    vec_label_edit_DQ0(il.get_initlabel_pda(), iVecNumArray, iPhyWvfIndexArray);
                }
            }
        }
    }

    return;
}

//====================================================================================================================//
//===== Training CA timing ===========================================================================================//
//====================================================================================================================//
void training_timing_ca(const string&     sSetupPin,
                        const string&     sResultPin,
                        const string&     sSpecOffset,
                        const string&     sSpecSweep,
                        const double&     dLinStart,
                        const double&     dLinStop,
                        const double&     dLinStep,
                        const double&     dBinStep,
                        const int&        iReportFlag,
                        SEARCH_FUNC_TASK& searchtask,
                        map<int, double>& dTrainingResult)
{

    dTrainingResult.clear();
    map<int, int>    iTmpResult;
    map<int, double> dRightBoundary;
    map<int, double> dLeftBoundary;

//    double dTCK = Primary.getSpecification().getSpecValue("tCK_ns");
    fwout << "SVLR? TIM,PRM,,\"tCK_ns\"" << endl;
    double dTCK = atof(fwresult[0][4].c_str());

    //================================================================================
    // Compensated spec variable is forced to be 0 before search 
    //================================================================================
    Primary.getSpecification().change(sSpecOffset, 0);
    FLUSH(TM::APRM);

    //================================================================================
    // Execute first spec search
    //================================================================================
    searchtask.pin        (sSetupPin)
              .spec       (sSpecSweep,TM::TIM)
              .method     (TM::LinBin)
              .start      (dLinStart)
              .stop       (dLinStop)
              .stepWidth  (dLinStep)
              .resolution (dBinStep)
              .resultPins (sResultPin);

    searchtask.execute();

    //================================================================================
    // Exstract first search results and define search conditions for second search
    //================================================================================
    FOR_EACH_SITE_BEGIN();
         int iSite = CURRENT_SITE_NUMBER();
         switch(searchtask.getResultSpec(sSetupPin)){
             case TM::TransitionPassFail :
                     dRightBoundary[iSite] = searchtask.getPassValue(sSetupPin);
                     searchtask.pin        (sSetupPin)
                               .spec       (sSpecSweep,TM::TIM)
                               .method     (TM::LinBin)
                               .start      (dRightBoundary[iSite] - dTCK*1.00)
                               .stop       (dRightBoundary[iSite] - dTCK*0.50)
                               .stepWidth  (dLinStep)
                               .resolution (dBinStep)
                               .resultPins (sResultPin);
                     iTmpResult[iSite] = 1;
                     break;
             case TM::TransitionFailPass :
                     dLeftBoundary[iSite]   = searchtask.getPassValue(sSetupPin);
                     searchtask.pin        (sSetupPin)
                               .spec       (sSpecSweep,TM::TIM)
                               .method     (TM::LinBin)
                               .start      (dLeftBoundary[iSite] + dTCK*0.50)
                               .stop       (dLeftBoundary[iSite] + dTCK*1.00)
                               .stepWidth  (dLinStep)
                               .resolution (dBinStep)
                               .resultPins (sResultPin);
                     iTmpResult[iSite] = 2;
                     break;
             case TM::AllPass :
                     iTmpResult[iSite] = -1;
                     break;
             case TM::AllFail :
                     iTmpResult[iSite] = -1;
                     break;
             default :
                     iTmpResult[iSite] = -1;
         }
    FOR_EACH_SITE_END();

    //================================================================================
    // Execute second spec search
    //================================================================================
    searchtask.execute();

    if ( iReportFlag ) {
        cout << left  << setw(20) << "Training_Item"
             << left  << setw(8)  << "site"
             << left  << setw(16) << "pin"
             << right << setw(12) << "left"
             << right << setw(12) << "right"
             << right << setw(12) << "center"
             << endl;
    }

    //================================================================================
    // Exstract second search results
    //================================================================================
    FOR_EACH_SITE_BEGIN();
        int iSite = CURRENT_SITE_NUMBER();
        switch(searchtask.getResultSpec(sSetupPin)){
            case TM::TransitionPassFail :
                    if ( iTmpResult[iSite] == 2 ){
                          dRightBoundary[iSite] = searchtask.getPassValue(sSetupPin);
                          iTmpResult[iSite] = 3;
                    }
                    else {
                          iTmpResult[iSite] = -1;
                    }
                    break;
            case TM::TransitionFailPass :
                    if ( iTmpResult[iSite] == 1 ){
                          dLeftBoundary[iSite]  = searchtask.getPassValue(sSetupPin);
                          iTmpResult[iSite] = 3;
                    }
                    else {
                          iTmpResult[iSite] = -1;
                    }
                    break;
            case TM::AllPass :
                    iTmpResult[iSite] = -1;
                    break;
            case TM::AllFail :
                    iTmpResult[iSite] = -1;
                    break;
            default :
                    iTmpResult[iSite] = -1;
        }

        //================================================================================
        // Compensate spec variable with optimal position
        //================================================================================
        if ( iTmpResult[iSite] == 3 ) {
            dTrainingResult[iSite] = dLeftBoundary[iSite] + (dRightBoundary[iSite]-dLeftBoundary[iSite])*0.50;
            Primary.getSpecification().change(sSpecOffset, dTrainingResult[iSite]);
            FLUSH(TM::APRM);
        }
        else {
            dTrainingResult[iSite] = -9999;
        }

        if ( iReportFlag ) {
            cout << left  << setw(20) << "Timing_CA"
                 << left  << setw(8)  << iSite
                 << left  << setw(16) << "CA_pins"
                 << right << setw(12) << fixed << setprecision(3) << dLeftBoundary[iSite]
                 << right << setw(12) << fixed << setprecision(3) << dRightBoundary[iSite]
                 << right << setw(12) << fixed << setprecision(3) << dTrainingResult[iSite]
                 << endl;
        } 

    FOR_EACH_SITE_END();

    return;
}

//====================================================================================================================//
//===== Training Writeleveling timing ================================================================================//
//====================================================================================================================//
void training_timing_writeleveling(const string&                sSetupPin,      // "DQS,DQSn" / "DQSL,DQSLn;DQSU,DQS"
                                   const string&                sResultPin,
                                   const string&                sSpecOffset,
                                   const string&                sSpecSweep,
                                   const double&                dLinStart,
                                   const double&                dLinStop,
                                   const double&                dLinStep,
                                   const double&                dBinStep,
                                   const int&                   iReportFlag,
                                   SEARCH_FUNC_TASK&            searchtask,
                                   map<int, map<int, double> >& dTrainingResult)
{

    dTrainingResult.clear();
    map<int, map<int, string> > sTransition; // [Site][Domain]

    vector<string> sSetupPinPerDomain;
    vector<string> sResultPinPerDomain;
    vector<string> sSpecOffsetPerDomain;
    tokenize(sSetupPin,   sSetupPinPerDomain,   ";");
    tokenize(sResultPin,  sResultPinPerDomain,  ";");
    tokenize(sSpecOffset, sSpecOffsetPerDomain, ";");
    
    int iDomainNum = sSetupPinPerDomain.size();
    
    if((iDomainNum == (int)sResultPinPerDomain.size())&&(iDomainNum == (int)sSpecOffsetPerDomain.size())&&(iDomainNum != 0)){

        //================================================================================
        // Compensated spec variable is forced to be 0 before search 
        //================================================================================
        for(int k=0; k<iDomainNum; k++){
            Primary.getSpecification().change(sSpecOffsetPerDomain[k], 0);
            FLUSH(TM::APRM);
        }
        
        //================================================================================
        // Execute spec search
        //================================================================================
        FOR_EACH_SITE_BEGIN();
            for(int k=0; k<iDomainNum; k++){
                searchtask.pin        (sSetupPinPerDomain[k])
                          .spec       (sSpecSweep,TM::TIM)
                          .method     (TM::LinBin)
                          .start      (dLinStart)
                          .stop       (dLinStop)
                          .stepWidth  (dLinStep)
                          .resolution (dBinStep)
                          .resultPins (sResultPinPerDomain[k]);
            }
        FOR_EACH_SITE_END();

        searchtask.execute();

        //================================================================================
        // Exstract search results
        //================================================================================
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            for(int k=0; k<iDomainNum; k++){
                switch(searchtask.getResultSpec(sSetupPinPerDomain[k])){
                    case TM::TransitionPassFail :
                            dTrainingResult[iSite][k] = -9999;
                            sTransition[iSite][k] = "PassFail";
                            break;
                    case TM::TransitionFailPass :
                            dTrainingResult[iSite][k] = searchtask.getPassValue(sSetupPinPerDomain[k]);
                            sTransition[iSite][k] = "FailPass";
                            break;
                    case TM::AllPass :
                            dTrainingResult[iSite][k] = -9999;
                            sTransition[iSite][k] = "AllPass";
                            break;
                    case TM::AllFail :
                            dTrainingResult[iSite][k] = -9999;
                            sTransition[iSite][k] = "AllFail";
                            break;
                    default :
                            dTrainingResult[iSite][k] = -9999;
                            sTransition[iSite][k] = "Default";
                 }
            }
        FOR_EACH_SITE_END();

        if ( iReportFlag ) {
            cout << left  << setw(20) << "Training_Item"
                 << left  << setw(8)  << "site"
                 << left  << setw(16) << "pin"
                 << right << setw(12) << "boundary"
                 << right << setw(12) << "type"
                 << endl;
        }

        //================================================================================
        // Compensate spec variable with optimal position
        //================================================================================
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            
            for(int k=0; k<iDomainNum; k++){
                if ( iReportFlag ) {
                    cout << left  << setw(20) << "Timing_WriteLeveling"
                         << left  << setw(8)  << iSite
                         << left  << setw(16) << sSetupPinPerDomain[k]
                         << right << setw(12) << fixed << setprecision(3) << dTrainingResult[iSite][k]
                         << right << setw(12)                             << sTransition[iSite][k]
                         << endl;
                } 
                if ( sTransition[iSite][k] == "FailPass" ){ 
                    Primary.getSpecification().change(sSpecOffsetPerDomain[k], dTrainingResult[iSite][k]);
                    FLUSH(TM::APRM);
                }
            }
        FOR_EACH_SITE_END();
    
    } else{
        cout << "ERROR: Please check TestMethod parameter sSetupPin/sResultPin/sSpecOffset for training_timing_writeleveling." << endl;
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            for(int k=0; k<iDomainNum; k++){
                dTrainingResult[iSite][k] = -9999;
            }
        FOR_EACH_SITE_END();
    }

    return;
}

//====================================================================================================================//
//===== Training DQ ReadWrite timing =================================================================================//
//====================================================================================================================//
void training_timing_readwrite(const string&                sMode,
                               const string&                sSetupPin,
                               const string&                sSpecOffset,
                               const double&                dLinStart_ns,
                               const double&                dLinStop_ns,
                               const double&                dLinStep_ns,
                               const double&                dBinStep_ns,
                               const int&                   iReportFlag,
                               map<int, map<int, double> >& dTrainingResult)
{

    TIMING_OFFSET_SEARCH_TASK searchtask;

    dTrainingResult.clear();

    vector<string> sSetupPinPerDomain;
    vector<string> sSpecOffsetPerDomain;
    tokenize(sSetupPin,   sSetupPinPerDomain,   ";");
    tokenize(sSpecOffset, sSpecOffsetPerDomain, ";");
    
    int iDomainNum = sSetupPinPerDomain.size();
    
    if((iDomainNum == (int)sSpecOffsetPerDomain.size())&&(iDomainNum != 0)){
    
        string sAllSetupPin = sSetupPin;
        replace_str(sAllSetupPin, ";", ",");
        
        //================================================================================
        // Compensated spec variable is forced to be 0 before search 
        //================================================================================
        for(int k=0; k<iDomainNum; k++){
            Primary.getSpecification().change(sSpecOffsetPerDomain[k], 0);
            FLUSH(TM::APRM);
        }
        
        //================================================================================
        // Execute spec search
        //================================================================================
        int iNumLinStep = int((dLinStop_ns - dLinStart_ns)/dLinStep_ns) + 1;
        
        TIMING_OFFSET_SEARCH::EDGE_TYPE edge_type;
        if(sMode == "READ") edge_type = TIMING_OFFSET_SEARCH::RECEIVE;
        else                edge_type = TIMING_OFFSET_SEARCH::FAST_DRIVE;
        
        searchtask.pin(sAllSetupPin)
                  .searchType(TIMING_OFFSET_SEARCH::EYE)
                  .start(dLinStart_ns/1E+9)
                  .numSteps(iNumLinStep)
                  .stepWidth(dLinStep_ns/1E+9)
                  .resolution(dBinStep_ns/1E+9)
                  .edgeType(edge_type)
                  .transitionDetection(TIMING_OFFSET_SEARCH::PF)
                  .restoreFlag(TIMING_OFFSET_SEARCH::YES); 

        searchtask.execute();

        //================================================================================
        // Exstract search results
        //================================================================================
        vector<string> sPinList;
        sPinList = PinUtility.getDigitalPinNamesFromPinList(sAllSetupPin,TM::ALL_DIGITAL,false,false,PIN_UTILITY::DEFINITION_ORDER);
        map<int, map<string, double> > dLeftBoundary;
        map<int, map<string, double> > dRightBoundary;

        FOR_EACH_SITE_BEGIN();

            int iSite = CURRENT_SITE_NUMBER();

            for ( unsigned int j=0; j<sPinList.size(); j++ ) {

                if ( searchtask.pin(sPinList[j]).TransitionOffsetFound() == TIMING_OFFSET_SEARCH::FOUND ){
                    dLeftBoundary[iSite][sPinList[j]]  = searchtask.pin(sPinList[j]).getLatestLBTransitionOffset()*1E+9;
                    dRightBoundary[iSite][sPinList[j]] = searchtask.pin(sPinList[j]).getLatestRBTransitionOffset()*1E+9;
                } 
                else{
                    dLeftBoundary[iSite][sPinList[j]]  = -9999;
                    dRightBoundary[iSite][sPinList[j]] = -9999;
                }
            }

        FOR_EACH_SITE_END();

        if ( iReportFlag ) {
            cout << left  << setw(20) << "Training_Item"
                 << left  << setw(8)  << "site"
                 << left  << setw(16) << "pin"
                 << right << setw(12) << "left"
                 << right << setw(12) << "right"
                 << right << setw(12) << "center"
                 << endl;
        }

        //================================================================================
        // Compensate spec variable with optimal position
        //================================================================================
        FOR_EACH_SITE_BEGIN();

            int iSite = CURRENT_SITE_NUMBER();
            
            for(int k=0; k<iDomainNum; k++){
                
                int    iCounter = 0;
                double dCenter  = 0.0;
                vector<string> sSetupPinArrayPerDomain;
                sSetupPinArrayPerDomain = PinUtility.getDigitalPinNamesFromPinList(sSetupPinPerDomain[k],TM::ALL_DIGITAL,false,false,PIN_UTILITY::DEFINITION_ORDER);
                
                for(unsigned int j=0; j<sSetupPinArrayPerDomain.size(); j++){
                    if (( dLeftBoundary[iSite][sSetupPinArrayPerDomain[j]] > -9990 )&&(dRightBoundary[iSite][sSetupPinArrayPerDomain[j]] > -9990)) { 
                        dCenter += (dLeftBoundary[iSite][sPinList[j]] + dRightBoundary[iSite][sPinList[j]])/2;
                        iCounter++;
                    }
                    if ( iReportFlag ) {
                        cout << left  << setw(20) << "Timing_ReadWrite"
                             << left  << setw(8)  << iSite
                             << left  << setw(16) << sSetupPinArrayPerDomain[j]
                             << right << setw(12) << fixed << setprecision(3) << dLeftBoundary[iSite][sSetupPinArrayPerDomain[j]]
                             << right << setw(12) << fixed << setprecision(3) << dRightBoundary[iSite][sSetupPinArrayPerDomain[j]]
                             << right << setw(12) << fixed << setprecision(3) << (dLeftBoundary[iSite][sSetupPinArrayPerDomain[j]] + dRightBoundary[iSite][sSetupPinArrayPerDomain[j]])/2
                             << endl;
                    } 
                }

                if(iCounter == 0){
                    dCenter = -9999;
                    dTrainingResult[iSite][k] = -9999;
                } else if(iCounter == (int)sSetupPinArrayPerDomain.size()){
                    dCenter = dCenter/iCounter;
                    dTrainingResult[iSite][k] = dCenter;
                } else{
                    dCenter = dCenter/iCounter;
                    dTrainingResult[iSite][k] = -9999;
                }
                
                if ( iReportFlag ) {
                    cout << left  << setw(20) << "Timing_ReadWrite"
                         << left  << setw(8)  << iSite
                         << left  << setw(16) << "Average"
                         << right << setw(12) << "-"
                         << right << setw(12) << "-"
                         << right << setw(12) << fixed << setprecision(3) << dCenter
                         << endl;
                }

                if ( dCenter > -9990 ) { 
                    Primary.getSpecification().change(sSpecOffsetPerDomain[k], dCenter);
                    FLUSH(TM::APRM);
                }
            }
        FOR_EACH_SITE_END();
        
    } else{
        cout << "ERROR: Please check TestMethod parameter sSetupPin/sSpecOffset for training_timing_readwrite." << endl;
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            for(int k=0; k<iDomainNum; k++){
                dTrainingResult[iSite][k] = -9999;
            }
        FOR_EACH_SITE_END();
    }
    
    return;
}

//====================================================================================================================//
//===== Training VthDQ level =========================================================================================//
//====================================================================================================================//
void training_level_vthdq(const string&     sSetupPin,
                          const string&     sSpecOffset,
                          const string&     sSpecSweep,
                          const double&     dLinStart_Low,
                          const double&     dLinStop_Low,
                          const double&     dLinStart_High,
                          const double&     dLinStop_High,
                          const double&     dLinStep,
                          const double&     dBinStep,
                          const int&        iReportFlag,
                          SEARCH_FUNC_TASK& searchtask,
                          map<int, double>& dTrainingResult)
{

    dTrainingResult.clear();
    map<int, int> iTmpResult;
    map<int, double> dBoundary_Low;
    map<int, double> dBoundary_High;
    map<int, string> sTransition_Low;
    map<int, string> sTransition_High;

    //================================================================================
    // Compensated spec variable is forced to be 0 before search 
    //================================================================================
    Primary.getLevelSpec().change(sSpecOffset, 0);
    FLUSH(TM::APRM);

    //================================================================================
    // Execute first spec search
    //================================================================================
    searchtask.pin        (sSetupPin)
              .spec       (sSpecSweep,TM::LEV)
              .method     (TM::Linear)
              .start      (dLinStart_Low)
              .stop       (dLinStop_Low)
              .stepWidth  (dLinStep)
              .resolution (dBinStep)
              .resultPins (sSetupPin);

    searchtask.execute();

    //================================================================================
    // Exstract first search results and define search conditions for second search
    //================================================================================
    FOR_EACH_SITE_BEGIN();
        int iSite = CURRENT_SITE_NUMBER();

        if ( searchtask.getResultSpec(sSetupPin) == TM::TransitionFailPass ){
            dBoundary_Low[iSite]  = searchtask.getPassValue(sSetupPin);
            iTmpResult[iSite] = 1;
        }
        else {
            dBoundary_Low[iSite]  = -9999;
            iTmpResult[iSite] = 0;
        }
    FOR_EACH_SITE_END();

    searchtask.pin        (sSetupPin)
              .spec       (sSpecSweep,TM::LEV)
              .method     (TM::Linear)
              .start      (dLinStart_High)
              .stop       (dLinStop_High)
              .stepWidth  (dLinStep)
              .resolution (dBinStep)
              .resultPins (sSetupPin);

    //================================================================================
    // Execute second spec search
    //================================================================================
    searchtask.execute();

    //================================================================================
    // Exstract second search results
    //================================================================================
    FOR_EACH_SITE_BEGIN();
        int iSite = CURRENT_SITE_NUMBER();
        if ( searchtask.getResultSpec(sSetupPin) == TM::TransitionPassFail ){
            dBoundary_High[iSite]  = searchtask.getPassValue(sSetupPin);
            iTmpResult[iSite] = iTmpResult[iSite] & 1;
        }
        else {
            dBoundary_High[iSite]  = -9999;
            iTmpResult[iSite] = iTmpResult[iSite] & 0;
        }
    FOR_EACH_SITE_END();

    if ( iReportFlag ) {
        cout << left  << setw(20) << "Training_Item"
             << left  << setw(8)  << "site"
             << left  << setw(16) << "pin"
             << right << setw(12) << "low"
             << right << setw(12) << "high"
             << right << setw(12) << "center"
             << endl;
    }

    //================================================================================
    // Compensate spec variable with optimal position
    //================================================================================
    FOR_EACH_SITE_BEGIN();
        int iSite = CURRENT_SITE_NUMBER();

        if ( iReportFlag ) {
            cout << left  << setw(20) << "Vth_DQ"
                 << left  << setw(8)  << iSite
                 << left  << setw(16) << sSetupPin
                 << right << setw(12) << fixed << setprecision(3) << dBoundary_Low[iSite]
                 << right << setw(12) << fixed << setprecision(3) << dBoundary_High[iSite]
                 << right << setw(12) << fixed << setprecision(3) << (dBoundary_Low[iSite] + dBoundary_High[iSite])/2
                 << endl;
        } 
        if ( iTmpResult[iSite] == 1 ){ 
            Primary.getLevelSpec().change(sSpecOffset, (dBoundary_Low[iSite] + dBoundary_High[iSite])/2);
            FLUSH(TM::APRM);
            dTrainingResult[iSite] = (dBoundary_Low[iSite] + dBoundary_High[iSite])/2;
        } else{
            dTrainingResult[iSite] = -9999;
        }
    FOR_EACH_SITE_END();

    return;
}

//====================================================================================================================//
//===== Training VrefDQ level =========================================================================================//
//====================================================================================================================//
void training_level_vrefdq(map<int, int>& iTrainingResult)
{

    iTrainingResult.clear();
    InitLabel il;

    //================================================================================
    // Physical Waveform Index for logical 0/1
    //================================================================================
    int    iWF0_Address = 0x0;    // 0 "0000"  "0000"  "...."   0000
    int    iWF1_Address = 0xf;    // f "1111"  "0000"  "...."   1111

    //================================================================================
    // Extract Vector# for VrefDQ in training init label
    //================================================================================
    vector<int> iVecNumInitArray;
    iVecNumInitArray.push_back(il.get_vecnum_training_mr6_common_vrefdq_enable());
    iVecNumInitArray.push_back(il.get_vecnum_training_mr6_common_vrefdq_set());
    iVecNumInitArray.push_back(il.get_vecnum_training_mr6_common_vrefdq_disable());

    //================================================================================
    // Functional test with VrefDQ code loop
    //================================================================================
    map<int, map<int, int> > iResult;
    for(int i=0x00; i<0x33; i++){

        vector<string> sDataArray;
        sDataArray.push_back(int2str(i));
        sDataArray.push_back(int2str(i));
        sDataArray.push_back(int2str(i));

        vec_label_edit_A0_A5(il.get_initlabel_training(), iVecNumInitArray, sDataArray, iWF0_Address, iWF1_Address);

        FUNCTIONAL_TEST();

        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            iResult[iSite][i] = GET_FUNCTIONAL_RESULT();
        FOR_EACH_SITE_END();

    }

    //================================================================================
    // Extract functional test results with VrefDQ code loop
    //================================================================================
    for(int i=0x00; i<0x33; i++){
        cout << "0x" << setfill('0') << setw(2) << right << uppercase << hex << i << setfill(' ') << dec << flush;
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            cout << " " << iResult[iSite][i] << flush;
        FOR_EACH_SITE_END();
        cout << endl;
    }

    //================================================================================
    // Calculate optimal VrefDQ code & Modify vectors with optimal VrefDQ code
    //================================================================================
    int iVrefDQ_Value_Sum = 0;
    int iCounter = 0;
    FOR_EACH_SITE_BEGIN();

        int iSite    = CURRENT_SITE_NUMBER();
        int iPassMin = -1;
        int iPassMax = -1;

        for(int i=0x00; i<0x33; i++){
            if(iResult[iSite][i] == 1){
                iPassMin = i;
                break;
            }
        }
        for(int i=0x33-1; i>=0; i--){
            if(iResult[iSite][i] == 1){
                iPassMax = i;
                break;
            }
        }
        if((iPassMin != -1)&&(iPassMax != -1)){
            int iVrefDQ_Value = int((iPassMax+iPassMin)/2);
            iVrefDQ_Value_Sum += iVrefDQ_Value;
            iCounter++;

            stringstream stVrefDQ_Value_Hex;
            stVrefDQ_Value_Hex << "0x" << setfill('0') << setw(2) << right << hex << uppercase << iVrefDQ_Value << dec ;
            cout << "stVrefDQ_Value_Hex=" << stVrefDQ_Value_Hex.str() << endl;
            SET_USER_STRING("VrefDQ_Value", stVrefDQ_Value_Hex.str());
            iTrainingResult[iSite] = iVrefDQ_Value;

        } else{
            cout << "Warning: There is no pass area sweeping VrefDQ in site" << iSite << "." << endl;
            SET_USER_STRING("VrefDQ_Value", int2str(-1));
            iTrainingResult[iSite] = -9999;
        }

    FOR_EACH_SITE_END();

    if(iCounter == 0){
        cout << "ERROR: There is no optimal VrefDQ through duts." << endl;
        FOR_EACH_SITE_BEGIN();
            SET_USER_STRING("VrefDQ_Value_Avg", int2str(-1));
        FOR_EACH_SITE_END();
    } else{
        int iVrefDQ_Value_Avg = iVrefDQ_Value_Sum/iCounter;
        stringstream stVrefDQ_Value_Avg_Hex;
        stVrefDQ_Value_Avg_Hex << "0x" << setfill('0') << setw(2) << right << hex << uppercase << iVrefDQ_Value_Avg << dec ;
        FOR_EACH_SITE_BEGIN();
            SET_USER_STRING("VrefDQ_Value_Avg", stVrefDQ_Value_Avg_Hex.str());
        FOR_EACH_SITE_END();
    }

    return;
}

//====================================================================================================================//
//===== All trainings in one function  ===============================================================================//
//====================================================================================================================//
void training_all(const string& sTrainingConfigurationFile, SEARCH_FUNC_TASK& T_CA_searchtask, SEARCH_FUNC_TASK& T_VthDQ_searchtask,  SEARCH_FUNC_TASK& T_WriteLeveling_searchtask, map<int, int>& iTrainingResult)
{
    int    iExec_Training = 0;
    string sFilePath;

    string sType_Init_Label;
    int    iFlag_MR0_Edit = 1;
    int    iFlag_MR1_Edit = 1;
    int    iFlag_MR2_Edit = 1;
    int    iFlag_MR3_Edit = 1;
    int    iFlag_MR4_Edit = 1;
    int    iFlag_MR5_Edit = 1;
    int    iFlag_MR6_Edit = 1;
    string sMR0_BurstLength;
    string sMR0_ReadBurstType;
    string sMR0_CASLatency;
    string sMR0_DLL_Reset;
    string sMR0_WriteRecovery;
    string sMR1_DLL_Enable;
    string sMR1_DriverImpedanceControl;
    string sMR1_AdditiveLatency;
    string sMR1_RTT_NOM;
    string sMR1_TDQS_Enable;
    string sMR2_CASWriteLatency;
    string sMR2_RTT_WR;
    string sMR2_WriteCRC_Enable;
    string sMR2_LP_ASR;
    string sMR3_GeardownMode;
    string sMR3_WriteCMDLatency;
    string sMR4_CALatency;
    string sMR4_SelfRefreshAbort_Enable;
    string sMR4_ReadPreamble;
    string sMR4_WritePreamble;
    string sMR5_CAParityLatency;
    string sMR5_RTT_PARK;
    string sMR5_DataMask_Enable;
    string sMR5_WriteDBI_Enable;
    string sMR5_ReadDBI_Enable;
    string sMR6_tCCD_L;
    string sMR6_VrefDQRange;
    string sMR6_VrefDQValue;
    string sMR6_PDA_VrefDQ_Enable;
    
    string sT_CA_TimingSpec;
    int    iT_CA_TimingSet;
    int    iT_CA_LevelEquation;
    int    iT_CA_LevelSpec;
    int    iT_CA_LevelSet;
    string sT_CA_Label;
    string sT_CA_SetupPin;
    string sT_CA_ResultPin;
    string sT_CA_SpecSweep;
    string sT_CA_SpecOffset;
    double dT_CA_LinStart;
    double dT_CA_LinStop;
    double dT_CA_LinStep;
    double dT_CA_BinStep;
    int    iT_CA_ReportFlag;
    map<int, double> dT_CA_Result;

    string sT_VthDQ_TimingSpec;
    int    iT_VthDQ_TimingSet;
    int    iT_VthDQ_LevelEquation;
    int    iT_VthDQ_LevelSpec;
    int    iT_VthDQ_LevelSet;
    string sT_VthDQ_Label;
    string sT_VthDQ_SetupPin;
    string sT_VthDQ_SpecSweep;
    string sT_VthDQ_SpecOffset;
    double dT_VthDQ_LinStart_Low;
    double dT_VthDQ_LinStop_Low;
    double dT_VthDQ_LinStart_High;
    double dT_VthDQ_LinStop_High;
    double dT_VthDQ_LinStep;
    double dT_VthDQ_BinStep;
    int    iT_VthDQ_ReportFlag;
    map<int, double> dT_VthDQ_Result;

    string sT_ReadDQ_TimingSpec;
    int    iT_ReadDQ_TimingSet;
    int    iT_ReadDQ_LevelEquation;
    int    iT_ReadDQ_LevelSpec;
    int    iT_ReadDQ_LevelSet;
    string sT_ReadDQ_Label;
    string sT_ReadDQ_SetupPin;
    string sT_ReadDQ_SpecOffset;
    double dT_ReadDQ_LinStart_ns;
    double dT_ReadDQ_LinStop_ns;
    double dT_ReadDQ_LinStep_ns;
    double dT_ReadDQ_BinStep_ns;
    int    iT_ReadDQ_ReportFlag;
    map<int, map<int, double> > dT_ReadDQ_Result;

    string sT_WriteLeveling_TimingSpec;
    int    iT_WriteLeveling_TimingSet;
    int    iT_WriteLeveling_LevelEquation;
    int    iT_WriteLeveling_LevelSpec;
    int    iT_WriteLeveling_LevelSet;
    string sT_WriteLeveling_Label;
    string sT_WriteLeveling_SetupPin;
    string sT_WriteLeveling_ResultPin;
    string sT_WriteLeveling_SpecSweep;
    string sT_WriteLeveling_SpecOffset;
    double dT_WriteLeveling_LinStart;
    double dT_WriteLeveling_LinStop;
    double dT_WriteLeveling_LinStep;
    double dT_WriteLeveling_BinStep;
    int    iT_WriteLeveling_ReportFlag;
    map<int, map<int, double> > dT_WriteLeveling_Result;

    string sT_VrefDQ_TimingSpec;
    int    iT_VrefDQ_TimingSet;
    int    iT_VrefDQ_LevelEquation;
    int    iT_VrefDQ_LevelSpec;
    int    iT_VrefDQ_LevelSet;
    string sT_VrefDQ_Label;
    map<int, int> iT_VrefDQ_Result;

    string sT_WriteDQ_TimingSpec;
    int    iT_WriteDQ_TimingSet;
    int    iT_WriteDQ_LevelEquation;
    int    iT_WriteDQ_LevelSpec;
    int    iT_WriteDQ_LevelSet;
    string sT_WriteDQ_Label;
    string sT_WriteDQ_SetupPin;
    string sT_WriteDQ_SpecOffset;
    double dT_WriteDQ_LinStart_ns;
    double dT_WriteDQ_LinStop_ns;
    double dT_WriteDQ_LinStep_ns;
    double dT_WriteDQ_BinStep_ns;
    int    iT_WriteDQ_ReportFlag;
    map<int, map<int, double> > dT_WriteDQ_Result;

    char     cDevDir[1000];
    GetDevPath( cDevDir );
    sFilePath = string(cDevDir) + "/" + sTrainingConfigurationFile;
    ifstream fin;
    fin.open(sFilePath.c_str(),ios::in);

    if(sTrainingConfigurationFile == ""){
    
        cout << "ERROR: Please specify " << sTrainingConfigurationFile << "." << endl;
        
    } else if(!fin){

        cout << "ERROR: File " << sFilePath << " not found" << endl;

    } else{

        int iCounter = 0;

        while(!fin.eof()){
            
            int n;
            string sLine;
            getline(fin, sLine);

            replace_str(sLine, " ", "");
            
            vector<string> sTokenizedLine;
            tokenize(sLine,sTokenizedLine,"\"");

            if((sTokenizedLine.size() == 3)&&((n = sLine.find("#")) != 0)){

                //==========================================================
                // MRS codes for trainig start-up
                //==========================================================
                if(sLine.find("MR0.BurstLength")             != string::npos){ sMR0_BurstLength             = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR0.ReadBurstType")           != string::npos){ sMR0_ReadBurstType           = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR0.CASLatency")              != string::npos){ sMR0_CASLatency              = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR0.DLL_Reset")               != string::npos){ sMR0_DLL_Reset               = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR0.WriteRecovery")           != string::npos){ sMR0_WriteRecovery           = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR1.DLL_Enable")              != string::npos){ sMR1_DLL_Enable              = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR1.DriverImpedanceControl")  != string::npos){ sMR1_DriverImpedanceControl  = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR1.AdditiveLatency")         != string::npos){ sMR1_AdditiveLatency         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR1.RTT_NOM")                 != string::npos){ sMR1_RTT_NOM                 = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR1.TDQS_Enable")             != string::npos){ sMR1_TDQS_Enable             = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR2.CASWriteLatency")         != string::npos){ sMR2_CASWriteLatency         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR2.RTT_WR")                  != string::npos){ sMR2_RTT_WR                  = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR2.WriteCRC_Enable")         != string::npos){ sMR2_WriteCRC_Enable         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR2.LP_ASR")                  != string::npos){ sMR2_LP_ASR                  = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR3.GeardownMode")            != string::npos){ sMR3_GeardownMode            = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR3.WriteCMDLatency")         != string::npos){ sMR3_WriteCMDLatency         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR4.CALatency")               != string::npos){ sMR4_CALatency               = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR4.SelfRefreshAbort_Enable") != string::npos){ sMR4_SelfRefreshAbort_Enable = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR4.ReadPreamble")            != string::npos){ sMR4_ReadPreamble            = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR4.WritePreamble")           != string::npos){ sMR4_WritePreamble           = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR5.CAParityLatency")         != string::npos){ sMR5_CAParityLatency         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR5.RTT_PARK")                != string::npos){ sMR5_RTT_PARK                = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR5.DataMask_Enable")         != string::npos){ sMR5_DataMask_Enable         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR5.WriteDBI_Enable")         != string::npos){ sMR5_WriteDBI_Enable         = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR5.ReadDBI_Enable")          != string::npos){ sMR5_ReadDBI_Enable          = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR6.tCCD_L")                  != string::npos){ sMR6_tCCD_L                  = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR6.VrefDQRange")             != string::npos){ sMR6_VrefDQRange             = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR6.VrefDQValue")             != string::npos){ sMR6_VrefDQValue             = sTokenizedLine[1]; iCounter++; }
                if(sLine.find("MR6.PDA_VrefDQ_Enable")       != string::npos){ sMR6_PDA_VrefDQ_Enable       = sTokenizedLine[1]; iCounter++; }

                //==========================================================
                // CA Timing Training
                //==========================================================
                if(sLine.find("Training_Timing_CA.primary.TimingSpec")                   != string::npos){ sT_CA_TimingSpec               = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_CA.primary.TimingSet")                    != string::npos){ iT_CA_TimingSet                = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.primary.LevelEquation")                != string::npos){ iT_CA_LevelEquation            = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.primary.LevelSpec")                    != string::npos){ iT_CA_LevelSpec                = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.primary.LevelSet")                     != string::npos){ iT_CA_LevelSet                 = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.primary.Label")                        != string::npos){ sT_CA_Label                    = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.sSetupPin")               != string::npos){ sT_CA_SetupPin                 = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.sResultPin")              != string::npos){ sT_CA_ResultPin                = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.sSpecSweep")              != string::npos){ sT_CA_SpecSweep                = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.sSpecOffset")             != string::npos){ sT_CA_SpecOffset               = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.dLinStart")               != string::npos){ dT_CA_LinStart                 = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.dLinStop")                != string::npos){ dT_CA_LinStop                  = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.dLinStep")                != string::npos){ dT_CA_LinStep                  = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.dBinStep")                != string::npos){ dT_CA_BinStep                  = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_CA.tm_parameter.iReportFlag")             != string::npos){ iT_CA_ReportFlag               = atoi(sTokenizedLine[1].c_str()); iCounter++; }

                //==========================================================
                // DQ Vth Level Training
                //==========================================================
                if(sLine.find("Training_Level_VthDQ.primary.TimingSpec")                 != string::npos){ sT_VthDQ_TimingSpec            = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Level_VthDQ.primary.TimingSet")                  != string::npos){ iT_VthDQ_TimingSet             = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.primary.LevelEquation")              != string::npos){ iT_VthDQ_LevelEquation         = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.primary.LevelSpec")                  != string::npos){ iT_VthDQ_LevelSpec             = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.primary.LevelSet")                   != string::npos){ iT_VthDQ_LevelSet              = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.primary.Label")                      != string::npos){ sT_VthDQ_Label                 = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.sSetupPin")             != string::npos){ sT_VthDQ_SetupPin              = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.sSpecSweep")            != string::npos){ sT_VthDQ_SpecSweep             = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.sSpecOffset")           != string::npos){ sT_VthDQ_SpecOffset            = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.dLinStart_Low")         != string::npos){ dT_VthDQ_LinStart_Low          = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.dLinStop_Low")          != string::npos){ dT_VthDQ_LinStop_Low           = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.dLinStart_High")        != string::npos){ dT_VthDQ_LinStart_High         = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.dLinStop_High")         != string::npos){ dT_VthDQ_LinStop_High          = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.dLinStep")              != string::npos){ dT_VthDQ_LinStep               = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.dBinStep")              != string::npos){ dT_VthDQ_BinStep               = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Level_VthDQ.tm_parameter.iReportFlag")           != string::npos){ iT_VthDQ_ReportFlag            = atoi(sTokenizedLine[1].c_str()); iCounter++; }

                //==========================================================
                // DQ Read Timing Training
                //==========================================================
                if(sLine.find("Training_Timing_ReadDQ.primary.TimingSpec")               != string::npos){ sT_ReadDQ_TimingSpec           = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.primary.TimingSet")                != string::npos){ iT_ReadDQ_TimingSet            = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.primary.LevelEquation")            != string::npos){ iT_ReadDQ_LevelEquation        = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.primary.LevelSpec")                != string::npos){ iT_ReadDQ_LevelSpec            = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.primary.LevelSet")                 != string::npos){ iT_ReadDQ_LevelSet             = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.primary.Label")                    != string::npos){ sT_ReadDQ_Label                = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.sSetupPin")           != string::npos){ sT_ReadDQ_SetupPin             = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.sSpecOffset")         != string::npos){ sT_ReadDQ_SpecOffset           = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.dLinStart_ns")        != string::npos){ dT_ReadDQ_LinStart_ns          = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.dLinStop_ns")         != string::npos){ dT_ReadDQ_LinStop_ns           = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.dLinStep_ns")         != string::npos){ dT_ReadDQ_LinStep_ns           = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.dBinStep_ns")         != string::npos){ dT_ReadDQ_BinStep_ns           = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_ReadDQ.tm_parameter.iReportFlag")         != string::npos){ iT_ReadDQ_ReportFlag           = atoi(sTokenizedLine[1].c_str()); iCounter++; }

                //==========================================================
                // DQS Writeleveling Timing Training
                //==========================================================
                if(sLine.find("Training_Timing_WriteLeveling.primary.TimingSpec")        != string::npos){ sT_WriteLeveling_TimingSpec    = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.primary.TimingSet")         != string::npos){ iT_WriteLeveling_TimingSet     = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.primary.LevelEquation")     != string::npos){ iT_WriteLeveling_LevelEquation = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.primary.LevelSpec")         != string::npos){ iT_WriteLeveling_LevelSpec     = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.primary.LevelSet")          != string::npos){ iT_WriteLeveling_LevelSet      = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.primary.Label")             != string::npos){ sT_WriteLeveling_Label         = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.sSetupPin")    != string::npos){ sT_WriteLeveling_SetupPin      = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.sResultPin")   != string::npos){ sT_WriteLeveling_ResultPin     = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.sSpecSweep")   != string::npos){ sT_WriteLeveling_SpecSweep     = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.sSpecOffset")  != string::npos){ sT_WriteLeveling_SpecOffset    = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.dLinStart")    != string::npos){ dT_WriteLeveling_LinStart      = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.dLinStop")     != string::npos){ dT_WriteLeveling_LinStop       = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.dLinStep")     != string::npos){ dT_WriteLeveling_LinStep       = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.dBinStep")     != string::npos){ dT_WriteLeveling_BinStep       = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteLeveling.tm_parameter.iReportFlag")  != string::npos){ iT_WriteLeveling_ReportFlag    = atoi(sTokenizedLine[1].c_str()); iCounter++; }

                //==========================================================
                // DQ Vref Level Training
                //==========================================================
                if(sLine.find("Training_OpCode_VrefDQ.primary.TimingSpec")               != string::npos){ sT_VrefDQ_TimingSpec           = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_OpCode_VrefDQ.primary.TimingSet")                != string::npos){ iT_VrefDQ_TimingSet            = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_OpCode_VrefDQ.primary.LevelEquation")            != string::npos){ iT_VrefDQ_LevelEquation        = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_OpCode_VrefDQ.primary.LevelSpec")                != string::npos){ iT_VrefDQ_LevelSpec            = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_OpCode_VrefDQ.primary.LevelSet")                 != string::npos){ iT_VrefDQ_LevelSet             = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_OpCode_VrefDQ.primary.Label")                    != string::npos){ sT_VrefDQ_Label                = sTokenizedLine[1];               iCounter++; }

                //==========================================================                                                                                         
                // DQ Write Timing Training
                //==========================================================
                if(sLine.find("Training_Timing_WriteDQ.primary.TimingSpec")              != string::npos){ sT_WriteDQ_TimingSpec          = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.primary.TimingSet")               != string::npos){ iT_WriteDQ_TimingSet           = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.primary.LevelEquation")           != string::npos){ iT_WriteDQ_LevelEquation       = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.primary.LevelSpec")               != string::npos){ iT_WriteDQ_LevelSpec           = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.primary.LevelSet")                != string::npos){ iT_WriteDQ_LevelSet            = atoi(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.primary.Label")                   != string::npos){ sT_WriteDQ_Label               = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.sSetupPin")          != string::npos){ sT_WriteDQ_SetupPin            = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.sSpecOffset")        != string::npos){ sT_WriteDQ_SpecOffset          = sTokenizedLine[1];               iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.dLinStart_ns")       != string::npos){ dT_WriteDQ_LinStart_ns         = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.dLinStop_ns")        != string::npos){ dT_WriteDQ_LinStop_ns          = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.dLinStep_ns")        != string::npos){ dT_WriteDQ_LinStep_ns          = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.dBinStep_ns")        != string::npos){ dT_WriteDQ_BinStep_ns          = atof(sTokenizedLine[1].c_str()); iCounter++; }
                if(sLine.find("Training_Timing_WriteDQ.tm_parameter.iReportFlag")        != string::npos){ iT_WriteDQ_ReportFlag          = atoi(sTokenizedLine[1].c_str()); iCounter++; }
            }
        }
        
        if(iCounter == 107) iExec_Training = 1;
    }
    fin.close();

    if(iExec_Training == 1){

        //==========================================================
        // Set MRS for "initialize_training"
        //==========================================================
        sType_Init_Label = "Training";
        edit_mrs_vectors(sType_Init_Label,
                         iFlag_MR0_Edit,
                         iFlag_MR1_Edit,
                         iFlag_MR2_Edit,
                         iFlag_MR3_Edit,
                         iFlag_MR4_Edit,
                         iFlag_MR5_Edit,
                         iFlag_MR6_Edit,
                         sMR0_BurstLength,
                         sMR0_ReadBurstType,
                         sMR0_CASLatency,
                         sMR0_DLL_Reset,
                         sMR0_WriteRecovery,
                         sMR1_DLL_Enable,
                         sMR1_DriverImpedanceControl,
                         sMR1_AdditiveLatency,
                         sMR1_RTT_NOM,
                         sMR1_TDQS_Enable,
                         sMR2_CASWriteLatency,
                         sMR2_RTT_WR,
                         sMR2_WriteCRC_Enable,
                         sMR2_LP_ASR,
                         sMR3_GeardownMode,
                         sMR3_WriteCMDLatency,
                         sMR4_CALatency,
                         sMR4_SelfRefreshAbort_Enable,
                         sMR4_ReadPreamble,
                         sMR4_WritePreamble,
                         sMR5_CAParityLatency,
                         sMR5_RTT_PARK,
                         sMR5_DataMask_Enable,
                         sMR5_WriteDBI_Enable,
                         sMR5_ReadDBI_Enable,
                         sMR6_tCCD_L,
                         sMR6_VrefDQRange,
                         sMR6_VrefDQValue,
                         sMR6_PDA_VrefDQ_Enable);

        //==========================================================
        // Run CA Training
        //==========================================================
        Primary.timing(SPECIFICATION(sT_CA_TimingSpec));
        Primary.timing(iT_CA_TimingSet);
        Primary.level(LEVEL_SPEC(iT_CA_LevelEquation,iT_CA_LevelSpec));
        Primary.level(iT_CA_LevelSet);
        Primary.label(sT_CA_Label);
        FLUSH();
        training_timing_ca(sT_CA_SetupPin,
                           sT_CA_ResultPin,
                           sT_CA_SpecOffset,
                           sT_CA_SpecSweep,
                           dT_CA_LinStart,
                           dT_CA_LinStop,
                           dT_CA_LinStep,
                           dT_CA_BinStep,
                           iT_CA_ReportFlag,
                           T_CA_searchtask,
                           dT_CA_Result);
        
        //==========================================================
        // Run VthDQ Training
        //==========================================================
        Primary.timing(SPECIFICATION(sT_VthDQ_TimingSpec));
        Primary.timing(iT_VthDQ_TimingSet);
        Primary.level(LEVEL_SPEC(iT_VthDQ_LevelEquation,iT_VthDQ_LevelSpec));
        Primary.level(iT_VthDQ_LevelSet);
        Primary.label(sT_VthDQ_Label);
        FLUSH();
        training_level_vthdq(sT_VthDQ_SetupPin,
                             sT_VthDQ_SpecOffset,
                             sT_VthDQ_SpecSweep,
                             dT_VthDQ_LinStart_Low,
                             dT_VthDQ_LinStop_Low,
                             dT_VthDQ_LinStart_High,
                             dT_VthDQ_LinStop_High,
                             dT_VthDQ_LinStep,
                             dT_VthDQ_BinStep,
                             iT_VthDQ_ReportFlag,
                             T_VthDQ_searchtask,
                             dT_VthDQ_Result);
        
        //==========================================================
        // Run ReadDQ Training
        //==========================================================
        Primary.timing(SPECIFICATION(sT_ReadDQ_TimingSpec));
        Primary.timing(iT_ReadDQ_TimingSet);
        Primary.level(LEVEL_SPEC(iT_ReadDQ_LevelEquation,iT_ReadDQ_LevelSpec));
        Primary.level(iT_ReadDQ_LevelSet);
        Primary.label(sT_ReadDQ_Label);
        FLUSH();
        training_timing_readwrite("READ",
                                  sT_ReadDQ_SetupPin,
                                  sT_ReadDQ_SpecOffset,
                                  dT_ReadDQ_LinStart_ns,
                                  dT_ReadDQ_LinStop_ns,
                                  dT_ReadDQ_LinStep_ns,
                                  dT_ReadDQ_BinStep_ns,
                                  iT_ReadDQ_ReportFlag,
                                  dT_ReadDQ_Result);

        //==========================================================
        // Run Writeleveling Training
        //==========================================================
        Primary.timing(SPECIFICATION(sT_WriteLeveling_TimingSpec));
        Primary.timing(iT_WriteLeveling_TimingSet);
        Primary.level(LEVEL_SPEC(iT_WriteLeveling_LevelEquation,iT_WriteLeveling_LevelSpec));
        Primary.level(iT_WriteLeveling_LevelSet);
        Primary.label(sT_WriteLeveling_Label);
        FLUSH();
        training_timing_writeleveling(sT_WriteLeveling_SetupPin,
                                      sT_WriteLeveling_ResultPin,
                                      sT_WriteLeveling_SpecOffset,
                                      sT_WriteLeveling_SpecSweep,
                                      dT_WriteLeveling_LinStart,
                                      dT_WriteLeveling_LinStop,
                                      dT_WriteLeveling_LinStep,
                                      dT_WriteLeveling_BinStep,
                                      iT_WriteLeveling_ReportFlag,
                                      T_WriteLeveling_searchtask,
                                      dT_WriteLeveling_Result);

        //==========================================================
        // Run VrefDQ Training
        //==========================================================
        Primary.timing(SPECIFICATION(sT_VrefDQ_TimingSpec));
        Primary.timing(iT_VrefDQ_TimingSet);
        Primary.level(LEVEL_SPEC(iT_VrefDQ_LevelEquation,iT_VrefDQ_LevelSpec));
        Primary.level(iT_VrefDQ_LevelSet);
        Primary.label(sT_VrefDQ_Label);
        FLUSH();
        training_level_vrefdq(iT_VrefDQ_Result);

        //==========================================================
        // Set MRS & optimal VrefDQ for "initialize_pda"
        //==========================================================
        sType_Init_Label       = "PDA";
        sMR6_VrefDQValue       = "Avg";     
        sMR6_PDA_VrefDQ_Enable = "1";
        edit_mrs_vectors(sType_Init_Label,
                         iFlag_MR0_Edit,
                         iFlag_MR1_Edit,
                         iFlag_MR2_Edit,
                         iFlag_MR3_Edit,
                         iFlag_MR4_Edit,
                         iFlag_MR5_Edit,
                         iFlag_MR6_Edit,
                         sMR0_BurstLength,
                         sMR0_ReadBurstType,
                         sMR0_CASLatency,
                         sMR0_DLL_Reset,
                         sMR0_WriteRecovery,
                         sMR1_DLL_Enable,
                         sMR1_DriverImpedanceControl,
                         sMR1_AdditiveLatency,
                         sMR1_RTT_NOM,
                         sMR1_TDQS_Enable,
                         sMR2_CASWriteLatency,
                         sMR2_RTT_WR,
                         sMR2_WriteCRC_Enable,
                         sMR2_LP_ASR,
                         sMR3_GeardownMode,
                         sMR3_WriteCMDLatency,
                         sMR4_CALatency,
                         sMR4_SelfRefreshAbort_Enable,
                         sMR4_ReadPreamble,
                         sMR4_WritePreamble,
                         sMR5_CAParityLatency,
                         sMR5_RTT_PARK,
                         sMR5_DataMask_Enable,
                         sMR5_WriteDBI_Enable,
                         sMR5_ReadDBI_Enable,
                         sMR6_tCCD_L,
                         sMR6_VrefDQRange,
                         sMR6_VrefDQValue,
                         sMR6_PDA_VrefDQ_Enable);

        //==========================================================
        // Run write DQ Training
        //==========================================================
        Primary.timing(SPECIFICATION(sT_WriteDQ_TimingSpec));
        Primary.timing(iT_WriteDQ_TimingSet);
        Primary.level(LEVEL_SPEC(iT_WriteDQ_LevelEquation,iT_WriteDQ_LevelSpec));
        Primary.level(iT_WriteDQ_LevelSet);
        Primary.label(sT_WriteDQ_Label);
        FLUSH();
        training_timing_readwrite("WRITE",
                                  sT_WriteDQ_SetupPin,
                                  sT_WriteDQ_SpecOffset,
                                  dT_WriteDQ_LinStart_ns,
                                  dT_WriteDQ_LinStop_ns,
                                  dT_WriteDQ_LinStep_ns,
                                  dT_WriteDQ_BinStep_ns,
                                  iT_WriteDQ_ReportFlag,
                                  dT_WriteDQ_Result);
        
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();

            if(dT_CA_Result[iSite]     > -9990) iTrainingResult[iSite] = iTrainingResult[iSite] & 1;
            else                                iTrainingResult[iSite] = iTrainingResult[iSite] & 0;

            if(dT_VthDQ_Result[iSite]  > -9990) iTrainingResult[iSite] = iTrainingResult[iSite] & 1;
            else                                iTrainingResult[iSite] = iTrainingResult[iSite] & 0;

            if(iT_VrefDQ_Result[iSite] > -9990) iTrainingResult[iSite] = iTrainingResult[iSite] & 1;
            else                                iTrainingResult[iSite] = iTrainingResult[iSite] & 0;
            
            if(dT_ReadDQ_Result[iSite].size() == 0){
                iTrainingResult[iSite] = iTrainingResult[iSite] & 0;
            } else{
                for(int k=0; k<(int)dT_ReadDQ_Result[iSite].size(); k++){
                    if(dT_ReadDQ_Result[iSite][k] > -9990) iTrainingResult[iSite] = iTrainingResult[iSite] & 1;
                    else                                   iTrainingResult[iSite] = iTrainingResult[iSite] & 0;
                }
            }

            if(dT_WriteDQ_Result[iSite].size() == 0){
                iTrainingResult[iSite] = iTrainingResult[iSite] & 0;
            } else{
                for(int k=0; k<(int)dT_WriteDQ_Result[iSite].size(); k++){
                    if(dT_WriteDQ_Result[iSite][k] > -9990) iTrainingResult[iSite] = iTrainingResult[iSite] & 1;
                    else                                    iTrainingResult[iSite] = iTrainingResult[iSite] & 0;
                }
            }

        FOR_EACH_SITE_END();

    } else{
        
        cout << "ERROR: Training was not executed! Please check file: " << sFilePath << endl;
        
        FOR_EACH_SITE_BEGIN();
            int iSite = CURRENT_SITE_NUMBER();
            iTrainingResult[iSite] = 0;
        FOR_EACH_SITE_END();
    }
    
    return;
}

//====================================================================================================================//
//===== Init Label Info ==============================================================================================//
//====================================================================================================================//
string InitLabel::initlabel_pda;
string InitLabel::comment_pda_mr0;
string InitLabel::comment_pda_mr1;
string InitLabel::comment_pda_mr2;
string InitLabel::comment_pda_mr3;
string InitLabel::comment_pda_mr4;
string InitLabel::comment_pda_mr5;
string InitLabel::comment_pda_mr6_common_vrefdq_enable;
string InitLabel::comment_pda_mr6_common_vrefdq_set;
string InitLabel::comment_pda_mr6_common_vrefdq_disable;
string InitLabel::comment_pda_mr3_pda_enable;
string InitLabel::comment_pda_mr3_pda_disable;
string InitLabel::comment_pda_mr6_pda_vrefdq_enable;
string InitLabel::comment_pda_prefix_mr6_pda_vrefdq_set;
string InitLabel::comment_pda_prefix_mr6_pda_vrefdq_disable;

string InitLabel::initlabel_training;
string InitLabel::comment_training_mr0;
string InitLabel::comment_training_mr1;
string InitLabel::comment_training_mr2;
string InitLabel::comment_training_mr3;
string InitLabel::comment_training_mr4;
string InitLabel::comment_training_mr5;
string InitLabel::comment_training_mr6_common_vrefdq_enable;
string InitLabel::comment_training_mr6_common_vrefdq_set;
string InitLabel::comment_training_mr6_common_vrefdq_disable;

map<string, int> InitLabel::vecnum_pda;
map<string, int> InitLabel::vecnum_training;

void InitLabel::set_initlabel_pda                             (const string& sLabel)  { initlabel_pda                              = sLabel;   return; }
void InitLabel::set_comment_pda_mr0                           (const string& sComment){ comment_pda_mr0                            = sComment; return; }
void InitLabel::set_comment_pda_mr1                           (const string& sComment){ comment_pda_mr1                            = sComment; return; }
void InitLabel::set_comment_pda_mr2                           (const string& sComment){ comment_pda_mr2                            = sComment; return; }
void InitLabel::set_comment_pda_mr3                           (const string& sComment){ comment_pda_mr3                            = sComment; return; }
void InitLabel::set_comment_pda_mr4                           (const string& sComment){ comment_pda_mr4                            = sComment; return; }
void InitLabel::set_comment_pda_mr5                           (const string& sComment){ comment_pda_mr5                            = sComment; return; }
void InitLabel::set_comment_pda_mr6_common_vrefdq_enable      (const string& sComment){ comment_pda_mr6_common_vrefdq_enable       = sComment; return; }
void InitLabel::set_comment_pda_mr6_common_vrefdq_set         (const string& sComment){ comment_pda_mr6_common_vrefdq_set          = sComment; return; }
void InitLabel::set_comment_pda_mr6_common_vrefdq_disable     (const string& sComment){ comment_pda_mr6_common_vrefdq_disable      = sComment; return; }
void InitLabel::set_comment_pda_mr3_pda_enable                (const string& sComment){ comment_pda_mr3_pda_enable                 = sComment; return; }
void InitLabel::set_comment_pda_mr3_pda_disable               (const string& sComment){ comment_pda_mr3_pda_disable                = sComment; return; }
void InitLabel::set_comment_pda_mr6_pda_vrefdq_enable         (const string& sComment){ comment_pda_mr6_pda_vrefdq_enable          = sComment; return; }
void InitLabel::set_comment_pda_prefix_mr6_pda_vrefdq_set     (const string& sComment){ comment_pda_prefix_mr6_pda_vrefdq_set      = sComment; return; }
void InitLabel::set_comment_pda_prefix_mr6_pda_vrefdq_disable (const string& sComment){ comment_pda_prefix_mr6_pda_vrefdq_disable  = sComment; return; }

void InitLabel::set_initlabel_training                        (const string& sLabel)  { initlabel_training                         = sLabel;   return; }
void InitLabel::set_comment_training_mr0                      (const string& sComment){ comment_training_mr0                       = sComment; return; }
void InitLabel::set_comment_training_mr1                      (const string& sComment){ comment_training_mr1                       = sComment; return; }
void InitLabel::set_comment_training_mr2                      (const string& sComment){ comment_training_mr2                       = sComment; return; }
void InitLabel::set_comment_training_mr3                      (const string& sComment){ comment_training_mr3                       = sComment; return; }
void InitLabel::set_comment_training_mr4                      (const string& sComment){ comment_training_mr4                       = sComment; return; }
void InitLabel::set_comment_training_mr5                      (const string& sComment){ comment_training_mr5                       = sComment; return; }
void InitLabel::set_comment_training_mr6_common_vrefdq_enable (const string& sComment){ comment_training_mr6_common_vrefdq_enable  = sComment; return; }
void InitLabel::set_comment_training_mr6_common_vrefdq_set    (const string& sComment){ comment_training_mr6_common_vrefdq_set     = sComment; return; }
void InitLabel::set_comment_training_mr6_common_vrefdq_disable(const string& sComment){ comment_training_mr6_common_vrefdq_disable = sComment; return; }

string InitLabel::get_initlabel_pda()     { return initlabel_pda;      }
string InitLabel::get_initlabel_training(){ return initlabel_training; }

int InitLabel::get_vecnum_training_mr0()                      { int iRet = -1; if(vecnum_training.count(comment_training_mr0))                       iRet = vecnum_training[comment_training_mr0];                       return iRet; }
int InitLabel::get_vecnum_training_mr1()                      { int iRet = -1; if(vecnum_training.count(comment_training_mr1))                       iRet = vecnum_training[comment_training_mr1];                       return iRet; }
int InitLabel::get_vecnum_training_mr2()                      { int iRet = -1; if(vecnum_training.count(comment_training_mr2))                       iRet = vecnum_training[comment_training_mr2];                       return iRet; }
int InitLabel::get_vecnum_training_mr3()                      { int iRet = -1; if(vecnum_training.count(comment_training_mr3))                       iRet = vecnum_training[comment_training_mr3];                       return iRet; }
int InitLabel::get_vecnum_training_mr4()                      { int iRet = -1; if(vecnum_training.count(comment_training_mr4))                       iRet = vecnum_training[comment_training_mr4];                       return iRet; }
int InitLabel::get_vecnum_training_mr5()                      { int iRet = -1; if(vecnum_training.count(comment_training_mr5))                       iRet = vecnum_training[comment_training_mr5];                       return iRet; }
int InitLabel::get_vecnum_training_mr6_common_vrefdq_enable() { int iRet = -1; if(vecnum_training.count(comment_training_mr6_common_vrefdq_enable))  iRet = vecnum_training[comment_training_mr6_common_vrefdq_enable];  return iRet; }
int InitLabel::get_vecnum_training_mr6_common_vrefdq_set()    { int iRet = -1; if(vecnum_training.count(comment_training_mr6_common_vrefdq_set))     iRet = vecnum_training[comment_training_mr6_common_vrefdq_set];     return iRet; }
int InitLabel::get_vecnum_training_mr6_common_vrefdq_disable(){ int iRet = -1; if(vecnum_training.count(comment_training_mr6_common_vrefdq_disable)) iRet = vecnum_training[comment_training_mr6_common_vrefdq_disable]; return iRet; }

int InitLabel::get_vecnum_pda_mr0()                           { int iRet = -1; if(vecnum_pda.count(comment_pda_mr0))                                 iRet = vecnum_pda[comment_pda_mr0];                                 return iRet; }
int InitLabel::get_vecnum_pda_mr1()                           { int iRet = -1; if(vecnum_pda.count(comment_pda_mr1))                                 iRet = vecnum_pda[comment_pda_mr1];                                 return iRet; }
int InitLabel::get_vecnum_pda_mr2()                           { int iRet = -1; if(vecnum_pda.count(comment_pda_mr2))                                 iRet = vecnum_pda[comment_pda_mr2];                                 return iRet; }
int InitLabel::get_vecnum_pda_mr3()                           { int iRet = -1; if(vecnum_pda.count(comment_pda_mr3))                                 iRet = vecnum_pda[comment_pda_mr3];                                 return iRet; }
int InitLabel::get_vecnum_pda_mr4()                           { int iRet = -1; if(vecnum_pda.count(comment_pda_mr4))                                 iRet = vecnum_pda[comment_pda_mr4];                                 return iRet; }
int InitLabel::get_vecnum_pda_mr5()                           { int iRet = -1; if(vecnum_pda.count(comment_pda_mr5))                                 iRet = vecnum_pda[comment_pda_mr5];                                 return iRet; }
int InitLabel::get_vecnum_pda_mr6_common_vrefdq_enable()      { int iRet = -1; if(vecnum_pda.count(comment_pda_mr6_common_vrefdq_enable))            iRet = vecnum_pda[comment_pda_mr6_common_vrefdq_enable];            return iRet; }
int InitLabel::get_vecnum_pda_mr6_common_vrefdq_set()         { int iRet = -1; if(vecnum_pda.count(comment_pda_mr6_common_vrefdq_set))               iRet = vecnum_pda[comment_pda_mr6_common_vrefdq_set];               return iRet; } 
int InitLabel::get_vecnum_pda_mr6_common_vrefdq_disable()     { int iRet = -1; if(vecnum_pda.count(comment_pda_mr6_common_vrefdq_disable))           iRet = vecnum_pda[comment_pda_mr6_common_vrefdq_disable];           return iRet; }
int InitLabel::get_vecnum_pda_mr3_pda_enable()                { int iRet = -1; if(vecnum_pda.count(comment_pda_mr3_pda_enable))                      iRet = vecnum_pda[comment_pda_mr3_pda_enable];                      return iRet; }
int InitLabel::get_vecnum_pda_mr3_pda_disable()               { int iRet = -1; if(vecnum_pda.count(comment_pda_mr3_pda_disable))                     iRet = vecnum_pda[comment_pda_mr3_pda_disable];                     return iRet; }
int InitLabel::get_vecnum_pda_mr6_pda_vrefdq_enable()         { int iRet = -1; if(vecnum_pda.count(comment_pda_mr6_pda_vrefdq_enable))               iRet = vecnum_pda[comment_pda_mr6_pda_vrefdq_enable];               return iRet; }

int InitLabel::get_vecnum_pda_mr6_pda_vrefdq_set(const int iCode){
    stringstream st;
    st << "0x" << setfill('0') << setw(2) << right << uppercase << hex << iCode << setfill(' ') << dec << flush;
    string sComment_pda_mr6_pda_vrefdq_set = comment_pda_prefix_mr6_pda_vrefdq_set + st.str();
    int iRet = -1;
    if(vecnum_pda.count(sComment_pda_mr6_pda_vrefdq_set)) iRet = vecnum_pda[sComment_pda_mr6_pda_vrefdq_set];
    return iRet;
}

int InitLabel::get_vecnum_pda_mr6_pda_vrefdq_disable(const int iCode){
    stringstream st;
    st << "0x" << setfill('0') << setw(2) << right << uppercase << hex << iCode << setfill(' ') << dec << flush;
    string sComment_pda_mr6_pda_vrefdq_disable = comment_pda_prefix_mr6_pda_vrefdq_disable + st.str();
    int iRet = -1;
    if(vecnum_pda.count(sComment_pda_mr6_pda_vrefdq_disable)) iRet = vecnum_pda[sComment_pda_mr6_pda_vrefdq_disable];
    return iRet;
}

void InitLabel::clean(){

    initlabel_pda                              = "";
    comment_pda_mr0                            = "";
    comment_pda_mr1                            = "";
    comment_pda_mr2                            = "";
    comment_pda_mr3                            = "";
    comment_pda_mr4                            = "";
    comment_pda_mr5                            = "";
    comment_pda_mr6_common_vrefdq_enable       = "";
    comment_pda_mr6_common_vrefdq_set          = "";
    comment_pda_mr6_common_vrefdq_disable      = "";
    comment_pda_mr3_pda_enable                 = "";
    comment_pda_mr3_pda_disable                = "";
    comment_pda_mr6_pda_vrefdq_enable          = "";
    comment_pda_prefix_mr6_pda_vrefdq_set      = "";
    comment_pda_prefix_mr6_pda_vrefdq_disable  = "";

    initlabel_training                         = "";
    comment_training_mr0                       = "";
    comment_training_mr1                       = "";
    comment_training_mr2                       = "";
    comment_training_mr3                       = "";
    comment_training_mr4                       = "";
    comment_training_mr5                       = "";
    comment_training_mr6_common_vrefdq_enable  = "";
    comment_training_mr6_common_vrefdq_set     = "";
    comment_training_mr6_common_vrefdq_disable = "";
    
    vecnum_pda.clear();
    vecnum_training.clear();
}

int InitLabel::extract_vecnum(){
    
    int iVecNum;
    int iPassCounter = 0;

    //================================================================================
    // Word search in initialize label with per dram addressability
    //================================================================================
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr0                      ); vecnum_pda[comment_pda_mr0                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr0                       << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr1                      ); vecnum_pda[comment_pda_mr1                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr1                       << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr2                      ); vecnum_pda[comment_pda_mr2                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr2                       << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr3                      ); vecnum_pda[comment_pda_mr3                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr3                       << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr4                      ); vecnum_pda[comment_pda_mr4                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr4                       << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr5                      ); vecnum_pda[comment_pda_mr5                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr5                       << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr6_common_vrefdq_enable ); vecnum_pda[comment_pda_mr6_common_vrefdq_enable ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr6_common_vrefdq_enable  << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr6_common_vrefdq_set    ); vecnum_pda[comment_pda_mr6_common_vrefdq_set    ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr6_common_vrefdq_set     << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr6_common_vrefdq_disable); vecnum_pda[comment_pda_mr6_common_vrefdq_disable] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr6_common_vrefdq_disable << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr3_pda_enable           ); vecnum_pda[comment_pda_mr3_pda_enable           ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr3_pda_enable            << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr3_pda_disable          ); vecnum_pda[comment_pda_mr3_pda_disable          ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr3_pda_disable           << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_pda, comment_pda_mr6_pda_vrefdq_enable    ); vecnum_pda[comment_pda_mr6_pda_vrefdq_enable    ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_pda_mr6_pda_vrefdq_enable     << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }

    for(int i=0x00; i<0x33; i++){
        stringstream st;
        st << "0x" << setfill('0') << setw(2) << right << uppercase << hex << i << setfill(' ') << dec << flush;

        string sComment_pda_mr6_pda_vrefdq_set     = comment_pda_prefix_mr6_pda_vrefdq_set     + st.str();
        string sComment_pda_mr6_pda_vrefdq_disable = comment_pda_prefix_mr6_pda_vrefdq_disable + st.str();
        iVecNum = get_vecnum_comment_search(initlabel_pda, sComment_pda_mr6_pda_vrefdq_set    ); vecnum_pda[sComment_pda_mr6_pda_vrefdq_set]     = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << sComment_pda_mr6_pda_vrefdq_set     << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
        iVecNum = get_vecnum_comment_search(initlabel_pda, sComment_pda_mr6_pda_vrefdq_disable); vecnum_pda[sComment_pda_mr6_pda_vrefdq_disable] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << sComment_pda_mr6_pda_vrefdq_disable << right << setw(4) << iVecNum << " vec# in " << initlabel_pda << endl; }
    }

    //================================================================================
    // Word search in initialize label for Vref training
    //================================================================================
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr0                      ); vecnum_training[comment_training_mr0                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr0                       << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr1                      ); vecnum_training[comment_training_mr1                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr1                       << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr2                      ); vecnum_training[comment_training_mr2                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr2                       << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr3                      ); vecnum_training[comment_training_mr3                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr3                       << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr4                      ); vecnum_training[comment_training_mr4                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr4                       << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr5                      ); vecnum_training[comment_training_mr5                      ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr5                       << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr6_common_vrefdq_enable ); vecnum_training[comment_training_mr6_common_vrefdq_enable ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr6_common_vrefdq_enable  << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr6_common_vrefdq_set    ); vecnum_training[comment_training_mr6_common_vrefdq_set    ] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr6_common_vrefdq_set     << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    iVecNum = get_vecnum_comment_search(initlabel_training, comment_training_mr6_common_vrefdq_disable); vecnum_training[comment_training_mr6_common_vrefdq_disable] = iVecNum; if(iVecNum != -1){ iPassCounter++; } if(reportflag){ cout << left << setw(32) << comment_training_mr6_common_vrefdq_disable << right << setw(4) << iVecNum << " vec# in " << initlabel_training << endl; }
    
    return iPassCounter;
}

