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
// File name:   cgame_precompiled.h
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_PRECOMPILED_H__

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

#include <framework/appConfig.h>
#include <framework/types.h>
#include <qcommon/q_platform.h>
#include <qcommon/q_shared.h>
#include <GPURenderer/r_types.h>
#include <sgame/tremulous.h>
#include <framework/SurfaceFlags_Tech3.h>
#include <API/cm_api.h>
#include <API/Parse_api.h>
#include <API/clientScreen_api.h>
#include <API/clientGUI_api.h>
#include <API/clientGame_api.h>
#include <API/clientLAN_api.h>
#include <qcommon/qfiles.h>
#include <API/CmdBuffer_api.h>
#include <API/CmdSystem_api.h>
#include <API/CVarSystem_api.h>
#include <API/renderer_api.h>
#include <API/sound_api.h>
#include <API/FileSystem_api.h>
#include <API/system_api.h>
#include <API/cgame_api.h>
#include <API/bgame_api.h>
#include <API/gui_api.h>
#include <bgame/bgame_local.h>
#include <GUI/keycodes.h>
#include <GUI/menudef.h>
#include <GUI/gui_shared.h>
#include <GUI/gui_local.h>
#include <API/gui_api.h>

#endif // !__GUI_PRECOMPILED_H__
