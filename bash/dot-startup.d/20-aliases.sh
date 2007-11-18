###########################################################################
#
#  20-aliases.sh
#
#  Sets up generic aliases for bash
#
###########################################################################

# Enable extended globbing (used by fgrs et al)

shopt -s extglob

alias a='alias'

a  dir='ls'
a  h='history|tail -24'
a  home='cd ~'
a  open='gnome-open'

a  findname='find . ! \( \( -iname "backup" -o -iname "obj" -o -iname "lib" \) -prune \)'
a  findsrc='findname \( -iname "*.[ch]" -o -iname "*.[ch]xx" -o -iname "*.[ch]pp" -o -iname "*.cc" -o -iname "*.s" -o -iname "*.asm" \)'
a  find_nocyg='find . -xdev ! \( -iname cygwin -prune \)'

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

a  bls=/bin/ls

if [ -z "${VSLICKPATH}" ]
then
   a  ls='ls --color=auto -F'
else
   a  ls='ls --color=auto -F -C --width=105'
fi

a  ll='ls -alG'
a  llr='ll -R'
a  m='less -r'
a  md='mkdir'
a  l='less -r'
a  rd='rmdir'
a  ren='cmd /c ren'
a  rehash='hash -r'
a  which='type'
a  whereis='type'

a  sa='. ~/.startup.d/20-aliases.sh'
a  sb='. ~/.bashrc'
a  se='. ~/.startup.d/01-env.sh'
a  sp='. ~/.startup.d/02-path.sh'
a  setpath='sp'

a  va='vi ~/.startup.d/20-aliases.sh'
a  vb='vi ~/.bashrc'
a  ve='vi ~/.startup.d/01-env.sh'
a  vp='vi ~/.startup.d/02-path.sh'

