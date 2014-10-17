#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Poco/File.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/DOM/TreeWalker.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/MD5Engine.h"
#include "UnitTest++.h"
#include "rrc_api.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLogger.h"

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rrc;
using namespace Poco;
using namespace Poco::XML;

//using namespace Poco::XML::NodeFilter;


extern string   gTempFolder;
extern string   gTestDataFolder;
extern string   gCompiler;

bool sameInstances(RRHandle a, RRHandle b);

SUITE(ALEX_TESTS)
{
    TEST(RESET_MODELS)
    {
        RRHandle mRR1                = createRRInstanceEx (gTempFolder.c_str(), gCompiler.c_str());
        RRHandle mRR2                = createRRInstanceEx (gTempFolder.c_str(), gCompiler.c_str());
        string TestModelFileName     = joinPath(gTestDataFolder, "Test_1.xml");
        loadSBMLFromFileE (mRR1, TestModelFileName.c_str(), true);
        loadSBMLFromFileE (mRR2, TestModelFileName.c_str(), true);

        cout << "\nWords here";
        cout << "\n";

        CHECK (simulateEx (mRR1, 0, 10, 100));
        reset (mRR1);
        CHECK (sameInstances (mRR1, mRR2));
        
        CHECK (setGlobalParameterByIndex (mRR1, 0, 20));
        resetAll (mRR1);
        CHECK (sameInstances (mRR1, mRR2));

        RRVectorPtr myVec = getFloatingSpeciesInitialConcentrations (mRR1);
        CHECK (setVectorElement (myVec, 0, 10));
        resetToOrigin (mRR1);
        CHECK (sameInstances (mRR1, mRR2));
        freeRRInstance (mRR1);
        freeRRInstance (mRR2);
    }

    TEST(GILLESPIE_MEANS_STDEV)
    {
        RRDoubleMatrix key;
        key->RSize() = 10;
        /* double key = [0.0	0.6391	0.6982	0.6604	
        0.0	0.6499	0.6776	0.6538	
        0.0	0.6481	0.6924	0.6543	
        0.0	0.6464	0.6932	0.6584	
        0.0	0.6437	0.6928	0.6609	
        0.0	0.6405	0.6965	0.6628	
        0.0	0.6394	0.6963	0.6630	
        0.0	0.6395	0.6979	0.6623	
        0.0	0.6391	0.6978	0.6630	
        0.0	0.6390	0.6966	0.6641	
        0.0	0.6391	0.6982	0.6604]; */
    }
}

bool sameInstances(RRHandle a, RRHandle b) {
        vector<char*> keys(19);
        keys[0] = "src";
        keys[1] = "waste";
        keys[2] = "Xo";
        keys[3] = "X1";
        keys[4] = "S1";
        keys[5] = "S2";
        keys[6] = "S3";
        keys[7] = "kk1";
        keys[8] = "k1";
        keys[9] = "k_1";
        keys[10] = "kk2";
        keys[11] = "kk2";
        keys[12] = "k2";
        keys[13] = "k_2";
        keys[14] = "kk3";
        keys[15] = "k3";
        keys[16] = "k_3";
        keys[17] = "kk4";
        keys[18] = "k4";
        keys[19] = "k_4";
        bool ret = true;

        for (int i = 0; i < keys.size(); i++) {
            double v1;
            double v2;
            
            getValue(a, keys[i], &v1);
            getValue(b, keys[i], &v2);
            if (v1 != v2) {
                ret = false;
                cout << keys[i] << " failed as v1 = " << v1 << " and v2 = " << v2 << "\n";
            }
        }

        RRVectorPtr i1;
        RRVectorPtr i2;
        i1 = getFloatingSpeciesInitialConcentrations(a);
        i2 = getFloatingSpeciesInitialConcentrations(b);

        if (i1->Count == i2->Count) {
            for (int i = 0; i < i1->Count; i++) {
                if (i1->Data[i] != i2->Data[i]) {
                    ret = false;
                }
            }
        } else {
            ret = false;
        }

        
        if (!ret)
            cout << "failed";
        return ret;
}