#include "SpectrometerDriver.h"
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
#include "krm_detector.hpp"
#include <stdint.h>
#include "kmkTime.h"
#include <unistd.h>

typedef char c8;
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;


using namespace std;
static char const DirectorySeparatorChar = '/';

const std::string currentDateTime() {
    time_t     now = time(0);
    char       buf[80];
    uint64_t msecRemainder = now % 1000;
    tm *timeInfo= localtime(&now);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << timeInfo->tm_year+1900 << "_";
    //ss << std::setfill('0') << std::setw(4) << timeInfo->tm_year+1601 << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_mon+1     << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_mday      << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_hour      << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_min       << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_sec       << "_";
    ss << std::setfill('0') << std::setw(3) << msecRemainder;
    //std::cout<<"\n"<<now<<" current time:"<<ss.str()<<" "<<msecRemainder<<std::endl;
    return ss.str();
}

const std::string  convert_ts_time(time_t ts) {
    tm *timeInfo= localtime(&ts);
    std::stringstream ss;
    uint64_t msecRemainder = ts % 1000;
    ss << std::setfill('0') << std::setw(4) << timeInfo->tm_year+1900 << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_mon+1     << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_mday      << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_hour      << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_min       << "_";
    ss << std::setfill('0') << std::setw(2) << timeInfo->tm_sec       << "_";
    ss << std::setfill('0') << std::setw(3) << msecRemainder;
    //std::cout<<"\n"<<ts<<" time:"<<ss.str()<<" "<<msecRemainder<<std::endl;
    return ss.str();
}

const std::string convert_mstime_ts(time_t ts) 
{
  uint64_t msecRemainder = ts % 1000;
  time_t sec_time = ts/ 1000.0;
  tm *timeInfo= localtime(&sec_time);
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(4) << timeInfo->tm_year+1900 << "_";
  ss << std::setfill('0') << std::setw(2) << timeInfo->tm_mon+1     << "_";
  ss << std::setfill('0') << std::setw(2) << timeInfo->tm_mday      << "_";
  ss << std::setfill('0') << std::setw(2) << timeInfo->tm_hour      << "_";
  ss << std::setfill('0') << std::setw(2) << timeInfo->tm_min       << "_";
  ss << std::setfill('0') << std::setw(2) << timeInfo->tm_sec       << "_";
  ss << std::setfill('0') << std::setw(3) << msecRemainder;
  
  //std::cout<<"\n"<<sec_time<<std::endl;
  //std::cout<<"\n"<<ts<<" time:"<<ss.str()<<" "<<msecRemainder<<std::endl;
  return ss.str();
}


