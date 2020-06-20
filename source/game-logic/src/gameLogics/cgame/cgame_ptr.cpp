////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2018 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_ptr.cpp
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2017, gcc 7.3.0
// Description: post timeout restoration handling
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.h>

#define PTRC_FILE "ptrc.cfg"

/*
===============
idCGamePtr::idCGamePtr
===============
*/
idCGamePtr::idCGamePtr( void )
{
}

/*
===============
idCGamePredict::~idCGamePredict
===============
*/
idCGamePtr::~idCGamePtr( void )
{
}

/*
===============
CG_ReadPTRCode

Read a PTR code from disk
===============
*/
sint idCGamePtr::ReadPTRCode( void )
{
    sint len;
    valueType text[ 16 ];
    fileHandle_t f;
    
    // load the file
    len = trap_FS_FOpenFile( PTRC_FILE, &f, FS_READ );
    if( len < 0 )
    {
        return 0;
    }
    
    // should never happen - malformed write
    if( len == 0 || len >= sizeof( text ) - 1 )
    {
        trap_FS_FCloseFile( f );
        return 0;
    }
    
    trap_FS_Read( text, len, f );
    text[ len ] = 0;
    trap_FS_FCloseFile( f );
    
    return atoi( text );
}

/*
===============
idCGamePtr::WritePTRCode

Write a PTR code to disk
===============
*/
void idCGamePtr::WritePTRCode( sint code )
{
    valueType text[ 16 ];
    fileHandle_t  f;
    
    Com_sprintf( text, 16, "%d", code );
    
    // open file
    if( trap_FS_FOpenFile( PTRC_FILE, &f, FS_WRITE ) < 0 )
    {
        return;
    }
    
    // write the code
    trap_FS_Write( text, strlen( text ), f );
    
    trap_FS_FCloseFile( f );
}
