#ifndef LIBHITSELECT_H
#define LIBHITSELECT_H

//includes
#include <vector>

using namespace std;

/*
 * class libHitselect
 *
 * Author	L.Kneale
 * Date		26/04/2022
 * Contact	e.kneale@sheffield.ac.uk
 */


class libHitSelect
{


	// define the public functions and variables
	public:

		libHitSelect();
		~libHitSelect();
		
		// Pubic functions
		int SelectHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);
		// Public for testing purposes - TODO move these to private when done
		int RemoveIsolatedHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);	
		int CheckCoincidence(int i, int j, vector<float> times_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);
		float DeltaDistance2(int i, int j, vector<float> pmtx, vector<float> pmty, vector<float> pmtz);
		int GetCausallyRelatedHits();	
		int CheckCausal(int i, int j);	

		// Post-selection hit pmts, times and charges, number of hits 
		// for use in generating starting points
		vector < float > times;
		vector < float > charges;
		int nhits;

	// define the private functions and variables
	private:

		int minhits = 3;
		int maxhits = 2000;

		vector<float> pmtx_isolated_removed;
		vector<float> pmty_isolated_removed;
		vector<float> pmtz_isolated_removed;
		vector<float> times_isolated_removed;
		vector<float> charges_isolated_removed;
		int nhits_isolated_removed;

		vector < float > pmtx_causally_related;
		vector < float > pmty_causally_related;
		vector < float > pmtz_causally_related;
		vector < float > times_causally_related;
		vector < float > charges_causally_related;
		int nhits_causally_related;

		int nhits_all;
		vector<float> times_all;
		vector<float> charges_all;
		vector<float> pmtx;
		vector<float> pmty;
		vector<float> pmtz;
		
	
		
		//int FindClusteredHits(int nhits_causally_related, int *pmtIDs_causally_related,vector<float> times_causally_related, vector<float> charges_causally_related);

		
};

#endif

