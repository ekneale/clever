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

	// This is where the runtime settings are defined.
	const int useCharge = 0; // Whether or not to use charge as well as timing
	const int useAngle = 1; // Whether or not to use angular constraint on likelihood

	// This is where the basic constants are defined.
	// These shouldn't need changing.
	
	const float dPMT = 50; // Search volume 50 cm in from PMT radius
	const int ncheck = 13; // Number of checks to do (TODO)
	const float cm_per_ns = 21.8;


	/*************************************************************************/
	// Constants for HitSelect process, in which clustered hits are selected.
	
	const float dlim = 0.1785; // PMT pair maximal distance fraction TODO do this in a different way
	const float tlim = 0.1079;// PMT pair maximal time difference fraction TODO do this in a different way
	const float tres = 1.0; // PMT time resolution in ns
	const float tcoinc = 1.0; // Maximum time difference between PMT hits in ns
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
   	
	// Testpoints closer than dmin are averaged in 2 steps to reduce number 
	// of initial points. TODO do this another way e.g. with a histo???
	const float dmin2_initial = 1600.; // dmin_initial = 40 cm.
	const float dmin2 = 900.; // dmin = 30 cm.

	/************************************************************************/
	// Constants for Maximisation.
	// Set the minimum radial distances for successive searches.
	const float initial_rmin 	= 100; // cm
	const float coarse_rmin 	= 100; // cm
	const float fine_rmin 		= 50;  // cm
	//Set the maximum radial distances for successive searches.
	const float coarse_rmax 	= 50; // cm
	const float fine_rmax 		= 30; // cm
	// Set the likelihood skim fraction for successive searches.
	// TODO These control how much is removed...
	const float coarse_skim_fraction 	= 0.04;
	const float fine_skim_fraction 		= 0.06;
	const float final_skim_fraction 	= 0.08;
	// Set the likelihood skim difference for successive searches.
	// TODO These control how much is removed...
	const float coarse_dlike	= 0.4;
	const float fine_dlike		= 0.5;
	const float final_dlike 	= 0.01;
	// Set the bin width for the time-of-flight subtracted timing distribution.
	const float binwidthTTof = 6.; // ns
	const float openangle = 90; // Search open angle
}
#endif
