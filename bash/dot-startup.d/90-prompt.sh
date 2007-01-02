#
# This file sets up the prompt for use under the bash shell
#

if [ `id -u` -gt 14 ]; then
	#
	# Logged in as a regular user
	#
	PROMPT_USER_COLOR=${DARK_GREEN}
   PROMPT_PATH_COLOR=${GREEN}
else
	#
	# Logged in as root user
	#
	PROMPT_USER_COLOR=${RED}
   PROMPT_PATH_COLOR=${GREEN}
fi

if [ -z "${VSLICKPATH}" ]
then
   export PS1="\[\033]0;\W - \u@\h \w\007\]${PROMPT_USER_COLOR}\u@\h ${PROMPT_PATH_COLOR}\w\n${PROMPT_USER_COLOR}\! >${NO_COLOR}"
else
   export PS1="\! \w>"
fi
