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
// File name:   cgame_attachment.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_ATTACHMENT_H__
#define __CGAME_ATTACHMENT_H__

class idCGameLocal;

class idCGameAttachment : public idCGameLocal
{
public:
    idCGameAttachment();
    ~idCGameAttachment();
    
    static bool AttachmentPoint( attachment_t* a, vec3_t v );
    static bool AttachmentDir( attachment_t* a, vec3_t v );
    static bool AttachmentAxis( attachment_t* a, vec3_t axis[3] );
    static bool AttachmentVelocity( attachment_t* a, vec3_t v );
    static bool Attached( attachment_t* a );
    static sint AttachmentCentNum( attachment_t* a );
    static void AttachToPoint( attachment_t* a );
    static void AttachToCent( attachment_t* a );
    static void AttachToTag( attachment_t* a );
    static void AttachToParticle( attachment_t* a );
    static void SetAttachmentPoint( attachment_t* a, vec3_t v );
    static void SetAttachmentCent( attachment_t* a, centity_t* cent );
    static void SetAttachmentTag( attachment_t* a, refEntity_t parent, qhandle_t model, valueType* tagName );
    static void SetAttachmentParticle( attachment_t* a, particle_t* p );
    static void SetAttachmentOffset( attachment_t* a, vec3_t v );
};

#endif //!__CGAME_ATTACHMENT_H__
