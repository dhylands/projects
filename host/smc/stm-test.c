/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   stm-test.c 
*
*   @brief  Test program for verifying
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>

#include <Log.h>

#include "stm.h"
#include "cruise-sm.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

int main( void )
{
	STM_StateMachine   *sm = &cruise_controlSm;

    LogInit( stdout );

    STM_SetDebug( sm, 1, 1, 1 );

	STM_AddInput( sm, ccin_enable_cruise );
	STM_AddInput( sm, ccin_start_incr_speed );
	STM_AddInput( sm, ccin_stop_incr_speed );
	STM_AddInput( sm, ccin_apply_brake );
	STM_AddInput( sm, ccin_resume_speed );
	STM_AddInput( sm, ccin_disable_cruise );

	STM_Execute( sm, NULL );

	return 0;
}

void ccac_read_rotation_rate( void ) {}
void ccac_dis_maint_const_spd( void ) {}
void ccac_enab_increase_spd( void ) {}
void ccac_dis_increase_spd( void ) {}
void ccac_enab_maint_const_spd( void ) {}
void ccac_do_nothing( void ) {}
void ccac_enab_ret_to_prev_spd( void ) {}
void ccac_dis_ret_to_prev_spd( void ) {}

//***************************************************************************
/**
*   Description goes here
*/

/** @} */

