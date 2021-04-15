////////////////////////////////////////////////////////////////////////////////////////
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
// File name:   cgame_particles.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: the particle system
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

static baseParticleSystem_t
baseParticleSystems[ MAX_BASEPARTICLE_SYSTEMS ];
static baseParticleEjector_t
baseParticleEjectors[ MAX_BASEPARTICLE_EJECTORS ];
static baseParticle_t baseParticles[ MAX_BASEPARTICLES ];
static sint numBaseParticleSystems = 0;
static sint numBaseParticleEjectors = 0;
static sint numBaseParticles = 0;

static particleSystem_t particleSystems[ MAX_PARTICLE_SYSTEMS ];
static particleEjector_t particleEjectors[ MAX_PARTICLE_EJECTORS ];
static particle_t particles[ MAX_PARTICLES ];
static particle_t *sortedParticles[ MAX_PARTICLES ];
static particle_t *radixBuffer[ MAX_PARTICLES ];

/*
===============
idCGameParticles::idCGameParticles
===============
*/
idCGameParticles::idCGameParticles(void) {
}

/*
===============
idCGameParticles::~idCGameParticles
===============
*/
idCGameParticles::~idCGameParticles(void) {
}

/*
===============
idCGameParticles::LerpValues

Lerp between two values
===============
*/
float32 idCGameParticles::LerpValues(float32 a, float32 b, float32 f) {
    if(b == PARTICLES_SAME_AS_INITIAL) {
        return a;
    } else {
        return ((a) + (f) * ((b) - (a)));
    }
}

/*
===============
idCGameParticles::RandomiseValue

Randomise some value by some variance
===============
*/
float32 idCGameParticles::RandomiseValue(float32 value, float32 variance) {
    if(value != 0.0f) {
        return value * (1.0f + (random() * variance));
    } else {
        return random() * variance;
    }
}

/*
===============
idCGameParticles::SpreadVector

Randomly spread a vector by some amount
===============
*/
void idCGameParticles::SpreadVector(vec3_t v, float32 spread) {
    vec3_t p, r1, r2;
    float32 randomSpread = crandom() * spread;
    float32 randomRotation = random() * 360.0f;

    PerpendicularVector(p, v);

    RotatePointAroundVector(r1, p, v, randomSpread);
    RotatePointAroundVector(r2, v, r1, randomRotation);

    VectorCopy(r2, v);
}

/*
===============
idCGameParticles::DestroyParticle

Destroy an individual particle
===============
*/
void idCGameParticles::DestroyParticle(particle_t *p,
                                       vec3_t impactNormal) {
    //this particle has an onDeath particle system attached
    if(p->_class->onDeathSystemName[ 0 ] != '\0') {
        particleSystem_t  *ps;

        ps = SpawnNewParticleSystem(p->_class->onDeathSystemHandle);

        if(IsParticleSystemValid(&ps)) {
            if(impactNormal) {
                SetParticleSystemNormal(ps, impactNormal);
            }

            idCGameAttachment::SetAttachmentPoint(&ps->attachment, p->origin);
            idCGameAttachment::AttachToPoint(&ps->attachment);
        }
    }

    p->valid = false;

    //this gives other systems a couple of
    //frames to realise the particle is gone
    p->frameWhenInvalidated = cg.clientFrame;
}

/*
===============
idCGameParticles::SpawnNewParticle

Introduce a new particle into the world
===============
*/
particle_t *idCGameParticles::SpawnNewParticle(baseParticle_t *bp,
        particleEjector_t *parent) {
    sint i, j;
    particle_t *p = nullptr;
    particleEjector_t *pe = parent;
    particleSystem_t *ps = parent->parent;
    vec3_t attachmentPoint, attachmentVelocity, transform[ 3 ];

    for(i = 0; i < MAX_PARTICLES; i++) {
        p = &particles[ i ];

        //FIXME: the + 1 may be unnecessary
        if(!p->valid && cg.clientFrame > p->frameWhenInvalidated + 1) {
            ::memset(p, 0, sizeof(particle_t));

            //found a free slot
            p->_class = bp;
            p->parent = pe;

            p->birthTime = cg.time;
            p->lifeTime = (sint)RandomiseValue((float32)bp->lifeTime,
                                               bp->lifeTimeRandFrac);

            p->radius.delay = (sint)RandomiseValue((float32)bp->radius.delay,
                                                   bp->radius.delayRandFrac);
            p->radius.initial = RandomiseValue(bp->radius.initial,
                                               bp->radius.initialRandFrac);
            p->radius.final = RandomiseValue(bp->radius.final,
                                             bp->radius.finalRandFrac);

            p->radius.initial += bp->scaleWithCharge * pe->parent->charge;

            p->alpha.delay = (sint)RandomiseValue((float32)bp->alpha.delay,
                                                  bp->alpha.delayRandFrac);
            p->alpha.initial = RandomiseValue(bp->alpha.initial,
                                              bp->alpha.initialRandFrac);
            p->alpha.final = RandomiseValue(bp->alpha.final, bp->alpha.finalRandFrac);

            p->rotation.delay = (sint)RandomiseValue((float32)bp->rotation.delay,
                                bp->rotation.delayRandFrac);
            p->rotation.initial = RandomiseValue(bp->rotation.initial,
                                                 bp->rotation.initialRandFrac);
            p->rotation.final = RandomiseValue(bp->rotation.final,
                                               bp->rotation.finalRandFrac);

            p->dLightRadius.delay = (sint)RandomiseValue((float32)
                                    bp->dLightRadius.delay, bp->dLightRadius.delayRandFrac);
            p->dLightRadius.initial = RandomiseValue(bp->dLightRadius.initial,
                                      bp->dLightRadius.initialRandFrac);
            p->dLightRadius.final = RandomiseValue(bp->dLightRadius.final,
                                                   bp->dLightRadius.finalRandFrac);

            p->colorDelay = RandomiseValue(bp->colorDelay, bp->colorDelayRandFrac);

            p->bounceMarkRadius = RandomiseValue(bp->bounceMarkRadius,
                                                 bp->bounceMarkRadiusRandFrac);
            p->bounceMarkCount = rint(RandomiseValue((float32)bp->bounceMarkCount,
                                      bp->bounceMarkCountRandFrac));
            p->bounceSoundCount = rint(RandomiseValue((float32)bp->bounceSoundCount,
                                       bp->bounceSoundCountRandFrac));

            if(bp->numModels) {
                p->model = bp->models[ rand() % bp->numModels ];

                if(bp->modelAnimation.frameLerp < 0) {
                    bp->modelAnimation.frameLerp = p->lifeTime / bp->modelAnimation.numFrames;
                    bp->modelAnimation.initialLerp = p->lifeTime /
                                                     bp->modelAnimation.numFrames;
                }
            }

            if(!idCGameAttachment::AttachmentPoint(&ps->attachment, attachmentPoint)) {
                return nullptr;
            }

            VectorCopy(attachmentPoint, p->origin);

            if(idCGameAttachment::AttachmentAxis(&ps->attachment, transform)) {
                vec3_t  transDisplacement;

                VectorMatrixMultiply(bp->displacement, transform, transDisplacement);
                VectorAdd(p->origin, transDisplacement, p->origin);
            } else {
                VectorAdd(p->origin, bp->displacement, p->origin);
            }

            for(j = 0; j <= 2; j++) {
                p->origin[j] += (crandom() * bp->randDisplacement);
            }

            switch(bp->velMoveType) {
                case PMT_STATIC:
                    if(bp->velMoveValues.dirType == PMD_POINT) {
                        VectorSubtract(bp->velMoveValues.point, p->origin, p->velocity);
                    } else if(bp->velMoveValues.dirType == PMD_LINEAR) {
                        VectorCopy(bp->velMoveValues.dir, p->velocity);
                    }

                    break;

                case PMT_STATIC_TRANSFORM:
                    if(!idCGameAttachment::AttachmentAxis(&ps->attachment, transform)) {
                        return nullptr;
                    }

                    if(bp->velMoveValues.dirType == PMD_POINT) {
                        vec3_t transPoint;

                        VectorMatrixMultiply(bp->velMoveValues.point, transform, transPoint);
                        VectorSubtract(transPoint, p->origin, p->velocity);
                    } else if(bp->velMoveValues.dirType == PMD_LINEAR) {
                        VectorMatrixMultiply(bp->velMoveValues.dir, transform, p->velocity);
                    }

                    break;

                case PMT_TAG:
                case PMT_CENT_ANGLES:
                    if(bp->velMoveValues.dirType == PMD_POINT) {
                        VectorSubtract(attachmentPoint, p->origin, p->velocity);
                    } else if(bp->velMoveValues.dirType == PMD_LINEAR) {
                        if(!idCGameAttachment::AttachmentDir(&ps->attachment, p->velocity)) {
                            return nullptr;
                        }
                    }

                    break;

                case PMT_NORMAL:
                    if(!ps->normalValid) {
                        Printf(S_COLOR_RED
                               "ERROR: a particle with velocityType normal has no normal\n");
                        return nullptr;
                    }

                    VectorCopy(ps->normal, p->velocity);

                    //normal displacement
                    VectorNormalize(p->velocity);
                    VectorMA(p->origin, bp->normalDisplacement, p->velocity, p->origin);
                    break;
            }

            VectorNormalize(p->velocity);
            SpreadVector(p->velocity, bp->velMoveValues.dirRandAngle);
            VectorScale(p->velocity, RandomiseValue(bp->velMoveValues.mag,
                                                    bp->velMoveValues.magRandFrac), p->velocity);

            if(idCGameAttachment::AttachmentVelocity(&ps->attachment,
                    attachmentVelocity)) {
                VectorMA(p->velocity, RandomiseValue(bp->velMoveValues.parentVelFrac,
                                                     bp->velMoveValues.parentVelFracRandFrac), attachmentVelocity, p->velocity);
            }

            p->lastEvalTime = cg.time;

            p->valid = true;

            //this particle has a child particle system attached
            if(bp->childSystemName[ 0 ] != '\0') {
                particleSystem_t  *ps = SpawnNewParticleSystem(bp->childSystemHandle);

                if(IsParticleSystemValid(&ps)) {
                    idCGameAttachment::SetAttachmentParticle(&ps->attachment, p);
                    idCGameAttachment::AttachToParticle(&ps->attachment);
                }
            }

            //this particle has a child trail system attached
            if(bp->childTrailSystemName[ 0 ] != '\0') {
                trailSystem_t *ts = idCGameTrails::SpawnNewTrailSystem(
                                        bp->childTrailSystemHandle);

                if(idCGameTrails::IsTrailSystemValid(&ts)) {
                    idCGameAttachment::SetAttachmentParticle(&ts->frontAttachment, p);
                    idCGameAttachment::AttachToParticle(&ts->frontAttachment);
                }
            }

            break;
        }
    }

    return p;
}


