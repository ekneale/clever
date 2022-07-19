//vim :set noexpandtab tabstop=4 wrap

//includes
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <libconstants.hpp>
#include <libfourhitcombos.hpp>

// ************************************************************************** //
// Main CalculateVertices functions take four-hit combinations from the list of 
// clustered hits output by SelectHits and calculates points of convergence in
// 3D space. These will be used as initial test vertices to begin the search.


//constructor function
FourHitCombos::FourHitCombos()
{
	GetFourHitCombos(hitinfo,combos_upper_bounds);
}

//destructor function
FourHitCombos::~FourHitCombos()
{
	// TODO
}


int FourHitCombos::GetFourHitCombos(vector<HitInfo>& hitinfo, vector<int>& combos_upper_bounds)
{

	// Calculates vertices from four-hit combinations.
	int nselected = size(hitinfo);

	// Create a vector to store the upper bound of hit combinations for each 
	// selected hit.
	int ncombos;

	// Define allowed ranges of hit numbers using absolute timing
	// to give as close to the ideal number of combinations as possible.
	ncombos = FindRanges(hitinfo,nselected,ncombos,combos_upper_bounds);

	return(ncombos);
}

//************************************************************************** //
// These are the principal functions called by the main CalculateVertices
// function.

int FourHitCombos::FindRanges(vector<HitInfo> hitinfo, int nselected, int& ncombos, vector<int>& combos_upper_bounds)
{
	// Find the time window which gives the number of combinations closest to 
	// the optimal number of combinations within the maximum time window of 
	// the time to cross the detector. 

	// Calculate the optimal number of combinations. This is a trade-off 
	// between time and accuracy.
	// TODO what is the relevance of the expression used to find the optimal
	// number of combinations????????????????
	int optimal_n3hitcombos = libConstants::max_combo_hits*(libConstants::max_combo_hits-1)*(libConstants::max_combo_hits-2)/6.;
	float ratio = (libConstants::max_combo_hits-3)/(nselected-3);
	float optimal_ncombos = (0.5+(nselected-3)*(1+ratio*(0.25*optimal_n3hitcombos-1)));


	// If nselected < 478 (or as defined in libConstants, ncombo calculated depending
	// on what it is divisible by.
	// else max ncombos = 2147483647; max for (nselected = 477) is 2130031575)
	if (nselected <= libConstants::max_nselected)
	{
		int nselected1 = nselected-1; 
	  	int nselected2 = nselected-2;
		int nselected3 = nselected-3;
		if (nselected%4==0) nselected/=4;
		else if (nselected1%4==0) nselected1/=4;
		else if (nselected2%4==0) nselected2/=4;
		else if (nselected3%4==0) nselected3/=4;
		if (nselected%3==0) nselected/=3;
		else if (nselected1%3==0) nselected1/=3;
		else if (nselected2%3==0) nselected2/=3;
		else if (nselected3%3==0) nselected3/=3;
		if (nselected%2==0) nselected/=2;
		else if (nselected1%2==0) nselected1/=2;
		else if (nselected2%2==0) nselected2/=2;
		else if (nselected3%2==0) nselected3/=2;
		ncombos = nselected*nselected1*nselected2*nselected3;
		
		// If the number of combinations is less than the optimal number, then
		// set the ranges to include all of the hits.
		if (ncombos<optimal_ncombos)
		{
			for(int hit=0; hit<nselected-3; hit++)
			{
				combos_upper_bounds.push_back(nselected-1);
				return(ncombos);
			}
		}
    }
  	else
	{
		ncombos=libConstants::max_ncombinations; 
	}

	// Now that we have a number of combinations max_ncombos or less,
	// we need to look for the range of hits that results in a number of 
	// combinations closest to the optimal number of combinations.
	// This means dcombos should be as low as possible.

	// Set the current lower and upper bounds to the 
	// times of the first and last hits.
	float lower_bound = hitinfo[0].time;
	float upper_bound = hitinfo[nselected-1].time-lower_bound;

	// Currently, ncombos is higher than the ideal optimal_ncombos so we start
	// with a time window that is half the hit time range.
	float time_window = 0.5 * upper_bound;

	// Now we do a scan over time ranges, starting with half of the hit time
	// range.
		
	// First, find the number of combinations in the initial time range.
	ncombos = FindNewCombinations(hitinfo, nselected, lower_bound, upper_bound, time_window);	
	
	// Set min_dcombos to the current value of dcombos.
	int dcombos = abs(ncombos-optimal_ncombos);
	int min_dcombos = dcombos;

	// Set starting value for optimal window to half the intial hit time range.
	float optimal_window = time_window;

	// If the number of combinations is lower than ideal then the 
	// set the lower bound to half of the hit time range.
	if (optimal_ncombos > ncombos)
	{
		lower_bound = time_window;
	}
	// If the number of combinations is higher than the ideal, set the upper
	// bound to half of the hit time range.
	else
	{
		upper_bound = time_window;
	}

	// Iterate over time windows until the optimal hit range is found. 
	while (upper_bound-lower_bound > 0.1)
	{
		// If dcombos is large, cut the time window in half.
		if (dcombos > 0.5*optimal_ncombos)
		{
			time_window = 0.5*(lower_bound+upper_bound);
		}
		// If dcombos is small, use the first-order Taylor approximation 
		// (for time_window**3) to get the time window
		else
		{
			time_window*=(2*ncombos+optimal_ncombos)/(3*ncombos);
		}

		// Check the number of combinations using the new time window.
		ncombos = FindNewCombinations(hitinfo, nselected, lower_bound, upper_bound, time_window);

		// If we now have the ideal ncombos, return.
		if (ncombos==optimal_ncombos)
		{
			return(ncombos);
		}

		// Otherwise, reset search range if necessary.
		// If the number of combinations is too low, change the
		// lower bound for the time range to the latest time window.
		else if (optimal_ncombos > ncombos)
		{
			lower_bound = time_window;
		}
		// If the number of combinations is too high, change the
		// upper bound for the search to the new time window.
		else
		{
			upper_bound = time_window;
		}

		dcombos = abs(ncombos-optimal_ncombos);
		// Now see if we have minimised dcombos
		if (dcombos < min_dcombos)
		{
			min_dcombos = dcombos;
			optimal_window = time_window;
		}
	}

	// Set the 4-hit combination ranges to the optimal time window.
	ncombos = SetNewInterval(hitinfo, nselected, optimal_window, combos_upper_bounds);
	return(ncombos);
}

	

