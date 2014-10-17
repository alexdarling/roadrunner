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

SUITE(CORE_TESTS)
{
    TEST(LOGGING)
    {
        // save log level
        int logLevel = rr::Logger::getLevel();

        RRHandle aRR = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        rr::Logger::setLevel(rr::Logger::LOG_INFORMATION);
        enableLoggingToFile(aRR);
        logMsg(CL_PRIO_INFORMATION, "A log message before closing the logger");
        disableLoggingToFile();
        logMsg(CL_PRIO_INFORMATION, "This message is not written to the logger");
        freeRRInstance(aRR);

        // restore log level
        rr::Logger::setLevel(logLevel);
    }


    TEST(RELOADING_MODEL_MODEL_RECOMPILIATION)
    {
        RRHandle aRR                 = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        string TestModelFileName     = joinPath(gTestDataFolder, "Test_1.xml");
        CHECK(fileExists(TestModelFileName));

        CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
        CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));
        freeRRInstance(aRR);
    }

    TEST(RELOADING_MODEL_NO_MODEL_RECOMPILIATION)
    {
        RRHandle aRR                 = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        string TestModelFileName     = joinPath(gTestDataFolder, "Test_1.xml");
        CHECK(fileExists(TestModelFileName));

        CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
        CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), false));
        freeRRInstance(aRR);
    }

    TEST(LOADING_MODEL_MULTIPLE_INSTANCES)
    {
        RRHandle aRR1                   = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        RRHandle aRR2                   = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        string TestModelFileName     = joinPath(gTestDataFolder, "Test_1.xml");

        CHECK(loadSBMLFromFileE(aRR1, TestModelFileName.c_str(), true));
        CHECK(loadSBMLFromFileE(aRR2, TestModelFileName.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
        CHECK(loadSBMLFromFileE(aRR1, TestModelFileName.c_str(), false));
        CHECK(loadSBMLFromFileE(aRR2, TestModelFileName.c_str(), false));

        freeRRInstance(aRR1);
        freeRRInstance(aRR2);
    }

    TEST(PARSING_MODEL_XML)
    {
        string modelXML = getListOfReactionsText(joinPath(gTestDataFolder, "Test_1.xml").c_str());
        CHECK(modelXML.size() > 0);
    }

    TEST(GENERATING_MODEL_HASH)
    {
        string content = getListOfReactionsText(joinPath(gTestDataFolder, "Test_1.xml"));
        MD5Engine md5;
        md5.update(content);
        string digestString(Poco::DigestEngine::digestToHex(md5.digest()));
        CHECK_EQUAL("d996bae1bec8f6efb81c4571aa7fc10d", digestString);
    }

    TEST(LOAD_MODEL_FROM_STRING)
    {
        string xml = getFileContent(joinPath(gTestDataFolder, "Test_1.xml"));
        RRHandle aRR1                   = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        RRHandle aRR2                   = createRRInstanceEx(gTempFolder.c_str(), gCompiler.c_str());
        CHECK(loadSBML(aRR1, xml.c_str()));
        CHECK(loadSBMLEx(aRR2, xml.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
        CHECK(loadSBMLEx(aRR1, xml.c_str(), false));
        CHECK(loadSBMLEx(aRR2, xml.c_str(), false));
        freeRRInstance(aRR1);
        freeRRInstance(aRR2);
    }

    TEST(GET_MICROSECONDS)
    {
        // make sure that the time is essentially the same as sleep time in
        // milliseconds.
        int64_t millis = 123;
        int64_t start = rr::getMicroSeconds();
        // sleep for milliseconds
        rr::sleep((int)millis);
        int64_t end = rr::getMicroSeconds();
        int64_t diff = end - start;

        cout << "microseconds, start: " << start << ", end: " << end << ", diff: " << diff 
             << ", expected sleep mu s: " << 1000 * millis 
             << ", diff between expeted and actual mu s: " << diff - (1000 * millis) << endl;

        // timer varies from system to system, but should be semi-close, like say 20%
        CHECK_CLOSE(diff, 1000 * millis, (1000 * millis) / 5 );

        // make sure its increasing
        CHECK(diff > 0);

        // now make sure that the microseconds is monotinically increasing
        const int len = 5;
        int64_t prev = rr::getMicroSeconds();
        for (int i = 0; i < len; ++i) {
            // the timer on Windows appears to be significantaly less 
            // accurate than standard Unix machines, so have to sleep 
            // for the miminal amount to time to get the alleged 'microseconds'
            // to move. Timer is also not super accurate in virtualbox. 
            rr::sleep(1);
            int64_t curr = rr::getMicroSeconds();
            CHECK(curr > prev);
            prev = curr;
        }
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





