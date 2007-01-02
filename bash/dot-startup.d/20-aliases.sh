###########################################################################
#
# .aliases.sh
#
# Sets up aliases for bash
#

# Enable extended globbing (used by fgrs et al)

shopt -s extglob


alias a='alias'

a  c:='cd /c'
a  d:='cd /d'
a  e:='cd /e'
a  f:='cd /f'
a  g:='cd /g'
a  h:='cd /h'
a  i:='cd /i'
a  j:='cd /j'
a  k:='cd /k'
a  l:='cd /l'
a  m:='cd /m'
a  n:='cd /n'
a  o:='cd /o'
a  p:='cd /p'
a  q:='cd /q'
a  r:='cd /r'
a  s:='cd /s'
a  t:='cd /t'
a  u:='cd /u'
a  v:='cd /v'
a  w:='cd /w'
a  x:='cd /x'
a  y:='cd /y'
a  z:='cd /z'

a  dir='ls'
a  etc='cd /c/WINDOWS/system32/drivers/etc'
a  h='history|tail -24'
a  home='cd ~'

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
a  vc="cd '${MSVCDIR}'"
a  vcinc="cd '${MSVCDIR}/include'"
a  which='type'
a  whereis='type'

a  start='cmd /c start'

function np
{
   local -a fileList
   local idx

   idx=0

   for file in "$@"
   do
      fileList[${idx}]=$(cygpath -w "${file}")
      idx=$(( ${idx} + 1 ))
   done
   start notepad "${fileList[@]}"
}

a  sa='. ~/.startup.d/20-aliases.sh'
a  sb='. ~/.bashrc'
a  se='. ~/.startup.d/01-env.sh'
a  sp='. ~/.startup.d/02-path.sh'
a  setpath='sp'

a  va='vi ~/.startup.d/20-aliases.sh'
a  vb='vi ~/.bashrc'
a  ve='vi ~/.startup.d/01-env.sh'
a  vp='vi ~/.startup.d/02-path.sh'

