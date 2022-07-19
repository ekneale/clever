#ifndef LIBHITSELECT_H
#define LIBHITSELECT_H

//includes
#include <vector>
#include <libhitinfo.hpp>

using namespace std;

/*
 * class Hitselect
 * removes isolated hits and those not in hit clusters
 * to output a list of hits that can be used to calculate
 * test vertices to begin the search
 *
 * Author	L.Kneale
 * Date		26/04/2022
 * Contact	e.kneale@sheffield.ac.uk
 */


class HitSelect
{


	// define the public functions and variables
	public:

		HitSelect();
		~HitSelect();
		
		
		// Main function called from outside class.
		int SelectHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz, vector<HitInfo>& hitinfo, float traverseTmax, float dTmax, float dRmax);
	
		// Principal functions which perform the hit selection and which are
		// called by the main SelectHits function.
		// (Strictly private functions but public to be available for 
		// running unit tests.)
		int RemoveIsolatedHits(int nhits_all, vector<HitInfo>& hitinfo, int& nhits_isolated_removed, float dTmax, float dRmax);	
		int GetCausallyRelatedHits(int nhits_isolated_removed, vector<HitInfo>& hitinfoi, int& nhits_causally_related, float traverseTmax);	
		int FindHitClusters(int nhits_causally_related, vector<HitInfo>& hitinfo);

		// Subsidiary functions called by the the principal functions
		// to check that two hits are related.
		// CheckCoincidence: check that two hits are not isolated from eachother
		// DeltaDistance2: calculate dt between hit pmts for use in CheckCausal
		// CheckCausal: check that two hits could have the same physical origin
		int CheckCoincidence(int i, int j, vector<HitInfo> hitinfo,float dTmax, float dRmax);
		float DeltaDistance2(int i, int j, vector<HitInfo> hitinfo);
		int CheckCausal(int i, int j, vector<HitInfo> hitinfo,float traverseTmax);	
		int FindClusterCandidate(int nhits_causally_related, int i, int j, vector<HitInfo>& hitinfo, vector<int>& cluster);

		// Post-selection hit pmts, times and charges, number of hits 
		// for use in generating starting points
		vector < float > times;
		vector < float > charges;
		int nhits;
		vector<HitInfo> hitinfo;

	// define the private functions and variables
	private:

		int minhits = 3;
		int maxhits = 2000;

		int nhits_isolated_removed;
		int nhits_causally_related;
		int nhits_all;
		
		vector <float>  times_all;
		vector <float> charges_all;
		vector <float> pmtx;
		vector <float> pmty;
		vector <float> pmtz;
		float traverseTmax;
		float dTmax;
		float dRmax;

		
};

#endif

