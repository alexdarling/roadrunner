#pragma hdrstop

#include "TestEvalModel.h"
#include "TestRoadRunner.h"
#include "GetBoundarySpeciesAmountTest.h"
#include "TestEvalInitialConditions.h"
#include "TestGetSetValues.h"
#include "TestCapabilities.h"
#include "TestEvalReactionRates.h"

#include "ConfigurableTest.h"

#include "rrRoadRunner.h"

#include "Integrator.h"

#include "rrLogger.h"

#include "TestVariant.h"

#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>
#include <time.h>
#include <stdio.h>
#include <cmath>



using namespace std;
using namespace rr;

/**
 * perform a stochastic ensemble calculation.
 *
 *   :param: r an existing roadrunner obj, loaded with a mode.
 *   :param: n how many ensembles to run.
 *   :param: seed the seed value, use None to continue with previous
 *           random number stream.
 *   :param: start start time
 *   :param: stop stop time
 *   :param: ntps how many points display in the result.
 */
DoubleMatrix ensemble(RoadRunner &r, int n, unsigned long seed, double start, double stop, int npts) {

    // set the seed value of the integrator
    Integrator *itg = r.getIntegrator(SimulateOptions::GILLESPIE);
    itg->setValue("seed", seed);

    // setup the simulation
    // create a sim opt obj, loads itself with
    // default values in ctor.
    SimulateOptions o;
    o.start = start;
    o.duration = stop;
    o.steps = npts;

    // we should reset the model each time we simulate,
    // set the RESET_MODEL bit.
    o.flags |= SimulateOptions::RESET_MODEL;

    o.integrator = SimulateOptions::GILLESPIE;

    // make a result var
    DoubleMatrix result(npts+1, n+1);

    for (int i = 0; i < n; ++i) {
        const DoubleMatrix &sim = *r.simulate(&o);

        // copy result data colum
        for (int row = 0; row < npts+1; ++row) {
            result(row, i+1) = sim(row, 1);
        }
    }

    // copy result time column
    const DoubleMatrix &sim = *r.getSimulationData();

    // copy result data colum
    for (int row = 0; row < npts+1; ++row) {
        result(row, 0) = sim(row, 0);
    }

    return result;
}


void run_ensemble(const char* fname, unsigned long seed)
{
    try {
        RoadRunner r(fname);

        DoubleMatrix result = ensemble(r, 5, seed, 0, 10, 150);

        cout.precision(6);

        for(int row = 0; row < result.numRows(); ++row) {
            for(int col = 0; col < result.numCols(); ++col) {
                cout << result(row, col) << ", ";
            }
            cout << endl;
        }
    } catch (std::exception& e) {
        cout << "Error running ensemble: " << e.what() << endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "usage: llvm_testing fname num_ensembles" << endl;
        return -1;
    }

    run_ensemble(argv[1], atol(argv[2]));

    return 0;
}






