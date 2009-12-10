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
#include "TrigRingerTools/matlab/MatlabReporter.h"
#include "TrigRingerTools/rbuild/util.h"
#include "TrigRingerTools/rbuild/RingConfig.h"
#include "TrigRingerTools/rbuild/RingSet.h"
#include "TrigRingerTools/data/Pattern.h"
#include "TrigRingerTools/data/Feature.h"
#include "TrigRingerTools/roiformat/Cell.h"
#include <string>
#include <vector>

#define RINGS 0
#define RINGS_DIST 1
#define	SEC_DIST 2
#define	NORM_TYPE 3
#define	OUT_RINGS 0

/// Function to normalize the rings stored in double format.
void normalizeFromDouble(mxArray *matRetRings, const size_t nEvents, const size_t nRings, 
                        const double *ringsDist, const size_t nRingsDist, 
                        const std::vector<rbuild::RingConfig::Section> &secDist,
                        const rbuild::RingConfig::Normalisation &normType, 
                        sys::Reporter *reporter)
{
  //Taking a double pointer for the rings and the rings distribution.
  double *rings = mxGetPr(matRetRings);
  const std::vector<roiformat::Cell::Sampling> cellSamp;

  //Iterating each ring.
  for (size_t i=0; i<nEvents; i++)
  {
    //Creating the RingSet obj.
    std::vector<rbuild::RingSet> ringSet;
    double *theRing = rings + (i*nRings);    
    size_t init = 0;
    for (size_t j=0; j<nRingsDist; j++)
    {
      //Creating the Patern obj.
      double *layer = theRing + init;
      const unsigned layerSize = static_cast<size_t>(ringsDist[j]);
      std::vector<data::Feature> v;
      for (int k=0; k<layerSize; k++) v.push_back(static_cast<data::Feature>(layer[k]));
      data::Pattern pat(v);
      
      //Creating the Config obj.
      const rbuild::RingConfig rConfig(0.1,0.1,layerSize,"Matlab",normType,secDist[j],cellSamp);
      
      //Creating the RingSet Object for this layer.
      rbuild::RingSet rSet(rConfig, pat);
      ringSet.push_back(rSet);
      
      //Going to the next layer.
      init += ringsDist[j];
    }
    
    //Normalizing the rings.
    rbuild::normalize_rings(reporter, ringSet);

    //Copying the normalized rings to the return vector.
    size_t k = 0;
    for (std::vector<rbuild::RingSet>::iterator itr = ringSet.begin(); itr != ringSet.end(); itr++)
    {
      for (size_t j=0; j<itr->pattern().size(); j++) theRing[k++] = static_cast<double>(itr->pattern()[j]);
    }
  }
}

/// Function to normalize the rings stored in float format.
void normalizeFromFloat(mxArray *matRetRings, const size_t nEvents, const size_t nRings, 
                        const double *ringsDist, const size_t nRingsDist, 
                        const std::vector<rbuild::RingConfig::Section> &secDist,
                        const rbuild::RingConfig::Normalisation &normType, 
                        sys::Reporter *reporter)
{
  //Taking a double pointer for the rings and the rings distribution.
  float *rings = static_cast<float*>(mxGetData(matRetRings));
  const std::vector<roiformat::Cell::Sampling> cellSamp;

  //Iterating each ring.
  for (size_t i=0; i<nEvents; i++)
  {
    //Creating the RingSet obj.
    std::vector<rbuild::RingSet> ringSet;
    float *theRing = rings + (i*nRings);    
    size_t init = 0;
    for (size_t j=0; j<nRingsDist; j++)
    {
      //Creating the Patern obj.
      float *layer = theRing + init;
      const unsigned layerSize = static_cast<size_t>(ringsDist[j]);
      std::vector<data::Feature> v;
      for (int k=0; k<layerSize; k++) v.push_back(static_cast<data::Feature>(layer[k]));
      data::Pattern pat(v);
      
      //Creating the Config obj.
      const rbuild::RingConfig rConfig(0.1,0.1,layerSize,"Matlab",normType,secDist[j],cellSamp);
      
      //Creating the RingSet Object for this layer.
      rbuild::RingSet rSet(rConfig, pat);
      ringSet.push_back(rSet);
      
      //Going to the next layer.
      init += ringsDist[j];
    }
    
    //Normalizing the rings.
    rbuild::normalize_rings(reporter, ringSet);

    //Copying the normalized rings to the return vector.
    size_t k = 0;
    for (std::vector<rbuild::RingSet>::iterator itr = ringSet.begin(); itr != ringSet.end(); itr++)
    {
      for (size_t j=0; j<itr->pattern().size(); j++) theRing[k++] = static_cast<float>(itr->pattern()[j]);
    }
  }
}