/*
===============
idCGameParticles::SpawnNewParticles

Check if there are any ejectors that should be
introducing new particles
===============
*/
void idCGameParticles::SpawnNewParticles(void) {
    sint i, j;
    particle_t *p;
    particleSystem_t *ps;
    particleEjector_t *pe;
    baseParticleEjector_t *bpe;
    float32 lerpFrac;
    sint count;

    for(i = 0; i < MAX_PARTICLE_EJECTORS; i++) {
        pe = &particleEjectors[ i ];
        ps = pe->parent;

        if(pe->valid) {
            //a non attached particle system can't make particles
            if(!idCGameAttachment::Attached(&ps->attachment)) {
                continue;
            }

            bpe = particleEjectors[ i ]._class;

            //if this system is scheduled for removal don't make any new particles
            if(!ps->lazyRemove) {
                while(pe->nextEjectionTime <= cg.time && (pe->count > 0 ||
                        pe->totalParticles == PARTICLES_INFINITE)) {
                    for(j = 0; j < bpe->numParticles; j++) {
                        SpawnNewParticle(bpe->particles[j], pe);
                    }

                    if(pe->count > 0) {
                        pe->count--;
                    }

                    //calculate next ejection time
                    lerpFrac = 1.0 - ((float32)pe->count / (float32)pe->totalParticles);
                    pe->nextEjectionTime = cg.time + (sint)RandomiseValue(LerpValues(
                                               pe->ejectPeriod.initial, pe->ejectPeriod.final, lerpFrac),
                                           pe->ejectPeriod.randFrac);
                }
            }

            if(pe->count == 0 || ps->lazyRemove) {
                count = 0;

                //wait for child particles to die before declaring this pe invalid
                for(j = 0; j < MAX_PARTICLES; j++) {
                    p = &particles[ j ];

                    if(p->valid && p->parent == pe) {
                        count++;
                    }
                }

                if(!count) {
                    pe->valid = false;
                }
            }
        }
    }
}


/*
===============
idCGameParticles::SpawnNewParticleEjector

Allocate a new particle ejector
===============
*/
particleEjector_t *idCGameParticles::SpawnNewParticleEjector(
    baseParticleEjector_t *bpe, particleSystem_t *parent) {
    sint i;
    particleEjector_t *pe = nullptr;
    particleSystem_t *ps = parent;

    for(i = 0; i < MAX_PARTICLE_EJECTORS; i++) {
        pe = &particleEjectors[ i ];

        if(!pe->valid) {
            ::memset(pe, 0, sizeof(particleEjector_t));

            //found a free slot
            pe->_class = bpe;
            pe->parent = ps;

            pe->ejectPeriod.initial = bpe->eject.initial;
            pe->ejectPeriod.final = bpe->eject.final;
            pe->ejectPeriod.randFrac = bpe->eject.randFrac;

            pe->nextEjectionTime = cg.time + (sint)RandomiseValue((
                                       float32)bpe->eject.delay, bpe->eject.delayRandFrac);
            pe->count = pe->totalParticles = (sint)rint(RandomiseValue((
                                                 float32)bpe->totalParticles, bpe->totalParticlesRandFrac));

            pe->valid = true;

            if(cg_debugParticles.integer >= 1) {
                Printf("PE %s created\n", ps->_class->name);
            }

            break;
        }
    }

    return pe;
}


/*
===============
idCGameParticles::SpawnNewParticleSystem

Allocate a new particle system
===============
*/
particleSystem_t *idCGameParticles::SpawnNewParticleSystem(
    qhandle_t psHandle) {
    sint i, j;
    particleSystem_t *ps = nullptr;
    baseParticleSystem_t *bps = &baseParticleSystems[ psHandle - 1 ];

    if(!bps->registered) {
        Printf(S_COLOR_RED
               "ERROR: a particle system has not been registered yet\n");
        return nullptr;
    }

    for(i = 0; i < MAX_PARTICLE_SYSTEMS; i++) {
        ps = &particleSystems[ i ];

        if(!ps->valid) {
            ::memset(ps, 0, sizeof(particleSystem_t));

            //found a free slot
            ps->_class = bps;

            ps->valid = true;
            ps->lazyRemove = false;

            for(j = 0; j < bps->numEjectors; j++) {
                SpawnNewParticleEjector(bps->ejectors[j], ps);
            }

            if(cg_debugParticles.integer >= 1) {
                Printf("PS %s created\n", bps->name);
            }

            break;
        }
    }

    return ps;
}

