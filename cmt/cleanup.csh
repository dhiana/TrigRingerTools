if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/cern.ch/sw/contrib/CMT/v1r20p20090520
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=TrigRingerTools -version= -path=/home/dhiana/testarea/AtlasOffline-15.5.0/Trigger $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

