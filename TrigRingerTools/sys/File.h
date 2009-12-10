//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/File.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre dos Anjos</a>
 * $Author: ssnyder $
 * $Revision: 1.7 $
 * $Date: 2009-01-12 03:26:20 $
 *
 * @brief This class describes a common interface to file readout.
 */

#ifndef RINGER_SYS_FILE_H
#define RINGER_SYS_FILE_H

#include <string>
#include <locale>
#include <bits/ios_base.h>
#include "TrigRingerTools/sys/FileImplementation.h"

namespace roiformat {
  class RoI;
  class Cell;
}

namespace sys {

  /**
   * Objects of this class can access data stored on different kinds of files
   * and serve them to higher level objects. Objects can also be dumped using
   * analog operations.
   */
  class File {

  public: //interface

    /**
     * Empty class constructor.
     */
    File() {m_fimpl = NULL;};
    
    /**
     * Creates an object of this type, taking as parameter the originating
     * filename.
     *
     * @param filename The name of the file to bind to.
     * @param m How to open the file, i.e., permissions
     * @param sep The standard separator to use
     * @param extra Extra information, implementation specific.
     */
    File (const std::string& filename, 
	  std::ios_base::openmode m = std::ios_base::in,
	  const char sep=',', const std::string &extra = "CollectionTree");

    /**
     * Creates an object of this type, taking as parameter the originating
     * filename.
     *
     * @param filename The name of the file to bind to.
     * @param m How to open the file, i.e., permissions
     * @param sep The standard separator to use
     * @param extra Extra information, implementation specific.
     */
    void open (const std::string& filename,
	  std::ios_base::openmode m = std::ios_base::in, const char sep=',',
	  const std::string &extra = "");

    /**
     * Standard destructor
     */
    virtual ~File ();

    /**
     * Writes an RoI to the file
     */
    File &operator<< (const roiformat::RoI &roi) { (*m_fimpl) << roi; return *this; }
    
    /**
     * Reads an RoI from the file
     */
    File &operator>> (roiformat::RoI &roi) { (*m_fimpl) >> roi; return *this; }
    
    /**
     * Writes a Cell to the file
     */
    File &operator<< (const roiformat::Cell &cell) { (*m_fimpl) << cell; return *this; }
    
    /**
     * Reads a Cell from the file
     */
    File &operator>> (roiformat::Cell &cell) { (*m_fimpl) >> cell; return *this; }

    File &writeEvent() { (*m_fimpl).writeEvent(); return *this; }
    /**
     * Says the filename this object is bound to, without the extension.
     */
    std::string filename_no_ext (void) const;

    /**
     * Tests if the file is at its end
     */
    bool eof (void) const;
    
    /**
     * Tests if the file is still good, meaning that the next read might
     * succeed.
     */
    bool good (void) const;

    /**
     * Returns <b>false</b> if the RoI finishes at the current position. This
     * should be used to define when the RoI finishes, in a while loop.
     */
    bool readmore (void) const;

    /**
     * Tests whether the stream is opened for read or write operations.
     * @return <b>True</b> if the stream is correctly opened. <b>False</b> otherwise.
     */
    bool is_open(void) const;

    /**
     * Closes the stream.
     */
    virtual void close(void) {if (m_fimpl) m_fimpl->close();} ;

  private: //representation

    sys::FileImplementation* m_fimpl; ///< The file impl. to use
    std::string m_filename; ///< The name of the file I'm bound to
    std::string m_extra; ///< Any extra information as how to write data (implementation specific)

  };

}

#endif /* RINGER_SYS_FILE_H */