/*
===============
idCGameParticles::RegisterParticleSystem

Load the shaders required for a particle system
===============
*/
qhandle_t idCGameParticles::RegisterParticleSystem(valueType *name) {
    sint i, j, k, l;
    baseParticleSystem_t *bps;
    baseParticleEjector_t *bpe;
    baseParticle_t *bp;

    for(i = 0; i < MAX_BASEPARTICLE_SYSTEMS; i++) {
        bps = &baseParticleSystems[ i ];

        if(!Q_stricmpn(bps->name, name, MAX_QPATH)) {
            //already registered
            if(bps->registered) {
                return i + 1;
            }

            for(j = 0; j < bps->numEjectors; j++) {
                bpe = bps->ejectors[ j ];

                for(l = 0; l < bpe->numParticles; l++) {
                    bp = bpe->particles[ l ];

                    for(k = 0; k < bp->numFrames; k++) {
                        bp->shaders[k] = trap_R_RegisterShader(bp->shaderNames[k]);
                    }

                    for(k = 0; k < bp->numModels; k++) {
                        bp->models[k] = trap_R_RegisterModel(bp->modelNames[k]);
                    }

                    if(bp->bounceMarkName[0] != '\0') {
                        bp->bounceMark = trap_R_RegisterShader(bp->bounceMarkName);
                    }

                    if(bp->bounceSoundName[0] != '\0') {
                        bp->bounceSound = trap_S_RegisterSound(bp->bounceSoundName);
                    }

                    //recursively register any children
                    if(bp->childSystemName[ 0 ] != '\0') {
                        //don't care about a handle for children since
                        //the system deals with it
                        RegisterParticleSystem(bp->childSystemName);
                    }

                    if(bp->onDeathSystemName[ 0 ] != '\0') {
                        //don't care about a handle for children since
                        //the system deals with it
                        RegisterParticleSystem(bp->onDeathSystemName);
                    }

                    if(bp->childTrailSystemName[0] != '\0') {
                        bp->childTrailSystemHandle = idCGameTrails::RegisterTrailSystem(
                                                         bp->childTrailSystemName);
                    }
                }
            }

            if(cg_debugParticles.integer >= 1) {
                Printf("Registered particle system %s\n", name);
            }

            bps->registered = true;

            //avoid returning 0
            return i + 1;
        }
    }

    Printf(S_COLOR_RED "ERROR: failed to register particle system %s\n", name);
    return 0;
}


/*
===============
idCGameParticles::ParseValueAndVariance

Parse a value and its random variance
===============
*/
void idCGameParticles::ParseValueAndVariance(valueType *token,
        float32 *value, float32 *variance, bool allowNegative) {
    valueType valueBuffer[ 16 ], varianceBuffer[ 16 ];
    valueType *variancePtr = nullptr, *varEndPointer = nullptr;
    float32 localValue = 0.0f, localVariance = 0.0f;

    Q_strncpyz(valueBuffer, token, sizeof(valueBuffer));
    Q_strncpyz(varianceBuffer, token, sizeof(varianceBuffer));

    variancePtr = strchr(valueBuffer, '~');

    //variance included
    if(variancePtr) {
        variancePtr[ 0 ] = '\0';
        variancePtr++;

        localValue = bggame->atof_neg(valueBuffer, allowNegative);

        varEndPointer = strchr(variancePtr, '%');

        if(varEndPointer) {
            varEndPointer[ 0 ] = '\0';
            localVariance = bggame->atof_neg(variancePtr, false) / 100.0f;
        } else {
            if(localValue != 0.0f) {
                localVariance = bggame->atof_neg(variancePtr, false) / localValue;
            } else {
                localVariance = bggame->atof_neg(variancePtr, false);
            }
        }
    } else {
        localValue = bggame->atof_neg(valueBuffer, allowNegative);
    }

    if(value != nullptr) {
        *value = localValue;
    }

    if(variance != nullptr) {
        *variance = localVariance;
    }
}

