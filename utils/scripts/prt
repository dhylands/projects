#!/bin/bash
###########################################################################
#
# Front end for enscript
#
###########################################################################

###########################################################################
#
# Usage - print the usage message for this script
#
###########################################################################

function Usage
{
   cat <<EOF
Usage: ${scriptName} [-12epntv][-o out-dev] file ...
Where:
   -1 Print 1 column
   -2 Print 2 columns (default)
   -e Only echo the enscript command, don't execute it
   -h Print this help page
   -n Print in 'normal', 1-Col mode
   -o Select output device (default=${PRT_OUTDEV})
   -p Select pages to print (s-e, -e, s-, page, odd, even)
      s-e  Prints pages from s to e
      s-   Prints pages from s to the end
      -e   Prints pages from the start to e
      page Prints a particular page
      even Prints even pages
      odd  Prints odd pages 
   -t Print on tabloid paper
   -v Verbose mode
EOF
}

###########################################################################

scriptName=$(basename $0)

if [ -z "${PRT_OUTDEV}" ]
then
   echo "PRT_OUTDEV environment variable should be set to the name of the"
   echo "printer to use."
   exit 1
fi

TAB='4'
FONT='Courier6'
COLUMNS='2'

SIMPLEX=
DUPLEX='--file-align=2 --setpagedevice=Duplex:true'
PLEX=${DUPLEX}

LETTER_SIZE='[612 792]'
TABLOID_SIZE='[792 1224]'

MEDIA='Letter'
PAGESIZE=${LETTER_SIZE}

# Tumble:true does short edge binding, Tumble:false does long edge binding.

LONG_EDGE_BINDING='--setpagedevice=Tumble:false'
SHORT_EDGE_BINDING='--setpagedevice=Tumble:true'

ORIENT="--landscape ${SHORT_EDGE_BINDING}"
PAGE_RANGE=''

verbose=false
execCmd=true

OPTS1='--line-numbers --borders --pretty-print --fancy-header=dhylands'

while getopts ":12eho:p:ntv" opt
do
   case ${opt} in
   
      1)
         COLUMNS='1'
         ;;
         
      2)
         COLUMNS='2'
         ;;
         
      e)
         execCmd=false
         verbose=true
         ;;
         
      h)
         Usage
         exit 0
         ;;
         
      o)
         PRT_OUTDEV=${OPTARG}
         ;;
         
      p)
         PAGE_RANGE="${PAGE_RANGE} --pages=${OPTARG}"
         ;;
         
      n)
         MEDIA='Letter'
         PAGESIZE=${LETTER_SIZE}
         ORIENT="--portrait ${LONG_EDGE_BINDING}"
         COLUMNS='1'
         ;;
      
      t)
         MEDIA='Tabloid'
         PAGESIZE=${TABLOID_SIZE}
         ORIENT="--portrait ${LONG_EDGE_BINDING}"
         ;;
         
      v)
         verbose=true
         ;;

      \?)
         echo "Unrecognized option: ${opt}"
         echo
         Usage
         exit 1
         ;;   
   esac
done
shift $(( ${OPTIND} - 1 ))

if [ $# -lt 1 ]
then
   echo "No files specified"
   echo
   Usage
   exit 1
fi

OPTS="${OPTS1} --columns=${COLUMNS} --media=${MEDIA} --font=${FONT} --tabsize=${TAB} ${ORIENT} --setpagedevice=PageSize:'${PAGESIZE}' ${PLEX}"
CMD="enscript -q ${PAGE_RANGE} ${OPTS} --printer ${PRT_OUTDEV}"

while [ -n "$1" ]
do
   #
   # Convert the path into a DOS format, but get the fully qualified filename 
   #
   
   #FILE="'$(cygpath -w -a $1)'"
   FILE="'$1'"
   
   echo "Printing ${FILE} to ${PRT_OUTDEV} ..."
   
   if ${verbose}
   then
      echo "About to run ${CMD} ${FILE}"
   fi

   if ${execCmd}
   then
      eval "${CMD} ${FILE}"
   fi
   shift
done

