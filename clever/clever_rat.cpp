/**********************************************************
 * Reads in the hit charge and time information from file 
 * and runs through the steps in the reconstruction.
 * *******************************************************/

#include <iostream>

#include <vector>

#include <libhitinfo.hpp>
#include <libhitselect.hpp>
#include <libgeometry.hpp>
#include <libconstants.hpp>
#include <libtestpointcalc.hpp>

using namespace std;

/*#include <RAT/DS/Run.hh>
#include <RAT/DS/PMTInfo.hh>
#include <RAT/DS/Root.hh>
#include <RAT/DS/MC.hh>
#include <RAT/DS/MCParticle.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/PMT.hh>*/

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

//Need to separate the Inner-Detector tubes from the Outer-Detector tubes
static const int innerPMTcode = 1;
static const int vetoPMTcode  = 2;

bool sortcol( const vector<float>& v1,const vector<float>& v2 ) {
  return v1[3] < v2[3];
  }


int main(int argc, char **argv){

	// ROOT stuff
	TFile *f;
	TTree *rat_tree,*run_tree,*data;
	Int_t n_events;
	TTree *run_summary;

	// rat stuff
	RAT::DS::Root *ds=new RAT::DS::Root();
	RAT::DS::Run  *run=new RAT::DS::Run();
	RAT::DS::EV *ev;
	RAT::DS::PMTInfo *pmtinfo;
	RAT::DS::MC *mc;
	RAT::DS::MCParticle *prim;
	RAT::DS::PMT *pmt;

	/***********************************************************************/
	// Open input file and get trees
	f= new TFile(argv[1]);
	
	rat_tree=(TTree*) f->Get("T");
	rat_tree->SetBranchAddress("ds", &ds);
	if (rat_tree==0x0 || run_tree==0x0)
	{
		printf("can't find trees T and runT in this file\n");
		return -1;
	}
	
	run_tree=(TTree*) f->Get("runT");
	run_tree->SetBranchAddress("run", &run);
	if (run_tree->GetEntries() != 1)
	{
		printf("More than one run! Ignoring all but the geometry for the first run\n");
	}
	run_tree->GetEntry(0);


	/**********************************************************************/
	// Set the inner-volume geometry using the detector PMT information.

	// Get number of inner PMTs in detector, plus their x, y and z positions.
	pmtinfo=run->GetPMTInfo();
	int nPMTs_all = pmtinfo->GetPMTCount();
	int nPMTs = 0;
	vector<float> pmt_x;
	vector<float> pmt_y;
	vector<float> pmt_z;

	float pmtBoundR = 0;
	float pmtBoundZ = 0;

	// Loop over all of the PMTs
	for(int pmtindex=0; pmtindex<nPMTs_all; pmtindex++)
	{
		// Get positions of inner PMTs
		if(pmtinfo->GetType(pmtindex)==innerPMTcode)
		{
			// Positions in cm.
			vector<float> pos=pmtinfo->GetPosition(pmtindex);
			pmt_x.push_back(pos[0]);
			pmt_y.push_back(pos[1]);
			pmt_z.push_back(pos[2]);
			// Check the dimensions of the support structure
			// (we can remove this later as this is done by SetGeometry).
			if (pos[0]>pmtBoundR) 
			{
				pmtBoundR = pos[0];
			}
			if (pos[2]>pmtBoundZ) 
			{
				pmtBoundZ = pos[2];
			}
			nPMTs++;
		}
	}

	printf("Inner PMT boundary (r,z):(%4.1f cm %4.1f, cm)\n",pmtBoundR,pmtBoundZ);
	printf("Inner search boundary (r,z):(%4.1f cm %4.1f, cm)\n",pmtBoundR-50,pmtBoundZ-50);

	Geometry geo;
	geo.SetGeometry(nPMTs,pmt_x,pmt_y,pmt_z);
	float traverseTmax = geo.max_traverse_time();
	float dTmax = geo.max_pmt_deltaT();
	float dRmax = geo.max_pmt_deltaR();
	float zmax = geo.search_radius();
	float rmax = geo.search_height();
	float rmax2 = rmax*rmax;

	printf("Inner search boundary from geo (r,z):(%4.1f cm %4.1f, cm)\n",pmtBoundR,pmtBoundZ);


	/************************************************************************/
	// Get number of hits, plus time, charge, pmtx, pmty and pmtz for all hits.
	
	int nhits = 0;
	vector<float> times;
	vector<float>charges;
	vector<float> pmtx;
	vector<float> pmty;
	vector<float> pmtz;
  		
	// TODO Loop over all events.
	//n_events = rat_tree->GetEntries();
  	//for (event = 0; event < n_events; event++)
	// Just look at the first event for now.
	for (int event = 0; event < 1; event++)
    {
		rat_tree->GetEntry(event);
      	// loop over all subevents
		for(int sub_event=0;sub_event<ds->GetEVCount();sub_event++)
		{
			ev = ds->GetEV(sub_event);
			int nhit =ev->GetPMTCount();
			// loop over all PMT hits for this subevent
        	for(int hit=0; hit<nhit; hit++)
          	{
          		pmt=ev->GetPMT(hit);
          		int id = pmt->GetID();
				//only use information from the inner pmts
				if(pmtinfo->GetType(id) == innerPMTcode)
            	{
					TVector3 pos = pmtinfo->GetPosition(id);
					pmtx.push_back(pos[0]*0.1);    			//x
              		pmty.push_back(pos[1]*0.1);       		//y
					pmtz.push_back(pos[2]*0.1);       		//z
					times.push_back(pmt->GetTime());   		//t
					charges.push_back(pmt->GetCharge());	//q
              		nhits++;
        		}
			}

		}

	// TODO 
	for (int i=0;i<nhits;i++)

	

	
	// Select the hits which will be used to calculate starting points (initial
	// test vertices) for the search.
	vector<HitInfo> hitinfo;
	HitSelect select;
	int nselected =	select.SelectHits(nhits,times,charges,pmtx,pmty,pmtz,hitinfo,traverseTmax,dTmax,dRmax);

	// Make sure at least 4 hits have made the final selection.
	if (nselected<=libConstants::min_selected_hits)
	{
		return(-1); //continue; TODO when looping over events
	}

	// TODO info logging - cout hitinfo for the selected hits.

	// Calculate the initial test vertices fr the search.
	TestPointCalc testpointcalc;
	vector<vector<float>> testpoints;
	testpointcalc.CalculateTestPoints(hitinfo,  rmax2, zmax, testpoints);

	// TODO perform the maximum likelihood fit starting from the final list
	// of testpoints calculated in the previous steps.
	// Initial search
	// Coarse search
	// Fine search
	// Final search

	// Get the vertex and additional variables.
	
	} // End of loop over sub events.
	
	return 0;
}
