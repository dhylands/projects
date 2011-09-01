#
# 01-env.sh
#
# This script is normally loaded through .profile. It's split off separately
# so that it can be edited using the 've' alias, and re entered into the
# environment using the 'se' alias.
#

export SVN_EDITOR=vi

export TERM=ansi

export ENV_INCLUDED=1

#export ENSCRIPT_LIBRARY='c:\Home\Utils\enscript'
#export PRT_OUTDEV='\\dave\LaserJet-2200'

if [ -z "${VSLICKPATH}" ]
then
   export PS1="\[\033]0;\W - \u@\h \w\007\]${PROMPT_USER_COLOR}\u@\h ${PROMPT_PATH_COLOR}\w\n${PROMPT_USER_COLOR}\! >${NO_COLOR}"
else
   export PS1="\! \w>"
fi
