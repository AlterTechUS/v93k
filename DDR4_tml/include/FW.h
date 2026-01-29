/*****************************************************************************
*
* Verigy, Inc. Company Confidential
* (c) Copyright |2007|, Verigy, Inc., all rights reserved.
* No part of this source code may be duplicated or distributed in any form.
*
******************************************************************************/

/*****************************************
  File: FW.h

   93K Firmware Interface
   Applications Developmentent Center, Semiconductor test, 2002
   Brian Buras
 
   $RCSfile$
   $Author$
   $Revision$
   $Date$
******************************************/
/**
  \class	fwstream
    This routine is used to safely and quickly communicate with the firmware interface. The SmarTest api for FW calls requires the user to create and manage their own buffers and variable types. If an error occurs then the code can do unexpected things. The FW class has been designed to be inherently type safe to avoid these errors and problems with unpredictable code execution due to a latent bug. 
  \author	Brian Buras
  \version	$Revision$ $Date$
  \remarks  <b>Getting Started:</b>

Make sure your Test Method project is properly configured:
- \subpage integration "Integration into an application"
- \subpage makefile "Makefile specific details"

You need to insert the following lines into your source code:
\code
#include "FW.h"
\endcode
	
<b>Basic usage</b>

To send a command to the firmware interface use the fwout command similar to cout.  i.e.

\code
fwout << "DFGP? (" << pinName << ")" << endl;
\endcode
		
the result is returned in the vector fwresult.

For example the the following test method code

\code
fwout << "DFPG? (IN_PINS_PULLUP)" << endl;
cout << "Input pins are = " << fwresult[0][1] << endl;
for (int i=0;i<fwresult.size();++i) {
	for (int j=0;j<fwresult[i].size();j++) {
		cout << "fwresult[" << i << "][" << j <<"] = \"" << fwresult[i][j] << "\"\n";
	}
} 
\endcode

Would result in the following output

Input pins are EXT_POR_L,TCK,TMS,TDI,JTAG_TRST_L,CODEC_RGDT_L,CODEC_SDI 
\verbatim
fwresult[0][0] = "I" 
fwresult[0][1] = "EXT_POR_L,TCK,TMS,TDI,JTAG_TRST_L,CODEC_RGDT_L,CODEC_SDI" 
fwresult[0][2] = "IN_PINS_PULLUP" 
fwresult[1][0] = "O" 
fwresult[1][1] = "EXT_POR_L,TCK,TMS,TDI,JTAG_TRST_L,CODEC_RGDT_L,CODEC_SDI"
fwresult[1][2] = "IN_PINS_PULLUP"
\endverbatim

<b>Binary Data</b>\n
Binary data is formatted just like any other param 

\code
// print first 28 waveform indices 
fwout << "VECD? PARA,VM,0,28,(CP)\n";
for(j=0;j<fwresult[i][5].size();j++) {
	cerr << "[" << fwresult[i][5][j]+0 << "]";
}
\endcode

You can send binary data using the FWBinData wrapper:

\code
// assume char *testdat2 points to something 
char bindata[256]; 
// put stuff into bindata
fwout << "VECD PARA,VM,0,28,(CP)," << FWBinData(bindata,28) << endl;
\endcode

<b>Firmware Answer Emulation</b>\n
For offline work that requires real data, you can map certain fw
commands to a file. fwresult will return the contents of the file.

\verbatim
fwAnswerMap["VECD? PARA,VM,0,28,(@)\n"] = "/tmp/VECD.bin";

where "/tmp/VECD.bin" was created by using the following command:
$ /opt/hp93000/soc/fw/bin/hpt -e "VECD? PARA,VM,0,28,(@)" >  /tmp/VECD.bin \n
\endverbatim

<b>Parse Vector File Example</b>
\code
loadSOCFile(fstream fin) {
	FWString fw; 
	while(!fin.eof()) {
		fin >> fw;
		if(fw.command == "VECC") {
			cout << "found " << fw.length() << " bytes at " << fw.offset() << endl;
			fin.seekg(fw.offset());
			fin.getc(mybuffer,fw.length());
		} 
	} 
} 
\endcode
 */
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::map;
using std::stringstream;
using std::streampos;
using std::istream;
using std::ostream;
using std::streambuf;
using std::cerr;
using std::string;

#ifndef FW_H
#define FW_H

//using namespace std;

const unsigned int answ_buf_size = 0x100000;     // 1 megabyte 
const unsigned int cmd_buf_size = 0x100000;     // 1 megabyte 

///@cond

class BaseException {
	//--- protected constructor to prevent instance of this class ---
protected:
	BaseException(const char* type, const char* file, int line, const string& msg) {
		cerr << "\n\nA " << type << " Exception has been thrown\n"
			  << "\tin file " << file << " on line " << line
			  << "\n\n" << msg << "\n\n";
	}
};

class Exception : public BaseException {
public:
	Exception(const char* file, int line, const string& msg) :
		BaseException("Exception",file,line,msg) {}
};

class FWException : public BaseException {
public:
	FWException(const char* file, int line, const string& msg) :
		BaseException("FWExec Exception",file,line,msg) {}
};

