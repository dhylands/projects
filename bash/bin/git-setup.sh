#!/bin/sh
#
# This script does some common git setup that I do for any new account/machine I use.
#
set -x

git config --global user.name "Dave Hylands"
git config --global user.email "dhylands@broadcom.com"
git config --global color.diff auto
git config --global color.status auto
git config --global color.branch auto
git config --global alias.status-report  '!sh -c "git log --pretty='\''%C(yellow)%h%Creset %cn %Cgreen%cr%Creset %s'\''" | grep Hylands'
git config --global alias.summary-report '!sh -c "git log --pretty='\''%C(yellow)%h%Creset %cn %Cgreen%cr%Creset %s'\''"'

