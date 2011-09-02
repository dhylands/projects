#
# This file sets up the prompt for use under the bash shell
#

if [ `id -u` -gt 14 ]; then
    #
    # Logged in as a regular user
    #
    PROMPT_USER_COLOR=${DARK_GREEN}
    PROMPT_PATH_COLOR=${GREEN}
    PROMPT_BRANCH_COLOR=${DARK_YELLOW}
else
    #
    # Logged in as root user
    #
    PROMPT_USER_COLOR=${RED}
    PROMPT_PATH_COLOR=${GREEN}
    PROMPT_BRANCH_COLOR=${DARK_YELLOW}
fi

#export GIT_PS1_SHOWDIRTYSTATE=y
#export GIT_PS1_SHOWUNTRACKEDFILES=y
#export GIT_PS1_SHOWUPSTREAM=auto

unset GIT_PS1_SHOWDIRTYSTATE
unset GIT_PS1_SHOWUNTRACKEDFILES
unset GIT_PS1_SHOWUPSTREAM

if [ $TERM = "screen" ]
then
    export PS1="\[\e]0;\W - \u@\h \w\007\]\[\e[0;32m\]\u@\h \e[1;32m\]\w\n\! >\[\e[0m\] "
else
    if [ -z "${VSLICKPATH}" ]
    then
       export PS1="\[\033]0;\W - \u@\h \w\007\]\[\033[0;32m\]${PROMPT_USER_COLOR}\u@\h ${PROMPT_PATH_COLOR}\w ${PROMPT_BRANCH_COLOR}\$(__git_ps1 ' (%s)')\n${PROMPT_USER_COLOR}\! >${NO_COLOR}"
    else
       export PS1="\! \w>"
    fi
fi