/*
===============
idCGameParticles::ParseColor
===============
*/
bool idCGameParticles::ParseColor(uchar8 *c, valueType **text_p) {
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
idCGameParticles::ParseParticle

Parse a particle section
===============
*/
bool idCGameParticles::ParseParticle(baseParticle_t *bp,
                                     valueType **text_p) {
    valueType *token;
    float32 number, randFrac;
    sint i;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "bounce")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "cull")) {
                bp->bounceCull = true;

                bp->bounceFrac = -1.0f;
                bp->bounceFracRandFrac = 0.0f;
            } else {
                ParseValueAndVariance(token, &number, &randFrac, false);

                bp->bounceFrac = number;
                bp->bounceFracRandFrac = randFrac;
            }

            continue;
        } else if(!Q_stricmp(token, "bounceMark")) {
            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->bounceMarkCount = number;
            bp->bounceMarkCountRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->bounceMarkRadius = number;
            bp->bounceMarkRadiusRandFrac = randFrac;

            token = COM_ParseExt(text_p, false);

            if(!*token) {
                break;
            }

            Q_strncpyz(bp->bounceMarkName, token, MAX_QPATH);

            continue;
        } else if(!Q_stricmp(token, "bounceSound")) {
            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->bounceSoundCount = number;
            bp->bounceSoundCountRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            Q_strncpyz(bp->bounceSoundName, token, MAX_QPATH);

            continue;
        } else if(!Q_stricmp(token, "shader")) {
            if(bp->numModels > 0) {
                Printf(S_COLOR_RED
                       "ERROR: 'shader' not allowed in conjunction with 'model'\n", token);
                break;
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "sync")) {
                bp->framerate = 0.0f;
            } else {
                bp->framerate = bggame->atof_neg(token, false);
            }

            token = COM_ParseExt(text_p, false);

            if(!*token) {
                break;
            }

            while(*token && bp->numFrames < MAX_PS_SHADER_FRAMES) {
                Q_strncpyz(bp->shaderNames[ bp->numFrames++ ], token, MAX_QPATH);
                token = COM_ParseExt(text_p, false);
            }

            continue;
        } else if(!Q_stricmp(token, "model")) {
            if(bp->numFrames > 0) {
                Printf(S_COLOR_RED
                       "ERROR: 'model' not allowed in conjunction with 'shader'\n", token);
                break;
            }

            token = COM_ParseExt(text_p, false);

            if(!*token) {
                break;
            }

            while(*token && bp->numModels < MAX_PS_MODELS) {
                Q_strncpyz(bp->modelNames[ bp->numModels++ ], token, MAX_QPATH);
                token = COM_ParseExt(text_p, false);
            }

            continue;
        } else if(!Q_stricmp(token, "modelAnimation")) {
            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            bp->modelAnimation.firstFrame = bggame->atoi_neg(token, false);

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            bp->modelAnimation.numFrames = atoi(token);
            bp->modelAnimation.reversed = false;
            bp->modelAnimation.flipflop = false;

            // if numFrames is negative the animation is reversed
            if(bp->modelAnimation.numFrames < 0) {
                bp->modelAnimation.numFrames = -bp->modelAnimation.numFrames;
                bp->modelAnimation.reversed = true;
            }

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            bp->modelAnimation.loopFrames = atoi(token);

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            if(!Q_stricmp(token, "sync")) {
                bp->modelAnimation.frameLerp = -1;
                bp->modelAnimation.initialLerp = -1;
            } else {
                float32 fps = bggame->atof_neg(token, false);

                if(fps == 0.0f) {
                    fps = 1.0f;
                }

                bp->modelAnimation.frameLerp = 1000 / fps;
                bp->modelAnimation.initialLerp = 1000 / fps;
            }

            continue;
        } else if(!Q_stricmp(token, "velocityType")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "static")) {
                bp->velMoveType = PMT_STATIC;
            } else if(!Q_stricmp(token, "static_transform")) {
                bp->velMoveType = PMT_STATIC_TRANSFORM;
            } else if(!Q_stricmp(token, "tag")) {
                bp->velMoveType = PMT_TAG;
            } else if(!Q_stricmp(token, "cent")) {
                bp->velMoveType = PMT_CENT_ANGLES;
            } else if(!Q_stricmp(token, "normal")) {
                bp->velMoveType = PMT_NORMAL;
            }

            continue;
        } else if(!Q_stricmp(token, "velocityDir")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "linear")) {
                bp->velMoveValues.dirType = PMD_LINEAR;
            } else if(!Q_stricmp(token, "point")) {
                bp->velMoveValues.dirType = PMD_POINT;
            }

            continue;
        } else if(!Q_stricmp(token, "velocityMagnitude")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->velMoveValues.mag = number;
            bp->velMoveValues.magRandFrac = randFrac;

            continue;
        } else if(!Q_stricmp(token, "parentVelocityFraction")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->velMoveValues.parentVelFrac = number;
            bp->velMoveValues.parentVelFracRandFrac = randFrac;

            continue;
        } else if(!Q_stricmp(token, "velocity")) {
            for(i = 0; i <= 2; i++) {
                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                bp->velMoveValues.dir[ i ] = bggame->atof_neg(token, true);
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, nullptr, &randFrac, false);

            bp->velMoveValues.dirRandAngle = randFrac;

            continue;
        } else if(!Q_stricmp(token, "velocityPoint")) {
            for(i = 0; i <= 2; i++) {
                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                bp->velMoveValues.point[ i ] = bggame->atof_neg(token, true);
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, nullptr, &randFrac, false);

            bp->velMoveValues.pointRandAngle = randFrac;

            continue;
        }
        ///
        else if(!Q_stricmp(token, "accelerationType")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "static")) {
                bp->accMoveType = PMT_STATIC;
            } else if(!Q_stricmp(token, "static_transform")) {
                bp->accMoveType = PMT_STATIC_TRANSFORM;
            } else if(!Q_stricmp(token, "tag")) {
                bp->accMoveType = PMT_TAG;
            } else if(!Q_stricmp(token, "cent")) {
                bp->accMoveType = PMT_CENT_ANGLES;
            } else if(!Q_stricmp(token, "normal")) {
                bp->accMoveType = PMT_NORMAL;
            }

            continue;
        } else if(!Q_stricmp(token, "accelerationDir")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "linear")) {
                bp->accMoveValues.dirType = PMD_LINEAR;
            } else if(!Q_stricmp(token, "point")) {
                bp->accMoveValues.dirType = PMD_POINT;
            }

            continue;
        } else if(!Q_stricmp(token, "accelerationMagnitude")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->accMoveValues.mag = number;
            bp->accMoveValues.magRandFrac = randFrac;

            continue;
        } else if(!Q_stricmp(token, "acceleration")) {
            for(i = 0; i <= 2; i++) {
                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                bp->accMoveValues.dir[ i ] = bggame->atof_neg(token, true);
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, nullptr, &randFrac, false);

            bp->accMoveValues.dirRandAngle = randFrac;

            continue;
        } else if(!Q_stricmp(token, "accelerationPoint")) {
            for(i = 0; i <= 2; i++) {
                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                bp->accMoveValues.point[ i ] = bggame->atof_neg(token, true);
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, nullptr, &randFrac, false);

            bp->accMoveValues.pointRandAngle = randFrac;

            continue;
        }
        ///
        else if(!Q_stricmp(token, "displacement")) {
            for(i = 0; i <= 2; i++) {
                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                bp->displacement[ i ] = bggame->atof_neg(token, true);
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, nullptr, &randFrac, false);

            bp->randDisplacement = randFrac;

            continue;
        } else if(!Q_stricmp(token, "normalDisplacement")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            bp->normalDisplacement = bggame->atof_neg(token, true);

            continue;
        } else if(!Q_stricmp(token, "overdrawProtection")) {
            bp->overdrawProtection = true;

            continue;
        } else if(!Q_stricmp(token, "realLight")) {
            bp->realLight = true;

            continue;
        } else if(!Q_stricmp(token, "dynamicLight")) {
            bp->dynamicLight = true;

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->dLightRadius.delay = (sint)number;
            bp->dLightRadius.delayRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->dLightRadius.initial = number;
            bp->dLightRadius.initialRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                bp->dLightRadius.final = PARTICLES_SAME_AS_INITIAL;
                bp->dLightRadius.finalRandFrac = 0.0f;
            } else {
                ParseValueAndVariance(token, &number, &randFrac, false);

                bp->dLightRadius.final = number;
                bp->dLightRadius.finalRandFrac = randFrac;
            }

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            if(!Q_stricmp(token, "{")) {
                if(!ParseColor(bp->dLightColor, text_p)) {
                    break;
                }

                token = COM_Parse(text_p);

                if(Q_stricmp(token, "}")) {
                    Printf(S_COLOR_RED "ERROR: missing '}'\n");
                    break;
                }
            }

            continue;
        } else if(!Q_stricmp(token, "cullOnStartSolid")) {
            bp->cullOnStartSolid = true;

            continue;
        } else if(!Q_stricmp(token, "radius")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->radius.delay = (sint)number;
            bp->radius.delayRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->radius.initial = number;
            bp->radius.initialRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                bp->radius.final = PARTICLES_SAME_AS_INITIAL;
                bp->radius.finalRandFrac = 0.0f;
            } else {
                ParseValueAndVariance(token, &number, &randFrac, false);

                bp->radius.final = number;
                bp->radius.finalRandFrac = randFrac;
            }

            continue;
        } else if(!Q_stricmp(token, "physicsRadius")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            bp->physicsRadius = atoi(token);
        } else if(!Q_stricmp(token, "alpha")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->alpha.delay = (sint)number;
            bp->alpha.delayRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->alpha.initial = number;
            bp->alpha.initialRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                bp->alpha.final = PARTICLES_SAME_AS_INITIAL;
                bp->alpha.finalRandFrac = 0.0f;
            } else {
                ParseValueAndVariance(token, &number, &randFrac, false);

                bp->alpha.final = number;
                bp->alpha.finalRandFrac = randFrac;
            }

            continue;
        } else if(!Q_stricmp(token, "color")) {
            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->colorDelay = (sint)number;
            bp->colorDelayRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!*token) {
                break;
            }

            if(!Q_stricmp(token, "{")) {
                if(!ParseColor(bp->initialColor, text_p)) {
                    break;
                }

                token = COM_Parse(text_p);

                if(Q_stricmp(token, "}")) {
                    Printf(S_COLOR_RED "ERROR: missing '}'\n");
                    break;
                }

                token = COM_Parse(text_p);

                if(!*token) {
                    break;
                }

                if(!Q_stricmp(token, "-")) {
                    bp->finalColor[ 0 ] = bp->initialColor[ 0 ];
                    bp->finalColor[ 1 ] = bp->initialColor[ 1 ];
                    bp->finalColor[ 2 ] = bp->initialColor[ 2 ];
                } else if(!Q_stricmp(token, "{")) {
                    if(!ParseColor(bp->finalColor, text_p)) {
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
        } else if(!Q_stricmp(token, "rotation")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->rotation.delay = (sint)number;
            bp->rotation.delayRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, true);

            bp->rotation.initial = number;
            bp->rotation.initialRandFrac = randFrac;

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                bp->rotation.final = PARTICLES_SAME_AS_INITIAL;
                bp->rotation.finalRandFrac = 0.0f;
            } else {
                ParseValueAndVariance(token, &number, &randFrac, true);

                bp->rotation.final = number;
                bp->rotation.finalRandFrac = randFrac;
            }

            continue;
        } else if(!Q_stricmp(token, "lifeTime")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bp->lifeTime = (sint)number;
            bp->lifeTimeRandFrac = randFrac;

            continue;
        } else if(!Q_stricmp(token, "childSystem")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            Q_strncpyz(bp->childSystemName, token, MAX_QPATH);

            continue;
        } else if(!Q_stricmp(token, "onDeathSystem")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            Q_strncpyz(bp->onDeathSystemName, token, MAX_QPATH);

            continue;
        } else if(!Q_stricmp(token, "childTrailSystem")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            Q_strncpyz(bp->childTrailSystemName, token, MAX_QPATH);

            continue;
        } else if(!Q_stricmp(token, "scaleWithCharge")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            bp->scaleWithCharge = atof(token);

            continue;
        } else if(!Q_stricmp(token, "}")) {
            return true; //reached the end of this particle
        } else {
            Printf(S_COLOR_RED "ERROR: unknown token '%s' in particle\n", token);
            return false;
        }
    }

    return false;
}

/*
===============
idCGameParticles::InitialiseBaseParticle
===============
*/
void idCGameParticles::InitialiseBaseParticle(baseParticle_t *bp) {
    ::memset(bp, 0, sizeof(baseParticle_t));
    ::memset(bp->initialColor, 0xFF, sizeof(bp->initialColor));
    ::memset(bp->finalColor, 0xFF, sizeof(bp->finalColor));
}

