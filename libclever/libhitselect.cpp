//vim :set noexpandtab tabstop=4 wrap

//includes
#include <iostream>
#include <libconstants.hpp>
#include <libhitselect.hpp>
#include <libgeometry.hpp>
#include <libhitinfo.hpp>
#include <cmath>
#include <algorithm>
#include <numeric>

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
HitSelect::HitSelect()
{
	nhits_isolated_removed = 0;
	nhits_causally_related = 0;
	SelectHits(nhits_all, times_all, charges_all, pmtx, pmty, pmtz);

}

//destructor function
HitSelect::~HitSelect()
{
	// Free the vectors for hit selection process
	// TODO is this necessary or will it be automatic
	// when this instance of SelectHits ends???
	// Free the variables for hit selection process
	nhits_isolated_removed = 0;
	nhits_causally_related = 0;
}

int HitSelect::SelectHits(int nhits_all, vector<float> times_all, vector<float> charges_all, vector<float> pmtx, vector<float> pmty, vector <float> pmtz)
{
	// Do not proceed with reconstruction if nhits outside reconstructable range
	if (nhits_all < minhits || nhits_all > maxhits) 
	{
		//TODO logging
		return(-1);
	}

	// collate all hit information using the HitInfo class
	// hitinfo vector intitialised in libhitselect.hpp
	// (set nrelated and is_selected to 0 for now)
	vector<int> is_rel(nhits_all);// set to zeros
	for (int i=0;i<nhits_all;i++)
	{
		hitinfo.push_back(HitInfo(0,0,is_rel,times_all[i],charges_all[i],pmtx[i],pmty[i],pmtz[i]));
	}
	// reject temporally and spatially isolated hits (hitsel.cc:313 hitsel::mrclean)
	// makes new list of hits without isolated hits
	if (RemoveIsolatedHits(nhits_all,hitinfo) < 0)
	{
		//TODO logging
		return(-1);
	}

	// make table of causally related hits (hitsel.cc:28 hitsel:make_causal_table)
	if (GetCausallyRelatedHits(nhits_isolated_removed,hitinfo) < 0)
	{
		//TODO logging	
		return(-1);
	}
	
	// find clusters (hitsel.cc:422 hitsel::clus_sel)
	// finds clusters and makes a list of all PMTs with high and medium occurrence
//	if (FindClusteredHits(nhits_causally_related,hitinfo) < 0)
		//TODO logging 
//		return(-1);

	// sort hits (times, charges, pmt positions) into time order qsort(selected,nsel)


	//TODO need to set nhits, charges, times to zero (or delete) once reconstruction is done.
	nhits = size(hitinfo);
	hitinfo.clear();
	return(nhits);
}


int HitSelect::RemoveIsolatedHits(int nhits_all, vector<HitInfo>& hitinfo)
{

	// reject temporally and spatially isolated hits; make new list of unisolated hits
	// (hitsel.cc:313 hitsel::mrclean)
	
	// Iterate over each hit and check to see if it is
	// within dlimit and tlimit of another hit 
	for (int i = 0; i < nhits_all; i++)
	{
		// iterate over all other hits and check not isolated
		// if hit (i) hasn't already been marked as selected
		// TODO check that this is definitely iterating over all 
		for (int j = i+1; j < nhits_all && !hitinfo[i].is_selected; j++)
		{
			// is the pair less than dlimit and tlimit apart?
			if (i!=j)
			{
				if ( CheckCoincidence(i,j,hitinfo) )
				{
					// mark the hit as selected
					hitinfo[i].is_selected = 1;
					hitinfo[j].is_selected = 1;
					nhits_isolated_removed += 2;
				}
			}
		}
	}
	
	// Return if the number of selected hits is less than the 
	// minimum required for reconstruction
	if (nhits_isolated_removed < minhits)
	{
		hitinfo.clear();
		return(-1);
	}

	// Make lists of hit pmt positions, times and charges with isolated hits removed
	// by erasing elements where elements are not selected
	hitinfo.erase(remove_if(hitinfo.begin(),hitinfo.end(),[](HitInfo const& hit)
	{
		return hit.is_selected==0;
	}
	),hitinfo.end());


	return(nhits_isolated_removed);
}

int HitSelect::CheckCoincidence(int i, int j, vector<HitInfo> hitinfo)
{
	// Check whether or not two hits are isolated from each other
	// return (1) if PMT pair are not isolated from each other
	float deltaT = fabs(hitinfo[i].time-hitinfo[j].time);
	float deltaD = DeltaDistance2(i,j,hitinfo);
	return( (deltaT<libConstants::tlim) && (deltaD<libConstants::dlim*libConstants::dlim) );

}

float HitSelect::DeltaDistance2(int i, int j, vector<HitInfo> hitinfo)
{
	// Calculate distance squared between two hit PMTs
	float dx = hitinfo[i].pmtx-hitinfo[j].pmtx;
	float dy = hitinfo[i].pmty-hitinfo[j].pmty;
	float dz = hitinfo[i].pmtz-hitinfo[j].pmtz;
	return( dx*dx + dy*dy + dz*dz );

}

