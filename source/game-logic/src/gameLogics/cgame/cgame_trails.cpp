////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_trails.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: the trail system
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

static baseTrailSystem_t baseTrailSystems[ MAX_BASETRAIL_SYSTEMS ];
static baseTrailBeam_t baseTrailBeams[ MAX_BASETRAIL_BEAMS ];
static sint numBaseTrailSystems = 0;
static sint numBaseTrailBeams = 0;

static trailSystem_t trailSystems[ MAX_TRAIL_SYSTEMS ];
static trailBeam_t trailBeams[ MAX_TRAIL_BEAMS ];

/*
===============
idCGameSnapshot::idCGameSnapshot
===============
*/
idCGameTrails::idCGameTrails(void) {
}

/*
===============
idCGameTrails::~idCGameTrails
===============
*/
idCGameTrails::~idCGameTrails(void) {
}

/*
===============
CG_CalculateBeamNodeProperties

Fills in trailBeamNode_t.textureCoord
===============
*/
void idCGameTrails::CalculateBeamNodeProperties(trailBeam_t *tb) {
    trailBeamNode_t *i = nullptr;
    trailSystem_t *ts;
    baseTrailBeam_t *btb;
    float32 nodeDistances[ MAX_TRAIL_BEAM_NODES ];
    float32 totalDistance = 0.0f, position = 0.0f;
    sint j, numNodes = 0;
    float32 TCRange, widthRange, alphaRange;
    vec3_t colorRange;
    float32 fadeAlpha = 1.0f;

    if(!tb || !tb->nodes) {
        return;
    }

    ts = tb->parent;
    btb = tb->_class;

    if(ts->destroyTime > 0 && btb->fadeOutTime) {
        fadeAlpha -= (cg.time - ts->destroyTime) / btb->fadeOutTime;

        if(fadeAlpha < 0.0f) {
            fadeAlpha = 0.0f;
        }
    }

    TCRange = tb->_class->backTextureCoord - tb->_class->frontTextureCoord;
    widthRange = tb->_class->backWidth - tb->_class->frontWidth;
    alphaRange = tb->_class->backAlpha - tb->_class->frontAlpha;
    VectorSubtract(tb->_class->backColor, tb->_class->frontColor, colorRange);

    for(i = tb->nodes; i && i->next; i = i->next) {
        nodeDistances[ numNodes++ ] = Distance(i->position, i->next->position);
    }

    for(j = 0; j < numNodes; j++) {
        totalDistance += nodeDistances[j];
    }

    for(j = 0, i = tb->nodes; i; i = i->next, j++) {
        if(tb->_class->textureType == TBTT_STRETCH) {
            i->textureCoord = tb->_class->frontTextureCoord + ((position /
                              totalDistance) * TCRange);
        } else if(tb->_class->textureType == TBTT_REPEAT) {
            if(tb->_class->clampToBack) {
                i->textureCoord = (totalDistance - position) / tb->_class->repeatLength;
            } else {
                i->textureCoord = position / tb->_class->repeatLength;
            }
        }

        i->halfWidth = (tb->_class->frontWidth + ((position / totalDistance) *
                        widthRange)) / 2.0f;
        i->alpha = (uchar8)((float32)0xFF * (tb->_class->frontAlpha + ((
                position / totalDistance) * alphaRange)) * fadeAlpha);

        VectorMA(tb->_class->frontColor, (position / totalDistance), colorRange,
                 i->color);

        position += nodeDistances[ j ];
    }
}

/*
===============
idCGameTrails::LightVertex

Lights a particular vertex
===============
*/
void idCGameTrails::LightVertex(vec3_t point, uchar8 alpha, uchar8 *rgba) {
    sint i;
    vec3_t alight, dlight, lightdir;

    trap_R_LightForPoint(point, alight, dlight, lightdir);

    for(i = 0; i <= 2; i++) {
        rgba[i] = (sint)alight[i];
    }

    rgba[ 3 ] = alpha;
}

