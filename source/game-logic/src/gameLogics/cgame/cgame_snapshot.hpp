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
// File name:   cgame_snapshot.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_SNAPSHOT_H__
#define __CGAME_SNAPSHOT_H__

class idCGameSnapshot;

class idCGameSnapshot : public idCGameLocal {
public:
    idCGameSnapshot();
    ~idCGameSnapshot();

    static void ResetEntity(centity_t *cent);
    static void TransitionEntity(centity_t *cent);
    static void SetInitialSnapshot(snapshot_t *snap);
    static void TransitionSnapshot(void);
    static void SetNextSnap(snapshot_t *snap);
    static snapshot_t *ReadNextSnapshot(void);
    static void ProcessSnapshots(void);
};

#endif //!__CGAME_SNAPSHOT_H__
