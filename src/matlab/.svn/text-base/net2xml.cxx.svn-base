/** 
@file  ntrain.cxx
@brief Exports a net structure into a Ringer XML format (for Athena use).
  This file will implemente the code that will take a Matlab's neural network structure and export
  it in the ringer format, so it can be used online (within Athena).
*/

#include <vector>

#include <mex.h>

#include "TrigRingerTools/matlab/MatlabReporter.h"
#include "TrigRingerTools/config/Network.h"
#include "TrigRingerTools/config/NeuronBackProp.h"
#include "TrigRingerTools/config/SynapseRProp.h"
#include "TrigRingerTools/config/SynapseBackProp.h"
#include "TrigRingerTools/config/type.h"
#include "TrigRingerTools/sys/Exception.h"

/// The Network structure.
const unsigned NET_STR_IDX = 0;

// The outut file name.
const unsigned OUT_FILE_IDX = 1;

// (Optional) The STD structure containing the subtracting and dividing factors for input normalization.
const unsigned STD_VECTOR_IDX = 2;

//Function to handle matrix index in matlab vectors.
inline unsigned getPos(const unsigned row, const unsigned col, const unsigned numRows)
{
  return (row + (numRows*col));
}

/// Reads the initial weights and biases from the matlab structure.
/**
 This method reads the weights and biases values from the matlab environment
 passed to this function.
 @param[in] mNet The network structure.
 @param[in] nNodes The vector containing the number of nodes in each layer (including the input layer).
 @param[out] w A pointer to where the weight values will be saved.
 @param[out] b A pointer to where the bias values will be saved.
*/
void readWeights(const mxArray *mNet, std::vector<unsigned> &nNodes, double ***&w, double **&b)
{
	//Allocating the space needed to store the weights and biases values
  const unsigned size = nNodes.size() - 1;
  //Allocating the bias matrix.
  b = new double* [size];
  //Allocating the matrix's collumns.
	for (unsigned i=0; i<size; i++) b[i] = new double [nNodes[i+1]];

	//Allocating the weight matrix.
	w = new double** [size];
	for (unsigned i=0; i<size; i++)
	{
    w[i] = new double* [nNodes[i+1]];
  	for (unsigned j=0; j<nNodes[i+1]; j++) w[i][j] = new double [nNodes[i]];
	}

	//Getting the bias cells vector.
	mxArray *lb = mxGetField(mNet, 0, "b");
	
	//Processing first the input layer.
	double *ib = mxGetPr(mxGetCell(lb, 0));
	mxArray *mat_iw = mxGetCell(mxGetField(mNet, 0, "IW"), 0);
	unsigned numRows_iw = static_cast<unsigned>(mxGetM(mat_iw));
	double *iw = mxGetPr(mat_iw);
	
	for (unsigned i=0; i<nNodes[1]; i++)
	{
		for (unsigned j=0; j<nNodes[0]; j++) w[0][i][j] = iw[getPos(i,j,numRows_iw)];
		b[0][i] = ib[i];
	}
	
	//Processing the other layers.
	//Getting the weights cell matrix.
  mxArray *lw = mxGetField(mNet, 0, "LW");
	
	for (unsigned i=1; i<size; i++)
	{
	  ib = mxGetPr(mxGetCell(lb, i));
		mat_iw = mxGetCell(lw, getPos(i,(i-1), mxGetM(lw)));
		numRows_iw = static_cast<unsigned>(mxGetM(mat_iw));
	  iw = mxGetPr(mat_iw);
	
		for (unsigned j=0; j<nNodes[(i+1)]; j++)
		{
			for (unsigned k=0; k<nNodes[i]; k++) w[i][j][k] = iw[getPos(j,k,numRows_iw)];
			b[i][j] = ib[j];
		}
	}
}


