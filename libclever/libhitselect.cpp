//vim :set noexpandtab tabstop=4 wrap

//includes
#include <iostream>
#include <libconstants.hpp>
#include <libhitselect.hpp>
#include <libgeometry.hpp>
#include <cmath>
#include <algorithm>

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
	nhits_isolated_removed = 0;
	nhits_causally_related = 0;
	SelectHits(nhits_all, times_all, charges_all, pmtx, pmty, pmtz);

}

//destructor function
libHitSelect::~libHitSelect(){
	// Free the vectors for hit selection process
	// TODO is this necessary or will it be automatic
	// when this instance of SelectHits ends???
	times_isolated_removed.clear();
	charges_isolated_removed.clear();
	pmtx_isolated_removed.clear();
	pmty_isolated_removed.clear();
	pmtz_isolated_removed.clear();
	// Free the variables for hit selection process
	nhits_isolated_removed = 0;
	nhits_causally_related = 0;
}

int libHitSelect::SelectHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector <float> pmtz)
{
	// Do not proceed with reconstruction if nhits outside reconstructable range
	if (nhits_all < minhits || nhits_all > maxhits) 
		//TODO error logging
		return(-1);

	// reject temporally and spatially isolated hits (hitsel.cc:313 hitsel::mrclean)
	// makes new list of hits without isolated hits
	if (RemoveIsolatedHits(nhits_all,times_all,charges_all, pmtx, pmty, pmtz) < 0)
		//TODO error logging
		return(-1);

	// make table of causally related hits (hitsel.cc:28 hitsel:make_causal_table)
	if (GetCausallyRelatedHits() < 0)
		//TODO error logging	
		return(-1);
	
	// find clusters (hitsel.cc:422 hitsel::clus_sel)
	// finds clusters and makes a list of all PMTs with high and medium occurrence
//	if (FindClusteredHits(nhits_causally_related,times_causally_related,charges_causally_related) < 0)
		//TODO error logging 
//		return(-1);

	// sort hits (times, charges, pmt positions) into time order qsort(selected,nsel)


	//TODO need to set nhits, charges, times to zero (or delete) once reconstruction is done.
	nhits = nhits_causally_related;
	return(nhits);
}


int libHitSelect::RemoveIsolatedHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz){

	// reject temporally and spatially isolated hits; make new list of unisolated hits
	// (hitsel.cc:313 hitsel::mrclean)
	
	// First create vector to mark whether or not hits are isolated.
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
		if ( count(is_selected.begin(), is_selected.end(), 1) < minhits){
			//TODO error logging
			is_selected.clear();
			return(-1);
		}

		// Save vectors of all hits not isolated in space or time
		if (is_selected[hit]){
			times_isolated_removed.push_back(times_all[hit]);
			charges_isolated_removed.push_back(charges_all[hit]);
			pmtx_isolated_removed.push_back(pmtx[hit]);
			pmty_isolated_removed.push_back(pmty[hit]);
			pmtz_isolated_removed.push_back(pmtz[hit]);
			nhits_isolated_removed++;
		}
	}

	is_selected.clear();
	return(nhits_isolated_removed);
}

int libHitSelect::CheckCoincidence(int i, int j, vector<float> times_all, vector<float> pmtx, vector<float> pmty, vector<float> pmtz){
	// Check whether or not two hits are isolated from each other
	// return (1) if PMT pair are not isolated from each other
	float deltaT = fabs(times_all[i]-times_all[j]);
	float deltaD = DeltaDistance2(i,j, pmtx, pmty, pmtz);
	return( (deltaT<libConstants::tlim) && (deltaD<libConstants::dlim*libConstants::dlim) );

}

float libHitSelect::DeltaDistance2(int i,int j, vector<float> pmtx, vector<float> pmty, vector<float> pmtz){

	// Calculate distance squared between two hit PMTs
	float dx = pmtx[i]-pmtx[j];
	float dy = pmty[i]-pmty[j];
	float dz = pmtz[i]-pmtz[j];
	return( dx*dx + dy*dy + dz*dz );

}

