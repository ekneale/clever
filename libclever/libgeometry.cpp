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
void libGeometry::SetGeometry(int num_PMTs, float* pmt_x, float* pmt_y, float* pmt_z)
{

	numPMTs = num_PMTs;
	pmtx = pmt_x;
	pmty = pmt_y;
	pmtz = pmt_z;
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
    
	std::vector<float> dimensions;
	// set maximum to 50 cm in from the PMT structure
	float dPMT = libConstants::dPMT;
	rmax = sqrt(r2) - dPMT;
	zmax = z - dPMT;
	tmax = 2*sqrt(r2+z*z)/libConstants::ns_to_cm;// maximum detector traversal time (across diagonal)

}
 
