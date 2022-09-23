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
	const int sUseCharge = 0; // Whether or not to use charge as well as timing
	const int sUseAngle = 1; // Whether or not to use angular constraint on likelihood

	// This is where the basic constants are defined.
	// These shouldn't need changing.
	
	const float sDPMT = 50; // Search volume 50 cm in from PMT radius
	const int sNcheck = 13; // Number of checks to do (TODO)
	const float sCmPerNs = 21.8;


	/*************************************************************************/
	// Constants for HitSelect process, in which clustered hits are selected.
	
	const float sDistanceLimitPMT = 0.1785; // PMT pair maximal distance fraction TODO do this in a different way
	const float sTimeLimitPMT = 0.1079;// PMT pair maximal time difference fraction TODO do this in a different way
	const float sTimeResolutionPMT = 1.0; // PMT time resolution in ns
	const float sTimeCoincidencePMT = 1.0; // Maximum time difference between PMT hits in ns
	const int sSelectedHitThreshold = 4; // minimum number of hits required for reconstruction


	/************************************************************************/
	// Constants for TestPointCalc process, in which initial test vertices
	// are calculated.
	
	// Maximum number of hits from which to get combinations 	
	const int sMaximumHitsForCombos = 15;
	// Maximum number of 4-hit combos (defaults to max_ncombinations if nsel>max_nselected)
	const int sMaximumCombinations = 2147483647; 
	//	(number of combinations for nsel between 277 and 278)
	
	//	Fractional distance for front-of-pmt test point
	//	wrt	side PMTs
	const float sFractionalXYDistance = 0.9882; // front-of-pmt test point 
	// wrt top and bottom PMTs
	const float sFractionalZDistance = 0.989; // front-of-pmt test point
	const int sMaximumSelectedHits = 477; // max. number of selected hits to calculate
   	
	// Testpoints closer than dmin are averaged in 2 steps to reduce number 
	// of initial points. TODO do this another way e.g. with a histo???
//	const float sMinDistance2Initial = 1600.; // dmin_initial = 40 cm.
	const float sMinPointSeparation2 = 900.; // dmin = 30 cm.

	/************************************************************************/
	// Constants for Maximisation.
	// Set the minimum radial distances for successive searches.
	const float sCoarseRmin 	= 100; // cm
	const float sFineRmin 		= 50;  // cm
	// Set radial distances of dodecahedron vertices for successive searches.
	const float sCoarseRmax 	= 50; // cm
	const float sFineRmax 		= 30; // cm
	// Set the likelihood skim fraction for successive searches.
	// TODO This is the fraction to keep/remove?
	const float sCoarseSkimFraction 	= 0.04;
	const float sFineSkimFraction 		= 0.06;
	const float sFinalSkimFraction	 	= 0.08;
	// TODO These may need to be changed if the likelihood is normalised in 
	// a different way to BONSAI
	const float sCoarseDlike	= 0.4;
	const float sFineDlike		= 0.5;
	const float sFinalDlike 	= 0.01;
	// Set the bin width and range for the fit to the peak of the time-of-flight
	// subtracted timing distribution.
	const float sBinwidthPeakFitTTOF = 0.4; // ns
	const float sRangePeakFitTTOF = 12.0; // ns

	// Constraining angle and angular corrections for non-isotropic light.
	// TODO these were optimised for Super-Kamiokande and may need to be 
	// optimisable.
	const float sConstrainingAngle = 90.0; // degrees
	const float sPositiveAngleCorrection = 8.0; // degrees
	const float sNegativeAngleCorrection = 19.12; // degrees
}
#endif
