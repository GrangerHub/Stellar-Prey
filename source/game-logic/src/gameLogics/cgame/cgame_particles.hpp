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
// File name:   cgame_particles.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_PARTICLES_H_
#define __CGAME_PARTICLES_H_

class idCGameLocal;

class idCGameParticles : public idCGameLocal {
public:
    idCGameParticles();
    ~idCGameParticles();

    static float32 LerpValues(float32 a, float32 b, float32 f);
    static float32 RandomiseValue(float32 value, float32 variance);
    static void SpreadVector(vec3_t v, float32 spread);
    static void DestroyParticle(particle_t *p, vec3_t impactNormal);
    static particle_t *SpawnNewParticle(baseParticle_t *bp,
                                        particleEjector_t *parent);
    static void SpawnNewParticles(void);
    static particleEjector_t *SpawnNewParticleEjector(baseParticleEjector_t
            *bpe, particleSystem_t *parent);
    static particleSystem_t *SpawnNewParticleSystem(qhandle_t psHandle);
    static qhandle_t RegisterParticleSystem(valueType *name);
    static void ParseValueAndVariance(valueType *token, float32 *value,
                                      float32 *variance, bool allowNegative);
    static bool ParseColor(uchar8 *c, valueType **text_p);
    static bool ParseParticle(baseParticle_t *bp, valueType **text_p);
    static void InitialiseBaseParticle(baseParticle_t *bp);
    static bool ParseParticleEjector(baseParticleEjector_t *bpe,
                                     valueType **text_p);
    static bool ParseParticleSystem(baseParticleSystem_t *bps,
                                    valueType **text_p, pointer name);
    static bool ParseParticleFile(pointer fileName);
    static void LoadParticleSystems(void);
    static void SetParticleSystemNormal(particleSystem_t *ps, vec3_t normal);
    static void DestroyParticleSystem(particleSystem_t **ps);
    static bool IsParticleSystemInfinite(particleSystem_t *ps);
    static bool IsParticleSystemValid(particleSystem_t **ps);
    static void GarbageCollectParticleSystems(void);
    static float32 CalculateTimeFrac(sint birth, sint life, sint delay);
    static void EvaluateParticlePhysics(particle_t *p);
    static void Radix(sint bits, sint size, particle_t **source,
                      particle_t **dest);
    static void RadixSort(particle_t **source, particle_t **temp, sint size);
    static void CompactAndSortParticles(void);
    static void RenderParticle(particle_t *p);
    static void AddParticles(void);
    static void ParticleSystemEntity(centity_t *cent);
    static void DestroyTestPS_f(void);
    static void TestPS_f(void);
};

#endif //!__CGAME_PARTICLES_H_
