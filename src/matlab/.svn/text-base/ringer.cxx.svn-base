 /**
 * @file ringer.cxx
 * @author <a href="mailto:Rodrigo.Torres@cern.ch">Rodrigo TORRES</a> 
 *
 * Matlab version of the ringer. Arranges calorimetry cell data (roiformat dump) 
 * in concentric rings of configurable size and center and dump an XML file 
 * containing the ring'ified information for every RoI and their metadata 
 * (LVL1 and RoI identifiers and etaxphi information).
 */


#include "mex.h"
#include "matrix.h"
#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/matlab/MatlabReporter.h"
#include "TrigRingerTools/sys/Exception.h"
#include "TrigRingerTools/sys/debug.h"
#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/OptParser.h"
#include "TrigRingerTools/data/Database.h"
#include "TrigRingerTools/roiformat/Database.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <map>
#include <string>

#define CONFIG_FILE 1
#define ROI_LIST 0
#define	GLOBAL_CENTER 2
#define	ETA_WINDOW 3
#define	PHI_WINDOW 4
#define LVL1_ID "LVL1_Id"
#define ROI_ID "RoI_Id"
#define LVL1_ETA "LVL1_Eta"
#define LVL1_PHI "LVL1_Phi"
#define RINGS "Rings"


void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{	
  sys::Reporter *reporter = new sys::MatlabReporter();

	/* Check for proper number of arguments. */
  if (nrhs < 2) {RINGER_FATAL(reporter, "Too few input parameters passed! See help.");}
  else if (nlhs > 1) {RINGER_FATAL(reporter, "Too many output arguments");}

	bool globalCenter = false;
	double phiWindow = 0.1;
	double etaWindow = 0.1;	
	if (nrhs == 3) globalCenter = static_cast<bool>(mxGetScalar(prhs[GLOBAL_CENTER]));
	if (nrhs == 4) etaWindow = mxGetScalar(prhs[ETA_WINDOW]);
	if (nrhs == 5) phiWindow = mxGetScalar(prhs[PHI_WINDOW]);

	if ( mxIsChar(prhs[CONFIG_FILE]) != 1) RINGER_FATAL(reporter, "Rings configuration file must be a string!");
	size_t nRoI = mxGetN(prhs[ROI_LIST]);

  try
  {
    // reading the config file.
    const std::string configFile = mxArrayToString(prhs[CONFIG_FILE]); 
    rbuild::Config config(configFile, reporter);
    RINGER_REPORT(reporter, "Loaded Ring configuration at \"" << configFile << "\".");
   
    //do the processing:
    //1. For each configured ring set, create a *real* RingSet
    std::vector<rbuild::RingSet> rset;
    const std::map<unsigned int, rbuild::RingConfig>& rconfig = config.config();
    
    unsigned int nrings = 0;
    for (std::map<unsigned int, rbuild::RingConfig>::const_iterator it=rconfig.begin(); it!=rconfig.end(); ++it)
    {
      rset.push_back(it->second);
      nrings += it->second.max();
    } //creates, obligatorily, ordered ring sets


		const char *strNames[5] = {LVL1_ID, ROI_ID, LVL1_ETA, LVL1_PHI, RINGS};
		int matDim[2] = {1, nRoI};
		mxArray *ret = mxCreateStructArray(2, matDim, 5, strNames);

    for (size_t i=0; i<nRoI; i++)
    {
    	size_t nCells = mxGetN(mxGetField(prhs[ROI_LIST], i, "ECells"));
    	double *matEnergy = mxGetPr(mxGetField(prhs[ROI_LIST], i, "ECells"));
    	double *matLayer = mxGetPr(mxGetField(prhs[ROI_LIST], i, "DetCells"));
    	double *matPhi = mxGetPr(mxGetField(prhs[ROI_LIST], i, "PhiCells"));
    	double *matEta = mxGetPr(mxGetField(prhs[ROI_LIST], i, "EtaCells"));
    	unsigned matLv1Id = static_cast<unsigned>(mxGetScalar(mxGetField(prhs[ROI_LIST], i, "LVL1_Id")));
    	unsigned matRoIId = static_cast<unsigned>(mxGetScalar(mxGetField(prhs[ROI_LIST], i, "RoI_Id")));
    	double matLv1Eta = mxGetPr(mxGetField(prhs[ROI_LIST], i, "LVL1_Eta"))[0];
    	double matLv1Phi = mxGetPr(mxGetField(prhs[ROI_LIST], i, "LVL1_Phi"))[0];

    	std::vector<roiformat::Cell> cells;
    	for (size_t j = 0; j<nCells; j++)
    	{
    		roiformat::Cell cell(static_cast<roiformat::Cell::Sampling>((int) matLayer[j]), matEta[j], matPhi[j], 0., 0., 0., 0., matEnergy[j]);
    		cells.push_back(cell);
    	}
    	const roiformat::RoI roi(cells, matLv1Id, matRoIId, matLv1Eta, matLv1Phi);
    
      RINGER_REPORT(reporter, "Calculating rings for RoI -> L1Id #" << roi.lvl1_id() << " and RoI #" << roi.roi_id());
      double eta, phi; //center values
      bool ok = rbuild::find_center(reporter, &roi, eta, phi);
      // only use layer 2 center, if an explicit request was made
      ok &= globalCenter; 
      rbuild::build_rings(reporter, &roi, rset, ok, eta, phi, etaWindow, phiWindow);
      rbuild::normalize_rings(reporter, rset);
     
      //Copying the rings for the Matlab return format.
      mxArray *matRings = mxCreateDoubleMatrix(1, nrings, mxREAL);
      double *rings = mxGetPr(matRings);
      size_t k = 0;
      for (std::vector<rbuild::RingSet>::iterator it=rset.begin(); it!=rset.end(); it++)
      {
      	data::Pattern pat = it->pattern();
      	for (size_t j=0; j<pat.size(); j++) rings[k++] = pat[j]; 
      }
      
      mxSetField(ret, i, LVL1_ID, mxCreateDoubleScalar(static_cast<double>(roi.lvl1_id())));
      mxSetField(ret, i, ROI_ID, mxCreateDoubleScalar(static_cast<double>(roi.roi_id())));
			mxSetField(ret, i, LVL1_ETA, mxCreateDoubleScalar(roi.eta()));
			mxSetField(ret, i, LVL1_PHI, mxCreateDoubleScalar(roi.phi()));
			mxSetField(ret, i, RINGS, matRings);

    } //for all RoI's

		plhs[0] = ret;

  } //try clause
  catch (sys::Exception& ex)
  {
    RINGER_FATAL(reporter, ex.what());
  }

  delete reporter;
}
