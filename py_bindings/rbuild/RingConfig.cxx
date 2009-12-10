#include <vector>

#include <boost/python.hpp>

#include "TrigRingerTools/rbuild/RingConfig.h"

using namespace boost::python;

struct vec_cells_to_pyobj
{
  static PyObject* convert(const std::vector<roiformat::Cell::Sampling> &x)
  {
    PyObject *ret = PyList_New(0);
    for (std::vector<roiformat::Cell::Sampling>::const_iterator itr = x.begin(); itr != x.end(); itr++)
    {
      converter::arg_to_python<roiformat::Cell::Sampling> pyObj(*itr);
      PyList_Append(ret, pyObj.get());
    }
    return ret;
  }
};

void wrap_RingConfig()
{
  class_<rbuild::RingConfig>("RingConfig")
    .def("eta_size", &rbuild::RingConfig::eta_size, return_value_policy<copy_const_reference>())
    .def("phi_size", &rbuild::RingConfig::phi_size, return_value_policy<copy_const_reference>())
    .def("max", &rbuild::RingConfig::max)
    .def("name", &rbuild::RingConfig::name, return_value_policy<copy_const_reference>())
    .def("section", &rbuild::RingConfig::section)
    .def("normalisation", &rbuild::RingConfig::normalisation)
    .def("detectors", &rbuild::RingConfig::detectors, return_value_policy<copy_const_reference>())
  ;

  to_python_converter< std::vector<roiformat::Cell::Sampling>, vec_cells_to_pyobj >();

  enum_<rbuild::RingConfig::Section>("Section")
    .value("EM", rbuild::RingConfig::EM)
    .value("HADRONIC", rbuild::RingConfig::HADRONIC)
  ;

  enum_<rbuild::RingConfig::Normalisation>("Normalisation")
    .value("EVENT", rbuild::RingConfig::EVENT)
    .value("SECTION", rbuild::RingConfig::SECTION)
    .value("SET", rbuild::RingConfig::SET)
    .value("SEQUENTIAL", rbuild::RingConfig::SEQUENTIAL)
    .value("NONE", rbuild::RingConfig::NONE)
  ;
}
