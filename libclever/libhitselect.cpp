//vim :set noexpandtab tabstop=4 wrap

//includes
#include <iostream>
#include <libconstants.hpp>
#include <libhitselect.hpp>
#include <libgeometry.hpp>
#include <libhitinfo.hpp>
#include <cmath> //size()
#include <algorithm> //count()
#include <numeric> //accumulate()
#include <cstring>//memset()

// ************************************************************************** //
// Main SelectHits function which reduces the hits and associated hit info 
// to include only causally related hits which occur in clusters.
// SelectHits follows the geometry setting and precedes the calculation of 
// the initial test vertices to be used to start off the search. 


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
	// (set nrelated, is_selected and noccurence to 0 for now)
	vector<int> is_related(nhits_all);// set to zeros
	for (int i=0;i<nhits_all;i++)
	{
		hitinfo.push_back(HitInfo(0,0,0,0,is_related,times_all[i],charges_all[i],pmtx[i],pmty[i],pmtz[i]));
	}
	// reject temporally and spatially isolated hits 
	// (hitsel.cc:313 hitsel::mrclean)
	// makes new list of hits without isolated hits
	if (RemoveIsolatedHits(nhits_all,hitinfo) < 0)
	{
		//TODO logging
		return(-1);
	}

	// make table of causally related hits 
	// (hitsel.cc:28 hitsel:make_causal_table)
	
	if (GetCausallyRelatedHits(nhits_isolated_removed,hitinfo) < 0)
	{
		//TODO logging	
		return(-1);
	}
	
	// find clusters (hitsel.cc:422 hitsel::clus_sel)
	// finds clusters and makes list of all PMTs with high and medium occurrence
	
	if (FindHitClusters(nhits_causally_related,hitinfo) < 0)
	{
		//TODO logging 
		return(-1);
	}
	
	//TODO what do we need to clear to avoid memory leak??????????
	//TODO How are we going to pass the hitinfo array back? [I think this is dealt with in the unit tests]
	nhits = size(hitinfo);
	hitinfo.clear();
	return(nhits);
}


//************************************************************************** //
// These are the principal functions called by the main SelectHits function.

int HitSelect::RemoveIsolatedHits(int nhits_all, vector<HitInfo>& hitinfo)
{

	// Reject temporally and spatially isolated hits.
	// Make new list of unisolated hits
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

	// Remove hit info for isolated hits by erasing
	// elements where elements are not marked as selected
	hitinfo.erase(remove_if(hitinfo.begin(),hitinfo.end(),[](HitInfo const& hit)
	{
		return hit.is_selected==0;
	}
	),hitinfo.end());


	return(nhits_isolated_removed);
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

			if ( !hitinfo[i].is_related[j] && i != j)
			{
				//will not look at i==j, only look at pairs of hits once
				// could the pair have come from the same event origin?
				if ( CheckCausal(i,j,hitinfo) )
				{
					// mark the hit as selected
					hitinfo[i].is_related[j] = 1; //pair i,j is related
					hitinfo[j].is_related[i] = 1; //pair j,i is related
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
					hitinfo[j].is_related[i] = 0; //pair j,i not related
					hitinfo[i].nrelated--;
					hitinfo[j].nrelated--;
				}
			}

		}

	}
	
	// Now remove the hits which are related to fewer than 3 other events.
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
	// This is to make the cluster-finding step more efficient. In general,
	// the hits that have the most relations will create the biggest clusters.
	auto sortRule = [](HitInfo const& h1, HitInfo const& h2) -> bool
	{
		// if the numbers of related pmts are equal, sort by charge
		if (h1.nrelated==h2.nrelated)
		{
			return h1.charge > h2.charge;
		}
		// otherwise sort by number of related pmts
		else
		{
			return h1.nrelated > h2.nrelated;
		}
	};

	sort(hitinfo.begin(), hitinfo.end(), sortRule);
	
	return(nhits_causally_related);

}

