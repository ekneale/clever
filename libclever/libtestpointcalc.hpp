#ifndef LIBTESTPOINTCALC_H
#define LIBTESTPOINTCALC_H

//includes
#include <vector>
#include <libhitinfo.hpp>

using namespace std;

/*
 * class TestPointCalc
 * Takes the list of clustered hits output from SelectHits and 
 * calculates test vertices to begin the search
 *
 * Author	L.Kneale
 * Date		26/04/2022
 * Contact	e.kneale@sheffield.ac.uk
 */


class TestPointCalc
{


	// define the public functions and variables
	public:

		TestPointCalc();
		~TestPointCalc();
		
		vector<HitInfo> hitinfo;
		vector<vector<float>> testpoints;
//		vector<vector<float>> testpoints_tmp;

		// Main function called from outside class.
		int CalculateTestPoints(vector<HitInfo>& hitinfo, float zmax, float rmax2, vector<vector<float>>& testpoints);

		// Principal functions which perform the test point calculation and 
		// which are called by the main CalculateTestPoints function.
		// (Strictly private functions but public to be available for 
		// running unit tests.)
		void FrontOfPMTTestPoints(float pmtx, float pmty, float pmtz, float rmax2, float zmax, vector<vector<float>>& testpoints);
		void FourHitComboTestPoints(vector<HitInfo> hitinfo,vector<int> combos_upper_bounds, vector<vector<float>>& testpoints_tmp);


		// Subsidiary functions called by the the principal functions
		void FourHitVertex(vector<HitInfo> hitinfo, vector<int> fourhitcombo, int combo, vector<vector<float>>& testpoints_tmp);


	// define the private functions and variables
	private:

		int ncombinations;
		float zmax;
		float rmax;
		


		
};

#endif