/*
===============
idCGameParticles::ParseParticleEjector

Parse a particle ejector section
===============
*/
bool idCGameParticles::ParseParticleEjector(baseParticleEjector_t *bpe,
        valueType **text_p) {
    valueType *token;
    float32 number, randFrac;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "{")) {
            InitialiseBaseParticle(&baseParticles[ numBaseParticles ]);

            if(!ParseParticle(&baseParticles[ numBaseParticles ], text_p)) {
                Printf(S_COLOR_RED "ERROR: failed to parse particle\n");
                return false;
            }

            if(bpe->numParticles == MAX_PARTICLES_PER_EJECTOR) {
                Printf(S_COLOR_RED "ERROR: ejector has > %d particles\n",
                       MAX_PARTICLES_PER_EJECTOR);
                return false;
            } else if(numBaseParticles == MAX_BASEPARTICLES) {
                Printf(S_COLOR_RED "ERROR: maximum number of particles (%d) reached\n",
                       MAX_BASEPARTICLES);
                return false;
            } else {
                //start parsing particles again
                bpe->particles[ bpe->numParticles ] = &baseParticles[ numBaseParticles ];
                bpe->numParticles++;
                numBaseParticles++;
            }

            continue;
        } else if(!Q_stricmp(token, "delay")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, &number, &randFrac, false);

            bpe->eject.delay = (sint)number;
            bpe->eject.delayRandFrac = randFrac;

            continue;
        } else if(!Q_stricmp(token, "period")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            bpe->eject.initial = bggame->atoi_neg(token, false);

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "-")) {
                bpe->eject.final = PARTICLES_SAME_AS_INITIAL;
            } else {
                bpe->eject.final = bggame->atoi_neg(token, false);
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            ParseValueAndVariance(token, nullptr, &bpe->eject.randFrac, false);

            continue;
        } else if(!Q_stricmp(token, "count")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "infinite")) {
                bpe->totalParticles = PARTICLES_INFINITE;
                bpe->totalParticlesRandFrac = 0.0f;
            } else {
                ParseValueAndVariance(token, &number, &randFrac, false);

                bpe->totalParticles = (sint)number;
                bpe->totalParticlesRandFrac = randFrac;
            }

            continue;
        } else if(!Q_stricmp(token, "particle")) {
            //acceptable text
            continue;
        } else if(!Q_stricmp(token, "}")) {
            //reached the end of this particle ejector
            return true;
        } else {
            Printf(S_COLOR_RED "ERROR: unknown token '%s' in particle ejector\n",
                   token);
            return false;
        }
    }

    return false;
}

/*
===============
idCGameParticles::ParseParticleSystem

Parse a particle system section
===============
*/
bool idCGameParticles::ParseParticleSystem(baseParticleSystem_t *bps,
        valueType **text_p, pointer name) {
    valueType *token;
    baseParticleEjector_t *bpe;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "{")) {
            if(!ParseParticleEjector(&baseParticleEjectors[ numBaseParticleEjectors ],
                                     text_p)) {
                Printf(S_COLOR_RED "ERROR: failed to parse particle ejector\n");
                return false;
            }

            bpe = &baseParticleEjectors[ numBaseParticleEjectors ];

            //check for infinite count + zero period
            if(bpe->totalParticles == PARTICLES_INFINITE &&
                    (bpe->eject.initial == 0.0f || bpe->eject.final == 0.0f)) {
                Printf(S_COLOR_RED
                       "ERROR: ejector with 'count infinite' potentially has zero period\n");
                return false;
            }

            if(bps->numEjectors == MAX_EJECTORS_PER_SYSTEM) {
                Printf(S_COLOR_RED "ERROR: particle system has > %d ejectors\n",
                       MAX_EJECTORS_PER_SYSTEM);
                return false;
            } else if(numBaseParticleEjectors == MAX_BASEPARTICLE_EJECTORS) {
                Printf(S_COLOR_RED
                       "ERROR: maximum number of particle ejectors (%d) reached\n",
                       MAX_BASEPARTICLE_EJECTORS);
                return false;
            } else {
                //start parsing ejectors again
                bps->ejectors[ bps->numEjectors ] =
                    &baseParticleEjectors[ numBaseParticleEjectors ];
                bps->numEjectors++;
                numBaseParticleEjectors++;
            }

            continue;
        } else if(!Q_stricmp(token, "thirdPersonOnly")) {
            bps->thirdPersonOnly = true;
        } else if(!Q_stricmp(token, "ejector")) {
            //acceptable text
            continue;
        } else if(!Q_stricmp(token, "}")) {
            if(cg_debugParticles.integer >= 1) {
                Printf("Parsed particle system %s\n", name);
            }

            return true; //reached the end of this particle system
        } else {
            Printf(S_COLOR_RED "ERROR: unknown token '%s' in particle system %s\n",
                   token, bps->name);
            return false;
        }
    }

    return false;
}

/*
===============
idCGameParticles::ParseParticleFile

Load the particle systems from a particle file
===============
*/
bool idCGameParticles::ParseParticleFile(pointer fileName) {
    valueType *text_p;
    sint i, len;
    valueType *token;
    valueType text[ 32000 ], psName[ MAX_QPATH ];
    bool psNameSet = false;
    fileHandle_t f;

    // load the file
    len = trap_FS_FOpenFile(fileName, &f, FS_READ);

    if(len < 0) {
        return false;
    }

    if(len == 0 || len >= sizeof(text) - 1) {
        trap_FS_FCloseFile(f);
        Printf(S_COLOR_RED "ERROR: particle file %s is %s\n", fileName,
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
            if(psNameSet) {
                //check for name space clashes
                for(i = 0; i < numBaseParticleSystems; i++) {
                    if(!Q_stricmp(baseParticleSystems[ i ].name, psName)) {
                        Printf(S_COLOR_RED "ERROR: a particle system is already named %s\n",
                               psName);
                        return false;
                    }
                }

                Q_strncpyz(baseParticleSystems[ numBaseParticleSystems ].name, psName,
                           MAX_QPATH);

                if(!ParseParticleSystem(&baseParticleSystems[ numBaseParticleSystems ],
                                        &text_p, psName)) {
                    Printf(S_COLOR_RED "ERROR: %s: failed to parse particle system %s\n",
                           fileName, psName);
                    return false;
                }

                //start parsing particle systems again
                psNameSet = false;

                if(numBaseParticleSystems == MAX_BASEPARTICLE_SYSTEMS) {
                    Printf(S_COLOR_RED
                           "ERROR: maximum number of particle systems (%d) reached\n",
                           MAX_BASEPARTICLE_SYSTEMS);
                    return false;
                } else {
                    numBaseParticleSystems++;
                }

                continue;
            } else {
                Printf(S_COLOR_RED "ERROR: unamed particle system\n");
                return false;
            }
        }

        if(!psNameSet) {
            Q_strncpyz(psName, token, sizeof(psName));
            psNameSet = true;
        } else {
            Printf(S_COLOR_RED "ERROR: particle system already named\n");
            return false;
        }
    }

    return true;
}


