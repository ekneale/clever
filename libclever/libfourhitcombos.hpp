#ifndef LIBFOURHITCOMBOS_H
#define LIBFOURHITCOMBOS_H

//includes
#include <vector>
#include <libhitinfo.hpp>

using namespace std;

/*
 * class FourHitCombos
 * Takes the list of clustered hits output from SelectHits and 
 * find all of the combinations from which to calculate test points
 * to initiate the search.
 *
 * Author	L.Kneale
 * Date		26/04/2022
 * Contact	e.kneale@sheffield.ac.uk
 */


class FourHitCombos
{


	// define the public functions and variables
	public:

		FourHitCombos();
		~FourHitCombos();
		
		vector<HitInfo> hitinfo;
		vector<int> combos_upper_bounds;

		// Main function called from outside class.
		int GetFourHitCombos(vector<HitInfo>& hitinfo, vector<int>& combos_upper_bounds);

		// Principal functions which perform the test point calculation and 
		// which are called by the main CalculateTestPoints function.
		// (Strictly private functions but public to be available for 
		// running unit tests.)
		int FindRanges(vector<HitInfo> hitinfo, int nsel, int& ncombos, vector<int>& combos_upper_bounds);
		// Subsidiary functions called by the the principal functions
		// to check that two hits are related.
		int FindNewCombinations(vector<HitInfo> hitinfo, int nselected, float lower_bound, float upper_bound, float time_window);
		int SetNewInterval(vector<HitInfo> hitinfo, int nselected, float optimal_window, vector<int>& combos_upper_bounds);


	// define the private functions and variables
	private:

		int ncombos;
		


		
};

#endif

