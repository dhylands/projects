###########################################################################
#
#  20-aliases.sh
#
#  Sets up generic aliases for bash
#
###########################################################################

# Enable extended globbing (used by fgrs et al)

shopt -s extglob

alias  dir='ls'
alias  h='history|tail -24'
alias  home='cd ~'

alias  findname='find . ! \( \( -iname "backup" -o -iname "obj" -o -iname "lib" \) -prune \)'
alias  findsrc='findname \( -iname "*.[ch]" -o -iname "*.[ch]xx" -o -iname "*.[ch]pp" -o -iname "*.cc" -o -iname "*.s" -o -iname "*.asm" \)'
alias  find_nocyg='find . -xdev ! \( -iname cygwin -prune \)'

function fgrs
{
   findsrc -exec grep -F -H -- "$@" '{}' ';'
}

function fgrsi
{
   findsrc -exec grep -F -H -i -- "$@" '{}' ';'
}

function grs
{
   findsrc -exec grep -H -- "$@" '{}' ';'
}

function grsi
{
   findsrc -exec grep -H -i -- "$@" '{}' ';'
}

function egrs
{
   findsrc -exec grep -E -H -- "$@" '{}' ';'
}

function egrsi
{
   findsrc -exec grep -E -H -i -- "$@" '{}' ';'
}

alias  bls=/bin/ls

if [ -z "${VSLICKPATH}" ]
then
   alias  ls='ls --color=auto -F'
else
   alias  ls='ls --color=auto -F -C --width=105'
fi

alias  ll='ls -alG'
alias  llr='ll -R'
alias  m='less -r'
alias  md='mkdir'
alias  l='less -r'
alias  rd='rmdir'
alias  ren='cmd /c ren'
alias  rehash='hash -r'
alias  which='type'
alias  whereis='type'
alias  open='cygstart'

alias  sa='. ~/.startup.d/20-aliases.sh'
alias  sb='. ~/.bashrc'
alias  se='. ~/.startup.d/01-env.sh'
alias  sp='. ~/.startup.d/02-path.sh'
alias  setpath='sp'

alias  va='vi ~/.startup.d/20-aliases.sh'
alias  vb='vi ~/.bashrc'
alias  ve='vi ~/.startup.d/01-env.sh'
alias  vp='vi ~/.startup.d/02-path.sh'