// ******************************************************************** //
// These are the subsidiary functions called by the principal functions 
// which are in turn called by the main CalculateVertices function.

int FourHitCombos::FindNewCombinations(vector<HitInfo> hitinfo, int nselected, float lower_bound, float upper_bound, float time_window)
{

	float time_current;
	// Use lower_bound and upper_bound to set the range over which to iterate.
	// Predicate to find the iterator for the first hit in the range 
	// (with time t >= lower_bound).
	auto first_in_range = [lower_bound](HitInfo const& h)
	{
		return h.time<lower_bound;
	};
	// Predicate to find the iterator for the upper bound.
	auto last_in_range = [upper_bound](HitInfo const& h)
	{
		return h.time<=upper_bound;
	};
	// Predicate to find the first hit that cannot be combined with current hit
	// (i.e. not between lower and upper bounds and with dt<=time_window).
	// Use this (below) to find the index of the last hit that can be combined.
	auto last_in_combo = [time_current,time_window](HitInfo const& h) 
	{
		return h.time-time_current<=time_window;
	};

	int minhits = 3; // ensure there are at least four hits i.e. one 4-hit combo
	
	// Find the iterator for the first element in the time range being scanned.
	// (Find_if_not finds the iterator of the first element to return false.)
	auto lower = find_if_not(hitinfo.begin(), hitinfo.end(), first_in_range);
	// Find the iterator for the last element in the time range being scanned.
	// This will actually point to the next element but that is resolved by 
	// other checks.
	auto upper = find_if_not(hitinfo.begin(), hitinfo.end(), last_in_range);

	// Initialise the iterator over the hits.
	auto current = hitinfo.begin();
	int ncombos = 0;

	// Loop over the hits. Stop if ncombos reaches the maximum.
	while (current != upper && ncombos < libConstants::max_ncombinations)
	{
		time_current = hitinfo[distance(hitinfo.begin(),current)].time;
		
		// Find the index of the last element in the time range 
		auto last = find_if_not(lower,upper,last_in_combo);
		
		// Find the number of hits that can be combined with the current one.
		int nhits = distance(current,last) - 1;

		// If nhits<3 there can be no combination made,
		// so move onto the next hit.
		current++;
		if (nhits < minhits)
		{
			continue;
		}

		// Calculate the number of 3-hit combinations with this starting hit 
		// and add it to the total number of 4-hit combinations.
		int n3hitcombos = nhits*(nhits-1)*(nhits-2)/6.;
		ncombos += n3hitcombos;

		// If ncombos exceeds the maximum allowed, reduce ncombos to the max.
		if (ncombos > libConstants::max_ncombinations) 
		{
			ncombos = libConstants::max_ncombinations;
		}


	} // End while loop over hits 

	return(ncombos);
}

