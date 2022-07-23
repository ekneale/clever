/**********************************************************
 * Reads in the hit charge and time information from file 
 * and runs through the steps in the reconstruction.
 * *******************************************************/

#include <iostream>
#include <libhitinfo.hpp>
#include <libhitselect.hpp>
#include <libgeometry.hpp>
#include <libconstants.hpp>
#include <libtestpointcalc.hpp>

int main(){

	// Get number of PMTs in detector, plus x, y and z positions of all of them.
	int nPMTs = 0;
	vector<float> pmt_x;
	vector<float> pmt_y;
	vector<float> pmt_z;
	
	// Set the inner-volume geometry using the detector PMT information.
	Geometry geo;
	geo.SetGeometry(nPMTs,pmt_x,pmt_y,pmt_z);
	float traverseTmax = geo.max_traverse_time();
	float dTmax = geo.max_pmt_deltaT();
	float dRmax = geo.max_pmt_deltaR();
	float zmax = geo.search_radius();
	float rmax2 = geo.search_height();

//	for (int hit=0;hit<nevents;hit++ TODO will need to loop over events.
	
	// Get number of hits, plus time, charge, pmtx, pmty and pmtz for all hits.
	//TODO feed in a set of values for the hit information
	
	// Select the hits which will be used to calculate starting points (initial
	// test vertices) for the search.
	HitSelect select;
	int nhits = 0;
	vector<float> times;
	vector<float>charges;
	vector<float> pmtx;
	vector<float> pmty;
	vector<float> pmtz;
	vector<HitInfo> hitinfo;

	int nselected =	select.SelectHits(nhits,times,charges,pmtx,pmty,pmtz,hitinfo,traverseTmax,dTmax,dRmax);

	// Make sure at least 4 hits have made the final selection.
	if (nselected<=libConstants::min_selected_hits)
	{
		return(-1); //continue; TODO when looping over events
	}

	// TODO info logging - cout hitinfo for the selected hits.

	// Calculate the initial test vertices fr the search.
	TestPointCalc testpointcalc;
	vector<vector<float>> testpoints;
	testpointcalc.CalculateTestPoints(hitinfo,  rmax2, zmax, testpoints);

	// TODO perform the maximum likelihood fit starting from the final list
	// of testpoints calculated in the previous steps.
	// Initial search
	// Coarse search
	// Fine search
	// Final search

	// Get the vertex and additional variables.
	
	return 0;
}
