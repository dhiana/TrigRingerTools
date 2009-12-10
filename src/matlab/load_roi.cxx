/**
* @file load_roi.cxx
* @author <a href="mailto:Rodrigo.Torres@cern.ch">Rodrigo TORRES</a> 
*
* Reads a ROOT NTuple and return the RoI found.
*/

#include <vector>

#include "mex.h"
#include "matrix.h"

#include "TrigRingerTools/matlab/MatlabReporter.h"
#include "TrigRingerTools/roiformat/roi_iterator.h"

const unsigned IN_NTUPLE = 0;
const unsigned OUT_STRUCT = 0;
const int NFIELDS = 11;
const char *FIELD_NAMES[] = {"lvl1_id", "roi_id", "lvl1_eta", "lvl1_phi", "lvl2_eta", "lvl2_phi", "rings", "detectors", "eta", "phi", "energy"};


template<class Type>
mxArray *createVector(std::vector<Type> &vec, const mxClassID classid)
{
  mxArray *ret = mxCreateNumericMatrix(1, vec.size(), classid, mxREAL);
  Type *ptr = static_cast<Type*>(mxGetData(ret));
  for (unsigned i=0; i<vec.size(); i++) ptr[i] = vec[i];
  vec.clear();
  return ret;
}

mxArray *createStructure(const roiformat::RoIIterator &itr)
{
  std::vector<unsigned char> det;
  std::vector<float> aux;
  
  mxArray *str = mxCreateStructMatrix(1,1,NFIELDS,FIELD_NAMES);
  mxSetField(str, 0, "lvl1_id", mxCreateDoubleScalar(static_cast<double>(itr.lvl1_id())));
  mxSetField(str, 0, "roi_id", mxCreateDoubleScalar(static_cast<double>(itr.roi_id())));
  mxSetField(str, 0, "lvl1_eta", mxCreateDoubleScalar(static_cast<double>(itr.lvl1_eta())));
  mxSetField(str, 0, "lvl1_phi", mxCreateDoubleScalar(static_cast<double>(itr.lvl1_phi())));
  mxSetField(str, 0, "lvl2_eta", mxCreateDoubleScalar(static_cast<double>(itr.lvl2_eta())));
  mxSetField(str, 0, "lvl2_phi", mxCreateDoubleScalar(static_cast<double>(itr.lvl2_phi())));
  
  itr.rings(aux);
  mxSetField(str, 0, "rings", createVector(aux, mxSINGLE_CLASS));
  itr.detectors(det);
  mxSetField(str, 0, "detectors", createVector(det, mxUINT8_CLASS));
  itr.eta(aux);
  mxSetField(str, 0, "eta", createVector(aux, mxSINGLE_CLASS));
  itr.phi(aux);
  mxSetField(str, 0, "phi", createVector(aux, mxSINGLE_CLASS));
  itr.energy(aux);
  mxSetField(str, 0, "energy", createVector(aux, mxSINGLE_CLASS));
  
  return str;
}


void mexFunction( int nargout, mxArray *outArgs[], int nargin, const mxArray *inArgs[] )
{	
  sys::Reporter *reporter = new sys::MatlabReporter();
	
	//Check for proper number of arguments.
  if (nargin < 1) {RINGER_FATAL(reporter, "Too few input parameters passed! See help.");}
  
  roiformat::RoIIterator itr;
  itr.add(mxArrayToString(inArgs[IN_NTUPLE]));
  
  mxArray *ret = mxCreateCellMatrix(1, itr.getNumRoIs());
  unsigned pos = 0;
  
  while (itr.next())
  {
    mxSetCell(ret, pos++, createStructure(itr));
  }

  outArgs[OUT_STRUCT] = ret;
  delete reporter;
}
