/**************************************************
 * Unit test for setgeometry function.
 *
 * *************************************************/

#include <libhitselect.hpp>
#include <libconstants.hpp>
#include <libhitselect.test.hpp>
#include <libhitinfo.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <vector>

//namespace{

TEST(HitSelectTest,TestDeltaDistance2){
	
	HitSelect distance2;
	int nhits = 10;
	vector<int> is_rel(1);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}
	float distance = distance2.DeltaDistance2(2,3,hitinfo);
	float d = 0;
	EXPECT_EQ(distance,d);

}


TEST(HitSelectTest,TestCheckCoincidence){
	
	HitSelect checkcoincidence;
	int nhits = 10;
	
	vector<int>  is_rel(1);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}
	int check  = checkcoincidence.CheckCoincidence(2,3,hitinfo);
	int checkpoint = 1;
	EXPECT_EQ(check,checkpoint);

}

TEST(HitSelectTest,TestCheckCausal){
	
	HitSelect checkcausal;
	int nhits = 10;
	
	vector<int>  is_rel(1);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}
	int check  = checkcausal.CheckCausal(2,3,hitinfo);
	int checkpoint = 1;
	EXPECT_EQ(check,checkpoint);

}


TEST(HitSelectTest,TestRemoveIsolatedHits){
	
	HitSelect remove;
	int nhits = 10;
	vector<int>  is_rel(1);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}

	int nsel = remove.RemoveIsolatedHits(nhits,hitinfo);
	int nsel_check = 10;
	EXPECT_EQ(nsel,nsel_check);
}


TEST(HitSelectTest,TestGetCausallyRelatedHits){
	
	HitSelect causal;
	int nhits = 10;
	vector<int>  is_rel(1);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}

	int nsel = causal.GetCausallyRelatedHits(nhits,hitinfo);
	int nsel_check = 10;
	EXPECT_EQ(nsel,nsel_check);
}

TEST(HitSelectTest,TestSelectHits){
	
	HitSelect select;
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
