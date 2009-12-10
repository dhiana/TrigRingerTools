#include <boost/python.hpp>

#include <vector>

#include "TrigRingerTools/roiformat/RoI.h"
#include "TrigRingerTools/bindings/RoIWrap.h"

using namespace boost::python;

void wrap_RoI()
{
  class_<RoIWrap>("RoI", init<unsigned, unsigned, double, double>())
    .def("roi_id", &RoIWrap::roi_id)
    .def("lvl1_id", &RoIWrap::lvl1_id)
    .def("eta", &RoIWrap::eta)
    .def("phi", &RoIWrap::phi)
    .def("insertCell", &RoIWrap::insertCell)
    .def("cells", &RoIWrap::all_cells)
    .def("cells", &RoIWrap::filter_cells)
  ;
}
