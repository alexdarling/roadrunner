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


string getListOfReactionsText(const string& fName);

SUITE(ALEX_TESTS)
{
    TEST(RESET_MODELS)
    {
        RRHandle mRR1                = createRRInstanceEx (gTempFolder.c_str(), gCompiler.c_str());
        RRHandle mRR2                = createRRInstanceEx (gTempFolder.c_str(), gCompiler.c_str());
        string TestModelFileName     = joinPath(gTestDataFolder, "Test_1.xml");
        loadSBMLFromFileE (aRR1, TestModelFileName.c_str(), true);
        loadSBMLFromFile E(mRR2, TestModelFileName.c_str(), true);

        CHECK (simulateEx (aRR, 0, 10, 100));
        reset (aRR)
        CHECK (mRR1 == mRR2);
        
        CHECK (setGlobalParameterByIndex (mRR1, 0, 20));
        resetAll (mRR1);
        CHECK (mRR1 == mRR2);

        RRVectorPtr myVec = getFloatingSpeciesInitialConcentrations (mRR1);
        CHECK (setVectorElement (myVec, 0, 10));
        resetToOrigin (mRR1);
        CHECK (mRR1 == mRR2);
        freeRRInstance (mRR1);
        freeRRInstance (mRR2);
    }

    TEST(GILLESPIE_MEANS_STDEV)
    {

    }
}

string getListOfReactionsText(const string& fName)
{
        ifstream in(joinPath(gTestDataFolder, "Test_1.xml").c_str());
        InputSource src(in);
        DOMParser parser;
        AutoPtr<Document> pDoc = parser.parse(&src);
        TreeWalker it(pDoc, Poco::XML::NodeFilter::SHOW_ALL);

        Node* pNode = it.nextNode();
        string result;
        while(pNode)
        {
            clog<<pNode->nodeName()<<endl;
            string nodeID = "listOfReactions";
            if(toUpper(pNode->nodeName()) == toUpper(nodeID))
            {
                DOMWriter aWriter;
                stringstream xml;
                aWriter.writeNode(xml, pNode);
                result = xml.str();
                break;
            }
            pNode = it.nextNode();
        }

        result.erase( std::remove_if( result.begin(), result.end(), ::isspace ), result.end() );
        return result;
}