#ifndef LIBCONSTANTS_H
#define LIBCONSTANTS_H

/************************************
 * filename: libconstants.hpp
 * purpose: global constants for lib
 * 
 * class 
 * class user
 *
 * 
 * **********************************/

namespace libConstants{

	// This is where the basic constants are defined.
	// These shouldn't need changing.
	
	const float dPMT = 50; // Search volume 50 cm in from PMT radius
	const int ncheck = 13; // Number of checks to do (TODO number of 
	const float cm_per_ns = 21.8;


	/*************************************************************************/
	// Constants for HitSelect process, in which clustered hits are selected.
	
	const float dlim = 0.1785; // PMT pair maximal distance fraction TODO do this in a different way
	const float tlim = 0.1079;// PMT pair maximal time difference fraction TODO do this in a different way
	const float tres = 1.0; // PMT time resolution in ns
	const float tcoinc = 1.0; // Maximum time difference between PMT hits
	const int min_selected_hits = 4; // minimum number of hits required for reconstruction


	/************************************************************************/
	// Constants for TestPointCalc process, in which initial test vertices
	// are calculated.
	
	const int max_combo_hits = 15; // maximum number of hits from which to get
	// combinations.
	const int max_combinations = 2147483647; // maximum number of combinations
	//	(number of combinations for nsel between 277 and 278)
	const float pmtxy_fractional_distance = 0.9882; // front-of-pmt test point 
	//	wrt	side PMTs
	const float pmtz_fractional_distance = 0.989; // front-of-pmt test point
	// wrt top and bottom PMTs
	const int max_nselected = 477; // max. number of selected hits to calculate
	// 	ncombinations (defaults to max_ncombinations if nsel>max_nselected)
	const int max_ncombinations = 2147483647; // maximum number of 4-hit combos


	/************************************************************************/
	// Constants for VertexSearch.
	const float openangle = 90; // Search open angle
}
#endif