int HitSelect::FindHitClusters(int nhits_causally_related, vector<HitInfo>& hitinfo)
{
	// Loop through is_related. Where hitinfo[i].is_related[j]==1, 
	// hits i and j are related.
	// Use each of these hits as seeds (seed1 & seed2) to create lists of clustered hits,
	// where each hit in a cluster is related to each of the original seeds.
	// hitsel.cc:423
	
	// Create a 2d vector (all_clusters) to store a 1d vector (cluster) of hits for each
	// cluster found.
	vector< vector<int> > all_clusters;
	// A cluster vector is created for each related pair of hits seed1 and 
	// seed2. If a particular hit is in a cluster, cluster[hit] is set to 1.
	// If there are sufficient interrelated hits in the cluster, then the 
	// cluster vector will be added to all_clusters.

	// Set the minimum cluster size to minhits (3). This will later be updated 
	// to the size of the maximum cluster found and will be used to avoid 
	// going through all of the loops.
	int min_cluster_size = minhits;
	// Make a vector to store the number of other related 
	// cluster hits for each hit in the cluster and 
	// intialise to zero.
	vector<int> nrel_cluster(nhits_causally_related);

	// Now loop over the hits and treat each pair of related hits as seeds 
	// for a cluster
	for (int seed1=0; seed1<nhits_causally_related-1; seed1++)
	{

		// Check that nrelated is greater than the maximum cluster size
		// so far. This saves time since we are only going to use the first 
		// occurrence of a cluster with the maximum cluster size.
		if (hitinfo[seed1].nrelated<min_cluster_size)
		{
			continue;
		}
		for (int seed2=seed1+1; seed2<nhits_causally_related; seed2++)
		{   
			memset(&nrel_cluster[0], 0, nrel_cluster.size() * sizeof nrel_cluster[0]);
			// TODO logging information
			// Create a vector to store which hits are in the cluster.
			vector <int> cluster(nhits_causally_related);
			int candidate_size;
			
			// If two hits are related then use the pair as a seed to check 
			// for a cluster. Also check that the second seed has sufficient
			// related hits (greater than or equal to the current max cluster 
			// size) to be in a cluster. 
			if (hitinfo[seed1].is_related[seed2] && hitinfo[seed2].nrelated>=min_cluster_size)
			{
				candidate_size = FindClusterCandidate(nhits_causally_related,seed1,seed2,hitinfo,cluster);
			} 

			// Skip to the next seed pair if not related or not enough hit
			if (candidate_size<minhits)
			{
				//TODO info logging
				cout << "Candidate cluster contains too few hits. Skipping to next seed pair" << endl;
				continue;
			}

			// Now that we have looked at all cluster candidates for the seed
			// pair, loop over all cluster candidates and check that they are 
			// all related to each other. (We already know they are related 
			// to the seed pair.)


			for (int hit1 = 0; hit1<nhits_causally_related-1; hit1++)
			{
				for (int hit2 = hit1+1; hit2<nhits_causally_related; hit2++)
				{
					// See how many other hits in the cluster each candidate 
					// is related to. 
					if ( cluster[hit1] && cluster[hit2] && hitinfo[hit1].is_related[hit2])
					{
						nrel_cluster[hit1]++;
						nrel_cluster[hit2]++;
					}
				}
			
			}

			// Loop over the rest of the cluster hits (not the seed hits).
			for (int hit1 = 2; hit1<nhits_causally_related; hit1++)
			{
				// For each hit, if the number of related hits in the same 
				// cluster (nrelated) is less than the number of hits in the 
				// cluster, then remove unrelated hits from the cluster in order
				// of descending number of unrelated hits.

				// First see how many (if any) other hits in the cluster are
				// unrelated.
				int unrelated = count(cluster.begin(),cluster.end(),1)-(nrel_cluster[hit1]+1);
				for (int hit2 = hit1+1; hit2<nhits_causally_related; hit2++)
				{
					while (unrelated>0) // Stop checking if there are no (more) 
										//unrelated hits.
					{
						// If hit 1 and hit 2 are unrelated
						if (!hitinfo[hit1].is_related[hit2])
						{
							// Remove the one with the fewest relations.
							// Or both if the numbers of relations are the same.
							// No need to remove is_related because these hits
							// will be removed from the cluster anyway.
							if (nrel_cluster[hit1]==nrel_cluster[hit2])
							{
								cluster[hit1] = 0;
								cluster[hit2] = 0;
								// remove 2 from the nrel tallies
								for(auto& nrel : nrel_cluster)
								{
									if (nrel>1)
									{
								    	nrel-=2;	
									}
									else if (nrel>0)
									{
										nrel--;
									}
								}
							}
							else if (nrel_cluster[hit1] < nrel_cluster[hit2])
							{
								cluster[hit1] = 0;
								// remove 1 from the nrel tallies
								for(auto& nrel : nrel_cluster)
								{
									if (nrel>0)
									{
								    	nrel --;	
									}
								}
							}
							else 
							{
								cluster[hit2] = 0;
								// remove 1 from the nrel tallies
								for(auto& nrel : nrel_cluster)
								{
									if (nrel>0)
									{
								    	nrel --;	
									}
								}
							}
							// remove this hit from the tally of 
							// unrelated hits for hit 1.
						}
						unrelated--;
					}
				}
			}

			// Save the cluster for this seed pair if it's big enough.
			//
			// Iterate backwards over the vectors and subtract from tally each
			// hit for which hitinfo[i].nrelations+1<min_cluster_size.
			if (size(cluster)>=min_cluster_size)
			{
				all_clusters.push_back(cluster);
				min_cluster_size = size(cluster); // set min_cluster_size to largest so far
			}
			nrel_cluster.clear();
		} // End loop over second hit seed2.
	} //end loop over first hit seed1

	// Unify all found clusters and note how often a PMT appears in a cluster
	// (hitsel.cc:526) 
	// Find the hit occurence in the clusters - merge together all clusters
	// by summing over columns. The column totals give the hit occurrence.
	vector <int> merged_clusters(nhits_causally_related);
	// Loop over columns (hits) in all_clusters
	for (int hit = 0; hit<nhits_causally_related; hit++)
	{
		// Loop over rows (clusters) in all_clusters
		for (int clus = 0; clus<(int)all_clusters.size(); clus++)
		{
			merged_clusters[hit] +=all_clusters[clus][hit];
		}
	// Note the number of times a hit occurs in the clusters found
	hitinfo[hit].noccurrence = merged_clusters[hit];
	}

	// remove any clusters which are less than the biggest cluster found 
	all_clusters.erase(remove_if(all_clusters.begin(), all_clusters.end(), [min_cluster_size](const vector <int>& clus)
	{
		return count(clus.begin(),clus.end(),1)<min_cluster_size;
	}
	), all_clusters.end());

	// Get the total number of clusters with the size of the biggest cluster.
	// This is used to define which hits have a high occurence in clusters.
	int nclusters = (int)all_clusters.size();
	// Mark the hits that are in the final cluster of all_clusters by updating
	// is_selected.
	// TODO this is what is done in BONSAI but is there a better way to select
	// the best cluster?????
	// Would it be worth keeping all of the hits which appear in the large 
	// clusters and then using hits where these don't overlap to create 
	// additional starting points?????
	for (int i=0; i<(int)all_clusters[nclusters-1].size(); i++)
	{
		if (all_clusters[nclusters-1][i])
		{
			hitinfo[i].is_selected = 1;
		}
		else hitinfo[i].is_selected = 0;
	}
	
	// Select all hits with high occurrence in clusters. Remove remainder.
	// TODO BONSAI mentions selecting those with medium occurrence if their 
	// charge is within a reasonable range but this is commented out. Should
	// consider whether this will be useful.
	int min_occurrence = 1+2*(nclusters-1)/3;
	hitinfo.erase(remove_if(hitinfo.begin(), hitinfo.end(), [nclusters, min_occurrence](HitInfo const& hit)
	{
		// TODO how is this defined????
		return hit.noccurrence<min_occurrence; 
	}
	), hitinfo.end());

	int nhits_high_occurrence = size(hitinfo);

	// With hits of low to medium occurrence removed, check that each
	// hit in the cluster has the minimum number of related hits within the
	// cluster. Remove hits that have fewer than 3 related cluster hits. 
	for (int hit1 = 0; hit1<nhits_high_occurrence; hit1++)
	{
		if (int nrel = hitinfo[hit1].nrelated < minhits)
		{
			for (int hit2 = 0; hit2<nhits_high_occurrence; hit2++)
			{
				while (nrel>0) // Stop if there are no (more) related hits.
				{

					// If hit 1 and hit 2 are unrelated
					if (hit1 != hit2 && !hitinfo[hit1].is_related[hit2])
					{
						// For hits to be removed, set is_selected = 0 and
						// adjust nrel (number of related cluster hits) tallies.
						if (nrel_cluster[hit1]==nrel_cluster[hit2])
						{
							hitinfo[hit1].is_selected = 0;
							hitinfo[hit2].is_selected = 0;
							// remove 2 from the nrel tallies
							for(auto& nrel_clus : nrel_cluster)
							{
								if (nrel_clus>1)
								{
									nrel_clus-=2;	
								}
								else if (nrel_clus>0)
								{
									nrel_clus--;
								}
							}
						}
						else if (nrel_cluster[hit1] < nrel_cluster[hit2])
						{
							hitinfo[hit1].is_selected = 0;
							// remove 1 from the nrel tallies
							for(auto& nrel_clus : nrel_cluster)
							{
								if (nrel_clus>0)
								{
									nrel_clus --;	
								}
							}
						}
						else 
						{
							hitinfo[hit2].is_selected = 0;
							// remove 1 from the nrel tallies
							for(auto& nrel_clus : nrel_cluster)
							{
								if (nrel_clus>0)
								{
									nrel_clus--;	
								}
							}
						}
						
					} // End related pair
					nrel--;
				} //nrel==0
			} //End loop over hit2
		} // End if nrel<minhits
	
		// Note the number of related cluster hits for each hit in the cluster
		hitinfo[hit1].nselected = nrel_cluster[hit1];
	} // End loop over hit1
	

	// Remove cluster candidates found to be related to too few other hits
	int minhit = minhits;

	hitinfo.erase(remove_if(hitinfo.begin(),hitinfo.end(),[minhit](HitInfo const& hit)
	{
		// This checks that there are more than 3 related cluster hits for each
		// hit in the cluster because min 4 hits are required to define a point
		// in space.
		return hit.nselected<minhit;
	}
	), hitinfo.end());

	// Check the number of hits in the final cluster is greater than 3
	if (size(hitinfo)<minhits)
	{
		return(-1);
	}

	// Finally sort hits (times, charges, pmt positions) into time order 
	// qsort(selected,nsel) sorting by time (hitsel:571)
	auto sortRule = [](HitInfo const& h1, HitInfo const& h2) -> bool
	{
		return h1.time > h2.time;
	};

	sort(hitinfo.begin(), hitinfo.end(), sortRule);

	int nhits_clustered = size(hitinfo);
	
	return(nhits_clustered);
	
}


