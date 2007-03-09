#!/bin/bash
#############################################################################
#
#  Tests the appearance of colors I like to use in my colorized directory 
#  listings.
#
#############################################################################

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

BLACK="\033[1;30m"
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
MAGENTA="\033[1;35m"
CYAN="\033[1;36m"
WHITE="\033[1;37m"

DARK_BLACK="\033[2;30m"
DARK_RED="\033[2;31m"
DARK_GREEN="\033[2;32m"
DARK_YELLOW="\033[2;33m"
DARK_BLUE="\033[2;34m"
DARK_MAGENTA="\033[2;35m"
DARK_CYAN="\033[2;36m"
DARK_WHITE="\033[2;37m"

NO_COLOR="\033[0m"

echo -e "${BLACK}BLACK ${NO_COLOR}"
echo -e "${RED}RED ${NO_COLOR}"
echo -e "${GREEN}GREEN ${NO_COLOR}"
echo -e "${YELLOW}YELLOW ${NO_COLOR}"
echo -e "${BLUE}BLUE ${NO_COLOR}"
echo -e "${MAGENTA}MAGENTA ${NO_COLOR}"
echo -e "${CYAN}CYAN ${NO_COLOR}"
echo -e "${WHITE}WHITE ${NO_COLOR}"

echo -e "${DARK_BLACK}DARK_BLACK ${NO_COLOR}"
echo -e "${DARK_RED}DARK_RED ${NO_COLOR}"
echo -e "${DARK_GREEN}DARK_GREEN ${NO_COLOR}"
echo -e "${DARK_YELLOW}DARK_YELLOW ${NO_COLOR}"
echo -e "${DARK_BLUE}DARK_BLUE ${NO_COLOR}"
echo -e "${DARK_MAGENTA}DARK_MAGENTA ${NO_COLOR}"
echo -e "${DARK_CYAN}DARK_CYAN ${NO_COLOR}"
echo -e "${DARK_WHITE}DARK_WHITE ${NO_COLOR}"

