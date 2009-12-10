#!/usr/bin/env python
# $Id: uniqroi.py,v 1.1 2007-04-06 22:12:10 rtorres Exp $
# Created by Andre Anjos <Andre.dos.Anjos@cern.ch>, 02-Nov-2005

# Makes sure there is only one entry of each cell in an RoI

import sys
import re
import string

def main ():
    """The main function..."""

    if len(sys.argv) == 1:
        print "usage: %s <file.rfd>" % sys.argv[0]
        sys.exit(1)

    stderr = file("/dev/stderr", "wt")
    my_file = file(sys.argv[1], 'rt')
    roi_counter = 0
    curr = []
    duplicate_counter = 0
    line_counter = 0
    for l in my_file:
        if re.match(r'RoI.*', l):
            "RoI starts"
            r = l.split()
            print >>stderr, "Processing RoI %s of Event %s" % (r[1], r[8])
            if len(curr) != 0:
                for v in curr:
                    print v
                curr = []
            print l,
            roi_counter += 1

        if re.match(r'[0-9]+,.*', l):
            "Simple line"
            line_counter += 1
            entry = string.join(l.split())
            if entry not in curr:
                curr.append(entry)
            else:
                duplicate_counter += 1

    if len(curr) != 0:
        for v in curr:
            print v

    my_file.close()
    print >>stderr, "Total of %d RoI's" % roi_counter
    print >>stderr, "Total of %d cells" % line_counter
    print >>stderr, "Total of %d duplicates, %.2f%%" % \
          (duplicate_counter, 100*float(duplicate_counter)/line_counter)
    stderr.close()

main()
