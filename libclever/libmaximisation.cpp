/*****************************************************
 * Performs likelihood maximisation over successively
 * narrower searches.
 *
 * ***************************************************/
#include <iostream>
#include <math.h>
#include <limits.h>

#include <libconstants.hpp>
#include <libhitinfo.hpp>
#include <libmaximisation.hpp>

//Maximise constructor
Maximisation::Maximisation()
{
	Maximise(hitinfo,testpoints);
}

Maximisation::~Maximisation()
{
	Maximise()
}


//*****************************************************************************
// This is the main Maximisation function which performs successive searches to
// find the testpoint with the best likelihood.

void Maximisation::Maximise(vector <HitInfo> hitinfo, vector<vector<float>> testpoints)
{
	// Calculate likelihood (or goodness???) for initial points.
	InitialLikelihoods(hitinfo,libConstants::intial_rmin,testpoints);

	// Calculate likelihood (or goodness????) for coarse search.
	Search(hitinfo,libConstants::coarse_rmin,libConstants::coarse_rmax,libConstants::coarse_dlike,libConstants::coarse_skim_fraction,testpoints);

	// Calculate likelihood (or goodness????) for fine search.
	Search(hitinfo,libConstants::fine_rmin,libConstants::fine_rmax,libConstants::fine_dlike,libConstants::fine_skim_fraction,testpoints);

	// Perform final search and get best fit vertex.
	FinalSearch(hitinfo,libConstants::final_rmin,libConstants::final_rmax,final_dlike,final_skim_fraction,testpoints);


}

//*****************************************************************************
// These are the principal functions called by Maximise.

void Maximisation::InitialLikelihoods(vector<HitInfo> hitinfo, float rmin, vector<vector<float>> testpoints, vector<vector<float>>&new_testpoints)
{
	int npoints = testpoints.size();
	vector<float> tp_likelihoods(npoints);
	// TODO decide how to save the points that we want to check next.
	for (int point; point<npoints; point++)
	{
		// Calculate the likelihood for each point.
		CalculateLikelihood(hitinfo,testpoints[point])
		// calculate goodness (fit quality)
		// get result: get emission time, gdn0, theta, phi, cosc

		// Save the likelihood to be accessed in the next step.
		
	}
}

void Maximisation::Search(vector<HitInfo> hitinfo, float rmin, float rmax, float dlike, float skim_fraction, vector<vector<float>> testpoints)
{

	float best = max(tp_likelihoods);
	float worst = min(tp_likelihoods);
	
	// If the difference between best and worst fit > dlike, keep active only
	// skim_fraction of the branches. 
	
	// Find the best for each active branch with cut-off radius rmax:
	// first get the points in a dodecahedron for each remaining branch,
	// then get the best likelihood for each remaining branch.
}

void Maximisation::FinalSearch()
{
	// Find best and worst likelihoods
	
	// Make final reduction in branches
	
	// Find best fit
}
 
//*****************************************************************************
// These are the subsidiary functions called by the successive searches.

void Maximisation::CalculateLikelihood(vector<HitInfo> hitinfo, vector<float> point)
{
	// Calculate time residual for each hit from the testpoint.
	// likelihood.cc:11 like0 = fittime(1,vertex,dirfit,dt)
	// timefit.cc:796 fittime calls makedirtof,fastaddloglik, returns makelike:
	
	// timefit.cc:168 makedirtof(vertex)
	vector<float> timeresidual;
	vector<vector<float>> directions;
	for (int hit = 0; hit<size(hitinfo); hit++)
	{
		timeresidual.push_back(hitinfo[hit].time-TimeOfFlight(hitinfo,point,direction,hit);

	// timefit.cc:216 fastaddloglik, sums log likelihood, does t0 and peak fit,
	// returns peak time.
	GetLogLikelihood(hitinfo,point,timeresiduals);
	vector<vector<float> directionFit = FitDirection(vertex,timeresidual,centr);
	// change to direction fit
	
	// timefit.cc:420 makelike(t0) (returns max log likelihood at that vertex)
	GetLogLikelihood
	MakeAngularCorrection();
	// Make angular correction if required
	/*dev = dirfit[3]-cang0
	  if (dev>0)
    if (plusdang==FIT_PARAM_NONE)
      like=like0;
    else
      like=like0-dev*dev*plusdang;
  else
    if (minusdang==FIT_PARAM_NONE)
      like=like0;
    else
      like=like0-dev*dev*minusdang;*/
	}

	// Add the log likelihood for each hit given the time residual.
  	//add_fit(vertex,like);
}

float Maximisation::TimeOfFlight(vector<HitInfo> hitinfo, vector<float> point, vector<vector<float>>& direction, int hit)
{
	// hits.inline:219 tof(vertex,dir,hit)
	direction[hit].push_back(hitinfo[hit].pmtx-point[0];
	direction[hit].push_back(hitinfo[hit].pmty-point[1];
	direction[hit].push_back(hitinfo[hit].pmtz-point[2];
	float distance = direction[hit][0]**2 + direction[hit][1]**2 + direction[hit][2]**2;
	if (distance ==0)
	{
		return(0);
	}
	distance = sqrt(distance);
	direction[hit][0]/=distance;
	direction[hit][1]/=distance;
	direction[hit][2]/=distance;
	return(distance/libConstants::cm_per_ns)
}

float Maximisation::GetLogLikelihood(vector<vector<float> timeresiduals, float& peaktime,vector<float>& loglikelihoods);
{
	// Sum up the log likelihood for all hits for the given vertex.
	// Get the likelihood (probability) from the pdf for each time residual.
	// Add the log(likelihood) for each of time residual.
	for (int hit = 0; hit < timeresiduals.size(); hit++)
	{
		float loglikelihood = 0;
		float max = numeric_limits<float>::lowest;
		if (useCharge = 1)
		{
			float likelihood = timedistribution2D->GetBinContent(timeresiduals[hit],hitinfo[hit].charge);
			loglikelihood += log(likelihood);
			if (likelihood>max)
			{
				max = likelihood;
				peaktime = timeresiduals[hit];
			}
		}
		else
		{
			float likelihood = timedistribution1D->GetBinContent(timeresiduals[hit]);
			loglikelihood += log(likelihood);
			if (likelihood>max)
			{
				max = likelihood;
				peaktime = timeresiduals[hit];
			}
		}
	}
	// Get the peak of the light from the event.
	// This is the time at which the likelihood is highest
	float peaktime 
}