/// Matlab 's main function.
void mexFunction(int nargout, mxArray *ret[], int nargin, const mxArray *args[])
{
  sys::Reporter *reporter = new sys::MatlabReporter();

	//Verifying if the number of input parameters is ok.
	if (nargin < 2) RINGER_FATAL(reporter, "Incorrect number of arguments! See help for information!");
    

	//Reading the configuration structure
	const mxArray *netStr = args[NET_STR_IDX];
	
	std::vector<unsigned> nNodes; 
	//Getting the number of nodes in the input layer.
	nNodes.push_back(static_cast<unsigned>(mxGetScalar(mxGetField(mxGetCell(mxGetField(netStr, 0, "inputs"), 0), 0, "size"))));
		
	//Getting the number of nodes and transfer function in each layer:
	const mxArray *layers = mxGetField(netStr, 0, "layers");
	std::vector<config::Parameter*> nodePar;
	std::vector<std::string> trfFunc;
	for (size_t i=0; i<mxGetM(layers); i++)
	{
		const mxArray *layer = mxGetCell(layers, i);
		nNodes.push_back(static_cast<unsigned>(mxGetScalar(mxGetField(layer, 0, "size"))));
		std::string transFunction = mxArrayToString(mxGetField(layer, 0, "transferFcn"));
		config::Parameter *par;
		if (transFunction == "tansig") par = new config::NeuronBackProp(config::NeuronBackProp::TANH);
		else if (transFunction == "purelin") par = new config::NeuronBackProp(config::NeuronBackProp::LINEAR);
    else RINGER_FATAL(reporter, "Invalid transfer function passed! t must be either 'purelin' or 'tansig'");
	  trfFunc.push_back(transFunction);
	  nodePar.push_back(par);
	}


	// Getting the training agorithm.		
	const std::string trnType = mxArrayToString(mxGetField(netStr, 0, "trainFcn"));
  config::Parameter *synPar;
  config::SynapseStrategyType synStrategy;
  if (trnType == "trainrp")
  {
    synStrategy = config::SYNAPSE_RPROP;
    synPar = new config::SynapseRProp();
  }
  else if (trnType == "traingd")
  {
    synStrategy = config::SYNAPSE_BACKPROP;
    synPar = new config::SynapseBackProp(0.1);
  }
  else RINGER_FATAL(reporter, "Train type must be 'trainrp' or 'traingd'!");

	double ***w;
	double **b;
	readWeights(netStr, nNodes, w, b);

  //Taking the subtract and divide values.
  std::vector<double> subVec, divVec;
  if (nargin == 3)
  {
    const double *subMatVec = mxGetPr(mxGetField(args[STD_VECTOR_IDX], 0, "xmean"));
    const double *divMatVec = mxGetPr(mxGetField(args[STD_VECTOR_IDX], 0, "xstd"));
    for (unsigned i=0; i<nNodes[0]; i++)
    {
      subVec.push_back(subMatVec[i]);
      divVec.push_back(divMatVec[i]);
    }    
  }
  else
  {
    // If the user did not pass a divide and subtract vector, we use default ones.
    for (unsigned i=0; i<nNodes[0]; i++)
    {
      subVec.push_back(0.);
      divVec.push_back(1.);
    }
  }

  unsigned nodeId, synId, startingNodeId;
  nodeId = synId = startingNodeId = 0;
  std::vector<config::Synapse*> synapses;
  std::vector<config::Neuron*> neurons;
  const config::NeuronStrategyType nodeStrategy = config::NEURON_BACKPROP;
  
  //Taking the nodes
	for (unsigned i=0; i<nNodes.size(); i++)
	{	  
	  startingNodeId += nNodes[i];
    for (unsigned j=0; j<nNodes[i]; j++)
    {
      config::Neuron *n;
      if (!i) n = new config::Neuron(nodeId, config::INPUT, &nodeStrategy, nodePar[i], 0., subVec[j], divVec[j]); 
      else if (i == (nNodes.size()-1)) n = new config::Neuron(nodeId, config::OUTPUT, &nodeStrategy, nodePar[i-1]);
      else n = new config::Neuron(nodeId, config::HIDDEN, &nodeStrategy, nodePar[i-1]);
      neurons.push_back(n);

      if (i != (nNodes.size()-1)) // Taking the synapses for all but the output layer.
      {
        for (unsigned k=0; k<nNodes[i+1]; k++)
        {
          config::Synapse *s = new config::Synapse(synId++, nodeId, (startingNodeId+k), w[i][k][j], synStrategy, synPar);
          synapses.push_back(s);       
        }
      }
      
      nodeId++;
    }
	}  

  //Taking the bias.
  neurons.push_back(new config::Neuron(nodeId, config::BIAS, &nodeStrategy, nodePar[0], 1.));
  startingNodeId = nNodes[0];
  for (unsigned i=1; i<nNodes.size(); i++)
  {
    for (unsigned j=0; j<nNodes[i]; j++)
    {
	    config::Synapse *s = new config::Synapse(synId++, nodeId, (startingNodeId+j), b[i-1][j], synStrategy, synPar);
	    synapses.push_back(s);
	  }
	  startingNodeId += nNodes[i];
  }

  try
  {
    //Finaly saving the file.
    config::Header header("Matlab", "net2xml", NR_VERSION, time(0), "");
    config::Network outNet(&header, synapses, neurons, reporter);
    outNet.save(mxArrayToString(args[OUT_FILE_IDX]));
  }
  catch (sys::Exception& ex)
  {
    RINGER_FATAL(reporter, ex.what());
  }
  
  //Releasing all the allocated memory.
  for (unsigned i=0; i<(nNodes.size()-1); i++) delete [] b[i];
  delete [] b;
  for (unsigned i=0; i<(nNodes.size()-1); i++)
  {
    for (unsigned j=0; j<nNodes[i+1]; j++) delete [] w[i][j];
		delete [] w[i];
  }
  delete [] w;
  for (std::vector<config::Synapse*>::iterator itr=synapses.begin(); itr!=synapses.end(); itr++) delete *itr;
  for (std::vector<config::Neuron*>::iterator itr=neurons.begin(); itr!=neurons.end(); itr++) delete *itr;
  for (std::vector<config::Parameter*>::iterator itr=nodePar.begin(); itr!=nodePar.end(); itr++)  delete *itr;
  delete synPar;
  delete reporter;
}
