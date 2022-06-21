#ifndef LIBHITSELECT_H
#define LIBHITSELECT_H

//includes
#include <vector>
#include <algorithm>

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
		int SelectHits(int nhits_all, float *times_all, float *charges_all, float *pmtx, float *pmty, float *pmtz);
		// Public for testing purposes
		int RemoveIsolatedHits(int nhits_all, float *times_all, float *charges_all, float *pmtx, float *pmty, float *pmtz);	
		int CheckCoincidence(int i, int j, float *times_all, float *pmtx, float *pmty, float *pmtz);
		float DeltaDistance2(int i, int j, float *pmtx, float *pmty, float *pmtz);

		// Post-selection hit pmts, times and charges, number of hits 
		// for use in generating starting points
		vector < float > times;
		vector < float > charges;
		int nhits;

	// define the private functions and variables
	private:

		vector < int > pmtx_isolated_hits_removed;
		vector < int > pmty_isolated_hits_removed;
		vector < int > pmtz_isolated_hits_removed;
		vector < float > times_isolated_hits_removed;
		vector < float > charges_isolated_hits_removed;
		int nhits_isolated_hits_removed;

		vector < int > pmtx_causally_related_hits;
		vector < int > pmty_causally_related_hits;
		vector < int > pmtz_causally_related_hits;
		vector < float > times_causally_related_hits;
		vector < float > charges_causally_related_hits;
		int nhits_causally_related_hits;

		int nhits_all;
		float *times_all;
		float *charges_all;
		float *pmtx;
		float *pmty;
		float *pmtz;
		
	
		//int GetCausallyRelatedHits(int nhits_isolated_hit_removed, int *pmtID_isolated_hits_removed, float *times_isolated_hits_removed, float *charges_isolated_hits_removed);	
		
		//int FindClusteredHits(int nhits_causally_related_hits, int *pmtIDs_causally_related_hits,float *times_causally_related_hits, float *charges_causally_related_hits);

		
};

#endif

