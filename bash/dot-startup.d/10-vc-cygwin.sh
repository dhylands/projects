#
# 10-vc.sh
#
#  This script sets up variables for using Visual Studio. These
#  partcular variables are setup for VC++ 6
#

export VSDIR="/c/Program Files/Microsoft Visual Studio 8"
export MSSDK="${VSDIR}/SDK"

if [ -d "${VSDIR}" ]
then
    export VSCOMMONDIR="${VSDIR}/Common7"
    export MSDEVDIR="${VSCOMMONDIR}/IDE"
    export MSVCDIR="${VSDIR}/VC"
    
    vc_LIB="${MSVCDIR}/lib:${MSVCDIR}/mfc/lib"
    vc_INCLUDE="${MSVCDIR}/atl/include:${MSVCDIR}/include:${MSVCDIR}/mfc/include"

    vc_LIB="${MSSDK}/lib:${vc_LIB}"
    vc_INCLUDE="${MSSDK}/include:${vc_INCLUDE}"
    
    export LIB=$(cygpath -p -w "${vc_LIB}")
    export INCLUDE=$(cygpath -p -w "${vc_INCLUDE}")
    
    PATH=${PATH}:"${MSDEVDIR}"
    PATH=${PATH}:"${MSVCDIR}/bin"
    PATH=${PATH}:"${VSCOMMONDIR}/Tools"
    PATH=${PATH}:"${MSSDK}/v2.0/bin"
fi