// ******************************************************************** //
// These are the subsidiary functions called by the principal functions 
// which are in turn called by the main HitSelect function.

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
	if (deltaT > geo.max_traverse_time())//*cm_to_ns*cylinder_diagonal)
	{
		return (0);
	}

	return(deltaT*deltaT<=deltaD2/libConstants::cm_per_ns);
}

int HitSelect::FindClusterCandidate(int nhits_causally_related, int seed1, int seed2, vector <HitInfo>& hitinfo, vector<int>& cluster)
{
	// Add the two seed hits to the cluster vector.
	cluster[seed1]=1;
	cluster[seed2]=1;

	// Now loop over the hits and find all hits that are
	// related to both of the original hits.
	for (int newhit = 0; newhit<nhits_causally_related; newhit++)
	{	
		// The hits are in order of number of relations. If the number of 
		// relations of the last hit is equal to the number of hits in the 
		// cluster so far, there can be no more hits in the cluster.
		// TODO This is what BONSAI does but is there a chance that some of 
		// the relations are to hits that are yet to be added to the cluster?
		// Commenting out for now.
		/*
		if (hitinfo[newhit].nrelated == count(cluster.begin(),cluster.end(),1))
		{
			break;
		}
		*/
		// Add to cluster vector if related to both seed hits.
		// This won't check a hit against itself because
		// hitinfo[i].is_related[i]==0
		if ( hitinfo[seed1].is_related[newhit] && hitinfo[seed2].is_related[newhit])
		{
			cluster[newhit] = 1;
		}
	
	} // End loop over newhit.


	// Return the number of hits in the cluster
	return ( count(cluster.begin(), cluster.end(), 1) );
	
}

