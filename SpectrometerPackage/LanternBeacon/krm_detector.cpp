/*  \file detector.cpp
*  \brief Implementation file defining KrmDetectorclass
*
*  \author Rustam Niyazov, <rustam@lanternuas.com>, Lantern UAS
*
*  \date 2019-08-18
*
*  \remarks
*      Copyright Lantern UAS, Inc. 2019
*      Company Proprietary 
*
*  \par    Revision History
*  \verbatim
*  *****************************************************************************
*  Date        Name                   Comment
*  *****************************************************************************
*  2019-08-18  Rustam Niyazov         Created
*  *****************************************************************************
*  \endverbatim
*/

#include "krm_detector.hpp"
#include "stdio.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <set>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

KrmDetector::KrmDetector()
{
  bSuccessfullyParsedFullFile_ = false;
}

KrmDetector::~KrmDetector()
{
}

// "Binning" Bin Definitions:
f64 bins[] =
{   30,   52 // bin  1
,   52,   70 // bin  2
,   70,   94 // bin  3
,   85,  112 // bin  4 - OVERLAP 1 (85-94)
,  112,  139 // bin  5
,  125,  147 // bin  6  - OVERLAP 2 (125-139)
,  147,  172 // bin  7
,  172,  199 // bin  8
,  199,  222 // bin  9
,  222,  248 // bin 10
,  248,  281 // bin 11
,  281,  319 // bin 12
,  299,  334 // bin 13  - OVERLAP 3 (299-319)
,  334,  380 // bin 14
,  380,  434 // bin 15
,  434,  490 // bin 16
,  490,  545 // bin 17
,  545,  609 // bin 18
,  578,  634 // bin 19  - OVERLAP 4 (578-609)
,  634,  691 // bin 20
,  691,  767 // bin 21
,  767,  850 // bin 22
,  850,  963 // bin 23
,  963, 1039 // bin 24
, 1039, 1130 // bin 25
, 1130, 1215 // bin 26
, 1215, 1287 // bin 27
, 1287, 1379 // bin 28
, 1379, 1510 // bin 29
, 1510, 1635 // bin 30
, 1635, 1895 // bin 31
, 1895, 2537 // bin 32
, 2537, 2691 // bin 33
};


void KrmDetector::checkForSpaceInVariableName(const char *variable)
{
    if (std::string::npos != std::string(variable).find_first_of(' ')) { std::cerr << "Illegal variable name (has a space): \"" << std::string(variable) << "\"\nExiting.\n"; exit(1);}
}


bool KrmDetector::getValueF32(const char *variable, f32 &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));

    if (itValues == values_.end()) return false;

    std::stringstream ss(itValues->second);
    ss >> value;
    return true;
}

bool KrmDetector::getValueF64(const char *variable, f64 &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));
    if (itValues == values_.end()) return false;

    std::stringstream ss(itValues->second);
    ss >> value;
    return true;
}

bool KrmDetector::getValueU32(const char *variable, u32 &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));
    if (itValues == values_.end()) return false;

    std::stringstream ss(itValues->second);
    ss >> value;
    return true;
}

bool KrmDetector::getValueU64(const char *variable, u64 &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));
    if (itValues == values_.end()) return false;

    std::stringstream ss(itValues->second);
    ss >> value;
    return true;
}

bool KrmDetector::getValueSTR(const char *variable, std::string &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));
    if (itValues == values_.end()) return false;
    value = itValues->second;
    if (value == "_NULL_") value = ""; // remap to empty string
    return true;
}

bool KrmDetector::getValueBool(const char *variable, bool &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));
    if (itValues == values_.end()) return false;
    char charValue = itValues->second[0];
    value = (charValue == 'y') ||
            (charValue == 'Y') ||
            (charValue == '1') ||
            (charValue == 't') ||
            (charValue == 'T');
    return true;
}

bool KrmDetector::getValueS32(const char *variable, s32 &value)
{
    checkForSpaceInVariableName(variable);
    std::map<std::string, std::string>::iterator itValues = values_.find(std::string(variable));
    if (itValues == values_.end()) return false;

    std::stringstream ss(itValues->second);
    ss >> value;
    return true;
}


