//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/CBNT.h
 * @author <a href="mailto:Danilo.Enoque.Ferreira.de.Lima@cern.ch">Danilo Enoque Ferreira de Lima</a> 
 * $Author $
 * $Revision: 1.1 $
 * $Date: 2008-05-22 16:53:55 $
 *
 * @brief Implements a plain text file read/write type.
 */

#ifndef RINGER_SYS_CBNT_H
#define RINGER_SYS_CBNT_H

#include "TrigRingerTools/sys/FileImplementation.h"
#include <locale>
#include <bits/ios_base.h>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include <vector>

namespace sys {

  /**
   * An interface to read data from CBNT Root files.
   */
  class CBNT : public FileImplementation {

  public: //interface

    /**
     * Opens a new Root file.
     *
     * @param filename The name of the file to open
     * @param m The openning mode
     */
    CBNT (const std::string& filename, std::ios_base::openmode m = std::ios_base::in, const std::string &treename = "CollectionTree");

    /**
     * A virtual destructor, to rhyme with the rest...
     */ 
    virtual ~CBNT ();

    /**
     * Streams in an RoI
     *
     * @param roi The RoI to write.
     */
    virtual FileImplementation& operator<< (const roiformat::RoI& roi);
    
    /**
     * Streams out an RoI
     *
     * @param roi The RoI to read.
     */
    virtual FileImplementation& operator>> (roiformat::RoI& roi);
    
    /**
     * Streams in a Cell
     *
     * @param cell The Cell to write.
     */
    virtual FileImplementation& operator<< (const roiformat::Cell& cell);
    
    /**
     * Streams out a Cell
     *
     * @param cell The Cell to read.
     */
    virtual FileImplementation& operator>> (roiformat::Cell& cell);
   
    /**
     * Writes an event (Fills the TTree)
     */
    virtual FileImplementation& writeEvent();
   
    /**
     * Tests if the file is at its end
     */
    virtual bool eof (void) const;

    /**
     * Tests if the file is still good, meaning that the next read might
     * succeed.
     */
    virtual bool good (void) const;

    /**
     * Returns <b>false</b> if the RoI finishes at the current position. This
     * should be used to define when the RoI finishes, in a while loop.
     */
    virtual bool readmore (void) const;

    /**
     * Tests whether the stream is opened for read or write operations.
     * @return <b>True</b> if the stream is correctly opened. <b>False</b> otherwise.
     */
    virtual bool is_open(void);

    /**
     * Closes the stream.
     */
    virtual void close(void);

  private: //representation

    std::string m_filename; ///< The name of the opened file
    std::string m_treename; ///< The name of the Tree to open inside the file
    std::string m_openMode; ///< The open mode being used as recognized by ROOT
    Long64_t m_entry; ///< The entry number being read
    TFile *m_file; ///< The file itself.
    TTree *m_tree; ///< The TTree inside de file.

    // Branches
    UInt_t Ringer_NClusters;
    std::vector<float> *Ringer_Rings;
    std::vector<unsigned char> *Ringer_DetCells;
    std::vector<float> *Ringer_EtaCells;
    std::vector<float> *Ringer_PhiCells;
    std::vector<float> *Ringer_EtaResCells;
    std::vector<float> *Ringer_PhiResCells;
    std::vector<float> *Ringer_ECells;
    std::vector<unsigned int> *Ringer_NCells;
    std::vector<unsigned int> *Ringer_Roi_Id;
    std::vector<unsigned int> *Ringer_LVL1_Id;
    std::vector<float> *Ringer_LVL1_Eta;
    std::vector<float> *Ringer_LVL1_Phi;
    std::vector<float> *Ringer_LVL2_Eta;
    std::vector<float> *Ringer_LVL2_Phi;
    std::vector<float> *Ringer_LVL2_Et;

    // Cluster number
    UInt_t m_cluster;

    // Cell number
    UInt_t m_cell;
  };

}

#endif /* RINGER_SYS_PLAIN_H */
