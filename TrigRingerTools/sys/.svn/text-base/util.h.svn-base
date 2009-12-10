//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/util.h
 *
 * @brief A set of utilities to find and calculate things around
 */

#ifndef RINGER_SYS_UTIL_H
#define RINGER_SYS_UTIL_H

#include <string>
#include <ctime>

namespace sys {

  /**
   * Returns, from the environment, the variable with the given name or, the
   * empty string
   */
  std::string getenv (const std::string& envname);

  /**
   * Parses a date/time information in ISO8601 (commonly used in XML documents
   * into a time_t variable that can be used in other ANSI/ISO systems as
   * well. The time_t value represents, for most architectures, the time
   * elapsed since 1/1/1970 00:00 in seconds and is usually represented by an
   * unsigned integer. 
   *
   * @param dt The date/time in ISO8601 format
   */
  time_t iso8601totime(const std::string& dt);
  
  /**
   * This does the contrary from iso8601totime() above. It takes a time_t as
   * input and produces a string that contains the date information in format
   * ISO8601.
   *
   * @param t The optional time in ISO time_t format. If it is not given, the
   * current time is considered
   */
  std::string timetoiso8601(const time_t* t=0);

  /**
   * This produces a readable date string based on a time_t variable
   *
   * @param t The time to convert into a string. If it is <code>NULL</code>
   * return the current time.
   */
  std::string stringtime (const time_t* t=0);

  /**
   * This produces a readable date string based on a struct tm variable
   *
   * @param t The time to convert into a string. If it is <code>NULL</code>
   * return the current time.
   */
  std::string stringtime (const struct tm* t=0);
  
  /**
   * Returns the date in the chosen format.
   *
   * @param format The format in which you want the date. This could
   * be:
   * -# long: something like "Tue May 21 13:46:22 1991"
   * -# short: something like "21.05.91-13:46:22"
   * -# time: something like "13:46:22"
   * -# day: something like "21.05.91"
   * -# template: a template for strftime() as described by
   * <code>$info libc</code>.
   */
  std::string currenttime (const std::string& format);

  /**
   * This non-member method checks the existence of a file by name
   *
   * @param filename The name of the file to check
   */
  bool exists (const std::string& filename);

  /**
   * This non-member method checks if a file exists and rename it by adding a
   * ~ to its name.
   *
   * @param filename The name of the file to backup
   */
  bool backup (const std::string& filename);

  /**
   * Returns the basename of a file, without its extension and diretory prefixes
   * 
   * @param fullname The name of the file
   */
  std::string stripname (const std::string& fullname);
  
  /**
  * Looks for a given file in a PATH like env var and returns the full path
  * if the file is found. Otherwise, an empty string.
  * @param[in] file_name The name of the file you want to find.
  * @param[in] path The string containing the list of directories, separated by a delimiter char
  * @param[in] delim The character used to separate the directories in the directories list.
  * @return The full path to the file if it was found. otherwise an empty string.
  */
  std::string look_in_path(const std::string &file_name, std::string path, const char delim = ':');
}

#endif /* RINGER_SYS_UTIL_H */