bool KrmDetector::parseConfigFile(std::string &configFile)
{
    std::ifstream file(configFile.c_str(),std::ios_base::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to read configuration file: " <<  configFile << std::endl;
        return false;
    }

    std::string line("");
    line.reserve(4096);

    std::vector<std::string> expectedVariables;
    std::vector<bool>        foundVariables;

    u32 lineNumber = 1;
    while (std::getline(file,line).eof() == false)
      {
	if (line.length() > 0)
	  {
	    if (line[0] == '#') // comment, skip this line
	      {
		lineNumber++;
		continue;
	      }
	    size_t endOfVariableName = line.find(std::string(":"));
	    if (endOfVariableName == std::string::npos)
	      {
		std::cerr << "Error parsing configuration file " << configFile << " at line # " << lineNumber << ", line contents: " << line <<  std::endl;
		return false;
	      }
	    std::string variableName = line.substr(0, endOfVariableName);
	    std::string value = line.substr(endOfVariableName+1);
	    values_[variableName] = value;
         }
         lineNumber++;
      }

    return true;
}

void KrmDetector::DetInitialise()
{
std::cout<<"Detector Initialize"<<std::endl;
kr_Initialise(NULL,NULL); 
}

unsigned int KrmDetector::ListDetectors(unsigned int did)
{
unsigned int detectorID = kr_GetNextDetector(did);
std::cout<<"List"<<" DetectorID: "<<detectorID<<std::endl;
int nameLength = 200;
const unsigned int cNumberOfCharacters = 126;  // max number of characters for a USB product.
char deviceName[cNumberOfCharacters];
int bytesOut;
kr_GetDeviceName(detectorID, deviceName, cNumberOfCharacters, &bytesOut);

char manufacturer[cNumberOfCharacters];
kr_GetDeviceManufacturer(detectorID, manufacturer, cNumberOfCharacters, &bytesOut);

char serialNumber[cNumberOfCharacters];
kr_GetDeviceSerial(detectorID, serialNumber, cNumberOfCharacters, &bytesOut);

int vendorID, productID;
kr_GetDeviceVendorID(detectorID, &vendorID);
kr_GetDeviceProductID(detectorID, &productID);


//kr_GetDeviceName(detectorID, nameBuffer, nameLength, &nameLength);
printf ("Detector Found - %u %s %s SN:%s\n", detectorID, deviceName,manufacturer,serialNumber);
return detectorID;
} 

void KrmDetector::SetLLDThreshold(unsigned int dID, unsigned int Threshold)
{
  std::cout<<"SetLLDThresholds:"<<Threshold<<std::endl;
  kr_SendInt16ConfigurationCommand(dID,  HIDREPORTNUMBER_SETLLD, Threshold);
}
int KrmDetector::BeginDataAcquisition(unsigned int did, int rt, int lt)
{
int res = kr_BeginDataAcquisition(did,rt*1000,lt*1000);//in milliseconds
return res;
}

int KrmDetector::BeginDataAcquisitionF(unsigned int did, int rt, int lt,int64_t *ts_out,std::vector<f64> calibF, std::string fN)
{
  //std::cout<<"Test arguments:"<<fN<<std::endl;
  char * cfN = new char [fN.length()+1];
  std::strcpy (cfN, fN.c_str());
  int64_t ts =0;
  int res = kr_BeginDataAcquisitionF(did,rt*1000,lt*1000, &ts,calibF[0],calibF[1],calibF[2],cfN);//in milliseconds
  *ts_out=ts;
  //std::cout<<"Test arguments2:"<<ts<<" "<<calibF[0]<<std::endl;
  return res;
}

unsigned int KrmDetector::AcquireData(unsigned int did)
{
int res = kr_IsAcquiringData(did);
return res;
}

void KrmDetector::GetAcquiredData(unsigned int dID, std::vector<unsigned int> *sD, unsigned int *tC,unsigned int *rT,unsigned int *lT)
{
std::cout<<"Acqured Data"<<std::endl;
unsigned int totalCounts = 0;
unsigned int realTime = 0;
unsigned int liveTime  = 0;
std::vector<unsigned int> spectrumData;
spectrumData.resize(TOTAL_RESULT_CHANNELS);

kr_GetAcquiredData(dID, spectrumData.data(), &totalCounts, &realTime, &liveTime);
//printf("%u %6.1f %6.1f\n", totalCounts, realTime/1000.0f, liveTime/1000.0f);
*tC=totalCounts;
*rT=realTime;
*lT=liveTime;
*sD=spectrumData;
}
