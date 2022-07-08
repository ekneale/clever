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
	vector<int> is_rel(nhits);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}
	float distance = distance2.DeltaDistance2(2,3,hitinfo);
	float d = 0;
	EXPECT_EQ(distance,d);

}


TEST(HitSelectTest,TestCheckCoincidence){
	
	HitSelect checkcoincidence;
	int nhits = 10;
	
	vector<int>  is_rel(nhits);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}
	int check  = checkcoincidence.CheckCoincidence(2,3,hitinfo);
	int checkpoint = 1;
	EXPECT_EQ(check,checkpoint);

}

TEST(HitSelectTest,TestCheckCausal){
	
	HitSelect checkcausal;
	int nhits = 10;
	
	vector<int>  is_rel(nhits);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}
	int check  = checkcausal.CheckCausal(2,3,hitinfo);
	int checkpoint = 1;
	EXPECT_EQ(check,checkpoint);

}


TEST(HitSelectTest,TestRemoveIsolatedHits){
	
	HitSelect remove;
	int nhits = 10;
	vector<int>  is_rel(nhits);
	vector<float> times = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_x = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z = {1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(0,0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}

	int nsel = remove.RemoveIsolatedHits(nhits,hitinfo);
	int nsel_check = 10;
	EXPECT_EQ(nsel,nsel_check);
}


TEST(HitSelectTest,TestGetCausallyRelatedHits){
	
	HitSelect causal;
	int nhits = 10;
	vector<int> is_rel(nhits);
	vector<int> nrelated 	= 	{1,5,6,0,7,8,9,8,3,3};
	vector<float> times 	= 	{1,1,1,2,1,1,1,1,1,1};
	vector<float> charges 	= 	{1,2,5,4,1,9,3,8,6,7};
	vector<float> pmt_x 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		hitinfo.push_back(HitInfo(nrelated[i],0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}

	int nsel = causal.GetCausallyRelatedHits(nhits,hitinfo);
	int nsel_check = 9;
	vector<float> charge_ordered;
	vector<int> nrelated_ordered;
	for (int i = 0; i<nsel; i++)
	{
		charge_ordered.push_back(hitinfo[i].charge);
		nrelated_ordered.push_back(hitinfo[i].nrelated);
	}
	vector<float> charge_check = {3,9,8,1,5,2,7,6,1};
	vector<float> nrelated_check = {9,8,7,6,5,3,3,1,0};
	EXPECT_EQ(nsel,nsel_check);
	EXPECT_EQ(charge_ordered,charge_check);
}

TEST(HitSelectTest,FindClusterCandidate){

	//TODO Need better test vectors here	
	HitSelect clus;
	int nhits = 10;
	vector<int> cluster(nhits);
	vector<int> is_rel(nhits);
	vector<int> nrelated 	= 	{9,8,7,6,5,3,3,2,1,0};
	vector<float> times 	= 	{1.2,1.3,1.4,2.0,1.5,1.6,1.7,1.8,1.9,1.95};
	vector<float> charges 	= 	{3,9,8,1,5,2,7,6,1,0};
	vector<float> pmt_x 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		fill(is_rel.begin(),is_rel.end(),1);
		is_rel[i] = 0;
		hitinfo.push_back(HitInfo(nrelated[i],0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}

	int nsel = clus.FindClusterCandidate(nhits,2,3,hitinfo,cluster);
	int nsel_check = 10;
	vector<int> nrelated_clus_check {1,1};
	vector<int> nrelated_clus;

	EXPECT_EQ(nsel,nsel_check);
}

TEST(HitSelectTest,TestFindHitClusters){
		
	HitSelect clus2;
	int nhits = 10;
	vector<int> is_rel(nhits);
	vector<int> nrelated 	= 	{9,8,7,6,5,3,3,2,1,0};
	vector<float> times 	= 	{1.2,1.3,1.4,2.0,1.5,1.6,1.7,1.8,1.9,1.95};
	vector<float> charges 	= 	{3,9,8,1,5,2,7,6,1,0};
	vector<float> pmt_x 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_y 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<float> pmt_z 	= 	{1,1,1,1,1,1,1,1,1,1};
	vector<HitInfo> hitinfo;
	for (int i = 0; i<nhits; i++)
	{
		fill(is_rel.begin(),is_rel.end(),1);
		is_rel[i] = 0;
		hitinfo.push_back(HitInfo(nrelated[i],0,0,0,is_rel,times[i],charges[i],pmt_x[i],pmt_y[i],pmt_z[i]));
	}

	int nsel = clus2.FindHitClusters(nhits,hitinfo);
	int nsel_check = 10;
	EXPECT_EQ(nsel,nsel_check);
	vector<float> time_ordered;
	for (int i = 0; i<nsel; i++)
	{
		time_ordered.push_back(hitinfo[i].time);
	}
	vector<float> time_check = {2.0,1.95,1.9,1.8,1.7,1.6,1.5,1.4,1.3,1.2};
	EXPECT_EQ(time_ordered,time_check);
}

TEST(HitSelectTest,TestSelectHits){
	
	HitSelect select;
	int nhits = 10;
	vector<float> times	  = {1,1,1,1,1,1,1,1,1,1};
	vector<float> charges = {1,2,5,4,1,9,3,8,6,7};
	vector<float> pmtx = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmty = {1,1,1,1,1,1,1,1,1,1};
	vector<float> pmtz = {1,1,1,1,1,1,1,1,1,1};

	int nsel_final = select.SelectHits(nhits,times,charges,pmtx,pmty,pmtz);
	int nsel_final_check = 10; //???? TODO can't be right????!!!!!
	EXPECT_EQ(nsel_final,nsel_final_check);

}


//}
