//vim :set noexpandtab tabstop=4 wrap

//includes
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <eigen3/Eigen/Dense>
#include <libconstants.hpp>
#include <libtestpointcalc.hpp>
#include <libfourhitcombos.hpp>

using namespace Eigen;

// ************************************************************************** //
// Main CalculateVertices functions take four-hit combinations from the list of 
// clustered hits output by SelectHits and calculates points of convergence in
// 3D space. These will be used as initial test vertices to begin the search.


//constructor function
TestPointCalc::TestPointCalc()
{
	CalculateTestPoints(hitinfo, rmax, zmax, testpoints);
}

//destructor function
TestPointCalc::~TestPointCalc()
{
	// TODO
}


int TestPointCalc::CalculateTestPoints(vector<HitInfo>& hitinfo, float rmax2, float zmax, vector<vector<float>>& testpoints)
{

	// Calculates vertices from four-hit combinations.
	int nselected = size(hitinfo);

	// Create a vector to store the upper bound of hit combinations for each 
	// selected hit.
	vector<int> combos_upper_bounds;
	int ncombos;

	// Define allowed ranges of hit numbers using absolute timing
	// to give as close to the ideal number of combinations as possible.
	// Also fills combos_upper_bounds with ranges for hit combinations.
	FourHitCombos fourhitcombos;
	ncombos = fourhitcombos.GetFourHitCombos(hitinfo,combos_upper_bounds);
	
	// For each hit, calculate a test point in front of each hit and add to 
	// the testpoints vector (without averaging over nearby hits).
	for (int hit=0; hit<ncombos-3; hit++)
	{
		FrontOfPMTTestPoints(hitinfo[hit].pmtx,hitinfo[hit].pmty,hitinfo[hit].pmtz, rmax2, zmax, testpoints);
	}

	// Then compute a testpoint for all four-hit combinations within the ranges
	// found and fill a temporary vector.
	vector<vector<float>> testpoints_tmp;
	FourHitComboTestPoints(hitinfo,combos_upper_bounds,testpoints_tmp);
	
	// sparsify(dmin) average points that are closer to each other than dmin
	// sparsify(init_grid_constant()); initial grid constant = 40 cm.
	// Check which is smaller out of bgrid and cgrid then sparsify with 
	// smallest first. BUT bgrid = 30 cm cgrid = 30 cm.
	//TODO finish this
	
	return(ncombos);
}

//************************************************************************** //
// These are the principal functions called by the main CalculateVertices
// function.

void TestPointCalc::FrontOfPMTTestPoints(float pmtx, float pmty, float pmtz, float rmax2, float zmax, vector<vector<float>>& testpoints)
{
	float x, y, z;
	//Calculates the first testpoints - these points in front of the hit PMTs
	if (fabs(pmtz)<zmax)
	{
		// if it's one of the side PMTs
		// TODO fractional distance from pmt is different for side PMTs in 
		// BONSAI (0.9882 compared to 0.989)
		x = pmtx*libConstants::pmtxy_fractional_distance;
		y = pmty*libConstants::pmtxy_fractional_distance;
		z = pmtz;
	}
	else
	{
		// if it's one of the top PMTs
		// point is (1-pmt_fractional_distance)*zmax in front of the PMT
		x = pmtx;
		y = pmty;
		z = pmtz*libConstants::pmtz_fractional_distance;
	}
	if (fabs(z)<zmax && (x*x+y*y)<rmax2)
	{
		testpoints.push_back({x,y,z});
	}

}

void TestPointCalc::FourHitComboTestPoints(vector<HitInfo> hitinfo, vector<int> combos_upper_bounds, vector<vector<float>>& testpoints_tmp)
{
	int combo = 0;
	// Loop over all 4-hit combinations.
    for (int hit1=0; hit1<combos_upper_bounds[hit1]-3; hit1++)
    {
		vector <int> fourhitcombo;
		int last_hit = combos_upper_bounds[hit1];
		
		for (int hit2 = hit1+1; hit2 < last_hit-2; hit2++)
		{
			for (int hit3 = hit2+1; hit3 < last_hit-1; hit3++)
			{
				for (int hit4 = hit3+1; hit4 < last_hit; hit4++)
				{
					fourhitcombo = {hit1,hit2,hit3,hit4};
					// Calculate the testpoint from the four-hit combination
					// and add to a list of temporary testpoints.
					FourHitVertex(hitinfo,fourhitcombo,combo,testpoints_tmp);
					combo++;

				}
			}
		}
	}

}
// ******************************************************************** //
// These are the subsidiary functions called by the principal functions 
// which are in turn called by the main CalculateVertices function.

