#ifndef LIBHITINFO_H
#define LIBHITINFO_H

//includes
#include <vector>

using namespace std;

/*
 * Class HitInfo
 * Creates the hit info class for storing vectors of hit information
 *
 * Author	L.Kneale
 * Date		26/04/2022
 * Contact	e.kneale@sheffield.ac.uk
 */


class HitInfo
{


	// define the public hit info class for storing all of the info needed
	// for the hit selection and reconstruction process
	public:

		vector<HitInfo> hitinfo;
		int nrelated;
		int nselected;
		int is_selected;
		int noccurrence;
		vector <int> is_related;
		float time;
		float charge;
		float pmtx;
		float pmty;
		float pmtz;

		HitInfo(int nrel, int nsel, int is_sel, int noccur, vector<int> is_rel, float t, float q, float x, float y, float z){ nrelated = nrel, nselected = nsel, is_selected = is_sel, noccurrence = noccur, is_related = is_rel, time = t, charge = q, pmtx = x, pmty = y, pmtz = z;};
		~HitInfo() {};

		void set_related(int i)
		{
			is_related[i] = 1;
		}

		int get_related(int i)
		{
			return is_related[i];
		}

		void unset_related(int i)
		{ 
//			for (int i = 0; i< n; i++)
//			{
//				is_related[i] = 0;
//			}
			is_related[i] = 0;
		}
		

	// define the private functions and variables
	//private:

};

#endif
