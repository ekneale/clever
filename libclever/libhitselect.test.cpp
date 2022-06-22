/**************************************************
 * Unit test for setgeometry function.
 *
 * *************************************************/

#include <libhitselect.hpp>
#include <libconstants.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <vector>

//namespace{

TEST(HitSelectTest,TestDeltaDistance2){
	
	libHitSelect distance2;
	int nhits = 10;
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtx = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmty = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtz = {1,1,1,1,1,1,1,1,1,1};
	float distance = distance2.DeltaDistance2(2,3,pmtx,pmty,pmtz);
	float d = 0;
	EXPECT_EQ(distance,d);

}


TEST(HitSelectTest,TestCheckCoincidence){
	
	libHitSelect checkcoincidence;
	int nhits = 10;
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtx = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmty = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtz = {1,1,1,1,1,1,1,1,1,1};

	int check  = checkcoincidence.CheckCoincidence(2,3,times,pmtx,pmty,pmtz);
	int checkpoint = 1;
	EXPECT_EQ(check,checkpoint);

}


TEST(HitSelectTest,TestRemoveIsolatedHits){
	
	libHitSelect remove;
	int nhits = 10;
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtx = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmty = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtz = {1,1,1,1,1,1,1,1,1,1};

	int nsel = remove.RemoveIsolatedHits(nhits,times,charges,pmtx,pmty,pmtz);
	int nsel_check = 10;
	EXPECT_EQ(nsel,nsel_check);

}

TEST(HitSelectTest,TestSelectHits){
	
	libHitSelect select;
	int nhits = 10;
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtx = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmty = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtz = {1,1,1,1,1,1,1,1,1,1};

	int nsel_final = select.SelectHits(nhits,times,charges,pmtx,pmty,pmtz);
	int nsel_final_check = 10;
	EXPECT_EQ(nsel_final,nsel_final_check);

}

//}