/*
===============
idCGameTrails::RenderBeam

Renders a beam
===============
*/
void idCGameTrails::RenderBeam(trailBeam_t *tb) {
    trailBeamNode_t *i = nullptr;
    trailBeamNode_t *prev = nullptr;
    trailBeamNode_t *next = nullptr;
    vec3_t up;
    polyVert_t verts[(MAX_TRAIL_BEAM_NODES - 1) * 4 ];
    sint numVerts = 0;
    baseTrailBeam_t *btb;
    trailSystem_t *ts;
    baseTrailSystem_t *bts;

    if(!tb || !tb->nodes) {
        return;
    }

    btb = tb->_class;
    ts = tb->parent;
    bts = ts->_class;

    if(bts->thirdPersonOnly &&
            (idCGameAttachment::AttachmentCentNum(&ts->frontAttachment) ==
             cg.snap->ps.clientNum ||
             idCGameAttachment::AttachmentCentNum(&ts->backAttachment) ==
             cg.snap->ps.clientNum) && !cg.renderingThirdPerson) {
        return;
    }

    CalculateBeamNodeProperties(tb);

    i = tb->nodes;

    do {
        prev = i->prev;
        next = i->next;

        if(prev && next) {
            //this node has two neighbours
            GetPerpendicularViewVector(cg.refdef.vieworg, next->position,
                                       prev->position, up);
        } else if(!prev && next) {
            //this is the front
            GetPerpendicularViewVector(cg.refdef.vieworg, next->position, i->position,
                                       up);
        } else if(prev && !next) {
            //this is the back
            GetPerpendicularViewVector(cg.refdef.vieworg, i->position, prev->position,
                                       up);
        } else {
            break;
        }

        if(prev) {
            VectorMA(i->position, i->halfWidth, up, verts[ numVerts ].xyz);
            verts[ numVerts ].st[ 0 ] = i->textureCoord;
            verts[ numVerts ].st[ 1 ] = 1.0f;

            if(btb->realLight) {
                LightVertex(verts[numVerts].xyz, i->alpha, verts[numVerts].modulate);
            } else {
                VectorCopy(i->color, verts[ numVerts ].modulate);
                verts[ numVerts ].modulate[ 3 ] = i->alpha;
            }

            numVerts++;

            VectorMA(i->position, -i->halfWidth, up, verts[ numVerts ].xyz);
            verts[ numVerts ].st[ 0 ] = i->textureCoord;
            verts[ numVerts ].st[ 1 ] = 0.0f;

            if(btb->realLight) {
                LightVertex(verts[numVerts].xyz, i->alpha, verts[numVerts].modulate);
            } else {
                VectorCopy(i->color, verts[ numVerts ].modulate);
                verts[ numVerts ].modulate[ 3 ] = i->alpha;
            }

            numVerts++;
        }

        if(next) {
            VectorMA(i->position, -i->halfWidth, up, verts[ numVerts ].xyz);
            verts[ numVerts ].st[ 0 ] = i->textureCoord;
            verts[ numVerts ].st[ 1 ] = 0.0f;

            if(btb->realLight) {
                LightVertex(verts[numVerts].xyz, i->alpha, verts[numVerts].modulate);
            } else {
                VectorCopy(i->color, verts[ numVerts ].modulate);
                verts[ numVerts ].modulate[ 3 ] = i->alpha;
            }

            numVerts++;

            VectorMA(i->position, i->halfWidth, up, verts[ numVerts ].xyz);
            verts[ numVerts ].st[ 0 ] = i->textureCoord;
            verts[ numVerts ].st[ 1 ] = 1.0f;

            if(btb->realLight) {
                LightVertex(verts[numVerts].xyz, i->alpha, verts[numVerts].modulate);
            } else {
                VectorCopy(i->color, verts[ numVerts ].modulate);
                verts[ numVerts ].modulate[ 3 ] = i->alpha;
            }

            numVerts++;
        }

        i = i->next;
    } while(i);

    trap_R_AddPolysToScene(tb->_class->shader, 4, &verts[ 0 ], numVerts / 4);
}

/*
===============
idCGameTrails::AllocateBeamNode

Allocates a trailBeamNode_t from a trailBeam_t's nodePool
===============
*/
trailBeamNode_t *idCGameTrails::AllocateBeamNode(trailBeam_t *tb) {
    baseTrailBeam_t *btb = tb->_class;
    sint i;
    trailBeamNode_t *tbn;

    for(i = 0; i < MAX_TRAIL_BEAM_NODES; i++) {
        tbn = &tb->nodePool[ i ];

        if(!tbn->used) {
            tbn->timeLeft = btb->segmentTime;
            tbn->prev = nullptr;
            tbn->next = nullptr;
            tbn->used = true;
            return tbn;
        }
    }

    // no space left
    return nullptr;
}

/*
===============
idCGameTrails::DestroyBeamNode

Removes a node from a beam
Returns the new head
===============
*/
trailBeamNode_t *idCGameTrails::DestroyBeamNode(trailBeamNode_t *tbn) {
    trailBeamNode_t *newHead = nullptr;

    if(tbn->prev) {
        if(tbn->next) {
            // node is in the middle
            tbn->prev->next = tbn->next;
            tbn->next->prev = tbn->prev;
        } else {
            // node is at the back
            tbn->prev->next = nullptr;
        }

        // find the new head (shouldn't have changed)
        newHead = tbn->prev;

        while(newHead->prev) {
            newHead = newHead->prev;
        }
    } else if(tbn->next) {
        //node is at the front
        tbn->next->prev = nullptr;
        newHead = tbn->next;
    }

    tbn->prev = nullptr;
    tbn->next = nullptr;
    tbn->used = false;

    return newHead;
}