class GPIBException : public BaseException {
public:
	GPIBException(const char* file, int line, const string& msg) :
		BaseException("GPIB Exception",file,line,msg) {}
};

class FWString {
 public:

    // construct object for file stream operations >>
    FWString():index(0),binlen(0),binaryindex(0) {
    }

    ~FWString();
	 
    /// Construct from FW command string (reply).
    // Future: change vector elems to string* to avoid copying
    FWString(const string result);
	 
    /// wrap a FW result string with FWString interface
    const char *wrap(const char* result, char *dest = 0);

    const char* getCommand() const;

    /// return pointer to bin data
    streampos offset() const{
		 return binpos;
    }
	 
    inline unsigned int blength() const{
		 return binlen;
    }
	 
    inline const char* bdata() const{
		 return params[binaryindex].c_str();
    }

    int count() { return params.size();}

    /** Return firmware result parameter specified by index
		  @param index parameter index starting from 0
		  @return parameter as string
		  Will throw an exception if out of range.
    */
    const string& operator[](int index) const;
	 
    FWString& operator=(const string& src);
	 
    /** Return param value as type
		  double d = fwres.getParam<double>(1);
		  @param index parameter index starting from 0
		  return oarameter as specified type
    */
    template<typename T> T getParam(int pindex) const {
		 stringstream istr;
		 T data = T();
		 istr << params[pindex];
		 istr >> data;      // oh thank you strstream
		 return data;
    }
	 
    // aCC v1.23 doesn't support member template specialization
    // an efficient accessor for int and double params
    int getIntParam(int) const;
    double getDoubleParam(int) const;

	// interface for std algorithms
	// I would get rid of this FWString class if I was to redesign this thing but for now just patch
	vector<string>::const_iterator begin() { return params.begin(); }
	vector<string>::const_iterator end() { return params.end(); }
	
    friend istream& operator>>(istream& str, FWString& fw); 
    friend ostream& operator<<(ostream& str, const FWString& fw); 
	 
	 
private:
    vector<string> params;
    streampos binpos;  // track position of binary data in input stream;
    int index;
    const char* parseString(const string& result, char *dest=0);
    const char* parseString(const char *, char *dest=0);
    char command[5];
    mutable unsigned int binlen;      // destructive copy.
    int binaryindex; // param index for bin data (if any)
};

/**
Replacement for streambuf. Send our formatted stream to MCD
*/
class fwstreambuf: public streambuf {
 public:
    fwstreambuf();
    ~fwstreambuf();
 private:
    void      put_buffer(void);
    
 protected:
    int       overflow(int);
    int       sync();
};
///@endcond


/** A multiple result firmware query.
Execute the given command and place formatted results into the 2nd parameter.
example:
 
\code
vector<FWResult> fwres;
fwexec("DFPN?",fwres);
for(vector<FWResult>::iterator itr = fwres.begin();itr!=fwres.end();itr++) {
    cout " pin " << (*itr)[2] << " at channelNo " << (*itr)[0] << endl;
}
\endcode

@param fwcmd firmware query string
@param fwres firmware result string
@param bindest optional pointer to pre-allocated binary data destination
*/
int fwexec(const char* fwcmd, int len, vector<FWString> &fwvec);


/**
Execute the given command and place formatted result into the 2nd parameter.
example:

\code
FWResult fwres;
fwexec("FTST?",fwres);
if(fwres[0] == "P") { // test passed}
\endcode

@param fwcmd firmware query string
@param fwres firmware result string
@param bindest optional pointer to pre-allocated binary data destination
*/
int fwexec(const string& fwcmd, FWString &fwres, char *bindest);

/** A multiple result firmware query.
Execute the given command and place formatted results into the 2nd parameter.
example:
 
\code
vector<FWResult> fwres;
fwexec("DFPN?",fwres);
for(vector<FWResult>::iterator itr = fwres.begin();itr!=fwres.end();itr++) {
    cout " pin " << (*itr)[2] << " at channelNo " << (*itr)[0] << endl;
}
\endcode

@param fwcmd firmware query string
@param fwres firmware result string
@param bindest optional pointer to pre-allocated binary data destination
*/
int fwexec(const string& fwcmd, vector<FWString> &fwvec);


// Execute a command without a result
int fwexec(const string& fwcmd);


/* Firmware stream container class
    
   Construct an object from a firwmware task result string 
   or from a file of soc firmware commands.
       
   Type-safe access to fields through template function getParam<TYPE>
   or get string fields through operator []
*/
class fwstream : public ostream {
public:
private:
	fwstream():ostream(new fwstreambuf()) {
	}
	friend fwstream& getfwstream();
};

///@cond

/// Wrapper for binary user data
struct FWBinData {
	const char *data;
	int size;
	FWBinData(const char*d, int sz):data(d),size(sz) {}
	//FWBinData(const vector<int> v):data(d),size(s) {}
};

///@endcond

ostream& operator<<(ostream& s, const FWBinData& d);

fwstream& getfwstream();

extern vector<FWString> fwresult;
extern fwstream& fwout;
extern int fwloglevel;
extern map <string, string> fwAnswerMap;

#endif