void TestPointCalc::FourHitVertex(vector<HitInfo> hitinfo, vector<int> fourhitcombo,int combo,vector<vector<float>>& testpoints_tmp)
{
	
	// Calculate testpoints from four-hit combinations.
	
	// Find the first hit. This will be used as the origin and thus set to zero.
	int firsthit = 0;

	for (int hit = 1; hit < 4; hit++)
	{

		if (hitinfo[fourhitcombo[hit]].time < hitinfo[fourhitcombo[firsthit]].time)
		{
			firsthit = hit;
		}
	}

	// Find the quad point X = (X,Y,Z,T).
	// Make three vectors (which may or may not be co-planar) from the four 
	// points by selecting the position vector of thefirst hit in time as x_0
	// and then finding the line x_i-x_0.
	// For co-planar vectors, the scalar triple product = 0; but we're not too
	// worrried about that here.
	// Set to zero the dot product of the coordinate difference vector for each
	// of the potentially co-planar line vectors x_i-x_0 and the quad point X.
	// (x_i - X)(x_i - X) = |x_i|**2 - 2x_iX + |X|**2 = 0
	// The solution to these simultaneous equations (where i in (1,3) gives X.
	// We express them in matrix form and use 'svd of an under-determined 
	// system' to find the near-zero value to give us X.
	
	// Define vectors by finding the positions and times of the other hits 
	// with respect to the first hit and fill a matrix with the quantities 
	// for calculating the vertex.
	// [1 -2dx -2dy -2dz -2dt s**2] . [|v|**2 vx vy vz 1]^T = 0
	// Omitting |v|**2, define a 3 x 5 matrix (3 vectors with 5 values each)
	// i.e. [-2dx -2dy -2dz +2dt s**2] . [vx vy vz 1]^T = 0
	MatrixXf matrix(3,5);
	VectorXf zero_vector{{0.,0.,0.,0.,0}};
	float dx, dy, dz, dt;
	for (int hit = 0; hit < 4; hit++)
	{
		if (hit != firsthit)
		{
			dx = hitinfo[fourhitcombo[hit]].pmtx - hitinfo[fourhitcombo[firsthit]].pmtx;
			dy = hitinfo[fourhitcombo[hit]].pmty - hitinfo[fourhitcombo[firsthit]].pmty;
			dz = hitinfo[fourhitcombo[hit]].pmtz - hitinfo[fourhitcombo[firsthit]].pmtz;
			dt = (hitinfo[fourhitcombo[hit]].time - hitinfo[fourhitcombo[firsthit]].time)*libConstants::cm_per_ns;
		}

		// Add line vectors to the matrix
		matrix << -2*dx;
		matrix << -2*dy;
		matrix << -2*dz;
		// Calculate 2*dt*cm_per_ns
		matrix << 2*dt;
		// Calculate dx**2 + dy**2 + dz**2 - (dt*cm_per_ns)**2
		matrix << dx*dx * dy*dy + dz*dz - dt*dt;
	}

	// Now need to find solutions for the 3 x 5 matrix.
	// Do this by performing Singular Value Decomposition (SVD) of the matrix
	// using eigen bdcSvD algorithm to find least squares solution. 
	// ComputeThinV gives a p x m matrix, such that the first m columns of V 
	// are the right singular vectors of the matrix being decomposed.
	// Find the least squares solution of Ax = 0
	BDCSVD<MatrixXf> svd(matrix, ComputeThinU | ComputeThinV);
	svd.solve(zero_vector);

	const MatrixXf &V = svd.matrixV();
	// U left singular vector maps contribution of data points to singular 
	// values.
	// S singular value importance of singular vectors decreasing rank order.
	// For Ax = 0, the solutions for x are the right singular vectors V 
	// corresponding to the  zero singular values(???)
	// Loop over the columns of V and save the vectors 
	for (int vtx = 0; vtx < V.cols(); vtx++)
	{
		vector <float> vertex(V.rows());
		Map<MatrixXf>(vertex.data(), V.rows(),1) = V.col(vtx);
		testpoints_tmp[combo].push_back(vertex[0]+hitinfo[fourhitcombo[firsthit]].pmtx);
		testpoints_tmp[combo].push_back(vertex[1]+hitinfo[fourhitcombo[firsthit]].pmty);
		testpoints_tmp[combo].push_back(vertex[2]+hitinfo[fourhitcombo[firsthit]].pmtz);
		testpoints_tmp[combo].push_back(vertex[3]+hitinfo[fourhitcombo[firsthit]].time);
	}

}

	