/*
===============
idCGameTrails::FindLastBeamNode

Returns the last beam node in a beam
===============
*/
trailBeamNode_t *idCGameTrails::FindLastBeamNode(trailBeam_t *tb) {
    trailBeamNode_t *i = tb->nodes;

    while(i && i->next) {
        i = i->next;
    }

    return i;
}

/*
===============
idCGameTrails::CountBeamNodes

Returns the number of nodes in a beam
===============
*/
sint idCGameTrails::CountBeamNodes(trailBeam_t *tb) {
    trailBeamNode_t *i = tb->nodes;
    sint numNodes = 0;

    while(i) {
        numNodes++;
        i = i->next;
    }

    return numNodes;
}

/*
===============
idCGameTrails::PrependBeamNode

Prepend a new beam node to the front of a beam
Returns the new node
===============
*/
trailBeamNode_t *idCGameTrails::PrependBeamNode(trailBeam_t *tb) {
    trailBeamNode_t *i;

    if(tb->nodes) {
        // prepend another node
        i = AllocateBeamNode(tb);

        if(i) {
            i->next = tb->nodes;
            tb->nodes->prev = i;
            tb->nodes = i;
        }
    } else { //add first node
        i = AllocateBeamNode(tb);

        if(i) {
            tb->nodes = i;
        }
    }

    return i;
}

/*
===============
idCGameTrails::AppendBeamNode

Append a new beam node to the back of a beam
Returns the new node
===============
*/
trailBeamNode_t *idCGameTrails::AppendBeamNode(trailBeam_t *tb) {
    trailBeamNode_t *last, *i;

    if(tb->nodes) {
        // append another node
        last = FindLastBeamNode(tb);
        i = AllocateBeamNode(tb);

        if(i) {
            last->next = i;
            i->prev = last;
            i->next = nullptr;
        }
    } else { //add first node
        i = AllocateBeamNode(tb);

        if(i) {
            tb->nodes = i;
        }
    }

    return i;
}

/*
===============
idCGameTrails::ApplyJitters
===============
*/
void idCGameTrails::ApplyJitters(trailBeam_t *tb) {
    trailBeamNode_t *i = nullptr;
    sint j;
    baseTrailBeam_t *btb;
    trailSystem_t *ts;
    trailBeamNode_t *start;
    trailBeamNode_t *end;

    if(!tb || !tb->nodes) {
        return;
    }

    btb = tb->_class;
    ts = tb->parent;

    for(j = 0; j < btb->numJitters; j++) {
        if(tb->nextJitterTimes[ j ] <= cg.time) {
            for(i = tb->nodes; i; i = i->next) {
                i->jitters[ j ][ 0 ] = (crandom() * btb->jitters[ j ].magnitude);
                i->jitters[ j ][ 1 ] = (crandom() * btb->jitters[ j ].magnitude);
            }

            tb->nextJitterTimes[ j ] = cg.time + btb->jitters[ j ].period;
        }
    }

    start = tb->nodes;
    end = FindLastBeamNode(tb);

    if(!btb->jitterAttachments) {
        if(idCGameAttachment::Attached(&ts->frontAttachment) && start->next) {
            start = start->next;
        }

        if(idCGameAttachment::Attached(&ts->backAttachment) && end->prev) {
            end = end->prev;
        }
    }

    for(i = start; i; i = i->next) {
        vec3_t forward, right, up;
        trailBeamNode_t *prev;
        trailBeamNode_t *next;
        float32 upJitter = 0.0f, rightJitter = 0.0f;

        prev = i->prev;
        next = i->next;

        if(prev && next) {
            //this node has two neighbours
            GetPerpendicularViewVector(cg.refdef.vieworg, next->position,
                                       prev->position, up);
            VectorSubtract(next->position, prev->position, forward);
        } else if(!prev && next) {
            //this is the front
            GetPerpendicularViewVector(cg.refdef.vieworg, next->position, i->position,
                                       up);
            VectorSubtract(next->position, i->position, forward);
        } else if(prev && !next) {
            //this is the back
            GetPerpendicularViewVector(cg.refdef.vieworg, i->position, prev->position,
                                       up);
            VectorSubtract(i->position, prev->position, forward);
        }

        VectorNormalize(forward);
        CrossProduct(forward, up, right);
        VectorNormalize(right);

        for(j = 0; j < btb->numJitters; j++) {
            upJitter += i->jitters[ j ][ 0 ];
            rightJitter += i->jitters[ j ][ 1 ];
        }

        VectorMA(i->position, upJitter, up, i->position);
        VectorMA(i->position, rightJitter, right, i->position);

        if(i == end) {
            break;
        }
    }
}

