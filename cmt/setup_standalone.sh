# setups up the ringer directories
cd ..
export DATAPATH=`pwd`/share/schema
export LD_LIBRARY_PATH=.:`pwd`/lib:$LD_LIBRARY_PATH
export PATH=`pwd`/bin:$PATH
export GSLPATH=/afs/cern.ch/sw/lcg/external/GSL/1.8/slc4_amd64_gcc34
export RINGER_SCHEMA_PATH=`pwd`/share/schema
export ROOTSYS=/afs/cern.ch/sw/lcg/external/root/5.18.00a/slc4_amd64_gcc34/root
export PATH=$PATH:$ROOTSYS/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib
#For mac os.
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH
cd -

