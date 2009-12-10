# echo "Setting TrigRingerTools  in /home/dhiana/testarea/AtlasOffline-15.5.0/Trigger"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/cern.ch/sw/contrib/CMT/v1r20p20090520; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=TrigRingerTools -version= -path=/home/dhiana/testarea/AtlasOffline-15.5.0/Trigger  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

