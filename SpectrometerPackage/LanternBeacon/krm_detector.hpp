/*  \file detector.hpp
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

#pragma once
#include "SpectrometerDriver.h"
#include <thread>
#include <string>
#include <linux/types.h>
#include "stdio.h"
#include <fstream>
#include <map>
#include <vector>

typedef char c8;
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;


class KrmDetector
{
public:
  
  unsigned int stdcall iDetector;
  int64_t StartTime1;
  int64_t StratTime2; 

  std::string configFile="/home/raspion_user/conf/lan_beacon.conf";
    
  KrmDetector();
  ~KrmDetector();
  
  bool getValueF32(const char *variable, f32 &value);
  bool getValueF64(const char *variable, f64 &value);
  bool getValueU32(const char *variable, u32 &value);
  bool getValueU64(const char *variable, u64 &value);
  bool getValueSTR(const char *variable, std::string &value);
  bool getValueBool(const char *variable, bool &value);
  bool getValueS32(const char *variable, s32 &value);

  void DetInitialise();
  bool parseConfigFile();
  bool parseConfigFile(std::string &configFile);
  unsigned int ListDetectors(unsigned int dID);
  void SetLLDThreshold(unsigned int dID, unsigned int Threshold);
  int BeginDataAcquisition(unsigned int dID, int rt, int lt);
  int BeginDataAcquisitionF(unsigned int dID, int rt, int lt, int64_t *ts, std::vector<f64> calibF, std::string fN);
  unsigned int AcquireData(unsigned int dID);
  //void GetAcquiredData(unsigned int dID,  std::vector< unsigned int> vect, unsigned int &totalCounts, unsigned int &realTime, unsigned int &liveTime);
  void GetAcquiredData(unsigned int dID, std::vector<unsigned int> *sD,unsigned int *totalCounts, unsigned int *realTime, unsigned int *liveTime);

  
private:
  size_t bytesReceived_;
  bool bSuccessfullyParsedFullFile_;
  const char **currentExpectedVariables_;
  std::map<std::string, std::string> values_;
  void checkForSpaceInVariableName(const char *variable);

};