/*
===============
idCGameParticles::LoadParticleSystems

Load particle systems from .particle files
===============
*/
void idCGameParticles::LoadParticleSystems(void) {
    sint i, j, numFiles, fileLen;
    valueType fileList[ MAX_PARTICLE_FILES * MAX_QPATH ];
    valueType fileName[ MAX_QPATH ];
    valueType *filePtr;

    //clear out the old
    numBaseParticleSystems = 0;
    numBaseParticleEjectors = 0;
    numBaseParticles = 0;

    for(i = 0; i < MAX_BASEPARTICLE_SYSTEMS; i++) {
        baseParticleSystem_t  *bps = &baseParticleSystems[ i ];
        ::memset(bps, 0, sizeof(baseParticleSystem_t));
    }

    for(i = 0; i < MAX_BASEPARTICLE_EJECTORS; i++) {
        baseParticleEjector_t  *bpe = &baseParticleEjectors[ i ];
        ::memset(bpe, 0, sizeof(baseParticleEjector_t));
    }

    for(i = 0; i < MAX_BASEPARTICLES; i++) {
        baseParticle_t  *bp = &baseParticles[ i ];
        ::memset(bp, 0, sizeof(baseParticle_t));
    }


    //and bring in the new
    numFiles = trap_FS_GetFileList("scripts", ".particle", fileList,
                                   MAX_PARTICLE_FILES * MAX_QPATH);
    filePtr = fileList;

    for(i = 0; i < numFiles; i++, filePtr += fileLen + 1) {
        fileLen = strlen(filePtr);
        strcpy(fileName, "scripts/");
        strcat(fileName, filePtr);
        //Printf( "...loading '%s'\n", fileName );
        ParseParticleFile(fileName);
    }

    //connect any child systems to their psHandle
    for(i = 0; i < numBaseParticles; i++) {
        baseParticle_t  *bp = &baseParticles[ i ];

        if(bp->childSystemName[ 0 ]) {
            //particle _class has a child, resolve the name
            for(j = 0; j < numBaseParticleSystems; j++) {
                baseParticleSystem_t  *bps = &baseParticleSystems[ j ];

                if(!Q_stricmp(bps->name, bp->childSystemName)) {
                    //FIXME: add checks for cycles and infinite children
                    bp->childSystemHandle = j + 1;

                    break;
                }
            }

            if(j == numBaseParticleSystems) {
                //couldn't find named particle system
                Printf(S_COLOR_YELLOW "WARNING: failed to find child %s\n",
                       bp->childSystemName);
                bp->childSystemName[ 0 ] = '\0';
            }
        }

        if(bp->onDeathSystemName[ 0 ]) {
            //particle _class has a child, resolve the name
            for(j = 0; j < numBaseParticleSystems; j++) {
                baseParticleSystem_t  *bps = &baseParticleSystems[ j ];

                if(!Q_stricmp(bps->name, bp->onDeathSystemName)) {
                    //FIXME: add checks for cycles and infinite children
                    bp->onDeathSystemHandle = j + 1;

                    break;
                }
            }

            if(j == numBaseParticleSystems) {
                //couldn't find named particle system
                Printf(S_COLOR_YELLOW "WARNING: failed to find onDeath system %s\n",
                       bp->onDeathSystemName);
                bp->onDeathSystemName[ 0 ] = '\0';
            }
        }
    }
}

/*
===============
idCGameParticles::SetParticleSystemNormal
===============
*/
void idCGameParticles::SetParticleSystemNormal(particleSystem_t *ps,
        vec3_t normal) {
    if(ps == nullptr || !ps->valid) {
        Printf(S_COLOR_YELLOW
               "WARNING: tried to modify a nullptr particle system\n");
        return;
    }

    ps->normalValid = true;
    VectorCopy(normal, ps->normal);
    VectorNormalize(ps->normal);
}

/*
===============
idCGameParticles::DestroyParticleSystem

Destroy a particle system

This doesn't actually invalidate anything, it just stops
particle ejectors from producing new particles so the
garbage collector will eventually remove this system.
However is does set the pointer to nullptr so the user is
unable to manipulate this particle system any longer.
===============
*/
void idCGameParticles::DestroyParticleSystem(particleSystem_t **ps) {
    sint i;
    particleEjector_t *pe;

    if(*ps == nullptr || !(*ps)->valid) {
        Printf(S_COLOR_YELLOW
               "WARNING: tried to destroy a nullptr particle system\n");
        return;
    }

    if(cg_debugParticles.integer >= 1) {
        Printf("PS destroyed\n");
    }

    for(i = 0; i < MAX_PARTICLE_EJECTORS; i++) {
        pe = &particleEjectors[ i ];

        if(pe->valid && pe->parent == *ps) {
            pe->totalParticles = pe->count = 0;
        }
    }

    *ps = nullptr;
}

/*
===============
idCGameParticles::IsParticleSystemInfinite

Test a particle system for 'count infinite' ejectors
===============
*/
bool idCGameParticles::IsParticleSystemInfinite(particleSystem_t *ps) {
    sint i;
    particleEjector_t *pe;

    if(ps == nullptr) {
        Printf(S_COLOR_YELLOW
               "WARNING: tried to test a nullptr particle system\n");
        return false;
    }

    if(!ps->valid) {
        Printf(S_COLOR_YELLOW
               "WARNING: tried to test an invalid particle system\n");
        return false;
    }

    //don't bother checking already invalid systems
    if(!ps->valid) {
        return false;
    }

    for(i = 0; i < MAX_PARTICLE_EJECTORS; i++) {
        pe = &particleEjectors[ i ];

        if(pe->valid && pe->parent == ps) {
            if(pe->totalParticles == PARTICLES_INFINITE) {
                return true;
            }
        }
    }

    return false;
}

/*
===============
idCGameParticles::IsParticleSystemValid

Test a particle system for validity
===============
*/
bool idCGameParticles::IsParticleSystemValid(particleSystem_t **ps) {
    if(*ps == nullptr || (*ps && !(*ps)->valid)) {
        if(*ps && !(*ps)->valid) {
            *ps = nullptr;
        }

        return false;
    }

    return true;
}

/*
===============
idCGameParticles::GarbageCollectParticleSystems

Destroy inactive particle systems
===============
*/
void idCGameParticles::GarbageCollectParticleSystems(void) {
    sint i, j, count;
    particleSystem_t *ps;
    particleEjector_t *pe;
    sint centNum;

    for(i = 0; i < MAX_PARTICLE_SYSTEMS; i++) {
        ps = &particleSystems[ i ];
        count = 0;

        //don't bother checking already invalid systems
        if(!ps->valid) {
            continue;
        }

        for(j = 0; j < MAX_PARTICLE_EJECTORS; j++) {
            pe = &particleEjectors[ j ];

            if(pe->valid && pe->parent == ps) {
                count++;
            }
        }

        if(!count) {
            ps->valid = false;
        }

        //check systems where the parent cent has left the PVS
        //( local player entity is always valid )
        if((centNum = idCGameAttachment::AttachmentCentNum(&ps->attachment)) >= 0
                && centNum != cg.snap->ps.clientNum) {
            if(!cg_entities[centNum].valid) {
                ps->lazyRemove = true;
            }
        }

        if(cg_debugParticles.integer >= 1 && !ps->valid) {
            Printf("PS %s garbage collected\n", ps->_class->name);
        }
    }
}


/*
===============
idCGameParticles::CalculateTimeFrac

Calculate the fraction of time passed
===============
*/
float32 idCGameParticles::CalculateTimeFrac(sint birth, sint life,
        sint delay) {
    float32 frac;

    frac = ((float32)cg.time - (float32)(birth + delay)) / (float32)(
               life - delay);

    if(frac < 0.0f) {
        frac = 0.0f;
    } else if(frac > 1.0f) {
        frac = 1.0f;
    }

    return frac;
}

