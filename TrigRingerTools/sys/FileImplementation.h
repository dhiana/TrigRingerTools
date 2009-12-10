//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/FileImplementation.h
 * @author <a href="mailto:Andre.dos.Anjos@cern.ch">Andre Rabello dos Anjos</a> 
 * $Author: dferreir $
 * $Revision: 1.5 $
 * $Date: 2008-05-22 16:53:55 $
 *
 * @brief Describes the abstract interface to read data from a stream.
 */

#ifndef RINGER_SYS_FILEIMPLEMENTATION_H
#define RINGER_SYS_FILEIMPLEMENTATION_H

#include <string>

namespace roiformat {
  class RoI;
  class Cell;
}

namespace sys {

  /**
   * A generic interface to read data from files.
   */
  class FileImplementation {

  public: //interface

    /**
     * A virtual destructor, to rhyme with the rest...
     */ 
    virtual ~FileImplementation () {};

    /**
     * A virtual serializer to write roiformat::RoI
     */
    virtual FileImplementation& operator<< (const roiformat::RoI &roi) = 0;
    
    /**
     * A virtual serializer to read roiformat::RoI
     */
    virtual FileImplementation& operator>> (roiformat::RoI &roi) = 0;

    /**
     * A virtual serializer to write roiformat::Cell
     */
    virtual FileImplementation& operator<< (const roiformat::Cell &cell) = 0;
    
    /**
     * A virtual serializer to read roiformat::Cell
     */
    virtual FileImplementation& operator>> (roiformat::Cell &cell) = 0;
   
    /**
     * A virtual function to finish writing the event
     */
    virtual FileImplementation& writeEvent() { return *this; }
   
    /**
     * Tests if the file is at its end
     */
    virtual bool eof (void) const =0;

    /**
     * Tests if the file is still good, meaning that the next read might
     * succeed.
     */
    virtual bool good (void) const =0;

    /**
     * Returns <b>false</b> if the RoI finishes at the current position. This
     * should be used to define when the RoI finishes, in a while loop.
     */
    virtual bool readmore (void) const =0;

    /**
     * Tests whether the stream is opened for read or write operations.
     * @return <b>True</b> if the stream is correctly opened. <b>False</b> otherwise.
     */
    virtual bool is_open(void) = 0;

    /**
     * Closes the stream.
     */
    virtual void close(void) = 0;

  };

}

#endif /* RINGER_SYS_FILEIMPLEMENTATION_H */
