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
	Maximise(hitInfoVector,testPointsVector);
}

Maximisation::~Maximisation()
{
	Maximise()
}


//*****************************************************************************
// This is the main Maximisation function which performs successive searches to
// find the testpoint with the best likelihood.

void Maximisation::Maximise(vector <HitInfo> hitInfoVector, vector<vector<float>> testPointsVector)
{
	// Calculate likelihood for initial testpoints.
	FindNegativeLogLikelihoods(hitInfoVector,testPointsVector,0);

	// Skim off the points with the best NLL values, remove the remainder
	Skim( libConstants::sCoarseDlike, libConstants::sCoarseSkimFraction, testPointsVector);

	// Get additional points for coarse search
	int nPreviousTestPoints = AddPoints(libConstants::sCoarseRadius,testPointsVector)

	// Calculate the negative log likelihoods for the new testpoints
	// in the coarse search
	FindNegativeLogLikelihoods(hitInfoVector,testPointsVector,nPreviousTestPoints-1);

	// Calculate likelihood for fine search.
	Search(hitInfoVector,libConstants::sFineRmin,libConstants::sFineRmax,libConstants::sFineDlike,libConstants::sFineSkimFraction,testPointsVector;

	// Perform final search and get best fit vertex.
	// Final search can be final step in annealing algorithm or alternatively 
	// Minuit search. Simulated annealing followed by Minuit search will give
	// the most precise results (Minuit) while avoiding local minima and 
	// retaining speed (simulated annealing).
	FinalSearch(hitInfoVector,libConstants::sFinalRmin,libConstants::sFinalRmax,sFinalDlike,libConstants::sFinalSkimFraction,testPointsVector);

	// TODO calculate goodness (fit quality)
	// TODO get result: get emission time, gdn0, theta, phi, cosc

}

//*****************************************************************************
// These are the principal functions called by Maximise.

void Maximisation::FindNegativeLogLikelihoods(vector<HitInfo> hitInfoVector, vector<vector<float>>& testPointsVector, int start)
{
	// Iterates over all of the test points for which negative log likelihood
	// still needs to be calculated and then sorts the test point vector
	// into order of increasing negative log likelihood.
	
	int nTestPoints = testPointsVector.size();
	vector<vector<float>> hitDirectionVector(nTestPoints-start,vector<float>(3));
	// Likelihoods are added to the test points vector. This makes it 
	// easier to skim off test points corresponding to the best (smallest) 
	// negative log likelihoods and remove the remainder (Skim function).
	for (int iTestPoint = start; iTestPoint<nTestPoints; iTestPoint++)
	{
		// Calculate the likelihood for each point and add it to the vector
		vector<float> testPointVtxVector = testPointsVector[iTestPoint];
		testPointsVector[iTestPoint].push_back(FindTestPointLikelihood(hitInfoVector,testPointVtxVector));
	}
	
	// Sort the testPointsVector as a function of the negative log likelihood
	// values, best (lowest) value first.
	sort(testPointsVector.begin(),
          testPointsVector.end(),
          [] (const vector<double> &testPoint1, const vector<double> &testPoint2)
          {
              return testPoint1[4] < testPoint2[4];
          });
}

void Maximisation::Skim(float dLike, float skimFraction, vector<vector<float>> testPointsVector)
{
	// Find the best (lowest) and worst (highest) NLL values
	float best = testPointsVector.begin(); // lowest NLL
	float worst = testPointVectors.end(); // highest NLL

	// If the difference between best and worst fit > dlike, keep active only
	// skimFraction of the branches.
	if (fabs(worst - best) > dLike)
	{
		int skimNumber = (int)libConstants::sSkimFraction*testPointsVector.size();
		testPointsVector.erase( skimNumber+1, testPointsVector.end() );
	}
}

void Maximisation::AddPoints(float r, vector<vector<float>> testPointsVector)
{
	// Get the points in a dodecahedron around the testpoints saved from the
	// previous search with each vertex at distance rmax from the testpoint
	// origin
	
	// Use each testpoint as the origin for a dodecahedron.
	int nSavedTestPoints = testPointsVector.size();
	for (int iTestPoint=0; iTestPoint < nSavedTestPoints; iTestPoint++)
	{
		// Get the co-ordinates of the 4D testpoint
		float x = testPointsVector[iTestPoint][0];
		float y = testPointsVector[iTestPoint][1];
		float z = testPointsVector[iTestPoint][2];
		float t0 = testPointsVector[iTestPoint][3];
		testPointsVector.append(FindDodecahedronVertices(x,y,z,t0,rmax);	
	}
	return(nSavedTestPoints);
}


void Maximisation::FinalSearch()
{
	// Find best and worst likelihoods
	
	// Make final reduction in branches
	
	// Find best fit
}
 

//*****************************************************************************
// These are the subsidiary functions called by the successive searches.

vector<float> FindDodecahedronVertices(float x,float y,float z,float t0, rmax)
{
	// Cartesian co-ordinates of a dodecahedron centred 
	float goldenRatio = (sqrt(5) - 1) / 2;
	float a = 1/sqrt(3);
	float b = a / goldenRatio;
	float c = a * goldenRatio;

	// Generate vertices and add to a vector
	// Generate vertices centred on a point at the origin (0,0,0)
	vector<vector<float>> newTestPointsVector;
	vector<int> coefficientVector = { -1, 1};
	for (int iCoefficient : coefficientVector)
	{
		for (int jCoefficient : coefficientVector)
		{
			newTestPointsVector.Add({
					x + 0,
					y + iCoefficient * c * rmax,
					z + jCoefficient * b * rmax
					});
			newTestPointsVector.Add({
					x + iCoefficient * c * rmax,
                    y + jCoefficient * b * rmax,
					z + 0
					});
			newTestPointsVector.Add({
					x + iCoefficient * b * rmax,
					y + 0,
					z + jCoefficient * c * rmax
					});
			for (int kCoefficient : coefficientVector)
			{
				newTestPointsVector.Add({
						x + iCoefficient * a * rmax,
						y + jCoefficient * a * rmax,
						z + kCoefficient * a * rmax
						})
			}
		}

	}


	return(newTestPointsVector);
}



float Maximisation::FindTestPointLikelihood(vector<HitInfo> hitInfoVector, vector<float>& testPointVtxVector)
{
	// likelihood.cc:11 like0 = fittime(1,vertex,dirfit,dt)
	// timefit.cc:796 fittime calls makedirtof,fastaddloglik, returns makelike:
	// Obtains the negative log likelihood relating to the test point.
	// This is done by first calculating the time - tof for each hit.
	// The t0 is obtained from a fit to the ttof distribution and then the
	// time residual (time - tof - t0) is calculated for each hit.
	// Once the time residual has been obtained the likelihood is obtained
	// from the pdf and the negative log likelihood calculated.
	// Finally, the negative log likelihood is adjusted for the angular
	// constraint if used. This is most useful for pure Cherenkov light.


	// Calculate time - time of flight (ttof) for each hit from testpoint.
	// Also save direction to each hit from the vertex for centroid fit.
	// timefit.cc:168 makedirtof(vertex)
	int nHits = size(hitInfoVector);
	vector<float> ttofVector;
	vector<vector<float>> hitDirectionsVector(nHits,vector<float>(3));

	for (int iHit = 0; iHit < nHits; iHit++)
	{
		ttofVector.push_back(hitInfoVector[iHit].time-TimeOfFlight(hitInfoVector,testPointVtxVector,iHit,hitDirectionsVector);
	}
	
	// Set t0 to the peak t-tof.
	float t0 = FindPeakTTof(ttofVector);

	// Find the total negative log likelihood given t0.
	float NLLikelihood = GetNegativeLogLikelihood(ttofVector,timeResidualPDF,t0);

	// Apply the angular constraint if using.
	float NLLikelihoodConstrained = NLLikelihood;
	if (UseAngle)
	{
		// Do the direction centroid fit for each testpoint only if we are 
		// going to do the angular correction to the likelihood.
		vector<float> directionVector;
		FitDirectionCentroid(hitDirectionsVector,vertexVector,t0,directionVector);
		
		// Find deviation of direction from constraining angle.
		float deviation = directionVector[3]-libConstants::sConstrainingAngle;
		// Make correction to likelihood. This varies depending on whether
		// the deviation is positive or negative.
		if (deviation > 0)
		{
			nLLikelihoodConstrained = nLLikelihood - deviation*deviation*libConstants::sPositiveAngleCorrection;
		}
		else
		{
			nLLikelihoodConstrained = nLLikelihood - deviation*deviation*libConstants::sNegativeAngleCorrection;	
		}
	}

	return(nLLikelihoodConstrained);
}

float Maximisation::TimeOfFlight(vector<HitInfo> hitInfoVector, vector<float>testPointVtxVector int iHit, vector<vector<float>>& hitDirectionsVector)
{
	// Calculates the time of flight of the light for each hit
	// in straight-line direction from the vertex being tested.
	// Saves the hit directions in a vector for the direction centroid fit.
	// hits.inline:219 tof(vertex,dir,hit)
	
	// First calculate direction of each hit from the vertex.
	hitDirectionsVector[iHit][0] = hitInfoVector[iHit].pmtx-testPointVtxVector[0];
	hitDirectionsVector[iHit][1] = hitInfoVector[iHit].pmty-testPointVtxVector[1];
	hitDirectionsVector[iHit][2] = hitInfoVector[iHit].pmtz-testPointVtxVector[2];
	float distance = sqrt(hitDirectionsVector[iHit][0]**2 + hitDirectionsVector[iHit][1]**2 + hitDirectionsVector[iHit][2]**2);
	if (distance ==0)
	{
		return(0);
	}
	// Divide all elements of vector by distance magnitude
	transform(hitDirectionsVector.begin(), hitDirectionsVector.end(), hitDirectionsVector.begin(), [distance](int &d){ return d/distance; });
	
	// Then convert to time and return.
	return(distance/libConstants::sCmPerNs)
}

float Maximisation::FindPeakTTof(vector<vector<float> ttofVector);
{
	// Find the peak time in the t-tof vector.
	// This is the time-residual range (default 0.4 ns bin width) 
	// with the highest number of hits.
	// BONSAI only include hits with t-tof > 0 but we'll look at all hits here.
	float ttofMin = *min_element(ttofVector.begin(),ttofVector.end()); 
	float ttofMax = *max_element(ttofVector.begin(),ttofVector.end());
	// Get number of bins required for given bin width (ceil rounds up to the
	// nearest integer).
	int nbins = ceil((ttofMax - ttofMin)/libConstants::sBinwidthTTOF);
	// Create a histogram for the t-tof values
	TH1F h = new TH1F("httofs","httofs",nbins,ttofMin,ttofMax+nbins*libConstants::sBinwidthTTOF);
	// Fill the histogram from the ttofs vector.
	for (float ttof : ttofVector)
	{
		httofs->Fill(ttof);
	}
	// Scale httofs
	//httofs->Scale(1/httofs->Integral());
	
	// Find the peak time bin (i.e. the one with most hits)
	int maxbin = httofs->GetMaximumBin();
	// Define the approximate peak time as the centre of the peak time bin
	float peak_ttof = httofs->GetBinCenter(maxbin);
	// Do a fit to httofs and get the x value corresponding to the max of the fit
	// Default range for peak fit +/- 12 ns
	// TODO should this be over a larger range??
	float timeRange = libConstants::sRangePeakFitTTOF*libConstants::sBinwidthPeakFitTTOF;
	TF1 *peakfit = new TF1("peakfit", "pol2", peakttofs_tmp-timeRange, peakttofs_tmp+timeRange);
	httofs->Fit("peakfit", "R");	
	peak_ttof = peakfit->GetMaximumX();
	
	// Delete ttof histogram
	delete httofs;
	
	return(peak_ttof);
}

void FitDirectionCentroid(vector<HitInfo> hitInfoVector,vector<float> ttofVector, float t0, vector<float>& directionVector)//, vector<float>& centroidFitVector)
{
	// Calculates the weight of each hit dependent on the value of the
	// time residual
	float tResLowerLimit = timeResolutionPDF->GetXaxis()->GetMinimum();
	float tResUpperLimit = timeResolutionPDF->GetXaxis()->GetMaximum();
	// First weight the hits on the 
	vector<float> weightsVector(hitInfoVector.size());
	for (int iHit = 0; iHit < ttofVector.size(); iHit++)
	{
		float time = ttofVector[iHit] - t0;
		// TODO get significance of 0.04, 0.125 and 10 and remove hard-coding
		if (time > 0)
		{
			weightsVector[iHit] -= 0.04 * time * time;
		}
		else
		{
			weightsVector[iHit] -= 0.125 * time * time;
		}
		if (weightsVector[iHit] > -10 && time > tResLowerLimit && time < tResUpperLimit)
		{
			weightsVector[iHit] = timeResolutionPDF->GetBinContent(timeResolutionPDF->FindBin(time)) * exp(weight[iHit]);
		}
		else 
		{
			weight[iHit] = 0;
		}

	}
	
	if (accumulate(weightsVector.begin(), weightsVector.end(), 0.0) > 0)
	{
		FindDirectionCentroid(hitDirectionsVector, weightsVector, directionVector);
	}
	
}


ivoid FindDirectionCentroid(vector<vector<float>> hitDirectionsVector, vector<float> weightsVector, vector<float>& directionVector)
{

	// TODO 
	vector<float> centroidVector(9);
	for (int iHit = 0; iHit< weightsVector.size(); iHit++)
	{
		if (weightsVector[iHit] > 0)
		{
			float x = hitDirectionsVector[iHit][0];
			float y = hitDirectionsVector[iHit][1];
			float z = hitDirectionsVector[iHit][2];
			float weight = weightsVector[iHit];
			float wTotal;
			directionVector[0] += x*weight;
			directionVector[1] += y*weight;
			directionVector[2] += z*weight;
			wTotal += weight;
		}
	}

	// Divide by sum of non-zero weights.
	transform(directionVector.begin(), directionVector.end(), directionVector.begin(), [wTotal](int &c){ return c/wTotal; });

	// Get magnitude of direction.
	directionVector[3] = sqrt( directionVector[0]*directionVector[0] + directionVector[1]*directionVector[1] + directionVector[2]*directionVector[2] );
	// Get unit vectors if magnitude > 0.
	if (directionVector[3] > 0)
	{
		directionVector[0]/=directionVector[3];
		directionVector[1]/=directionVector[3];
		directionVector[2]/=directionVector[3];
	}
	else
	{
		directionVector[4] = 0;
		return;
	}

	// Find the weighted median cos theta.
	// First get cos theta between the centroid and each hit.
	vector<vector<float>> cosThetaVector(hitDirectionsVector.size(),2);
	for (int iHit = 0; iHit< hitDirectionsVector.size(); iHit++)
	{
		float cosTheta = (directionVector[0]*hitDirectionsVector[iHit][0] + directionVector[1]*hitDirectionsVector[iHit][1] + directionVector[2]*hitDirectionsVector[iHit][2])/directionVector[3];
		cosThetaVector.push_back( {cosTheta,weightsVector[iHit]} );
	}

	// Sort cosThetaVector in order of increasing cos theta.
	sort(cosThetaVector.begin(),
          cosThetaVector.end(),
          [] (const vector<double> &vec1, const vector<double> &vec2)
          {
              return vec1[0] < vec2[0];
          });

	// Find value of minimum positive value of cos theta for which 
	// 200*weight > 0.05*weight[cosTheta=1]
	int medianIndex = 0;
	float sum = wTotal - cosThetaVector[0][1];
	while (sum > cosThetaVector[iHit])
	{
		++medianIndex;
		sum -= weightsVector[medianIndex];
	}
	float medianCosTheta = cosThetaVector[medianIndex];
	
	//For each hit, calculate
	//bin = (200 * direction[0]*hitDirection[0]+direction[1]+hitDirection[1]+Direction[2]+hitDirection[2])/direction[3];
	// Add weights to histogram
	//
	// Then calculate angle??
	// Find the first (lowest-number bin) for which weight > weights of hits beyond 200 
	// then do mucang = mu cos theta??? 
	//mucang = (bin+0.5) * direction[3]/200;
	return;	
}

float GetNegativeLogLikelihood(vector<float> ttofVector, TH1F timeResidualPDF, float t0)
{
	// Sum up the log likelihood for all hits for the given vertex and time t0.
	// Get the likelihood (probability) from the pdf for each ttof - t0.
	// Add the negative log(likelihood) for each value of ttof - t0.
	for (int ihit = 0; ihit < ttofVector.size(); ihit++)
	{
		float negativeLogLikelihood = 0;
		float max = numeric_limits<float>::lowest;//lowest number for comparison
		if (libConstants::sUseCharge == 1)
		{
			int bin = timeResidualPDF->FindBin(ttofVector[ihit]-t0,hitInfoVector[ihit].charge);
			float likelihood = GetBinContent(bin);
			negativeloglikelihood += -log(likelihood);
		}
		else
		{
			int bin = timeResidualPDF->GetBinContent(ttofVector[ihit]-t0);
			float likelihood = GetBinContent(bin);
			negativeLogLikelihood += -log(likelihood);
		}
	}

	return(negativeLogLikelihood)
	
}


/*
 	// Potentially have a function which can scale or add the DR.
	// The following is a starting point but will need to be tweaked.
	// Set the signal range
	// Find the maximum and minimum time residuals
	float ttofMin = *min_element(ttofVector.begin(),ttofVector.end()) - t0;
	float ttofMax = *max_element(ttofVector.begin(),ttofVector.end()) - t0;
	float timeRange = ttofMax - ttofMin;
	//int minbin = timeResidualPDF->GetXaxis()->FindBin(ttofMin);
	//int maxbin = timeResidualPDF->GetXaxis()->FindBin(ttofMax);

	// Get the renormalisation factor for reducing the 
	// pdf to the signal range
	// probability * integrated noise * signal over Noise / integrated signal
	float renormalisation;
	float timeRangePDF = timeResidualPDF->GetXaxis()->GetMaximum() - timeResidualPDF->GetXaxis()->GetMinimum();
	float noiseIntegral = libConstants::darkrate*timeRangePDF;
	float signalIntegral = timeResidualPDF->Integral();
	timeResidualPDF->GetXaxis()->SetMaximumBin(maxbin);
	timeResidualPDF->GetXaxis()->SetMinimumBin(minbin);

	if (addDR)
	{
		// Estimate dark noise and scale the pdf accordingly.
		// Used if the timeResidualPDF does not include dark rate.
		// TODO Estimate dark noise from ttofVector or read from pmt info.
		// Get signal over noise for scaling
		float noiseReducedIntegral = libConstants::darkrate*time_range;
		float signalReducedIntegral = timeResidualPDF->Integral();
		float signalOverNoise = signalReducedIntegral/noiseReducedIntegral;
		float scaleFactor = NoiseIntegral * signalOver
		timeResidualPDF->Scale(1+signalOverNoise);
		
	}
	
*/
			


