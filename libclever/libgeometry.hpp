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
using namespace std;

class libGeometry
{


	public:

		libGeometry();

		void SetGeometry(int numPMTs, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);
	
		int numPMTs;
		vector<float> pmtx;
		vector<float> pmty;
		vector<float> pmtz;

		float rmax; //search radius
		float zmax; //search height
		float tmax; //maximum traverse time (time to cross diagonal)
		float deltaDmax; //maximum distance to reject isolated hits
		float deltaTmax; //maximum time to reject isolated hits

		// The following are the run-time 'constants' calculated as a function 
		// of the detector dimensions and accessible outside the class.

		// maximum radius of inner volume
		// libConstants::dPMT from the PMTs
		inline float search_radius(void){
			return(rmax);
		}

		// maximum height of inner volume
		// libConstants::dPMT from the PMTs
		inline float search_height(void){
			return(zmax);
		}
		
		//maximum time to cross the entire detector
		inline float max_traverse_time(void){
			return(tmax);
		}

		// maximum time between pmts
		// for rejecting isolated hits
		inline float max_pmt_deltaT(void){
			return(deltaTmax);
		}

		// maximum distance between pmts
		// for rejecting isolated hits
		inline float max_pmt_deltaD(void){
			return(deltaDmax);
		}
		
	private: 


};

#endif
