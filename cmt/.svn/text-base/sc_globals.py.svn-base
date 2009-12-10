import re
import os
import commands
import sys
import platform

NR_VERSION = "0.8.0"
DEBUG_LVL = 0
VERBOSE_LVL = 0

def getSourceFiles(sourcesDir):
	srcFilter = re.compile('\.cxx\Z|\.cpp\Z|\.c\Z')
	return ['%s%s%s' % (sourcesDir, os.path.sep, f) for f in os.listdir(sourcesDir) if srcFilter.search(f) is not None]


def getLibXMLPaths():
	#Asking where the libxml include files are.
	status, libXMLIncPath = commands.getstatusoutput('xml2-config --cflags')
	if status is not 0:
		print "You must have libxml2 installed! Exiting..."
		sys.exit(1)

	status, libXMLLibPath = commands.getstatusoutput('xml2-config --libs')
	return (libXMLIncPath, libXMLLibPath)


#Change here the DEBUG, VERBOSE levels and version number.
genCPPFlags = ['-DNR_VERSION=\\"%s\\"' % NR_VERSION, \
              '-DRINGER_DEBUG=%d' % DEBUG_LVL, '-DRINGER_VERBOSE=%d' % VERBOSE_LVL, \
              '-Wno-long-long']

libXMLIncPath = "/usr/include/libxml2"
poptPath = "/sw/include/"
rootIncPath = '%s%sinclude' % (os.getenv('ROOTSYS'), os.path.sep)
rootLibPath = '%s%slib' % (os.getenv('ROOTSYS'), os.path.sep)

#For now this will work only in MAc OS. Change that later!
pythonIncPath = ['/usr/include/python2.5']
pythonLibPath = []

#boostLibName = 'boost_python-gcc42-1_34_1';
#boostLibName = 'boost_python-gcc41-1_34_1';
boostLibName = 'boost_python';

#Am I using a MAC computer? Then I apply some optimizations for it
if 'Darwin' in platform.system():
  genCPPFlags += ['-fast']
  pythonIncPath = ['/usr/local/include/boost-1_36', '/Developer/SDKs/MacOSX10.5.sdk/System/Library/Frameworks/Python.framework/Versions/2.5/include/python2.5']
  pythonLibPath = ['/usr/local/lib', '/usr/lib/python2.5/config']
  boostLibName = 'boost_python-xgcc40-mt-1_36';
  
incPath = ['../', poptPath, libXMLIncPath, rootIncPath, '%s%sinclude' % (os.getenv('GSLPATH'), os.path.sep)]
libPath = ['./', '/usr/lib', '/sw/lib', rootLibPath, '%s%slib' % (os.getenv('GSLPATH'), os.path.sep)]
rootLibs = ['RIO', 'Core', 'vectorDict', 'Cint', 'Net', 'Tree', 'dl', 'Matrix', 'Thread', 'MathCore'];
