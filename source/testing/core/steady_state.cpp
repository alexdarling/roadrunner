#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "rrIniFile.h"
using namespace UnitTest;
using namespace rr;

extern RoadRunner* 		gRR;
extern string 			gSBMLModelsPath;
extern string 			gCompiler;
extern string 			gSupportCodeFolder;
extern string 			gTempFolder;

SUITE(SteadyState)
{
	TEST(AllocateRR)
	{
		if(!gRR)
		{
			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
		}

		CHECK(gRR!=NULL);
	}

	//Test that model files for the tests in this suite are present
    TEST(TEST_MODEL_FILES)
    {
    	//Populate models
        vector<string> models;
        models.push_back("TestModel_1.xml");

        string tm_folder(JoinPath(gSBMLModelsPath, "test_models"));

    	for(int i = 0 ; i < models.size(); i++)
        {
    		CHECK(FileExists(JoinPath(tm_folder, models[i])));
        }
    }

	//Test that reference data files for the tests in this suite are present
    TEST(REFERENCE_DATA)
    {
        string ref_data_folder(JoinPath(gSBMLModelsPath, "test_models"));
        ref_data_folder = JoinPath(ref_data_folder, "reference_data");
		CHECK(FileExists(JoinPath(ref_data_folder, "SSTest_1.txt")));
    }

    TEST(STEADY_STATE_CONCENTRATIONS)
	{
		if(!gRR)
		{
			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
		}
		CHECK(gRR!=NULL);

        //Load the model
        string model =  JoinPath(gSBMLModelsPath, "TestModel_1.xml");
		CHECK(gRR->loadSBMLFromFile(model));

        //Compute Steady state
        gRR->steadyState();

    	string ref_data_folder(JoinPath(gSBMLModelsPath, "test_models"));
        ref_data_folder = JoinPath(ref_data_folder, "reference_data");
        string refDataFile = JoinPath(ref_data_folder, "SSTest_1.txt");
        //Read in the reference data, as a ini file
        IniFile aFile(refDataFile, true);

		if(aFile.SectionExists("STEADY_STATE_CONCENTRATIONS"))
        {
        	rrIniSection* aSection = aFile.GetSection("STEADY_STATE_CONCENTRATIONS");

            for(int i = 0 ; i < aSection->KeyCount(); i++)
            {
	            rrIniKey *aKey = aSection->GetKey(i);
            	double val = gRR->getValue(aKey->mKey);

	            //Check concentrations
                CHECK_CLOSE(aKey->AsFloat(), val, 1e-6);
            }

        }
    }

    TEST(JACOBIAN)
	{
      	CHECK(false);
    }

    TEST(EIGEN_VALUES)
	{
      	CHECK(false);
    }

    TEST(STOICHIOMETRY_MATRIX)
	{
      	CHECK(false);
    }

    TEST(LINK_MATRIX)
	{
    	CHECK(false);
    }

    TEST(UNSCALED_ELASTICITY_MATRIX)
	{
      	CHECK(false);
    }

    TEST(SCALED_ELASTICITY_MATRIX)
	{
      	CHECK(false);
    }

    TEST(UNSCALED_CONCENTRATION_CONTROL_MATRIX)
	{
      	CHECK(false);
    }

    TEST(UNSCALED_FLUX_CONTROL_MATRIX)
    {
      	CHECK(false);
    }

//    TEST(SS_SYMBOLS)
//    {
//        string model =  JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");
//        gRR->ComputeAndAssignConservationLaws(true);
//		CHECK(gRR->loadSBMLFromFile(model));
//    	NewArrayList aList = gRR->getAvailableSteadyStateSymbols();
//
//        string symbols = aList.AsString();
//		string expectedSymbols = "{{\"Floating Species\",{\"S1\",\"S3\",\"S2\"}},{\"Boundary Species\",{\"Xo\",\"X1\"}},{\"Floating Species (amount)\",{\"[S1]\",\"[S3]\",\"[S2]\"}},{\"Boundary Species (amount)\",{\"[Xo]\",\"[X1]\"}},{\"Global Parameters\",{\"k1\",\"k2\",\"k3\",\"k4\"}},{\"Volumes\",{\"compartment\"}},{\"Fluxes\",{\"_J1\",\"_J2\",\"_J3\",\"_J4\"}},{\"Flux Control Coefficients\",{{\"_J1\",{\"CC:_J1,k1\",\"CC:_J1,k2\",\"CC:_J1,k3\",\"CC:_J1,k4\",\"CC:_J1,Xo\",\"CC:_J1,X1\"}},{\"_J2\",{\"CC:_J2,k1\",\"CC:_J2,k2\",\"CC:_J2,k3\",\"CC:_J2,k4\",\"CC:_J2,Xo\",\"CC:_J2,X1\"}},{\"_J3\",{\"CC:_J3,k1\",\"CC:_J3,k2\",\"CC:_J3,k3\",\"CC:_J3,k4\",\"CC:_J3,Xo\",\"CC:_J3,X1\"}},{\"_J4\",{\"CC:_J4,k1\",\"CC:_J4,k2\",\"CC:_J4,k3\",\"CC:_J4,k4\",\"CC:_J4,Xo\",\"CC:_J4,X1\"}}}},{\"Concentration Control Coefficients\",{{\"S1\",{\"CC:S1,k1\",\"CC:S1,k2\",\"CC:S1,k3\",\"CC:S1,k4\",\"CC:S1,Xo\",\"CC:S1,X1\"}},{\"S3\",{\"CC:S3,k1\",\"CC:S3,k2\",\"CC:S3,k3\",\"CC:S3,k4\",\"CC:S3,Xo\",\"CC:S3,X1\"}},{\"S2\",{\"CC:S2,k1\",\"CC:S2,k2\",\"CC:S2,k3\",\"CC:S2,k4\",\"CC:S2,Xo\",\"CC:S2,X1\"}}}},{\"Unscaled Concentration Control Coefficients\",{{\"S1\",{\"uCC:S1,k1\",\"uCC:S1,k2\",\"uCC:S1,k3\",\"uCC:S1,k4\",\"uCC:S1,Xo\",\"uCC:S1,X1\"}},{\"S3\",{\"uCC:S3,k1\",\"uCC:S3,k2\",\"uCC:S3,k3\",\"uCC:S3,k4\",\"uCC:S3,Xo\",\"uCC:S3,X1\"}},{\"S2\",{\"uCC:S2,k1\",\"uCC:S2,k2\",\"uCC:S2,k3\",\"uCC:S2,k4\",\"uCC:S2,Xo\",\"uCC:S2,X1\"}}}},{\"Elasticity Coefficients\",{{\"_J1\",{\"EE:_J1,S1\",\"EE:_J1,S3\",\"EE:_J1,S2\",\"EE:_J1,Xo\",\"EE:_J1,X1\",\"EE:_J1,k1\",\"EE:_J1,k2\",\"EE:_J1,k3\",\"EE:_J1,k4\"}},{\"_J2\",{\"EE:_J2,S1\",\"EE:_J2,S3\",\"EE:_J2,S2\",\"EE:_J2,Xo\",\"EE:_J2,X1\",\"EE:_J2,k1\",\"EE:_J2,k2\",\"EE:_J2,k3\",\"EE:_J2,k4\"}},{\"_J3\",{\"EE:_J3,S1\",\"EE:_J3,S3\",\"EE:_J3,S2\",\"EE:_J3,Xo\",\"EE:_J3,X1\",\"EE:_J3,k1\",\"EE:_J3,k2\",\"EE:_J3,k3\",\"EE:_J3,k4\"}},{\"_J4\",{\"EE:_J4,S1\",\"EE:_J4,S3\",\"EE:_J4,S2\",\"EE:_J4,Xo\",\"EE:_J4,X1\",\"EE:_J4,k1\",\"EE:_J4,k2\",\"EE:_J4,k3\",\"EE:_J4,k4\"}}}},{\"Unscaled Elasticity Coefficients\",{{\"_J1\",{\"uEE:_J1,S1\",\"uEE:_J1,S3\",\"uEE:_J1,S2\",\"uEE:_J1,Xo\",\"uEE:_J1,X1\",\"uEE:_J1,k1\",\"uEE:_J1,k2\",\"uEE:_J1,k3\",\"uEE:_J1,k4\"}},{\"_J2\",{\"uEE:_J2,S1\",\"uEE:_J2,S3\",\"uEE:_J2,S2\",\"uEE:_J2,Xo\",\"uEE:_J2,X1\",\"uEE:_J2,k1\",\"uEE:_J2,k2\",\"uEE:_J2,k3\",\"uEE:_J2,k4\"}},{\"_J3\",{\"uEE:_J3,S1\",\"uEE:_J3,S3\",\"uEE:_J3,S2\",\"uEE:_J3,Xo\",\"uEE:_J3,X1\",\"uEE:_J3,k1\",\"uEE:_J3,k2\",\"uEE:_J3,k3\",\"uEE:_J3,k4\"}},{\"_J4\",{\"uEE:_J4,S1\",\"uEE:_J4,S3\",\"uEE:_J4,S2\",\"uEE:_J4,Xo\",\"uEE:_J4,X1\",\"uEE:_J4,k1\",\"uEE:_J4,k2\",\"uEE:_J4,k3\",\"uEE:_J4,k4\"}}}},{\"Eigenvalues\",{\"eigen_S1\",\"eigen_S3\",\"eigen_S2\"}}}";
//        CHECK_EQUAL(expectedSymbols, symbols);
//    }

}