int HitSelect::GetCausallyRelatedHits(int nhits_isolated_removed,vector<HitInfo>& hitinfo)
{

	// reject hits which could not have come from the same origin of light
	// and make new list of related hits
	// (hitsel.cc:28 hitsel::make_causal_table)
	for (int i = 0; i < nhits_isolated_removed; i++)
	{
		// iterate over all other hits and check how many other
		// hits are causally related.
		for (int j = 0; j < nhits_isolated_removed ; j++)
		{

			if ( hitinfo[i].is_related[j] != 1 && i != j)
			{
				//will not look at i==j, only look at pairs of hits once
				// could the pair have come from the same event origin?
				if ( CheckCausal(i,j,hitinfo) ) //TODO 
				{	
					// mark the hit as selected
					// TODO figure out how to set is_related
					hitinfo[i].is_related[j]; //pair i,j is related
					hitinfo[j].is_related[i] = 1; //pair j,i is related TODO need this later?
					//increment number of related hits by 1
					hitinfo[i].nrelated++;
					hitinfo[j].nrelated++;
				}
			}
		}
	}

	// Reset is_related and reduce the nrelated tally if the number of 
	// related hits is less than minhits (default 2)
	// TODO this will also reduce the nrelated tally for any related hits,
	// PRIOR to the removal of hits with too few related hits, which means
	// that some others could also be taken below the threshold. 
	// Is this correct??????????????? (Ask Michael?)
	for (int i = 0; i< nhits_isolated_removed; i++)
	{

		if (hitinfo[i].nrelated<minhits && hitinfo[i].nrelated>0)
		{
			// not enough related hits
			// remove from associated previous and subsequent hit counts
			// and set is_related to 0
			for (int j = 0; j<nhits_isolated_removed; j++)
			{//TODO reduce the range to run from i+1?
				if (hitinfo[i].is_related[j])
				{
					hitinfo[i].is_related[j] = 0;// pair i,j not related
					hitinfo[j].is_related[i] = 0; //pair j,i not related TODO need this later??
					hitinfo[i].nrelated--;
					hitinfo[j].nrelated--;
				}
			}

		}

	}
	
	// Now remove the hits which are related to a small number of events
	hitinfo.erase(remove_if(hitinfo.begin(),hitinfo.end(),[](HitInfo const& hit)
	{
		return hit.nrelated < 3;
	}
	),hitinfo.end());

	nhits_causally_related = size(hitinfo);
	// return if there are not enough causally related hits
	if (nhits_causally_related<=minhits)
	{
		hitinfo.clear();
		return (-1);
	}
	

	// So now we have a vector of class objects to show which hits are related
	// This needs to be sorted by the number of related hits in descending order.
	
	// Sort the hit indices by the number of related hits (nrelated[i]) 
	// in descending order. If the number of related hits are equal, 
	// sort by charge in descending order.
	auto sortRule = [](HitInfo const& h1, HitInfo const& h2) -> bool
	{
		// if the numbers of related pmts are equal, sort by charge
		if (h1.nrelated==h2.nrelated)
		{
			return h1.charge > h2.charge;
		}
		else
		{
			return h1.nrelated > h2.nrelated;
		}
		// otherwise sort by number of related pmts
	};

	sort(hitinfo.begin(), hitinfo.end(), sortRule);
	
	//find clusters:
	//use each pair of related hits as a seed for a cluster
	//find hits related to both seed hits 
	//(a hit related to both seed hits is a candidate)
	//deactivate all candidate's relations to cluster pmts
	//if the number of relations of the last hit = hits in cluster so far,
	//can't be any more hits in cluster
	//remove hits not related to the new hit
	//see if cluster is large enough to be stored
	//see if cluster is larger than largest cluster so far
	//adjust minimum allowed size and remove hits with too few
	//relations as seed candidates
	//unify all clusters into one and note how often a pmt occurs
	//take all pmts with high occurrence and
	//all pmts with minimum occurence if charges are reasonable
	
	//return(hitinfo);
	return(nhits_causally_related);

}

int HitSelect::CheckCausal(int i,int j, vector<HitInfo> hitinfo)
{
	
	float deltaT = fabs(hitinfo[i].time-hitinfo[j].time);
	float deltaD2 = DeltaDistance2(i,j,hitinfo);

	// First check that the time and distance between PMTs do not 
	// exceed the detector constraints
	if (deltaT > libConstants::tcoinc)
	{
		return (0);
	}

	//TODO reinstate the check on time difference	
	libGeometry geo;
	if (deltaT > geo.max_traverse_time())
	{
		return (0);//TODO cm_to_ns*cylinder_diagonal
	}
	return(deltaT*deltaT<=deltaD2/libConstants::cm_per_ns);
}


// FUNCTIONS IN HITSEL.CC(?)
// make_causal_table function to create causal table (called by clus_sel)
// reduce function to remove multiple hits etc (called by clus_sel)
// function to create selection array and do hit selection (select - calls clus_sel)
// function to find centroid of PMT hits and three main axes //NOT USED
// function to find list of PMTs where each pair could be hit (clus_sel)
   // by direct light from a common vertex
// function to print cable, time, charge for each hit
