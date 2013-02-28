#pragma hdrstop
#include <stdio.h>
#include <string.h>
#include "rr_c_api.h"

int main(int argc, char* argv[])
{
   //Some Declarations (has to be here because this is C)
	RRInstanceListHandle 	rrs;
    RRThreadPoolHandle 	    tpHandle;		//ThreadPool handle.. use to check when a pool of threads has finished..
    char tempFolder[1024];

	//This path should work on both windows and linux..
	char* modelFileName = "../models/test_1.xml";
    int   handleCount = 10;
    int   threadCount = 1;
    int   i;
   	char  errorBuf[2048];

	printf("Starting C program...\n");
    if(argc > 1)
    {
        handleCount = atoi(argv[1]);
        if(argc > 2)
        {
            threadCount = atoi(argv[2]);
        }
    }

    printf("Allocating %d handles and %d threads\n\n", handleCount, threadCount);
    rrs = createRRInstances(handleCount);

    if(!rrs)
    {
        printf("No handles...\n");
    }
    else
    {
	    printf("%d handles allocated succesfully..\n", handleCount);
    }

   	setLogLevel("Info");
	enableLoggingToConsole();

    strcpy(tempFolder, "../temp");
    for(i = 0; i < handleCount; i++)
    {
        if(!setTempFolder(rrs->Handle[i], tempFolder))
        {
            printf("The temp file folder \'%s\' do not exist. Exiting...\n", tempFolder);
            exit(0);
        }
    }

   	enableLoggingToFile(rrs->Handle[0]);

	//loadSBML models in threads instead
    tpHandle = loadSBMLFromFileTP(rrs, modelFileName, threadCount);

    //waitForJobs will block until all threads have finished
	//Instead, one can could check for activeJobs, i.e. non blocking (see below)
    waitForJobs(tpHandle);

    //Non blocking code waiting for threadpool to finish
//    while(true)
//    {
//        if (isWorkingOnJobs(tpHandle) == false)
//        {
//           	logMsg(3, "All jobs are done!!!\n");
//        	break;
//        }
//        else
//        {
//        }
//	}

    //Set parameters
    logMsg(clInfo, " ---------- SETTING PARAMETERS -------------");

    //Setup instances with different variables
    for(i = 0; i < handleCount; i++)
    {
        double val = 0;
        getValue(rrs->Handle[i], "k1", &val);
        setValue(rrs->Handle[i], "k1", val/(2.5*(i + 1)));
        setNumPoints(rrs->Handle[i], 10);
        setTimeEnd(rrs->Handle[i], 150);
        setTimeCourseSelectionList(rrs->Handle[i], "S1");
    }

    //Simulate
    logMsg(clInfo, " ---------- SIMULATING ---------------------");

    //Simulate them using a pool of threads..
    tpHandle = simulateTP(rrs, threadCount);
    waitForJobs(tpHandle);

  	//Write data to a file
	writeMultipleRRData(rrs, "allData.dat");

	// Cleanup
    freeRRInstances(rrs);

	if(hasError())
    {
        char* error = getLastError();
        sprintf(errorBuf, "Last error %s \n", error);
    }
	return 0;
}

#pragma link "rr_c_api.lib"

//Non blocking code waiting for threadpool to finish
//    while(true)
//    {
//		int nrOfRemainingJobs = getNumberOfRemainingJobs(tpHandle);
//        if (nrOfRemainingJobs == 0)
//        {
//           	logMsg(lInfo, "All jobs are done!!!\n");
//        	break;
//        }
//        else
//        {
//        	sprintf(errorBuf, "There are %d remaining jobs\n", nrOfRemainingJobs);
//        	logMsg(lInfo, errorBuf);
//            sleep(0.1);
//        }
//	}