int libHitSelect::GetCausallyRelatedHits(){

	// reject hits which could not have come from the same origin of light; make new list of related hits
	// (hitsel.cc:28 hitsel::make_causal_table)
	
	// First create 2d vector to mark whether or not pairs of hits are related
	vector< vector<int> > is_related(nhits_isolated_removed-1,vector<int>(nhits_isolated_removed-1));
	// Then create vector to store the number of related hits found for each hit.
	// Initialised to zero - increases by 1 for each related hit found.
	vector<int> nrelated(nhits_isolated_removed);
	vector<int> related(nhits_isolated_removed);

	for (int i = 0; i < nhits_isolated_removed; i++){
		// iterate over all other hits and check how many other
		// hits are causally related.
		for (int j = i+1; j < nhits_isolated_removed ; j++){
			// could the pair have come from the same event origin?
			if (i!=j){
				if ( CheckCausal(i,j) ){
					// mark the hit as selected
					is_related[i][j]=1; //pair i,j is related
					//increment number of related hits by 1
					nrelated[i]++;
					nrelated[j]++;
				}
			}
		}
	}

	// Remove the hits which are related to a small number of events
	for (int i = 0; i< nhits_isolated_removed; i++){
	

		if (nrelated[i]<minhits && nrelated[i]>0){
			// not enough related hits
			// remove from associated previous and subsequent hit counts
			// and set is_related to 0
			for (int j = 0; j<nhits_isolated_removed; j++){
				if (is_related[i][j]){//will not check i==j as is_related = 0 for i==j
					is_related[i][j] = 0;
					nrelated[i]--;
					nrelated[j]--;
				}
			}

		}
		// set related[i] to 1 if there is another related hit
		for (int j = i+1; j<nhits_isolated_removed; j++){
			if (related[i]!=1){
				if (is_related[i][j]){
					related[i]=1;
					related[j]=1;
				}
			}
		}


	}

	// Make lists of hit pmt positions, times and charges with unrelated hits removed
	for (int hit =0; hit<nhits_isolated_removed;hit++){

		// Return if the number of related hits is less than the 
		// minimum required for reconstruction
		if ( count(related.begin(), related.end(), 1) < minhits){
			//TODO error logging
			is_related.clear();
			nrelated.clear();
			related.clear();
			return(-1);
		}
	
		times_causally_related.push_back(times_isolated_removed[hit]);
		charges_causally_related.push_back(charges_isolated_removed[hit]);
		nhits_causally_related++;
		pmtx_causally_related.push_back(pmtx_isolated_removed[hit]);
		pmty_causally_related.push_back(pmty_isolated_removed[hit]);
		pmtz_causally_related.push_back(pmtz_isolated_removed[hit]);
	
	}

	// sort hits by number of relations????

	is_related.clear();
	nrelated.clear();
	related.clear();
	return(nhits_causally_related);

}

int libHitSelect::CheckCausal(int i,int j){
	
	float deltaT = fabs(times_isolated_removed[i]-times_isolated_removed[j]);
	float deltaD = DeltaDistance2(i,j,pmtx_isolated_removed,pmty_isolated_removed,pmtz_isolated_removed);
	if (deltaT < libConstants::tcoinc) return 1;
	libGeometry geo;
	if (deltaT > geo.max_traverse_time()) return 0;//TODO cm_to_ns*cylinder_diagonal
	return(deltaT*deltaT<deltaD/libConstants::cm_per_ns);
}

/*
void libHitSelect::FindClusteredHits(int nhits_causally_related, vector<float> times_causally_related, vector<float> charges_causally_related){

}*/

// FUNCTIONS IN HITSEL.CC(?)
// make_causal_table function to create causal table (called by clus_sel)
// reduce function to remove multiple hits etc (called by clus_sel)
// function to create selection array and do hit selection (select - calls clus_sel)
// function to find centroid of PMT hits and three main axes //NOT USED
// function to find list of PMTs where each pair could be hit (clus_sel)
   // by direct light from a common vertex
// function to print cable, time, charge for each hit
