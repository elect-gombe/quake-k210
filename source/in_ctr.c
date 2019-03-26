/*
Copyright (C) 2015 Felipe Izzo
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// in_ctr.c -- for the Nintendo 3DS

#include "quakedef.h"

#include "ctr.h"
#include "dualshock2.h"


void IN_Init (void)
{
}

void IN_Shutdown (void)
{
}

void IN_Commands (void)
{
}

extern uint8_t keyboardToggled;

void IN_Move (usercmd_t *cmd)
{
  {
    float yaw,pitch;
    yaw = PS2X_Analog(PSS_RX)-0x80;
    pitch = PS2X_Analog(PSS_RY)-0x80;
    if(fabs(yaw) < 25.f)yaw = 0.f;
    if(fabs(pitch) < 25.f)pitch = 0.f;
    yaw /= 128.f;
    pitch /= 128.f;
    yaw*=3;
    pitch*=3;
    cl.viewangles[YAW] -= yaw;
    cl.viewangles[PITCH] += pitch;
  }
  {
    float forward,side;
    forward = PS2X_Analog(PSS_LY)-0x80;
    side = PS2X_Analog(PSS_LX)-0x80;
    if(fabs(forward) < 25.f)forward = 0.f;
    if(fabs(side) < 25.f)side = 0.f;
    forward /= -128.f;
    side /= 128.f;
    cmd->forwardmove  += forward*cl_forwardspeed.value*2;
    cmd->sidemove += side*cl_forwardspeed.value*2;
  }

  if(!lookspring.value)
    V_StopPitchDrift (); 
}
