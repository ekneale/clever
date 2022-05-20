//vim :set noexpandtab tabstop=4 wrap

//includes
#include <iostream>
#include <libconstants.hpp>
#include <libhitselect.hpp>
#include <libgeometry.hpp>

/* Performs the hit selection
 * Selects causally related hits such that
 * the time for light to travel between PMTs 
 * is less than the time to travel from a 
 * common point in the detector.
 */

// ***************************
// main hit selection function
// ***************************

//constructor function
libHitSelect::libHitSelect()
{
	
	SelectHits();

	//TODO decide what the constructor should do
	//set the constants for this run
	float dlimit = libConstants::dlim;
	float tlimit = libConstants::tlim;
	float pmtres = libConstants::tres;
	float pmtcoincidence = libConstants::tcoinc;
	libGeometry geo;
	float maxt = geo.max_traverse_time();
}

//destructor function
//libhitselect::~libhitselect():selecthits(){
	//delete [] selected; // frees the selected hits array
	//TODO decide what the destructor should do
//}

//libhitselect::libhitselect():selecthits(nsets, set, geo, n_raw, cable_raw, t_raw, q_raw){
void libHitSelect::SelectHits()
{
	//dlimit = spatial limit
	//tlimit = temporal limit
	//tres = time resolution of PMTs
	//tcoincidence = maximum time difference of perfect coincidence
	//TODO can we read in the above from the constants????
	float dlimit = libConstants::dlim;
	float tlimit = libConstants::tlim;
	float pmtres = libConstants::tres;
	float pmtcoincidence = libConstants::tcoinc;
	libGeometry geo;
	float maxt = geo.max_traverse_time();
	std::cout << maxt << std::endl;
	
	// get the list of hits (hits.cc:265 hits::hits()
	// counts number of hits per charge bin and creates offset 'pointers'
	// TODO get_all_hits() function goes here
	
	// reject temporally and spatially isolated hits (hitsel.cc:313 hitsel::mrclean)
	// makes new list of hits without isolated hits
	// TODO remove_isolated_hits() function goes here
	//
	// make table of causally related hits (hitsel.cc:28 hitsel:make_causal_table)
	// TODO get_related_hits() function goes here (or more descriptive name)
	//
	// find clusters (hitsel.cc:422 hitsel::clus_sel)
	// finds clusters and makes a list of all PMTs with high and medium occurrence
	// TODO find_hits_in_clusters() function goes here (or more descriptive name)
	

}

// FUNCTIONS IN HITSEL.CC(?)
// make_causal_table function to create causal table (called by clus_sel)
// reduce function to remove multiple hits etc (called by clus_sel)
// function to create selection array and do hit selection (select - calls clus_sel)
// function to find centroid of PMT hits and three main axes //NOT USED
// function to find list of PMTs where each pair could be hit (clus_sel)
   // by direct light from a common vertex
// function to print cable, time, charge for each hit
