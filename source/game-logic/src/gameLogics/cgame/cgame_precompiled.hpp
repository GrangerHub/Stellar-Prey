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
// File name:   cgame_precompiled.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CG_PRECOMPILED_H__
#define __CG_PRECOMPILED_H__

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
#include <GPURenderer/r_types.hpp>
#include <sgame/tremulous.hpp>
#include <framework/SurfaceFlags_Tech3.hpp>
#include <qcommon/qfiles.hpp>
#include <API/cm_api.hpp>
#include <API/Parse_api.hpp>
#include <API/clientScreen_api.hpp>
#include <API/clientAVI_api.hpp>
#include <API/clientGame_api.hpp>
#include <API/clientGUI_api.hpp>
#include <API/clientLAN_api.hpp>
#include <API/CmdBuffer_api.hpp>
#include <API/CmdSystem_api.hpp>
#include <API/CVarSystem_api.hpp>
#include <API/renderer_api.hpp>
#include <API/sound_api.hpp>
#include <API/FileSystem_api.hpp>
#include <API/system_api.hpp>
#include <API/cgame_api.hpp>
#include <API/bgame_api.hpp>
#include <API/gui_api.hpp>
#include <GUI/gui_shared.hpp>
#include <cgame/cgame_local.hpp>
#include <API/bgame_api.hpp>
#include <bgame/bgame_local.hpp>
#include <GUI/keycodes.hpp>
#include <GUI/menudef.hpp>
#include <GUI/gui_local.hpp>

#include <cgame/cgame_animation.hpp>
#include <cgame/cgame_animmapobj.hpp>
#include <cgame/cgame_attachment.hpp>
#include <cgame/cgame_buildable.hpp>
#include <cgame/cgame_consolecmds.hpp>
#include <cgame/cgame_draw.hpp>
#include <cgame/cgame_drawtools.hpp>
#include <cgame/cgame_ents.hpp>
#include <cgame/cgame_event.hpp>
#include <cgame/cgame_main.hpp>
#include <cgame/cgame_marks.hpp>
#include <cgame/cgame_mem.hpp>
#include <cgame/cgame_particles.hpp>
#include <cgame/cgame_players.hpp>
#include <cgame/cgame_playerstate.hpp>
#include <cgame/cgame_predict.hpp>
#include <cgame/cgame_ptr.hpp>
#include <cgame/cgame_scanner.hpp>
#include <cgame/cgame_servercmds.hpp>
#include <cgame/cgame_snapshot.hpp>
#include <cgame/cgame_trails.hpp>
#include <cgame/cgame_tutorial.hpp>
#include <cgame/cgame_view.hpp>
#include <cgame/cgame_weapons.hpp>

#endif // !__CG_PRECOMPILED_H__
