//Dear emacs, this is -*- c++ -*-

/**
 * @file sys/sys/XMLProcessor.h
 *
 * @brief Chooser between the different XML Parsing backends.
 */

#ifndef RINGER_SYS_XMLPROCESSOR_H
#define RINGER_SYS_XMLPROCESSOR_H

#ifdef XERCES_XML_BACK_END
#include "TrigRingerTools/sys/xerces_XMLProcessor.h"
#else
#include "TrigRingerTools/sys/libxml2_XMLProcessor.h"
#endif

#endif /* RINGER_SYS_XMLPROCESSOR_H */