/*
===============
idCGameTrails::UpdateBeam

Updates a beam
===============
*/
void idCGameTrails::UpdateBeam(trailBeam_t *tb) {
    baseTrailBeam_t *btb;
    trailSystem_t *ts;
    trailBeamNode_t *i;
    sint deltaTime;
    sint nodesToAdd;
    sint j;
    sint numNodes;

    if(!tb) {
        return;
    }

    btb = tb->_class;
    ts = tb->parent;

    deltaTime = cg.time - tb->lastEvalTime;
    tb->lastEvalTime = cg.time;

    // first make sure this beam has enough nodes
    if(ts->destroyTime <= 0) {
        nodesToAdd = btb->numSegments - CountBeamNodes(tb) + 1;

        while(nodesToAdd--) {
            i = AppendBeamNode(tb);

            if(!tb->nodes->next && idCGameAttachment::Attached(&ts->frontAttachment)) {
                // this is the first node to be added
                if(!idCGameAttachment::AttachmentPoint(&ts->frontAttachment,
                                                       i->refPosition)) {
                    DestroyTrailSystem(&ts);
                }
            } else if(i->prev) {
                VectorCopy(i->prev->refPosition, i->refPosition);
            }
        }
    }

    numNodes = CountBeamNodes(tb);

    for(i = tb->nodes; i; i = i->next) {
        VectorCopy(i->refPosition, i->position);
    }

    if(idCGameAttachment::Attached(&ts->frontAttachment) &&
            idCGameAttachment::Attached(&ts->backAttachment)) {
        // beam between two attachments
        vec3_t dir, front, back;

        if(ts->destroyTime > 0 &&
                (cg.time - ts->destroyTime) >= btb->fadeOutTime) {
            tb->valid = false;
            return;
        }

        if(!idCGameAttachment::AttachmentPoint(&ts->frontAttachment, front)) {
            DestroyTrailSystem(&ts);
        }

        if(!idCGameAttachment::AttachmentPoint(&ts->backAttachment, back)) {
            DestroyTrailSystem(&ts);
        }

        VectorSubtract(back, front, dir);

        for(j = 0, i = tb->nodes; i; i = i->next, j++) {
            float32 scale = (float32)j / (float32)(numNodes - 1);

            VectorMA(front, scale, dir, i->position);
        }
    } else if(idCGameAttachment::Attached(&ts->frontAttachment)) {
        // beam from one attachment

        // cull the trail tail
        i = FindLastBeamNode(tb);

        if(i && i->timeLeft >= 0) {
            i->timeLeft -= deltaTime;

            if(i->timeLeft < 0) {
                tb->nodes = DestroyBeamNode(i);

                if(!tb->nodes) {
                    tb->valid = false;
                    return;
                }

                // if the ts has been destroyed, stop creating new nodes
                if(ts->destroyTime <= 0) {
                    PrependBeamNode(tb);
                }
            } else if(i->timeLeft >= 0 && i->prev) {
                vec3_t dir;
                float32 length;

                VectorSubtract(i->refPosition, i->prev->refPosition, dir);
                length = VectorNormalize(dir) * ((float32)i->timeLeft /
                                                 (float32)tb->_class->segmentTime);

                VectorMA(i->prev->refPosition, length, dir, i->position);
            }
        }

        if(tb->nodes) {
            if(!idCGameAttachment::AttachmentPoint(&ts->frontAttachment,
                                                   tb->nodes->refPosition)) {
                DestroyTrailSystem(&ts);
            }

            VectorCopy(tb->nodes->refPosition, tb->nodes->position);
        }
    }

    ApplyJitters(tb);
}

/*
===============
idCGameTrails::ParseTrailBeamColor
===============
*/
bool idCGameTrails::ParseTrailBeamColor(uchar8 *c, valueType **text_p) {
    valueType *token;
    sint i;

    for(i = 0; i <= 2; i++) {
        token = COM_Parse(text_p);

        if(!Q_stricmp(token, "")) {
            return false;
        }

        c[ i ] = (sint)((float32)0xFF * bggame->atof_neg(token, false));
    }

    return true;
}

