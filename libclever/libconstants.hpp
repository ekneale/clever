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
	const int ncheck = 13; // Number of checks to do??? number of starting points over which to iterate
	const float dlim = 0.1785; // PMT pair maximal distance fraction TODO do this in a different way
	const float tlim = 0.1079;// PMT pair maximal time difference fraction TODO do this in a different way
	const float tres = 1.0; // PMT time resolution in ns
//	static inline float const tcoinc = 1.0; // Maximum time difference for perfect PMT coincidence
	const float tcoinc = 1.0;

	// This is where the optimisable variables are defined.
	// These might depend on your detector size, medium, etc.

	//static inline float const cm_per_ns = 21.8; // Group speed of light in medium
	const float cm_per_ns = 21.8;
	const int min_selected_hits = 4; // minimum number of hits required for reconstruction
	const float openangle = 90; // Search open angle	

}
#endif
