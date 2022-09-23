/**************************************************
 * Sets the volume dimensions for the fit.
 * Inputs: number of PMTs and arrays of PMT x, y and z positions;
 * 		   distance from PMTs for search radius dimensions.
 * Outputs: volume dimensions rmax, zmax.
 * Notes: rmax and zmax are dPMT = 50 cm in from inner PMTs.
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
Geometry::Geometry()
{
	SetGeometry(numPMTs, pmtx, pmty, pmtz);
}

//Geometry::~Geometry()
//{
	//TODO decide what the destructor should do
//}

//Geometry member function
void Geometry::SetGeometry(int num_PMTs, vector<float> pmtx, vector<float> pmty, vector<float> pmtz)
{

	numPMTs = num_PMTs;
	// Set the maximum search radius and height
	// from the maximum PMT positions
	float r2 = 0;
	float z = 0;
	for (int pmt = 0; pmt< numPMTs; pmt++)
	{
		float r2_tmp = pmtx[pmt]*pmtx[pmt]+pmty[pmt]*pmty[pmt];
		float z_tmp =fabs(pmtz[pmt]);
        if (r2<r2_tmp) r2=r2_tmp;
        if (z<z_tmp) z=z_tmp;
	}
    
	float r = sqrt(r2);
	float dimension = 2*(r+z);//TODO why this value???
	deltaRmax = libConstants::sDistanceLimitPMT*dimension; // distance limit for removing isolated hits
	deltaTmax = libConstants::sTimeLimitPMT*dimension/libConstants::sCmPerNs; //time limit for removing isolated hits
	tmax = 2*sqrt(r2+z*z)/libConstants::sCmPerNs;// maximum detector traversal time (across diagonal)

	// set maximum to 50 cm in from the PMT structure
	float dPMT = libConstants::sDPMT;
	rmax = r - dPMT;
	zmax = z - dPMT; 

}
 
