////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 - 2019 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the License,
// or (at your option) any later version.
//
// OpenWolf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
//
// -------------------------------------------------------------------------------------
// File name:   sgame_precompiled.h
// Version:     v1.03
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_PRECOMPILED_H__
#define __SGAME_PRECOMPILED_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <signal.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btGeometryUtil.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <framework/appConfig.h>
#include <framework/types.h>
#include <sgame/tremulous.h>
#include <qcommon/q_platform.h>
#include <qcommon/q_shared.h>
#include <framework/SurfaceFlags_Tech3.h>
#include <API/Parse_api.h>
#include <API/cm_api.h>
#include <API/CmdBuffer_api.h>
#include <API/CmdSystem_api.h>
#include <API/CVarSystem_api.h>
#include <GPURenderer/r_types.h>
#include <API/renderer_api.h>
#include <API/sound_api.h>
#include <API/FileSystem_api.h>
#include <API/serverDemo_api.h>
#include <API/serverGame_api.h>
#include <API/serverInit_api.h>
#include <API/serverMain_api.h>
#include <API/serverWorld_api.h>
#include <API/CmdSystem_api.h>
#include <API/system_api.h>
#include <API/bgame_api.h>
#include <bgame/bgame_local.h>
#include <sgame/tremulous.h>
#include <API/sgame_api.h>
#include <sgame/sgame_admin.h>
#include <sgame/sgame_local.h>
#include <sgame/sgame_admin.h>
#include <GUI/gui_shared.h>
#include <qcommon/qfiles.h>

#include <sgame/sgame_weapon.h>
#include <sgame/sgame_utils.h>
#include <sgame/sgame_trigger.h>
#include <sgame/sgame_team.h>
#include <sgame/sgame_target.h>
#include <sgame/sgame_svcmds.h>
#include <sgame/sgame_spawn.h>
#include <sgame/sgame_session.h>
#include <sgame/sgame_ptr.h>
#include <sgame/sgame_physics.h>
#include <sgame/sgame_mover.h>
#include <sgame/sgame_missile.h>
#include <sgame/sgame_misc.h>
#include <sgame/sgame_maprotation.h>
#include <sgame/sgame_main.h>
#include <sgame/sgame_combat.h>
#include <sgame/sgame_cmds.h>
#include <sgame/sgame_client.h>
#include <sgame/sgame_buildable.h>
#include <sgame/sgame_active.h>
#include <sgame/sgame_bulletphysics.h>

#endif // !__SGAME_PRECOMPILED_H__
