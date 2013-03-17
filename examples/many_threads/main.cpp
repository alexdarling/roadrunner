#include <iostream>
#include "rrRoadRunner.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLoadModel.h"
#include "rrSimulate.h"
#include "rrRoadRunnerList.h"
int main(int argc, char** argv)
{
	try
    {
        LogOutput::mLogToConsole = true;

        //Create some roadrunners
        const int instanceCount 	= 5000;
        const int threadCount  		= 8;

        //Use a list of roadrunners

		const char* rootPath = "..";

		string tmpFolder = JoinPath(rootPath, "temp");
        RoadRunnerList rrs(instanceCount, tmpFolder);

        const string modelFile = JoinPath(rootPath, "models", "test_1.xml");

        //Load modelFiles..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";

        LoadModel loadModel(rrs, modelFile, threadCount);
        loadModel.waitForFinish();

      	//Set parameters
        Log(lInfo)<<" ---------- SETTING PARAMETERS -------------";

        //Setup instances with different variables
        for(int i = 0; i < instanceCount; i++)
        {
            double val = rrs[i]->getValue("k1");
            rrs[i]->setValue("k1", val/(2.5*(i + 1)));
            rrs[i]->setNumPoints(500);
            rrs[i]->setTimeEnd(150);
            rrs[i]->setTimeCourseSelectionList("S1");
        }

        //Simulate
        Log(lInfo)<<" ---------- SIMULATING ---------------------";

        //Simulate them using a pool of threads..
        Simulate simulate(rrs, threadCount);
        simulate.waitForFinish();

        //Write data to a file
        SimulationData allData;
        for(int i = instanceCount -1 ; i >-1 ; i--) //"Backwards" because bad plotting program..
        {
            RoadRunner* rr = rrs[i];
            SimulationData data = rr->getSimulationResult();
            allData.append(data);
        }

        allData.writeTo(JoinPath(rootPath, "allData.dat"));
    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

    return 0;
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#endif