/*
===============
idCGameParticles::EvaluateParticlePhysics

Compute the physics on a specific particle
===============
*/
void idCGameParticles::EvaluateParticlePhysics(particle_t *p) {
    particleSystem_t *ps = p->parent->parent;
    baseParticle_t *bp = p->_class;
    vec3_t acceleration, newOrigin, mins, maxs, transform[3];
    float32 deltaTime, bounce, radius, dot;
    trace_t trace;

    if(p->atRest) {
        VectorClear(p->velocity);
        return;
    }

    switch(bp->accMoveType) {
        case PMT_STATIC:
            if(bp->accMoveValues.dirType == PMD_POINT) {
                VectorSubtract(bp->accMoveValues.point, p->origin, acceleration);
            } else if(bp->accMoveValues.dirType == PMD_LINEAR) {
                VectorCopy(bp->accMoveValues.dir, acceleration);
            }

            break;

        case PMT_STATIC_TRANSFORM:
            if(!idCGameAttachment::AttachmentAxis(&ps->attachment, transform)) {
                return;
            }

            if(bp->accMoveValues.dirType == PMD_POINT) {
                vec3_t transPoint;

                VectorMatrixMultiply(bp->accMoveValues.point, transform, transPoint);
                VectorSubtract(transPoint, p->origin, acceleration);
            } else if(bp->accMoveValues.dirType == PMD_LINEAR) {
                VectorMatrixMultiply(bp->accMoveValues.dir, transform, acceleration);
            }

            break;

        case PMT_TAG:
        case PMT_CENT_ANGLES:
            if(bp->accMoveValues.dirType == PMD_POINT) {
                vec3_t point;

                if(!idCGameAttachment::AttachmentPoint(&ps->attachment, point)) {
                    return;
                }

                VectorSubtract(point, p->origin, acceleration);
            } else if(bp->accMoveValues.dirType == PMD_LINEAR) {
                if(!idCGameAttachment::AttachmentDir(&ps->attachment, acceleration)) {
                    return;
                }
            }

            break;

        case PMT_NORMAL:
            if(!ps->normalValid) {
                return;
            }

            VectorCopy(ps->normal, acceleration);

            break;
    }

#define MAX_ACC_RADIUS 1000.0f

    if(bp->accMoveValues.dirType == PMD_POINT) {
        //FIXME: so this fall off is a bit... odd -- it works..
        float32 r2 = DotProduct(acceleration, acceleration);   // = radius^2
        float32 scale = (MAX_ACC_RADIUS - r2) / MAX_ACC_RADIUS;

        if(scale > 1.0f) {
            scale = 1.0f;
        } else if(scale < 0.1f) {
            scale = 0.1f;
        }

        scale *= RandomiseValue(bp->accMoveValues.mag,
                                bp->accMoveValues.magRandFrac);

        VectorNormalize(acceleration);
        SpreadVector(acceleration, bp->accMoveValues.dirRandAngle);
        VectorScale(acceleration, scale, acceleration);
    } else if(bp->accMoveValues.dirType == PMD_LINEAR) {
        VectorNormalize(acceleration);
        SpreadVector(acceleration, bp->accMoveValues.dirRandAngle);
        VectorScale(acceleration, RandomiseValue(bp->accMoveValues.mag,
                    bp->accMoveValues.magRandFrac), acceleration);
    }

    // Some particles have a visual radius that differs from their collision radius
    if(bp->physicsRadius) {
        radius = bp->physicsRadius;
    } else {
        radius = LerpValues(p->radius.initial, p->radius.final,
                            CalculateTimeFrac(p->birthTime, p->lifeTime, p->radius.delay));
    }

    VectorSet(mins, -radius, -radius, -radius);
    VectorSet(maxs, radius, radius, radius);

    bounce = RandomiseValue(bp->bounceFrac, bp->bounceFracRandFrac);

    deltaTime = (float32)(cg.time - p->lastEvalTime) * 0.001;
    VectorMA(p->velocity, deltaTime, acceleration, p->velocity);
    VectorMA(p->origin, deltaTime, p->velocity, newOrigin);
    p->lastEvalTime = cg.time;

    // we're not doing particle physics, but at least cull them in solids
    if(!cg_bounceParticles.integer) {
        sint contents = trap_CM_PointContents(newOrigin, 0);

        if((contents & CONTENTS_SOLID) || (contents & CONTENTS_NODROP)) {
            DestroyParticle(p, nullptr);
        } else {
            VectorCopy(newOrigin, p->origin);
        }

        return;
    }

    idCGamePredict::Trace(&trace, p->origin, mins, maxs, newOrigin,
                          idCGameAttachment::AttachmentCentNum(&ps->attachment), CONTENTS_SOLID);

    //not hit anything or not a collider
    if(trace.fraction == 1.0f || bounce == 0.0f) {
        VectorCopy(newOrigin, p->origin);
        return;
    }

    //remove particles that get into a CONTENTS_NODROP brush
    if((trap_CM_PointContents(trace.endpos, 0) & CONTENTS_NODROP) ||
            (bp->cullOnStartSolid && trace.startsolid)) {
        DestroyParticle(p, nullptr);
        return;
    } else if(bp->bounceCull) {
        DestroyParticle(p, trace.plane.normal);
        return;
    }

    //reflect the velocity on the trace plane
    dot = DotProduct(p->velocity, trace.plane.normal);
    VectorMA(p->velocity, -2.0f * dot, trace.plane.normal, p->velocity);

    VectorScale(p->velocity, bounce, p->velocity);

    if(trace.plane.normal[2] > 0.5f && (p->velocity[2] < 40.0f ||
                                        p->velocity[2] < -cg.frametime * p->velocity[2])) {
        p->atRest = true;
    }

    if(bp->bounceMarkName[ 0 ] && p->bounceMarkCount > 0) {
        idCGameMarks::ImpactMark(bp->bounceMark, trace.endpos, trace.plane.normal,
                                 random() * 360, 1, 1, 1, 1, true, bp->bounceMarkRadius, false);
        p->bounceMarkCount--;
    }

    if(bp->bounceSoundName[ 0 ] && p->bounceSoundCount > 0) {
        trap_S_StartSound(trace.endpos, ENTITYNUM_WORLD, CHAN_AUTO,
                          bp->bounceSound);
        p->bounceSoundCount--;
    }

    VectorCopy(trace.endpos, p->origin);
}


#define GETKEY(x,y) (((x)>>y)&0xFF)

/*
===============
idCGameParticles::Radix
===============
*/
void idCGameParticles::Radix(sint bits, sint size, particle_t **source,
                             particle_t **dest) {
    sint count[ 256 ], index[ 256 ], i;

    ::memset(count, 0, sizeof(count));

    for(i = 0; i < size; i++) {
        count[GETKEY(source[i]->sortKey, bits)]++;
    }

    index[ 0 ] = 0;

    for(i = 1; i < 256; i++) {
        index[i] = index[i - 1] + count[i - 1];
    }

    for(i = 0; i < size; i++) {
        dest[index[GETKEY(source[i]->sortKey, bits)]++] = source[i];
    }
}

/*
===============
idCGameParticles::RadixSort

Radix sort with 4 byte size buckets
===============
*/
void idCGameParticles::RadixSort(particle_t **source, particle_t **temp,
                                 sint size) {
    Radix(0, size, source, temp);
    Radix(8, size, temp, source);
    Radix(16, size, source, temp);
    Radix(24, size, temp, source);
}

/*
===============
idCGameParticles::CompactAndSortParticles

Depth sort the particles
===============
*/
void idCGameParticles::CompactAndSortParticles(void) {
    sint i, j = 0, numParticles;
    vec3_t  delta;

    for(i = 0; i < MAX_PARTICLES; i++) {
        sortedParticles[i] = &particles[i];
    }

    if(!cg_depthSortParticles.integer) {
        return;
    }

    for(i = MAX_PARTICLES - 1; i >= 0; i--) {
        if(sortedParticles[ i ]->valid) {
            //find the first hole
            while(j < MAX_PARTICLES && sortedParticles[j]->valid) {
                j++;
            }

            //no more holes
            if(j >= i) {
                break;
            }

            sortedParticles[ j ] = sortedParticles[ i ];
        }
    }

    numParticles = i;

    //set sort keys
    for(i = 0; i < numParticles; i++) {
        VectorSubtract(sortedParticles[ i ]->origin, cg.refdef.vieworg, delta);
        sortedParticles[ i ]->sortKey = (sint)DotProduct(delta, delta);
    }

    RadixSort(sortedParticles, radixBuffer, numParticles);

    //FIXME: wtf?
    //reverse order of particles array
    for(i = 0; i < numParticles; i++) {
        radixBuffer[i] = sortedParticles[numParticles - i - 1];
    }

    for(i = 0; i < numParticles; i++) {
        sortedParticles[i] = radixBuffer[i];
    }
}

