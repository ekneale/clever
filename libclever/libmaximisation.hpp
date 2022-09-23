#ifndef LIBMAXIMISATION_H
#define LIBMAXIMISATION_H

//includes
#include <vector>
#include <libhitinfo.hpp>

using namespace std;

/*
 * class Maximisation
 *
 * Author	L.Kneale
 * Date		26/04/2022
 * Contact	e.kneale@sheffield.ac.uk
 */


class Maximisation
{


	// define the public functions and variables
	public:

		Maximisation();
		~Maximisation();
		
		// Main function called from outside class.
		Maximise(vector <HitInfo> hitInfoVector, vector<vector<float>> testPointsVector);
		// Principal functions which perform the likelihood calculation and 
		// which are called by the main Maximise() function.
		// (Strictly private functions but public to be available for 
		// running unit tests.)
		FindInitialLikelihoods(vector<HitInfo> hitInfoVector, float rmin, vector<vector<float>> testPointsVector);


		// Subsidiary functions called by the the principal functions.
		CalculateLikelihood(vector<HitInfo> hitInfoVector, vector<float>& pointVector);

	// define the private functions and variables
	private:

		


		
};

#endif