/*
===============
idCGameTrails::ParseTrailBeam

Parse a trail beam
===============
*/
bool idCGameTrails::ParseTrailBeam(baseTrailBeam_t *btb,
                                   valueType **text_p) {
    valueType *token;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "segments")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->numSegments = bggame->atoi_neg(token, false);

            if(btb->numSegments >= MAX_TRAIL_BEAM_NODES) {
                btb->numSegments = MAX_TRAIL_BEAM_NODES - 1;
                Printf(S_COLOR_YELLOW "WARNING: too many segments in trail beam\n");
            }

            continue;
        } else if(!Q_stricmp(token, "width")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->frontWidth = bggame->atof_neg(token, false);

            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                btb->backWidth = btb->frontWidth;
            } else {
                btb->backWidth = bggame->atof_neg(token, false);
            }

            continue;
        } else if(!Q_stricmp(token, "alpha")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->frontAlpha = bggame->atof_neg(token, false);

            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                btb->backAlpha = btb->frontAlpha;
            } else {
                btb->backAlpha = bggame->atof_neg(token, false);
            }

            continue;
        } else if(!Q_stricmp(token, "color")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            if(!Q_stricmp(token, "{")) {
                if(!ParseTrailBeamColor(btb->frontColor, text_p)) {
                    break;
                }

                token = COM_Parse(text_p);

                if(Q_stricmp(token, "}")) {
                    Printf(S_COLOR_RED "ERROR: missing '}'\n");
                    break;
                }

                token = COM_Parse(text_p);

                if(!Q_stricmp(token, "")) {
                    break;
                }

                if(!Q_stricmp(token, "-")) {
                    btb->backColor[ 0 ] = btb->frontColor[ 0 ];
                    btb->backColor[ 1 ] = btb->frontColor[ 1 ];
                    btb->backColor[ 2 ] = btb->frontColor[ 2 ];
                } else if(!Q_stricmp(token, "{")) {
                    if(!ParseTrailBeamColor(btb->backColor, text_p)) {
                        break;
                    }

                    token = COM_Parse(text_p);

                    if(Q_stricmp(token, "}")) {
                        Printf(S_COLOR_RED "ERROR: missing '}'\n");
                        break;
                    }
                } else {
                    Printf(S_COLOR_RED "ERROR: missing '{'\n");
                    break;
                }
            } else {
                Printf(S_COLOR_RED "ERROR: missing '{'\n");
                break;
            }

            continue;
        } else if(!Q_stricmp(token, "segmentTime")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->segmentTime = bggame->atoi_neg(token, false);
            continue;
        } else if(!Q_stricmp(token, "fadeOutTime")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->fadeOutTime = bggame->atoi_neg(token, false);
            continue;
        } else if(!Q_stricmp(token, "shader")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            Q_strncpyz(btb->shaderName, token, MAX_QPATH);

            continue;
        } else if(!Q_stricmp(token, "textureType")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            if(!Q_stricmp(token, "stretch")) {
                btb->textureType = TBTT_STRETCH;

                token = COM_Parse(text_p);

                if(!Q_stricmp(token, "")) {
                    break;
                }

                btb->frontTextureCoord = bggame->atof_neg(token, false);

                token = COM_Parse(text_p);

                if(!Q_stricmp(token, "")) {
                    break;
                }

                btb->backTextureCoord = bggame->atof_neg(token, false);
            } else if(!Q_stricmp(token, "repeat")) {
                btb->textureType = TBTT_REPEAT;

                token = COM_Parse(text_p);

                if(!Q_stricmp(token, "")) {
                    break;
                }

                if(!Q_stricmp(token, "front")) {
                    btb->clampToBack = false;
                } else if(!Q_stricmp(token, "back")) {
                    btb->clampToBack = true;
                } else {
                    Printf(S_COLOR_RED "ERROR: unknown textureType clamp \"%s\"\n", token);
                    break;
                }

                token = COM_Parse(text_p);

                if(!Q_stricmp(token, "")) {
                    break;
                }

                btb->repeatLength = bggame->atof_neg(token, false);
            } else {
                Printf(S_COLOR_RED "ERROR: unknown textureType \"%s\"\n", token);
                break;
            }

            continue;
        } else if(!Q_stricmp(token, "realLight")) {
            btb->realLight = true;

            continue;
        } else if(!Q_stricmp(token, "jitter")) {
            if(btb->numJitters == MAX_TRAIL_BEAM_JITTERS) {
                Printf(S_COLOR_RED "ERROR: too many jitters\n", token);
                break;
            }

            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->jitters[ btb->numJitters ].magnitude = bggame->atof_neg(token, false);

            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            btb->jitters[ btb->numJitters ].period = bggame->atoi_neg(token, false);

            btb->numJitters++;

            continue;
        } else if(!Q_stricmp(token, "jitterAttachments")) {
            btb->jitterAttachments = true;

            continue;
        } else if(!Q_stricmp(token, "}")) {
            return true; //reached the end of this trail beam
        } else {
            Printf(S_COLOR_RED "ERROR: unknown token '%s' in trail beam\n", token);
            return false;
        }
    }

    return false;
}

/*
===============
idCGameTrails::InitialiseBaseTrailBeam
===============
*/
void idCGameTrails::InitialiseBaseTrailBeam(baseTrailBeam_t *btb) {
    ::memset(btb, 0, sizeof(baseTrailBeam_t));

    btb->numSegments = 1;
    btb->frontWidth = btb->backWidth = 1.0f;
    btb->frontAlpha = btb->backAlpha = 1.0f;

    ::memset(btb->frontColor, 0xFF, sizeof(btb->frontColor));
    ::memset(btb->backColor, 0xFF, sizeof(btb->backColor));

    btb->segmentTime = 100;

    btb->textureType = TBTT_STRETCH;
    btb->frontTextureCoord = 0.0f;
    btb->backTextureCoord = 1.0f;
}