int main(int argc, char* argv[])
{
       KrmDetector *krm = new KrmDetector(); 
       krm->parseConfigFile(krm->configFile);
       std::string test_var="KRM_1_CalibA0";
       vector<f64> calibF(3);  
       u32 numDetectors;
       krm->getValueU32("NumberOfDetectors", numDetectors);
       krm->getValueF64("KRM_1_CalibA0",calibF[0]);
       krm->getValueF64("KRM_1_CalibA1",calibF[1]);
       krm->getValueF64("KRM_1_CalibA2",calibF[2]);
       //std::cout<<"TestCONF:  "<<calibF[0]<<" "<<calibF[1]<<" "<<calibF[2]<<std::endl;
       

       FILE *fp;
       FILE *fp1;
       int rt = 5;//5 second collection time by default 
       int lt = rt;
       unsigned int detectorID = 0;
       std::vector<unsigned int> spectrumData;
       static const int LLD_threshold=54;
       spectrumData.resize(TOTAL_RESULT_CHANNELS);
       std::string datadir="/home/raspion_user/data";
       std::string outputFile1;
       std::string outputFile2;
       std::string outputFile3;
       std::string tstamp;
       std::stringstream ss;
       
       if (argc < 2) {
	 std::cerr << "Usage: " << argv[0] << " Collection Time (s) OutFileName" << std::endl;
	return 1;
       }
       if (atoi(argv[1]) > 0) {
	   std::cout<<"Collection Time:"<<atoi(argv[1])<<" s"<<std::endl;
	   rt = atoi(argv[1]);
	   lt = rt;
	 }
       
       // Initialise the library with no error callback (not recommended)
       krm->DetInitialise();
       detectorID=krm->ListDetectors(detectorID);

       tstamp=currentDateTime();
       usleep(2000);//2s sleep
	if (argc == 3)
	 {
	   ss << datadir <<"/Beacon_"<<argv[2];
	   outputFile1 = ss.str()+ ".sp";
	   outputFile2=ss.str() + ".spe";
	   outputFile3 =ss.str() + ".txt";

	   //outputFile = ss.str();
	   //ss <<"e"; 
	   //outputFile1 =ss.str();
	 }
       else{
	 ss << datadir << "/Beacon_"<<tstamp.c_str();
	 outputFile1 = ss.str()+ ".sp";
	 //ss <<".sp"; 
	 outputFile2=ss.str() + ".spe";
	 //ss <<"e"; 
	 outputFile3 =ss.str() + ".txt";
       }
       
       int nameLength = 200;
       unsigned int totalCounts = 0;
       unsigned int realTime = 0;
       unsigned int liveTime  = 0;	
       fp = fopen(outputFile1.c_str(),"w");
       fp1 = fopen(outputFile2.c_str(),"w");
       krm->SetLLDThreshold(detectorID,LLD_threshold);

       auto start = std::chrono::system_clock::now();
       int64_t tss =0;
       //ss.str("");
       //ss.clear();
       //ss << datadir << "/Beacon_"<<tstamp.c_str() <<".txt";
       //std::string outputTimeData = ss.str();
       std::cout<<"Output FileNames: "<<outputFile1<<" "<<outputFile2<<" "<<outputFile3<<std::endl;

       if (krm->BeginDataAcquisitionF(detectorID,rt,lt,&tss,calibF,outputFile3) != ERROR_OK) //arguments are set in milliseconds
	 {
	   printf("\n Unable to read from device \n");
	 }
        std::cout<<"Start Acquisition Time "<<tss<<" "<<convert_mstime_ts(tss)<<std::endl;
 

       printf("Acquiring Data for %8.1f s\n", (float)rt);
       while (krm->AcquireData(detectorID)) ;
       auto end = std::chrono::system_clock::now();
       std::chrono::duration<double> elapsed_seconds = end-start;
       std::time_t end_time = std::chrono::system_clock::to_time_t(end);
       // Print out the total counts acquired during the acquisition
       unsigned int tC,rT,lT;
       krm->GetAcquiredData(detectorID,&spectrumData,&tC,&rT,&lT);
       //fprintf(fp,"%s %s %u %8.3f %8.3f\n", tstamp.c_str(),outputFile.c_str(),tC,rT/1000.0f, lT/1000.0f);
       fprintf(fp,"%s %s %u %8.3f %8.3f %8.6f %8.6f  %10.9f\n", tstamp.c_str(),outputFile1.c_str(),tC,rT/1000.0f, lT/1000.0f,calibF[0],calibF[1],calibF[2]);
       fprintf(fp1,"%s %s %u %8.3f %8.3f %8.6f %8.6f  %10.9f\n", tstamp.c_str(),outputFile1.c_str(),tC,rT/1000.0f, lT/1000.0f,calibF[0],calibF[1],calibF[2]);
       for (int x = 0; x < 4096; x++)
 	 {
 	   //std::cout << x<<" "<<spectrumData[x] << std::endl;
 	   fprintf(fp,"%d %d\n",x,spectrumData[x]);
	   f64 energy=(f32)x*(f32)x*calibF[2]+(f32)x*calibF[1]+calibF[0];
	   fprintf(fp1,"%8.3f %d\n",energy,spectrumData[x]);
 	 }
       
       fclose(fp);
       fclose(fp1);
       //printf("\nTotal Counts: %u RealTime: %6.1f LiveTime: %6.1f\n", tC, rT/1000.0f, lT/1000.0f);
       printf("\nTotal Counts: %u RealTime: %d LiveTime: %d\n", tC, rT, lT);

       std::cout << "finished computation at " << std::ctime(&end_time)
		 << "elapsed time: " << elapsed_seconds.count() << "s\n";

    // Cleanup the library
    
	kr_Destruct();
	return 0;
}