int FourHitCombos::SetNewInterval(vector<HitInfo> hitinfo, int nselected, float optimal_window, vector<int>& combos_upper_bounds)
{
	// Saves a list of upper bounds in the ranges from which to draw the 4-hit 
	// combinations for each hit in nselected.
	int minhits = 3; // ensure there are at least four hits i.e. one 4-hit combo
	auto current = hitinfo.begin(); // Iterator over hitinfo.
	int icurrent = 0; // Index of current hit
	float time_current; 
	int ncombos;

	// Predicate to find the first hit that cannot be combined with current hit
	// (i.e. not between lower and upper bounds and with dt<=time_window).
	// Use this (below) to find the index of the last hit that can be combined.
	auto last_in_combo = [time_current,optimal_window](HitInfo const& h) 
	{
		return h.time-time_current<=optimal_window;
	};

	

	// Loop over the hitinfo vector until the end of the possible starting hits
	// (nselected - 3 when looking for 4-hit combos).
	while (current != hitinfo.end()-minhits)
	{
		// Get the time of the current hit, so we can search for all hits within
		// the range timeofcurrent+time_window.
		time_current = hitinfo[distance(hitinfo.begin(),current)].time;

		// Find the index of the last element in the time range 
		// (first that is out of range - 1).
		auto last = find_if_not(current,hitinfo.end(),last_in_combo);
		
		// Find the number of hits that can be combined with the current one.
		int nhits = distance(current,last);

		// If nhits<3 there can be no combination made,
		// so move onto the next hit.
		current++;

		if (nhits < minhits)
		{
			continue;
		}

		// Make a note of the final hit in the range from which
		// 4-hit combinations can be drawn for the current hit.
		combos_upper_bounds.push_back(distance(hitinfo.begin(),last));

		// Then calculate the number of combinations if
		// max_ncombinations has not already been exceeded.
		if (ncombos<libConstants::max_ncombinations)
		{
			// Calculate the number of 3-hit combinations possible with the 
			// current hit and add it to the total number of 4-hit combinations.
			int n3hitcombos = nhits*(nhits-1)*(nhits-2)/6.;
			ncombos += n3hitcombos;

			// If ncombos exceeds the max allowed, reduce ncombos to the max.
			if (ncombos > libConstants::max_ncombinations) 
			{
				ncombos = libConstants::max_ncombinations;
			}
		}


	} // End while loop over hits 

	return(ncombos);
}




