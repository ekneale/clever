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
		
		// Pubic functions
		int SelectHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);
		// Public for testing purposes - TODO move these to private when done
//		int RemoveIsolatedHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);	
		int RemoveIsolatedHits(int nhits_all, vector<HitInfo> hitinfo);	
		int CheckCoincidence(int i, int j, vector<HitInfo> hitinfo);
		float DeltaDistance2(int i, int j, vector<HitInfo> hitinfo);
		int GetCausallyRelatedHits(int nhits_isolated_removed, vector<HitInfo> hitinfo);	
		int CheckCausal(int i, int j, vector<HitInfo> hitinfo);	

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
	
	

		
		//int FindClusteredHits(int nhits_causally_related, int *pmtIDs_causally_related,vector<float> times_causally_related, vector<float> charges_causally_related);

		
};

#endif

