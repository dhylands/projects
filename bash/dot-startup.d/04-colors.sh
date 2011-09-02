# Attributes
# 0	Reset all attributes
# 1	Bright
# 2	Dim
# 4	Underscore	
# 5	Blink
# 7	Reverse
# 8	Hidden
# 
# Foreground Colours
# 30	Black
# 31	Red
# 32	Green
# 33	Yellow
# 34	Blue
# 35	Magenta
# 36	Cyan
# 37	White
# 
# Background Colours
# 40	Black
# 41	Red
# 42	Green
# 43	Yellow
# 44	Blue
# 45	Magenta
# 46	Cyan
# 47	White

RED="\[\033[1;31m\]"
GREEN="\[\033[1;32m\]"
YELLOW="\[\033[1;33m\]"
DARK_GREEN="\[\033[2;32m\]"
DARK_YELLOW="\[\033[2;33m\]"
DARK_CYAN="\[\033[2;36m\]"
NO_COLOR="\[\033[0m\]"

#
# NOTE: dircolors is sensitive to the setting of the TERM environment variable
#
eval `dircolors --sh ~/.dircolors`

