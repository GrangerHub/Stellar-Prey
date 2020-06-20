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
// File name:   cgame_mem.cpp
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2017, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.h>

#define POOLSIZE (256 * 1024)
#define FREEMEMCOOKIE ((sint)0xDEADBE3F) // Any unlikely to be used value
#define ROUNDBITS 31          // Round to 32 bytes

struct freememnode
{
    // Size of ROUNDBITS
    sint cookie, size;       // Size includes node (obviously)
    struct freememnode* prev, *next;
};

static valueType memoryPool[ POOLSIZE ];
static struct freememnode* freehead;
static sint freemem;

/*
===============
idCGameMem::idCGameMem
===============
*/
idCGameMem::idCGameMem( void )
{
}

/*
===============
idCGameMem::~idCGameMem
===============
*/
idCGameMem::~idCGameMem( void )
{
}

/*
=================
idCGameMem::Alloc
=================
*/
void* idCGameMem::Alloc( sint size )
{
    // Find a free block and allocate.
    // Does two passes, attempts to fill same-sized free slot first.
    struct freememnode* fmn, *prev, *next, *smallest;
    sint allocsize, smallestsize;
    valueType* endptr;
    sint* ptr;
    
    allocsize = ( size + sizeof( sint ) + ROUNDBITS ) & ~ROUNDBITS; // Round to 32-byte boundary
    ptr = nullptr;
    
    smallest = nullptr;
    smallestsize = POOLSIZE + 1;    // Guaranteed not to miss any slots :)
    for( fmn = freehead; fmn; fmn = fmn->next )
    {
        if( fmn->cookie != FREEMEMCOOKIE )
        {
            Error( "idCGameMem::Alloc: Memory corruption detected!\n" );
        }
        
        if( fmn->size >= allocsize )
        {
            // We've got a block
            if( fmn->size == allocsize )
            {
                // Same size, just remove
                prev = fmn->prev;
                next = fmn->next;
                
                if( prev )
                {
                    prev->next = next;      // Point previous node to next
                }
                
                if( next )
                {
                    next->prev = prev;      // Point next node to previous
                }
                
                if( fmn == freehead )
                {
                    freehead = next;      // Set head pointer to next
                }
                
                ptr = ( sint* ) fmn;
                break;              // Stop the loop, this is fine
            }
            else
            {
                // Keep track of the smallest free slot
                if( fmn->size < smallestsize )
                {
                    smallest = fmn;
                    smallestsize = fmn->size;
                }
            }
        }
    }
    
    if( !ptr && smallest )
    {
        // We found a slot big enough
        smallest->size -= allocsize;
        endptr = ( valueType* ) smallest + smallest->size;
        ptr = ( sint* ) endptr;
    }
    
    if( ptr )
    {
        freemem -= allocsize;
        
        if( cg_debugAlloc.integer )
        {
            Printf( "idCGameMem::Alloc of %i bytes (%i left)\n", allocsize, freemem );
        }
        
        ::memset( ptr, 0, allocsize );
        *ptr++ = allocsize; // Store a copy of size for deallocation
        return( ( void* ) ptr );
    }
    
    Error( "idCGameMem::Alloc: failed on allocation of %i bytes\n", size );
    return( nullptr );
}

/*
=================
idCGameMem::Free
=================
*/
void idCGameMem::Free( void* ptr )
{
    // Release allocated memory, add it to the free list.
    struct freememnode* fmn;
    valueType* freeend;
    sint* freeptr;
    
    freeptr = ( sint* )ptr;
    freeptr--;
    
    freemem += *freeptr;
    
    if( cg_debugAlloc.integer )
    {
        Printf( "idCGameMem::Free of %i bytes (%i left)\n", *freeptr, freemem );
    }
    
    for( fmn = freehead; fmn; fmn = fmn->next )
    {
        freeend = ( ( valueType* ) fmn ) + fmn->size;
        if( freeend == ( valueType* ) freeptr )
        {
            // Released block can be merged to an existing node
            fmn->size += *freeptr; // Add size of node.
            return;
        }
    }
    
    // No merging, add to head of list
    fmn = ( struct freememnode* ) freeptr;
    fmn->size = *freeptr;       // Set this first to avoid corrupting *freeptr
    fmn->cookie = FREEMEMCOOKIE;
    fmn->prev = nullptr;
    fmn->next = freehead;
    freehead->prev = fmn;
    freehead = fmn;
}

/*
=================
idCGameMem::InitMemory
=================
*/
void idCGameMem::InitMemory( void )
{
    // Set up the initial node
    freehead = ( struct freememnode* ) memoryPool;
    freehead->cookie = FREEMEMCOOKIE;
    freehead->size = POOLSIZE;
    freehead->next = nullptr;
    freehead->prev = nullptr;
    freemem = sizeof( memoryPool );
}

/*
=================
idCGameMem::DefragmentMemory
=================
*/
void idCGameMem::DefragmentMemory( void )
{
    // If there's a frenzy of deallocation and we want to
    // allocate something big, this is useful. Otherwise...
    // not much use.
    
    struct freememnode* startfmn, *endfmn, *fmn;
    
    for( startfmn = freehead; startfmn; )
    {
        endfmn = ( struct freememnode* )( ( ( valueType* ) startfmn ) + startfmn->size );
        
        for( fmn = freehead; fmn; )
        {
            if( fmn->cookie != FREEMEMCOOKIE )
            {
                Error( "idCGameMem::DefragmentMemory: Memory corruption detected!\n" );
            }
            
            if( fmn == endfmn )
            {
                // We can add fmn onto startfmn.
                if( fmn->prev )
                {
                    fmn->prev->next = fmn->next;
                }
                
                if( fmn->next )
                {
                    if( !( fmn->next->prev = fmn->prev ) )
                    {
                        freehead = fmn->next; // We're removing the head node
                    }
                }
                
                startfmn->size += fmn->size;
                ::memset( fmn, 0, sizeof( struct freememnode ) ); // A redundant call, really.
                
                startfmn = freehead;
                endfmn = fmn = nullptr;        // Break out of current loop
            }
            else
            {
                fmn = fmn->next;
            }
        }
        
        if( endfmn )
        {
            startfmn = startfmn->next;    // endfmn acts as a 'restart' flag here
        }
    }
}
