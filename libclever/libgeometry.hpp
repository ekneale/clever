#ifndef LIBGEOMETRY_H
#define LIBGEOMETRY_H

/*
 * class libGeometry
 *
 * Author   L.Kneale
 * Date     26/04/2022
 * Contact  e.kneale@sheffield.ac.uk
 */

#include <vector>

class libGeometry
{
    int numPMTs;
    float* pmtx;
    float* pmty;
    float* pmtz;

	public:

		libGeometry();

		void SetGeometry(int nPMTs, float* x, float* y, float* z);
	
		float rmax;
		float zmax;
		float tmax;

		// These are the run-time 'constants' calculated as a function of 
		// the detector dimensions.
		// Could set it up so that this sort of thing is calculated as part
		// of an optimisation step but this might make it a pain to run 
		// with many detector configs. Unless we can pass a constants file
		// on the command line. Something to consider.
		inline float search_radius(void){
			return(rmax);
		}

		inline float search_height(void){
			return(zmax);
		}

		inline float max_traverse_time(void){
			return(tmax);
		}
	    //TODO
		//define the maximum distance to neighbouring pmt
		//define the maximum time to neighbouring pmt
	
	private: 

//		float r;
//		float z;

};

#endif
