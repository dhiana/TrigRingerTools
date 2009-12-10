#include <string>
#include <map>

#include <boost/python.hpp>

#include "TrigRingerTools/rbuild/Config.h"
#include "TrigRingerTools/sys/LocalReporter.h"

using namespace boost::python;

class PyConfig
{
private:
  sys::LocalReporter *m_reporter;
  rbuild::Config *m_conf;
public:
  PyConfig(const std::string& filename)
  {
    m_reporter = new sys::LocalReporter();
    m_conf = new rbuild::Config(filename, m_reporter);
  }
  
  dict config()
  {
    dict ret;
    const std::map<unsigned int, rbuild::RingConfig>& mConf = m_conf->config();
    for (std::map<unsigned int, rbuild::RingConfig>::const_iterator itr = mConf.begin(); itr != mConf.end(); itr++)
    {
      ret[itr->first] = itr->second;
    }
    return ret;
  }
  
  ~PyConfig()
  {
    delete m_reporter;
    delete m_conf;
  }
};

void wrap_Config()
{
  class_<PyConfig>("Config", init<const std::string&>())
    .def("config", &PyConfig::config)
  ;
}
