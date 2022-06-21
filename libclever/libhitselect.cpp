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
	SelectHits(nhits_all, times_all, charges_all, pmtx, pmty, pmtz);

}

//destructor function
libHitSelect::~libHitSelect(){
	// Free the vectors for hit selection process
	// TODO is this necessary or will it be automatic
	// when this instance of SelectHits ends???
	times_isolated_hits_removed.clear();
	charges_isolated_hits_removed.clear();
	pmtx_isolated_hits_removed.clear();
	pmty_isolated_hits_removed.clear();
	pmtz_isolated_hits_removed.clear();
//	delete &nhits_isolated_hits_removed;
	// Free the variables for hit selection process
//	delete &nhits_causally_related_hits;
}

int libHitSelect::SelectHits(int nhits_all, float *times_all, float *charges_all, float *pmtx, float *pmty, float *pmtz)
{
	// Do not proceed with reconstruction if nhits outside reconstructable range
	if (nhits_all < libConstants::minhits | nhits_all > libConstants::maxhits) 
		//TODO error logging
		return(-1);

	// reject temporally and spatially isolated hits (hitsel.cc:313 hitsel::mrclean)
	// makes new list of hits without isolated hits
	if (RemoveIsolatedHits(nhits_all,times_all,charges_all, pmtx, pmty, pmtz) < 0)
		//TODO error logging
		return(-1);

	// make table of causally related hits (hitsel.cc:28 hitsel:make_causal_table)
//	if (GetCausallyRelatedHits(nhits_isolated_hits_removed,times_isolated_hits_removed,charges_isolated_hits_removed) < 0);
		//TODO error logging	
		return(-1);

	// find clusters (hitsel.cc:422 hitsel::clus_sel)
	// finds clusters and makes a list of all PMTs with high and medium occurrence
//	if (FindClusteredHits(nhits_causally_related_hits,times_causally_related_hits,charges_causally_related_hits) < 0)
		//TODO error logging 
		return(-1);

	// sort hits (times, charges, pmt positions) into time order qsort(selected,nsel)


	//TODO need to set nhits, charges, times to zero (or delete) once reconstruction is done.
}


int libHitSelect::RemoveIsolatedHits(int nhits_all, float *times_all, float *charges_all, float *pmtx, float *pmty, float *pmtz){

	// reject temporally and spatially isolated hits; make new list of unisolated hits
	// (hitsel.cc:313 hitsel::mrclean)
	
	// Create vector to mark whether or not hits are isolated.
	// Initialised to zero - elements changed to 1 if hit is not isolated.
	vector < int > is_selected(nhits_all-1);
	
	// Iterate over each hit and check to see if it is
	// within dlimit and tlimit of another hit 
	for (int i = 0; i < nhits_all; i++){
		// iterate over all other hits and check not isolated
		// if hit (i) hasn't already been marked as selected
		// TODO check that this is definitely iterating over all 
		for (int j = i+1; j < nhits_all && !is_selected[i]; j++){
			// is the pair less than dlimit and tlimit apart?
			if (i!=j){
				if ( CheckCoincidence(i,j, times_all, pmtx, pmty, pmtz) ){
					// mark the hit as selected
					is_selected[i] = 1;
					is_selected[j] = 1;
				}
			}
		}
	}
	
	// Make lists of hit pmt positions, times and charges with isolated hits removed
	for (int hit =0; hit<nhits_all;hit++){

		// Return if the number of selected hits is less than the 
		// minimum required for reconstruction
		if ( count(is_selected.begin(), is_selected.end(), 1) < libConstants::minhits)
				//TODO error logging
				return(-1);

		// Save vectors of all hits not isolated in space or time
		if (is_selected[hit]){
			times_isolated_hits_removed.push_back(times_all[hit]);
			charges_isolated_hits_removed.push_back(charges_all[hit]);
			pmtx_isolated_hits_removed.push_back(pmtx[hit]);
			pmty_isolated_hits_removed.push_back(pmty[hit]);
			pmtz_isolated_hits_removed.push_back(pmtz[hit]);
			nhits_isolated_hits_removed++;
		}
	}

	is_selected.clear();
	return(nhits_isolated_hits_removed);
}

int libHitSelect::CheckCoincidence(int i, int j, float *times_all, float *pmtx, float *pmty, float *pmtz){
	// Check whether or not two hits are isolated from each other
	// return (1) if PMT pair are not isolated from each other
	float tdiff = abs(times[i]-times[j]);
	return( (tdiff<libConstants::tlim) && (DeltaDistance2(i,j, pmtx, pmty, pmtz)<libConstants::dlim*libConstants::dlim) );

}

float libHitSelect::DeltaDistance2(int i,int j, float *pmtx, float *pmty, float *pmtz){

	// Calculate distance squared between two hit PMTs
	float dx = pmtx[i]-pmtx[j];
	float dy = pmty[i]-pmty[j];
	float dz = pmtz[i]-pmtz[j];
	return( dx*dx + dy*dy + dz*dz );

}

/*int  libHitSelect::GetCausallyRelatedHits(int nhits_isolated_hits_removed, float *times_isolated_hits_removed, float *charges_isolated_hits_removed){


	times_causally_related_hits[nhits_causally_related_hits] = ;
	charges_causally_related_hits[nhits_causally_related_hits] = ;
	nhits_causally_related_hits++;

	return(nhits_causally_related_hits);

}

void libHitSelect::FindClusteredHits(int nhits_causally_related_hits, float *times_causally_related_hits, float *charges_causally_related_hits){

}*/

// FUNCTIONS IN HITSEL.CC(?)
// make_causal_table function to create causal table (called by clus_sel)
// reduce function to remove multiple hits etc (called by clus_sel)
// function to create selection array and do hit selection (select - calls clus_sel)
// function to find centroid of PMT hits and three main axes //NOT USED
// function to find list of PMTs where each pair could be hit (clus_sel)
   // by direct light from a common vertex
// function to print cable, time, charge for each hit