void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{	
  sys::Reporter *reporter = new sys::MatlabReporter();

	/* Check for proper number of arguments. */
  if (nrhs < 4) {RINGER_FATAL(reporter, "Too few input parameters passed! See help.");}
  else if (nlhs > 1) {RINGER_FATAL(reporter, "Too many output arguments");}

  //Taking the normalization method.
  const std::string desiredNormType = mxArrayToString(prhs[NORM_TYPE]);
  rbuild::RingConfig::Normalisation normType;
  if (desiredNormType == "event") normType = rbuild::RingConfig::EVENT;
  else if (desiredNormType == "section") normType = rbuild::RingConfig::SECTION;
  else if (desiredNormType == "set") normType = rbuild::RingConfig::SET;
  else if (desiredNormType == "sequential") normType = rbuild::RingConfig::SEQUENTIAL;
  else RINGER_FATAL(reporter, "Invalid normalization type passed! See help!");
  RINGER_REPORT(reporter, "Applying " << desiredNormType << " normalization to the rings...");

  //Taking the rings matrix dimensions.
  const size_t nRings = static_cast<size_t>(mxGetM(prhs[RINGS]));
  const size_t nEvents = static_cast<size_t>(mxGetN(prhs[RINGS]));
  const size_t nRingsDist = static_cast<size_t>(mxGetN(prhs[RINGS_DIST]));
  const size_t nSecDist = static_cast<size_t>(mxGetN(prhs[SEC_DIST]));

  //Checking whether the ringsDist and secDist vectors are of the same size.
  if (nRingsDist != nSecDist)
    RINGER_FATAL(reporter, "The number of sections in the ringsDist and secDist vectors are not the same.");

  //Creating the return vector.
  mxArray *matRetRings = mxDuplicateArray(prhs[RINGS]);

  //Taking a double pointer for the the rings distribution.
  const double *ringsDist = mxGetPr(prhs[RINGS_DIST]);
  
  // Checking whether the summation of the rings in the ringsDist vector 
  // is equal to the actual number of rings.
  unsigned totalRings = 0;
  for (size_t i=0; i<nRingsDist; i++) totalRings += ringsDist[i];
  if (totalRings != nRings)
    RINGER_FATAL(reporter, "The total number of rings in the 'ringsDist' vector does not match the number of rings in the 'ring' vector.");

  std::vector<rbuild::RingConfig::Section> secDist;
  for (size_t i=0; i<nSecDist; i++)
  {
    const std::string sec = mxArrayToString(mxGetCell(prhs[SEC_DIST], i));
    if (sec == "em") secDist.push_back(rbuild::RingConfig::EM);
    else if (sec == "had") secDist.push_back(rbuild::RingConfig::HADRONIC);
    else RINGER_FATAL(reporter, "Section type " << sec << " does not exist!");
  }
  
  // Doing the normalization considering whether the input data is Float of Double type.
  const mxClassID dataType = mxGetClassID(matRetRings);
  if (dataType ==  mxSINGLE_CLASS)
    normalizeFromFloat(matRetRings, nEvents, nRings, ringsDist, nRingsDist, secDist, normType, reporter);
  else if (dataType ==  mxDOUBLE_CLASS)
    normalizeFromDouble(matRetRings, nEvents, nRings, ringsDist, nRingsDist, secDist, normType, reporter);
  else RINGER_FATAL(reporter, "Data must be either float (single) or double!");

  plhs[0] = matRetRings;
  delete reporter;
}
