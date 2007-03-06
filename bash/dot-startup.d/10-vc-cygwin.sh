#
# 10-vc.sh
#
#  This script sets up variables for using Visual Studio. These
#  partcular variables are setup for VC++ 6
#

export MSSDK="/c/Program Files/Microsoft Platform SDK"
export VSDIR="/c/Program Files/Microsoft Visual Studio"
export VSCOMMONDIR="${VSDIR}/Common"
export MSDEVDIR="${VSDIR}/Common/msdev98"
export MSVCDIR="${VSDIR}/VC98"

export LIB=$(cygpath -p -w "${MSSDK}/lib:${MSVCDIR}/lib:${MSVCDIR}/mfc/lib")
export INCLUDE=$(cygpath -p -w "${MSSDK}/include:${MSVCDIR}/atl/include:${MSVCDIR}/include:${MSVCDIR}/mfc/include")

PATH=${PATH}:"${VSCOMMONDIR}/Tools/WinNT"
PATH=${PATH}:"${VSCOMMONDIR}/Tools"
PATH=${PATH}:"${MSDEVDIR}/bin"
PATH=${PATH}:"${MSVCDIR}/bin"

