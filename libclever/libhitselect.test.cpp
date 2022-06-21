/**************************************************
 * Unit test for setgeometry function.
 *
 * *************************************************/

#include <libhitselect.hpp>
#include <libconstants.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <libgeometry.hpp>

//namespace{

TEST(DISABLED_HitSelectTest,TestDeltaDistance2){
	
	libHitSelect distance2;
	int nhits = 10;
	float times[10] = {1,1,1,1,1,1,1,1,1,1};
	float charges[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmtx[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmty[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmtz[10] = {1,1,1,1,1,1,1,1,1,1};
	float distance = distance2.DeltaDistance2(2,3,pmtx,pmty,pmtz);
	float d = 0;
	EXPECT_EQ(distance,d);

}


TEST(HitSelectTest,TestCheckCoincidence){
	
	libHitSelect checkcoincidence;
	int nhits = 10;
	float times[10] = {1,1,1,1,1,1,1,1,1,1};
	float charges[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmtx[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmty[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmtz[10] = {1,1,1,1,1,1,1,1,1,1};

	int check  = checkcoincidence.CheckCoincidence(2,3,times,pmtx,pmty,pmtz);
	int checkpoint = 1;
	EXPECT_EQ(check,checkpoint);

}


TEST(DISABLED_HitSelectTest,TestRemoveIsolatedHits){
	
	libHitSelect remove;
	int nhits = 10;
	float times[10] = {1,1,1,1,1,1,1,1,1,1};
	float charges[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmtx[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmty[10] = {1,1,1,1,1,1,1,1,1,1};
	float pmtz[10] = {1,1,1,1,1,1,1,1,1,1};

	int nsel = remove.RemoveIsolatedHits(nhits,times,charges,pmtx,pmty,pmtz);
	EXPECT_EQ(nsel,10);

}


//}
