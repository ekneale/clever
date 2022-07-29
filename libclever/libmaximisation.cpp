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

#include <TH1F.h>
#include <TF1.h>

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
	vector<vector<float>> directionfit(testpoints.size());
	int npoints = testpoints.size();
	vector<float> tp_likelihoods(npoints);
	// TODO decide how to save the points that we want to check next.
	for (int point; point<npoints; point++)
	{
		// Calculate the likelihood for each point.
		CalculateLikelihood(hitinfo,testpoints[point],directionfit)
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

void Maximisation::CalculateLikelihood(vector<HitInfo> hitinfo, vector<float>& point)
{
	// likelihood.cc:11 like0 = fittime(1,vertex,dirfit,dt)
	// timefit.cc:796 fittime calls makedirtof,fastaddloglik, returns makelike:
	
	// Calculate time - time of flight (ttof) for each hit from the testpoint.
	// Also saves direction to each hit from the vertex.
	// timefit.cc:168 makedirtof(vertex)
	vector<float> ttofs;
	for (int hit = 0; hit<size(hitinfo); hit++)
	{
		ttofs.push_back(hitinfo[hit].time-TimeOfFlight(hitinfo,point,hitDirections,hit);
	}
	vector<float> direction;
	vector<float> centroidfit;
	FitDirection(hitinfo,point,direction,centroidfit);
	// Adjust results of direction fit
	//

	// Set t0 to the peak t-tof time
	float t0 = GetPeakTTof(ttofs);
	
	// Find the total log likelihood given t0
	float loglikelihood = GetLogLikelihood(t0);
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

	// Add the log likelihood for each hit given the t-tof-t0.
  	//add_fit(vertex,like);
}

float Maximisation::TimeOfFlight(vector<HitInfo> hitinfo, vector<float> point, vector<vector<float>>& hitDirections, int hit)
{
	// hits.inline:219 tof(vertex,dir,hit)
	// Calculates direction of each hit from the vertex.
	hitDirections[hit].push_back(hitinfo[hit].pmtx-point[0];
	hitDirections[hit].push_back(hitinfo[hit].pmty-point[1];
	hitDirections[hit].push_back(hitinfo[hit].pmtz-point[2];
	float distance = sqrt(hitDirections[hit][0]**2 + hitDirections[hit][1]**2 + hitDirections[hit][2]**2);
	if (distance ==0)
	{
		return(0);
	}
	hitDirections[hit][0]/=distance;
	hitDirections[hit][1]/=distance;
	hitDirections[hit][2]/=distance;
	return(distance/libConstants::cm_per_ns)
}

float Maximisation::GetPeakTTof(vector<vector<float> ttofs);
{
	// Find the peak time in the ttofs vector.
	// This is the time-residual range (default 0.4 ns) with the highest 
	// number of hits.
	float ttof_min = 0; // Only include hits with t-tof > 0
	float ttof_max = *max_element(ttofs.begin(),ttofs.end());
	// Get number of bins required for given bin width (ceil rounds up to the
	// nearest integer).
	int nbins = ceil((ttof_max - ttof_min)/libConstants::binwidthTTof);
	// Create a histogram for the t-tof values above 0
	TH1F h = new TH1F("httofs","httofs",nbins,ttof_min,ttof_max+nbins*libConstants::binwidthTTof);
	// Fill the histogram from the ttofs vector.
	for (float ttof : ttofs)
	{
		httofs->Fill(ttof);
	}
	// Scale httofs
	httofs->Scale(1/httofs->Integral());
	
	// Find the peak time bin (i.e. the one with most hits)
	int maxbin = httofs->GetMaximumBin();
	// Define the approximate peak time as the centre of the peak time bin
	float peak_ttof = httofs->GetBinCenter(maxbin);
	// Do a fit to httofs and get the x value corresponding to the max of the fit
	// TODO should this be over a larger range??
	float timeRange = libConstants::ttofs_range*libConstants::binwidthTRes;
	TF1 *peakfit = new TF1("peakfit", "pol2", peakttofs_tmp-timeRange, peakttofs_tmp+timeRange);
	httofs->Fit("peakfit", "R");	
	peak_ttof = peakfit->GetMaximumX();
//	int peak_ttof_bin = httofs->FindBin(peak_ttof);

	
	// Delete ttof histogram
	delete httofs;
	
	// Return the peak likelihood i.e. the likelihood corresponding to 
	// the peak time bin.
	// TODO not sure what the point of this is. For scaling purposes? 
	return(peak_ttof);
}

float GetLogLikelihood(vector<HitInfo> hitinfo, vector<float> ttofs, TH1F tResPDF, float t0)
{
	// Set the signal range and re-normalise the pdf
	// Find the maximum and minimum time residuals
	int min = min_element(ttofs.begin(),ttofs.end());
	int max = max_element(ttofs.begin(),ttofs.end());
	int ttof_min = *min - t0; 
	int ttof_max = *max - t0;
	int minbin = tResPDF->GetXaxis()->FindBin(ttof_min);
	int maxbin = tResPDF->GetXaxis()->FindBin(ttof_max);
	
	float renormalisation;
	if (useCharge)
	{
		//TODO is it ok to normalise to maximum of q given max tres?
		int maxqbin = tResPDF->GetYaxis()->GetMaximum(ttof_min)
		renormalisation = tResPDF->GetBinContent(maxbin)
	}
	else
	{
		renormalisation = tResPDF->GetBinContent(maxbin);
	}
	// Sum up the log likelihood for all hits for the given vertex and time t0.
	// Get the likelihood (probability) from the pdf for each ttof - t0.
	// Add the log(likelihood) for each value of ttof - t0.
	for (int hit = 0; hit < ttofs.size(); hit++)
	{
		float loglikelihood = 0;
		float max = numeric_limits<float>::lowest;//lowest number for comparison
		if (libConstants::useCharge == 1)
		{
			int bin = tResPDF->FindBin(ttofs[hit]-t0,hitinfo[hit].charge);
			float likelihood = GetBinContent(bin);
			loglikelihood += log(likelihood);
		}
		else
		{
			int bin = tResPDF->GetBinContent(ttofs[hit]-t0);
			float likelihood = GetBinContent(bin);
			loglikelihood += log(likelihood);
		}
	}

	return(loglikelihood)
	
}

void FitDirection(vector<HitInfo> hitinfo,vector<float> point, vector<float>& direction, vector<float>& centroidfit)
{
	vector<float> weight(hitinfo.size());
	for (float ttof : ttofs.size())
	{
		float time = ttof - ???;

	}
}
