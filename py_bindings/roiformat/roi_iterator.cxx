#include <boost/python.hpp>
#include <boost/python/list.hpp>

#include <vector>
#include <string>

#include "TrigRingerTools/bindings/RoIWrap.h"
#include "TrigRingerTools/roiformat/roi_iterator.h"
#include "TrigRingerTools/bindings/RoIWrap.h"
#include "TrigRingerTools/roiformat/Cell.h"


using namespace boost::python;


class RoiIteratorWrap : public roiformat::RoIIterator
{
public:

  RoiIteratorWrap(const std::string &outputNtupleFileName) : roiformat::RoIIterator(outputNtupleFileName){};
  RoiIteratorWrap() : roiformat::RoIIterator(""){};

  boost::python::list rings()
  {
    boost::python::list ret;
    for (unsigned i = ringStart; i < ringEnd; i++) ret.append(rootRings->at(i));
    return ret;
  }

  boost::python::list detectors()
  {
    boost::python::list ret;
    for (unsigned i = roiStart; i < roiEnd; i++) ret.append(rootDetCells->at(i));
    return ret;
  }

  boost::python::list eta()
  {
    boost::python::list ret;
    for (unsigned i = roiStart; i < roiEnd; i++) ret.append(rootEta->at(i));
    return ret;
  }

  boost::python::list phi()
  {
    boost::python::list ret;
    for (unsigned i = roiStart; i < roiEnd; i++) ret.append(rootPhi->at(i));
    return ret;
  }

  boost::python::list energy()
  {
    boost::python::list ret;
    for (unsigned i = roiStart; i < roiEnd; i++) ret.append(rootEnergy->at(i));
    return ret;
  }
  
  RoIWrap *roi()
  {
    RoIWrap *ret = new RoIWrap(lvl1_id(), roi_id(), lvl1_eta(), lvl1_phi());

    std::vector<unsigned char> detCells;
    std::vector<float> etaCells;
    std::vector<float> phiCells;
    std::vector<float> eCells;
    RoIIterator::detectors(detCells);
    RoIIterator::eta(etaCells);
    RoIIterator::phi(phiCells);
    RoIIterator::energy(eCells);
    
    for (unsigned i=0; i<detCells.size(); i++)
    {
      roiformat::Cell c(static_cast<roiformat::Cell::Sampling>(detCells[i]), etaCells[i], phiCells[i], 0, 0, 0, 0, eCells[i]);
      ret->insertCell(c);
    }
    return ret;
  }
};


void wrap_roi_iterator()
{
  class_<RoiIteratorWrap>("RoIIterator")
    .def(init<std::string>())
    .def("setBranchStatus", &RoiIteratorWrap::setBranchStatus)
    .def("saveRoI", &RoiIteratorWrap::saveRoI)
    .def("add", &RoiIteratorWrap::add)
    .def("getEntries", &RoiIteratorWrap::getEntries)
    .def("getNumRoIs", &RoiIteratorWrap::getNumRoIs)
    .def("next", &RoiIteratorWrap::next)
    .def("nClusters", &RoiIteratorWrap::nClusters)
    .def("roi_id", &RoiIteratorWrap::roi_id)
    .def("lvl1_id", &RoiIteratorWrap::lvl1_id)
    .def("lvl1_eta", &RoiIteratorWrap::lvl1_eta)
    .def("lvl1_phi", &RoiIteratorWrap::lvl1_phi)
    .def("lvl2_eta", &RoiIteratorWrap::lvl2_eta)
    .def("lvl2_phi", &RoiIteratorWrap::lvl2_phi)
    .def("lvl2_et", &RoiIteratorWrap::lvl2_et)
    .def("nCells", &RoiIteratorWrap::nCells)
    .def("rings", &RoiIteratorWrap::rings)
    .def("detectors", &RoiIteratorWrap::detectors)
    .def("eta", &RoiIteratorWrap::eta)
    .def("phi", &RoiIteratorWrap::phi)
    .def("energy", &RoiIteratorWrap::energy)
    .def("roi", &RoiIteratorWrap::roi, return_value_policy<manage_new_object>())
  ;
}
