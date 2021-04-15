////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   bgame_alloc.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifdef CGAMEDLL
#include <cgame/cgame_precompiled.hpp>
#elif GAMEDLL
#include <sgame/sgame_precompiled.hpp>
#else
#include <GUI/gui_precompiled.hpp>
#endif // !GAMEDLL

#ifdef GAMEDLL
# define POOLSIZE ( 1024UL * 1024UL )
#else
# define POOLSIZE ( 256UL * 1024UL )
#endif

#define FREEMEMCOOKIE ((sint)0xDEADBE3F)  // Any unlikely to be used value
#define ROUNDBITS 31UL                   // Round to 32 bytes

typedef struct freeMemNode_s {
    // Size of ROUNDBITS
    size_t cookie, size;        // Size includes node (obviously)
    struct freeMemNode_s *prev, *next;
} freeMemNode_t;

static valueType memoryPool[POOLSIZE];
static freeMemNode_t *freeHead;
static size_t freeMem;

/*
===============
idBothGamesLocal::AddEvent
===============
*/
void *idBothGamesLocal::Alloc(size_t size) {
    // Find a free block and allocate.
    // Does two passes, attempts to fill same-sized free slot first.

    freeMemNode_t *fmn, * prev, * next, * smallest;
    size_t allocsize, smallestsize, si;
    valueType *endptr;
    sint *ptr;

    si = sizeof(int);

    allocsize = (size + si + ROUNDBITS) &
                ~ROUNDBITS;    // Round to 32-byte boundary
    ptr = nullptr;

    smallest = nullptr;
    smallestsize = POOLSIZE + 1;    // Guaranteed not to miss any slots :)

    for(fmn = freeHead; fmn; fmn = fmn->next) {
        if(fmn->cookie != FREEMEMCOOKIE) {
            Com_Error(ERR_DROP, "idBothGamesLocal: Memory corruption detected!\n");
        }

        if(fmn->size >= allocsize) {
            // We've got a block
            if(fmn->size == allocsize) {
                // Same size, just remove

                prev = fmn->prev;
                next = fmn->next;

                if(prev) {
                    prev->next = next;      // Point previous node to next
                }

                if(next) {
                    next->prev = prev;      // Point next node to previous
                }

                if(fmn == freeHead) {
                    freeHead = next;      // Set head pointer to next
                }

                ptr = (sint *) fmn;
                break;              // Stop the loop, this is fine
            } else {
                // Keep track of the smallest free slot
                if(fmn->size < smallestsize) {
                    smallest = fmn;
                    smallestsize = fmn->size;
                }
            }
        }
    }

    if(!ptr && smallest) {
        // We found a slot big enough
        smallest->size -= allocsize;
        endptr = (valueType *) smallest + smallest->size;
        ptr = (sint *) endptr;
    }

    if(ptr) {
        freeMem -= allocsize;
        ::memset(ptr, 0, allocsize);
        *ptr++ = allocsize;        // Store a copy of size for deallocation
        return((void *) ptr);
    }

    Com_Error(ERR_DROP,
              "idBothGamesLocal: failed on allocation of %ld bytes\n", size);
    return(nullptr);
}

/*
===============
idBothGamesLocal::Free
===============
*/
void idBothGamesLocal::Free(void *ptr) {
    // Release allocated memory, add it to the free list.
    freeMemNode_t *fmn;
    valueType *freeend;
    sint *freeptr;

    freeptr = (sint *)ptr;
    freeptr--;

    freeMem += *freeptr;

    for(fmn = freeHead; fmn; fmn = fmn->next) {
        freeend = ((valueType *) fmn) + fmn->size;

        if(freeend == (valueType *) freeptr) {
            // Released block can be merged to an existing node

            fmn->size += *freeptr;    // Add size of node.
            return;
        }
    }

    // No merging, add to head of list

    fmn = (freeMemNode_t *) freeptr;
    fmn->size = *freeptr;        // Set this first to avoid corrupting *freeptr
    fmn->cookie = FREEMEMCOOKIE;
    fmn->prev = nullptr;
    fmn->next = freeHead;
    freeHead->prev = fmn;
    freeHead = fmn;
}

/*
===============
idBothGamesLocal::Free
===============
*/
void idBothGamesLocal::InitMemory(void) {
    // Set up the initial node
    freeHead = (freeMemNode_t *)memoryPool;
    freeHead->cookie = FREEMEMCOOKIE;
    freeHead->size = POOLSIZE;
    freeHead->next = nullptr;
    freeHead->prev = nullptr;
    freeMem = sizeof(memoryPool);
}

/*
===============
idBothGamesLocal::DefragmentMemory
===============
*/
void idBothGamesLocal::DefragmentMemory(void) {
    // If there's a frenzy of deallocation and we want to
    // allocate something big, this is useful. Otherwise...
    // not much use.
    freeMemNode_t *startfmn, *endfmn, *fmn;

    for(startfmn = freeHead; startfmn;) {
        endfmn = (freeMemNode_t *)(((valueType *) startfmn) + startfmn->size);

        for(fmn = freeHead; fmn;) {
            if(fmn->cookie != FREEMEMCOOKIE) {
                Com_Error(ERR_DROP,
                          "idBothGamesLocal::DefragmentMemory: Memory corruption detected!\n");
            }

            if(fmn == endfmn) {
                // We can add fmn onto startfmn.
                if(fmn->prev) {
                    fmn->prev->next = fmn->next;
                }

                if(fmn->next) {
                    if(!(fmn->next->prev = fmn->prev)) {
                        freeHead = fmn->next;    // We're removing the head node
                    }
                }

                startfmn->size += fmn->size;
                ::memset(fmn, 0, sizeof(freeMemNode_t));     // A redundant call, really.

                startfmn = freeHead;
                endfmn = fmn = nullptr;        // Break out of current loop
            } else {
                fmn = fmn->next;
            }
        }

        if(endfmn) {
            startfmn = startfmn->next;    // endfmn acts as a 'restart' flag here
        }
    }
}