/*
===============
idCGameParticles::RenderParticle

Actually render a particle
===============
*/
void idCGameParticles::RenderParticle(particle_t *p) {
    refEntity_t re;
    float32 timeFrac, scale;
    sint index, i;
    baseParticle_t *bp = p->_class;
    particleSystem_t *ps = p->parent->parent;
    baseParticleSystem_t *bps = ps->_class;
    vec3_t alight, dlight, lightdir, up = { 0.0f, 0.0f, 1.0f };

    ::memset(&re, 0, sizeof(refEntity_t));

    timeFrac = CalculateTimeFrac(p->birthTime, p->lifeTime, 0);

    scale = LerpValues(p->radius.initial, p->radius.final,
                       CalculateTimeFrac(p->birthTime, p->lifeTime, p->radius.delay));

    re.shaderTime = p->birthTime / 1000.0f;

    if(bp->numFrames) {       //shader based
        re.reType = RT_SPRITE;

        //apply environmental lighting to the particle
        if(bp->realLight) {
            trap_R_LightForPoint(p->origin, alight, dlight, lightdir);

            for(i = 0; i <= 2; i++) {
                re.shaderRGBA[i] = (uchar8)alight[i];
            }
        } else {
            vec3_t  colorRange;

            VectorSubtract(bp->finalColor, bp->initialColor, colorRange);

            VectorMA(bp->initialColor, CalculateTimeFrac(p->birthTime, p->lifeTime,
                     p->colorDelay), colorRange, re.shaderRGBA);
        }

        re.shaderRGBA[ 3 ] = (uchar8)((float32)0xFF * LerpValues(p->alpha.initial,
                                      p->alpha.final, CalculateTimeFrac(p->birthTime, p->lifeTime,
                                              p->alpha.delay)));

        re.radius = scale;

        re.rotation = LerpValues(p->rotation.initial, p->rotation.final,
                                 CalculateTimeFrac(p->birthTime, p->lifeTime, p->rotation.delay));

        // if the view would be "inside" the sprite, kill the sprite
        // so it doesn't add too much overdraw
        if(Distance(p->origin, cg.refdef.vieworg) < re.radius &&
                bp->overdrawProtection) {
            return;
        }

        if(bp->framerate == 0.0f) {
            //sync animation time to lifeTime of particle
            index = (sint)(timeFrac * (bp->numFrames + 1));

            if(index >= bp->numFrames) {
                index = bp->numFrames - 1;
            }

            re.customShader = bp->shaders[ index ];
        } else {
            //looping animation
            index = (sint)(bp->framerate * timeFrac * p->lifeTime * 0.001) %
                    bp->numFrames;
            re.customShader = bp->shaders[ index ];
        }

    } else if(bp->numModels) { //model based
        re.reType = RT_MODEL;

        re.hModel = p->model;

        if(p->atRest) {
            AxisCopy(p->lastAxis, re.axis);
        } else {
            // convert direction of travel into axis
            VectorNormalize2(p->velocity, re.axis[ 0 ]);

            if(re.axis[0][0] == 0.0f && re.axis[0][1] == 0.0f) {
                AxisCopy(axisDefault, re.axis);
            } else {
                ProjectPointOnPlane(re.axis[ 2 ], up, re.axis[ 0 ]);
                VectorNormalize(re.axis[ 2 ]);
                CrossProduct(re.axis[ 2 ], re.axis[ 0 ], re.axis[ 1 ]);
            }

            AxisCopy(re.axis, p->lastAxis);
        }

        if(scale != 1.0f) {
            VectorScale(re.axis[ 0 ], scale, re.axis[ 0 ]);
            VectorScale(re.axis[ 1 ], scale, re.axis[ 1 ]);
            VectorScale(re.axis[ 2 ], scale, re.axis[ 2 ]);
            re.nonNormalizedAxes = true;
        } else {
            re.nonNormalizedAxes = false;
        }

        p->lf.animation = &bp->modelAnimation;

        //run animation
        idCGameAnimation::RunLerpFrame(&p->lf, 1.0f);

        re.oldframe = p->lf.oldFrame;
        re.frame    = p->lf.frame;
        re.backlerp = p->lf.backlerp;
    }

    if(bps->thirdPersonOnly &&
            idCGameAttachment::AttachmentCentNum(&ps->attachment) ==
            cg.snap->ps.clientNum && !cg.renderingThirdPerson) {
        re.renderfx |= RF_THIRD_PERSON;
    }

    if(bp->dynamicLight && !(re.renderfx & RF_THIRD_PERSON)) {
        trap_R_AddLightToScene(p->origin, LerpValues(p->dLightRadius.initial,
                               p->dLightRadius.final, CalculateTimeFrac(p->birthTime, p->lifeTime,
                                       p->dLightRadius.delay)),
                               (float32)bp->dLightColor[ 0 ] / (float32)0xFF,
                               (float32)bp->dLightColor[ 1 ] / (float32)0xFF,
                               (float32)bp->dLightColor[ 2 ] / (float32)0xFF);
    }

    VectorCopy(p->origin, re.origin);

    trap_R_AddRefEntityToScene(&re);
}

/*
===============
idCGameParticles::AddParticles

Add particles to the scene
===============
*/
void idCGameParticles::AddParticles(void) {
    sint i, numPS = 0, numPE = 0, numP = 0;
    particle_t *p;

    //remove expired particle systems
    GarbageCollectParticleSystems();

    //check each ejector and introduce any new particles
    SpawnNewParticles();

    //sorting
    CompactAndSortParticles();

    for(i = 0; i < MAX_PARTICLES; i++) {
        p = sortedParticles[ i ];

        if(p->valid) {
            if(p->birthTime + p->lifeTime > cg.time) {
                //particle is active
                EvaluateParticlePhysics(p);
                RenderParticle(p);
            } else {
                DestroyParticle(p, nullptr);
            }
        }
    }

    if(cg_debugParticles.integer >= 2) {
        for(i = 0; i < MAX_PARTICLE_SYSTEMS; i++) {
            if(particleSystems[i].valid) {
                numPS++;
            }
        }

        for(i = 0; i < MAX_PARTICLE_EJECTORS; i++) {
            if(particleEjectors[i].valid) {
                numPE++;
            }
        }

        for(i = 0; i < MAX_PARTICLES; i++) {
            if(particles[i].valid) {
                numP++;
            }
        }

        Printf("PS: %d  PE: %d  P: %d\n", numPS, numPE, numP);
    }
}

/*
===============
idCGameParticles::ParticleSystemEntity

Particle system entity client code
===============
*/
void idCGameParticles::ParticleSystemEntity(centity_t *cent) {
    entityState_t *es;

    es = &cent->currentState;

    if(es->eFlags & EF_NODRAW) {
        if(IsParticleSystemValid(&cent->entityPS) &&
                IsParticleSystemInfinite(cent->entityPS)) {
            DestroyParticleSystem(&cent->entityPS);
        }

        return;
    }

    if(!IsParticleSystemValid(&cent->entityPS) && !cent->entityPSMissing) {
        cent->entityPS = SpawnNewParticleSystem(
                             cgs.gameParticleSystems[ es->modelindex ]);

        if(IsParticleSystemValid(&cent->entityPS)) {
            idCGameAttachment::SetAttachmentPoint(&cent->entityPS->attachment,
                                                  cent->lerpOrigin);
            idCGameAttachment::SetAttachmentCent(&cent->entityPS->attachment, cent);
            idCGameAttachment::AttachToPoint(&cent->entityPS->attachment);
        } else {
            cent->entityPSMissing = true;
        }
    }
}

static particleSystem_t *testPS;
static qhandle_t testPSHandle;

/*
===============
idCGameParticles::DestroyTestPS_f

Destroy the test a particle system
===============
*/
void idCGameParticles::DestroyTestPS_f(void) {
    if(IsParticleSystemValid(&testPS)) {
        DestroyParticleSystem(&testPS);
    }
}

/*
===============
idCGameParticles::TestPS_f

Test a particle system
===============
*/
void idCGameParticles::TestPS_f(void) {
    vec3_t origin, up = { 0.0f, 0.0f, 1.0f };
    valueType psName[ MAX_QPATH ];

    if(trap_Argc() < 2) {
        return;
    }

    Q_strncpyz(psName, idCGameMain::Argv(1), MAX_QPATH);
    testPSHandle = RegisterParticleSystem(psName);

    if(testPSHandle) {
        DestroyTestPS_f();

        testPS = SpawnNewParticleSystem(testPSHandle);

        VectorMA(cg.refdef.vieworg, 100, cg.refdef.viewaxis[ 0 ], origin);

        if(IsParticleSystemValid(&testPS)) {
            idCGameAttachment::SetAttachmentPoint(&testPS->attachment, origin);
            SetParticleSystemNormal(testPS, up);
            idCGameAttachment::AttachToPoint(&testPS->attachment);
        }
    }
}
