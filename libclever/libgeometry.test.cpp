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

	float pmtx[6] = {0,100,200,300,400,500};
	float pmty[6] = {0,100,200,300,400,500};
	float pmtz[6] = {0,100,200,300,400,500};
	float numPMTs = 6;

	float r = sqrt(pow(500,2)+pow(500,2));
	float z = 500;
	float t = 2*sqrt(r*r+z*z)/libConstants::ns_to_cm;
	r -= 50;
	z -= 50;

	libGeometry geo;
	geo.SetGeometry(numPMTs,pmtx,pmty,pmtz);
	float search_r = geo.search_radius();
	float search_z = geo.search_height();
	float maxtim = geo.max_traverse_time();

	EXPECT_EQ(search_r,r);
	EXPECT_EQ(search_z,z);
	EXPECT_EQ(maxtim,t);

}

}
