/**************************************************
 * Unit test for setgeometry function.
 *
 * *************************************************/

#include <libgeometry.hpp>
#include <libconstants.hpp>
#include <gtest/gtest.h>
#include <math.h>

namespace{

TEST(GeometryTest,TestSetGeometry){

	vector<float> pmtx = {0,100,200,300,400,500};
	vector<float> pmty = {0,100,200,300,400,500};
	vector<float> pmtz = {0,100,200,300,400,500};
	int numPMTs = 6;

	float r = sqrt(pow(500,2)+pow(500,2));
	float z = 500;
	float t = 2*sqrt(r*r+z*z)/libConstants::cm_per_ns;
	r -= 50;
	z -= 50;

	Geometry geo;
	geo.SetGeometry(numPMTs,pmtx,pmty,pmtz);

	float search_r = geo.search_radius();
	float search_z = geo.search_height();
	float maxtime = geo.max_traverse_time();
//	float maxdt = geo.max_pmt_deltaT();
//	float maxdd = geo.max_pmt_deltaD();

	//TODO check maxdt and maxdd
	EXPECT_EQ(search_r,r);
	EXPECT_EQ(search_z,z);
	EXPECT_EQ(maxtime,t);

}

}
