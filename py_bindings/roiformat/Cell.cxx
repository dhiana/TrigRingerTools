#include <boost/python.hpp>
#include "TrigRingerTools/roiformat/Cell.h"

using namespace boost::python;


PyObject *max (PyObject *vcell)
{
  std::vector<const roiformat::Cell*> aux;
  for (size_t i=0; i<PyList_Size(vcell); i++)
  {
    PyObject *item = PyList_GetItem(vcell,i);
    arg_from_python<roiformat::Cell*> obj(item);
    const roiformat::Cell* c = obj();
    aux.push_back(c);  
  }

  double eta, phi;
  roiformat::max(aux, eta, phi);
  
  PyObject *ret = PyTuple_New(2);
  PyTuple_SetItem(ret, 0, PyFloat_FromDouble(eta));
  PyTuple_SetItem(ret, 1, PyFloat_FromDouble(phi));
  
  return ret;
}

PyObject *max_region (PyObject *vcell, const double& eta_ref, const double& phi_ref, 
                      const double& eta_window, const double& phi_window)
{
  std::vector<const roiformat::Cell*> aux;
  for (size_t i=0; i<PyList_Size(vcell); i++)
  {
    PyObject *item = PyList_GetItem(vcell,i);
    arg_from_python<roiformat::Cell*> obj(item);
    const roiformat::Cell* c = obj();
    aux.push_back(c);  
  }

  double eta, phi;
  roiformat::max(aux, eta, phi, eta_ref, phi_ref, eta_window, phi_window);
  PyObject *ret = PyTuple_New(2);
  PyTuple_SetItem(ret, 0, PyFloat_FromDouble(eta));
  PyTuple_SetItem(ret, 1, PyFloat_FromDouble(phi));
  return ret;
}

roiformat::Cell::Sampling num2Samp(const unsigned samp)
{
  return static_cast<roiformat::Cell::Sampling>(samp);
}

roiformat::Cell::Sampling str2Samp(const std::string &samp)
{
  if (samp.size() > 1) return roiformat::str2sampling(samp);
  // The else is because python converts unsigned char to str...
  else return static_cast<roiformat::Cell::Sampling>(samp[0]);
}




void wrap_Cell()
{
  def("check_wrap_around", roiformat::check_wrap_around);
  def("fix_wrap_around", roiformat::fix_wrap_around);
  def("max", max);
  def("max", max_region);
  def("getSamp", str2Samp);
  def("getSamp", num2Samp);
    
  enum_<roiformat::Cell::Sampling>("Sampling")
    .value("PSBARREL", roiformat::Cell::PSBARREL)
    .value("EMBARREL1", roiformat::Cell::EMBARREL1)
    .value("EMBARREL2", roiformat::Cell::EMBARREL2)
    .value("EMBARREL3", roiformat::Cell::EMBARREL3)
    .value("PSENDCAP", roiformat::Cell::PSENDCAP)
    .value("EMENDCAP1", roiformat::Cell::EMENDCAP1)
    .value("EMENDCAP2", roiformat::Cell::EMENDCAP2)
    .value("EMENDCAP3", roiformat::Cell::EMENDCAP3)
    .value("HADENCAP0", roiformat::Cell::HADENCAP0)
    .value("HADENCAP1", roiformat::Cell::HADENCAP1)
    .value("HADENCAP2", roiformat::Cell::HADENCAP2)
    .value("HADENCAP3", roiformat::Cell::HADENCAP3)
    .value("TILEBARREL0", roiformat::Cell::TILEBARREL0)
    .value("TILEBARREL1", roiformat::Cell::TILEBARREL1)
    .value("TILEBARREL2", roiformat::Cell::TILEBARREL2)
    .value("TILEGAPSCI0", roiformat::Cell::TILEGAPSCI0)
    .value("TILEGAPSCI1", roiformat::Cell::TILEGAPSCI1)
    .value("TILEGAPSCI2", roiformat::Cell::TILEGAPSCI2)
    .value("TILEEXTB0", roiformat::Cell::TILEEXTB0)
    .value("TILEEXTB1", roiformat::Cell::TILEEXTB1)
    .value("TILEEXTB2", roiformat::Cell::TILEEXTB2)
    .value("FORWCAL0", roiformat::Cell::FORWCAL0)
    .value("FORWCAL1", roiformat::Cell::FORWCAL1)
    .value("FORWCAL2", roiformat::Cell::FORWCAL2)
    .value("UNKNOWN", roiformat::Cell::UNKNOWN)
  ;

  class_<roiformat::Cell>("Cell")
    .def(init<roiformat::Cell::Sampling, const double&, const double&, const double&, const double&, const double&, const double&, const double&>())
    .def("sampling", &roiformat::Cell::sampling)
    .def("eta", &roiformat::Cell::eta, return_value_policy<copy_const_reference>())
    .def("phi", &roiformat::Cell::phi, return_value_policy<copy_const_reference>())
    .def("r", &roiformat::Cell::r, return_value_policy<copy_const_reference>())
    .def("deta", &roiformat::Cell::deta, return_value_policy<copy_const_reference>())
    .def("dphi", &roiformat::Cell::dphi, return_value_policy<copy_const_reference>())
    .def("dr", &roiformat::Cell::dr, return_value_policy<copy_const_reference>())
    .def("energy", &roiformat::Cell::energy, return_value_policy<copy_const_reference>())
    .def(self == self)
  ;
}
