//Dear emacs, this is -*- c++ -*- 

/**
 * @file sys/src/util.cxx
 *
 * Implements system utilities
 */

#include "TrigRingerTools/sys/util.h"
#include "TrigRingerTools/sys/debug.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

std::string sys::getenv (const std::string& envname)
{
  const char* s = 0;
  if (envname.length()) s = std::getenv(envname.c_str());
  if (!s) return "";
  return s;
}

/**
 * @warning This may be problematic for multi-threading!! Solution: protect
 * debug printouts with a lock.
 */
time_t sys::iso8601totime(const std::string& dt)
{
  struct tm temp;
  memset(&temp, 0, sizeof(struct tm));
  strptime(dt.c_str(), "%FT%T", &temp);
  RINGER_DEBUG3("Converting ISO8601 date \"" << dt << "\" to time gives me \""
	      << sys::stringtime(&temp) << "\".");
  return mktime(&temp);
}

/**
 * @warning This may be problematic for multi-threading!! Solution: protect
 * debug printouts with a lock.
 */
std::string sys::timetoiso8601(const time_t* t)
{
  time_t consider;
  if (t) consider = *t;
  else consider = time(0);
  struct tm temp;
  localtime_r(&consider, &temp);
  char ts[32];
  strftime(ts, 32, "%Y-%m-%dT%H:%M:%S", &temp);
  RINGER_DEBUG3("Converting ISO date \"" << sys::stringtime(&consider)
	      << "\" to ISO8601 format gives me \"" << ts << "\".");
  return ts;
}

std::string sys::stringtime (const time_t* t)
{
  time_t consider;
  if (t) consider = *t;
  else consider = time(0);
  struct tm bdt; //broken-down time
  gmtime_r(&consider, &bdt);
  char ts[32];
  strftime(ts, 32, "%a %b %d %H:%M:%S %Y", &bdt);
  return ts;
}

std::string sys::stringtime (const struct tm* t)
{
  struct tm consider;
  if (t) consider = *t;
  else {
    time_t temp = time(0);
    localtime_r(&temp, &consider);
  }
  char ts[32];
  strftime(ts, 32, "%a %b %d %H:%M:%S %Y", &consider);
  return ts;
}

std::string sys::currenttime (const std::string& format)
{
  struct tm current;
  time_t ctime = time(0);
  localtime_r(&ctime, &current); //get broken down time (thread-safe)
    
  std::string temp;
  if ( format == "long" ) temp = "%a, %d %b %T %Y";
  else if ( format == "short" ) temp = "%d.%m.%y-%T";
  else if ( format == "time" ) temp = "%T";
  else if ( format == "day" ) temp = "%d.%m.%y";
  else temp = format;
  
  char cache[32];
  strftime(cache, 32, temp.c_str(), &current);
  return cache;
}

bool sys::exists (const std::string& filename)
{
  struct stat temp;
  int error = stat(filename.c_str(), &temp);
  if (error != 0) return false;
  return true;
}

bool sys::backup (const std::string& filename)
{
  if (sys::exists(filename)) {
    std::string backupname = filename + "~";
    int error = rename(filename.c_str(), backupname.c_str());
    if (error != 0) return false;
    RINGER_DEBUG2("I made a back-up of \"" << filename << "\" into \""
		<< backupname << "\".");
    return true;
  }
  //Notice I'll return true if the file doesn't exist!
  return true;
}

std::string sys::stripname (const std::string& fullname)
{
  //find location of last '/'
  size_t start = fullname.rfind("/");
  if (start == std::string::npos) start = 0;
  else ++start;
    //stopped here
  size_t end = fullname.rfind(".");
  return fullname.substr(start, end-start);
}

std::string sys::look_in_path(const std::string &file_name, std::string path, const char delim)
{
  std::string::size_type p;
  while((p = path.find(delim)) != std::string::npos)
  {
    const std::string full_file_name = path.substr(0,p) + "/" + file_name;
    if (exists(full_file_name))
    {
      RINGER_DEBUG1("Found file in " << full_file_name);
      return full_file_name;
    }
    path = path.substr(p+1);
  }
  
  const std::string full_file_name = path + "/" + file_name;
  if (exists(full_file_name))
  {
    RINGER_DEBUG1("Found file in " << full_file_name);
    return full_file_name;
  }
  
  RINGER_DEBUG1("File " << file_name << " not found in " << path);
  return "";
}