/*
===============
idCGameTrails::ParseTrailSystem

Parse a trail system section
===============
*/
bool idCGameTrails::ParseTrailSystem(baseTrailSystem_t *bts,
                                     valueType **text_p, pointer name) {
    valueType *token;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "{")) {
            InitialiseBaseTrailBeam(&baseTrailBeams[ numBaseTrailBeams ]);

            if(!ParseTrailBeam(&baseTrailBeams[ numBaseTrailBeams ], text_p)) {
                Printf(S_COLOR_RED "ERROR: failed to parse trail beam\n");
                return false;
            }

            if(bts->numBeams == MAX_BEAMS_PER_SYSTEM) {
                Printf(S_COLOR_RED "ERROR: trail system has > %d beams\n",
                       MAX_BEAMS_PER_SYSTEM);
                return false;
            } else if(numBaseTrailBeams == MAX_BASETRAIL_BEAMS) {
                Printf(S_COLOR_RED "ERROR: maximum number of trail beams (%d) reached\n",
                       MAX_BASETRAIL_BEAMS);
                return false;
            } else {
                //start parsing beams again
                bts->beams[ bts->numBeams ] = &baseTrailBeams[ numBaseTrailBeams ];
                bts->numBeams++;
                numBaseTrailBeams++;
            }

            continue;
        } else if(!Q_stricmp(token, "thirdPersonOnly")) {
            bts->thirdPersonOnly = true;
        } else if(!Q_stricmp(token, "lifeTime")) {
            token = COM_Parse(text_p);

            if(!Q_stricmp(token, "")) {
                break;
            }

            bts->lifeTime = bggame->atoi_neg(token, false);
            continue;
        } else if(!Q_stricmp(token, "beam")) {  //acceptable text
            continue;
        } else if(!Q_stricmp(token, "}")) {
            if(cg_debugTrails.integer >= 1) {
                Printf("Parsed trail system %s\n", name);
            }

            return true; //reached the end of this trail system
        } else {
            Printf(S_COLOR_RED "ERROR: unknown token '%s' in trail system %s\n", token,
                   bts->name);
            return false;
        }
    }

    return false;
}

/*
===============
idCGameTrails::ParseTrailFile

Load the trail systems from a trail file
===============
*/
bool idCGameTrails::ParseTrailFile(pointer fileName) {
    valueType *text_p;
    sint i, len;
    valueType *token;
    valueType text[ 32000 ];
    valueType tsName[ MAX_QPATH ];
    bool tsNameSet = false;
    fileHandle_t f;

    // load the file
    len = trap_FS_FOpenFile(fileName, &f, FS_READ);

    if(len <= 0) {
        return false;
    }

    if(len == 0 || len >= sizeof(text) - 1) {
        trap_FS_FCloseFile(f);
        Printf(S_COLOR_RED "ERROR: trail file %s is %s\n", fileName,
               len == 0 ? "empty" : "too long");
        return false;
    }

    trap_FS_Read(text, len, f);
    text[ len ] = 0;
    trap_FS_FCloseFile(f);

    // parse the text
    text_p = text;

    // read optional parameters
    while(1) {
        token = COM_Parse(&text_p);

        if(!Q_stricmp(token, "")) {
            break;
        }

        if(!Q_stricmp(token, "{")) {
            if(tsNameSet) {
                //check for name space clashes
                for(i = 0; i < numBaseTrailSystems; i++) {
                    if(!Q_stricmp(baseTrailSystems[ i ].name, tsName)) {
                        Printf(S_COLOR_RED "ERROR: a trail system is already named %s\n", tsName);
                        return false;
                    }
                }

                Q_strncpyz(baseTrailSystems[ numBaseTrailSystems ].name, tsName,
                           MAX_QPATH);

                if(!ParseTrailSystem(&baseTrailSystems[ numBaseTrailSystems ], &text_p,
                                     tsName)) {
                    Printf(S_COLOR_RED "ERROR: %s: failed to parse trail system %s\n",
                           fileName, tsName);
                    return false;
                }

                //start parsing trail systems again
                tsNameSet = false;

                if(numBaseTrailSystems == MAX_BASETRAIL_SYSTEMS) {
                    Printf(S_COLOR_RED "ERROR: maximum number of trail systems (%d) reached\n",
                           MAX_BASETRAIL_SYSTEMS);
                    return false;
                } else {
                    numBaseTrailSystems++;
                }

                continue;
            } else {
                Printf(S_COLOR_RED "ERROR: unamed trail system\n");
                return false;
            }
        }

        if(!tsNameSet) {
            Q_strncpyz(tsName, token, sizeof(tsName));
            tsNameSet = true;
        } else {
            Printf(S_COLOR_RED "ERROR: trail system already named\n");
            return false;
        }
    }

    return true;
}

