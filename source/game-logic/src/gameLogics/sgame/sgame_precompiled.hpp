////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 - 2021 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the License,
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
// File name:   sgame_precompiled.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
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

#include <framework/appConfig.hpp>
#include <framework/types.hpp>
#include <qcommon/q_platform.hpp>
#include <qcommon/q_shared.hpp>
#include <sgame/tremulous.hpp>
#include <framework/SurfaceFlags_Tech3.hpp>
#include <API/Parse_api.hpp>
#include <API/cm_api.hpp>
#include <API/CmdBuffer_api.hpp>
#include <API/CmdSystem_api.hpp>
#include <API/CVarSystem_api.hpp>
#include <GPURenderer/r_types.hpp>
#include <API/renderer_api.hpp>
#include <API/sound_api.hpp>
#include <API/FileSystem_api.hpp>
#include <API/serverDemo_api.hpp>
#include <API/serverGame_api.hpp>
#include <API/serverInit_api.hpp>
#include <API/serverMain_api.hpp>
#include <API/serverWorld_api.hpp>
#include <API/CmdSystem_api.hpp>
#include <API/system_api.hpp>
#include <API/bgame_api.hpp>
#include <bgame/bgame_local.hpp>
#include <sgame/tremulous.hpp>
#include <API/sgame_api.hpp>
#include <sgame/sgame_admin.hpp>
#include <sgame/sgame_local.hpp>
#include <sgame/sgame_admin.hpp>
#include <GUI/gui_shared.hpp>
#include <qcommon/qfiles.hpp>

#include <sgame/sgame_weapon.hpp>
#include <sgame/sgame_utils.hpp>
#include <sgame/sgame_trigger.hpp>
#include <sgame/sgame_team.hpp>
#include <sgame/sgame_target.hpp>
#include <sgame/sgame_svcmds.hpp>
#include <sgame/sgame_spawn.hpp>
#include <sgame/sgame_session.hpp>
#include <sgame/sgame_ptr.hpp>
#include <sgame/sgame_physics.hpp>
#include <sgame/sgame_mover.hpp>
#include <sgame/sgame_missile.hpp>
#include <sgame/sgame_misc.hpp>
#include <sgame/sgame_maprotation.hpp>
#include <sgame/sgame_main.hpp>
#include <sgame/sgame_combat.hpp>
#include <sgame/sgame_cmds.hpp>
#include <sgame/sgame_client.hpp>
#include <sgame/sgame_buildable.hpp>
#include <sgame/sgame_active.hpp>

#endif // !__SGAME_PRECOMPILED_H__
