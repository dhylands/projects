/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file   bioloid-rs.c
*
*   @brief  Causes the robostix specific version of bioloid.c to be built.
*
*           We use the trick of #including bioloid.c but listing bioloid-rs.c
*           in the Makefile so that different object files get produced.
*
****************************************************************************/

#define  ROBOSTIX 1

#include "bioloid.c"