/*
===============
idCGameTrails::LoadTrailSystems

Load trail system templates
===============
*/
void idCGameTrails::LoadTrailSystems(void) {
    sint i, numFiles, fileLen;
    valueType fileList[ MAX_TRAIL_FILES * MAX_QPATH ];
    valueType fileName[ MAX_QPATH ];
    valueType *filePtr;

    //clear out the old
    numBaseTrailSystems = 0;
    numBaseTrailBeams = 0;

    for(i = 0; i < MAX_BASETRAIL_SYSTEMS; i++) {
        baseTrailSystem_t  *bts = &baseTrailSystems[ i ];
        ::memset(bts, 0, sizeof(baseTrailSystem_t));
    }

    for(i = 0; i < MAX_BASETRAIL_BEAMS; i++) {
        baseTrailBeam_t  *btb = &baseTrailBeams[ i ];
        ::memset(btb, 0, sizeof(baseTrailBeam_t));
    }

    //and bring in the new
    numFiles = trap_FS_GetFileList("scripts", ".trail", fileList,
                                   MAX_TRAIL_FILES * MAX_QPATH);
    filePtr = fileList;

    for(i = 0; i < numFiles; i++, filePtr += fileLen + 1) {
        fileLen = strlen(filePtr);
        strcpy(fileName, "scripts/");
        strcat(fileName, filePtr);
        //Printf( "...loading '%s'\n", fileName );
        ParseTrailFile(fileName);
    }
}

/*
===============
idCGameTrails::RegisterTrailSystem

Load the media that a trail system needs
===============
*/
qhandle_t idCGameTrails::RegisterTrailSystem(valueType *name) {
    sint i, j;
    baseTrailSystem_t *bts;
    baseTrailBeam_t *btb;

    for(i = 0; i < MAX_BASETRAIL_SYSTEMS; i++) {
        bts = &baseTrailSystems[ i ];

        if(!Q_stricmp(bts->name, name)) {
            //already registered
            if(bts->registered) {
                return i + 1;
            }

            for(j = 0; j < bts->numBeams; j++) {
                btb = bts->beams[ j ];

                btb->shader = trap_R_RegisterShader(btb->shaderName);
            }

            if(cg_debugTrails.integer >= 1) {
                Printf("Registered trail system %s\n", name);
            }

            bts->registered = true;

            //avoid returning 0
            return i + 1;
        }
    }

    Printf(S_COLOR_RED "ERROR: failed to register trail system %s\n", name);
    return 0;
}


/*
===============
idCGameTrails::SpawnNewTrailBeam

Allocate a new trail beam
===============
*/
trailBeam_t *idCGameTrails::SpawnNewTrailBeam(baseTrailBeam_t *btb,
        trailSystem_t *parent) {
    sint i;
    trailBeam_t *tb = nullptr;
    trailSystem_t *ts = parent;

    for(i = 0; i < MAX_TRAIL_BEAMS; i++) {
        tb = &trailBeams[ i ];

        if(!tb->valid) {
            ::memset(tb, 0, sizeof(trailBeam_t));

            //found a free slot
            tb->_class = btb;
            tb->parent = ts;

            tb->valid = true;

            if(cg_debugTrails.integer >= 1) {
                Printf("TB %s created\n", ts->_class->name);
            }

            break;
        }
    }

    return tb;
}


/*
===============
idCGameTrails::SpawnNewTrailSystem

Spawns a new trail system
===============
*/
trailSystem_t *idCGameTrails::SpawnNewTrailSystem(qhandle_t psHandle) {
    sint i, j;
    trailSystem_t *ts = nullptr;
    baseTrailSystem_t *bts = &baseTrailSystems[ psHandle - 1 ];

    if(!bts->registered) {
        Printf(S_COLOR_RED "ERROR: a trail system has not been registered yet\n");
        return nullptr;
    }

    for(i = 0; i < MAX_TRAIL_SYSTEMS; i++) {
        ts = &trailSystems[ i ];

        if(!ts->valid) {
            ::memset(ts, 0, sizeof(trailSystem_t));

            //found a free slot
            ts->_class = bts;

            ts->valid = true;
            ts->destroyTime = -1;
            ts->birthTime = cg.time;

            for(j = 0; j < bts->numBeams; j++) {
                SpawnNewTrailBeam(bts->beams[j], ts);
            }

            if(cg_debugTrails.integer >= 1) {
                Printf("TS %s created\n", bts->name);
            }

            break;
        }
    }

    return ts;
}

/*
===============
idCGameTrails::DestroyTrailSystem

Destroy a trail system
===============
*/
void idCGameTrails::DestroyTrailSystem(trailSystem_t **ts) {
    (*ts)->destroyTime = cg.time;

    if(idCGameAttachment::Attached(&(*ts)->frontAttachment) &&
            !idCGameAttachment::Attached(&(*ts)->backAttachment)) {
        vec3_t v;

        // attach the trail head to a static point
        idCGameAttachment::AttachmentPoint(&(*ts)->frontAttachment, v);
        idCGameAttachment::SetAttachmentPoint(&(*ts)->frontAttachment, v);
        idCGameAttachment::AttachToPoint(&(*ts)->frontAttachment);

        (*ts)->frontAttachment.centValid = false;   // a bit naughty
    }

    ts = nullptr;
}

