/**************************************************
 * Sets the volume dimensions for the fit.
 * Inputs: number of PMTs and arrays of PMT x, y and z positions;
 * 		   dPMT set fit_parameters.
 * Outputs: volume dimensions rmax, zmax.
 * Notes: rmax and zmax are 50 cm in from inner PMTs.
 *
 * NB arrays of PMT x, y and z positions should be handled by
 * a setup function (in the main program or during optimisation?)
 * 
 *
 * *************************************************/
#include <iostream>
#include <math.h>

#include <libconstants.hpp>
#include <libgeometry.hpp>

//libGeometry constructor
libGeometry::libGeometry()
	{
		SetGeometry(numPMTs, pmtx, pmty, pmtz);
	}

//TODO decide what the destructor should do
//libGeometry::~libGeometry()
//    {
//        delete rmax;
//		delete zmax;
//    }

//libGeometry member function
void libGeometry::SetGeometry(int num_PMTs, vector<float> pmtx, vector<float> pmty, vector<float> pmtz)
{

	numPMTs = num_PMTs;
	// Set the maximum search radius and height
	// from the maximum PMT positions
	float r2 = 0;
	float z = 0;
	for (int pmt = 0; pmt< numPMTs; pmt++){
		float r2_tmp = pmtx[pmt]*pmtx[pmt]+pmty[pmt]*pmty[pmt];
		float z_tmp =fabs(pmtz[pmt]);
        if (r2<r2_tmp) r2=r2_tmp;
        if (z<z_tmp) z=z_tmp;
	}
    
	float r = sqrt(r2);
	float dimension = 2*(r+z);//TODO why this value???
	deltaDmax = libConstants::dlim*dimension; // distance limit for removing isolated hits
	deltaTmax = libConstants::tlim*dimension/libConstants::cm_per_ns; //time limit for removing isolated hits
	tmax = 2*sqrt(r2+z*z)/libConstants::cm_per_ns;// maximum detector traversal time (across diagonal)

	// set maximum to 50 cm in from the PMT structure
	float dPMT = libConstants::dPMT;
	rmax = r - dPMT;
	zmax = z - dPMT; 

}
 