/*
===============
idCGameTrails::IsTrailSystemValid

Test a trail system for validity
===============
*/
bool idCGameTrails::IsTrailSystemValid(trailSystem_t **ts) {
    if(*ts == nullptr || (*ts && !(*ts)->valid)) {
        if(*ts && !(*ts)->valid) {
            *ts = nullptr;
        }

        return false;
    }

    return true;
}

/*
===============
idCGameTrails::GarbageCollectTrailSystems

Destroy inactive trail systems
===============
*/
void idCGameTrails::GarbageCollectTrailSystems(void) {
    sint i, j, count;
    trailSystem_t *ts;
    trailBeam_t *tb;
    sint centNum;

    for(i = 0; i < MAX_TRAIL_SYSTEMS; i++) {
        ts = &trailSystems[ i ];
        count = 0;

        //don't bother checking already invalid systems
        if(!ts->valid) {
            continue;
        }

        for(j = 0; j < MAX_TRAIL_BEAMS; j++) {
            tb = &trailBeams[ j ];

            if(tb->valid && tb->parent == ts) {
                count++;
            }
        }

        if(!count) {
            ts->valid = false;
        }

        //check systems where the parent cent has left the PVS
        //( local player entity is always valid )
        if((centNum = idCGameAttachment::AttachmentCentNum(&ts->frontAttachment))
                >= 0 && centNum != cg.snap->ps.clientNum) {
            trailSystem_t *tempTS = ts;

            if(!cg_entities[centNum].valid) {
                DestroyTrailSystem(&tempTS);
            }
        }

        if((centNum = idCGameAttachment::AttachmentCentNum(&ts->backAttachment)) >=
                0 && centNum != cg.snap->ps.clientNum) {
            trailSystem_t *tempTS = ts;

            if(!cg_entities[centNum].valid) {
                DestroyTrailSystem(&tempTS);
            }
        }

        // lifetime expired
        if(ts->destroyTime <= 0 && ts->_class->lifeTime &&
                ts->birthTime + ts->_class->lifeTime < cg.time) {
            trailSystem_t *tempTS = ts;

            DestroyTrailSystem(&tempTS);

            if(cg_debugTrails.integer >= 1) {
                Printf("TS %s expired (born %d, lives %d, now %d)\n", ts->_class->name,
                       ts->birthTime, ts->_class->lifeTime, cg.time);
            }
        }

        if(cg_debugTrails.integer >= 1 && !ts->valid) {
            Printf("TS %s garbage collected\n", ts->_class->name);
        }
    }
}

/*
===============
idCGameTrails::AddTrails

Add trails to the scene
===============
*/
void idCGameTrails::AddTrails(void) {
    sint i;
    trailBeam_t *tb;
    sint numTS = 0, numTB = 0;

    //remove expired trail systems
    GarbageCollectTrailSystems();

    for(i = 0; i < MAX_TRAIL_BEAMS; i++) {
        tb = &trailBeams[ i ];

        if(tb->valid) {
            UpdateBeam(tb);
            RenderBeam(tb);
        }
    }

    if(cg_debugTrails.integer >= 2) {
        for(i = 0; i < MAX_TRAIL_SYSTEMS; i++) {
            if(trailSystems[i].valid) {
                numTS++;
            }
        }

        for(i = 0; i < MAX_TRAIL_BEAMS; i++) {
            if(trailBeams[i].valid) {
                numTB++;
            }
        }

        Printf("TS: %d  TB: %d\n", numTS, numTB);
    }
}

static trailSystem_t *testTS;
static qhandle_t testTSHandle;

/*
===============
idCGameTrails::DestroyTestTS_f

Destroy the test a trail system
===============
*/
void idCGameTrails::DestroyTestTS_f(void) {
    if(IsTrailSystemValid(&testTS)) {
        DestroyTrailSystem(&testTS);
    }
}

/*
===============
idCGameTrails::TestTS_f

Test a trail system
===============
*/
void idCGameTrails::TestTS_f(void) {
    valueType tsName[ MAX_QPATH ];

    if(trap_Argc() < 2) {
        return;
    }

    Q_strncpyz(tsName, idCGameMain::Argv(1), MAX_QPATH);
    testTSHandle = RegisterTrailSystem(tsName);

    if(testTSHandle) {
        DestroyTestTS_f();

        testTS = SpawnNewTrailSystem(testTSHandle);

        if(IsTrailSystemValid(&testTS)) {
            idCGameAttachment::SetAttachmentCent(&testTS->frontAttachment,
                                                 &cg_entities[ 0 ]);
            idCGameAttachment::AttachToCent(&testTS->frontAttachment);
        }
    }
}
