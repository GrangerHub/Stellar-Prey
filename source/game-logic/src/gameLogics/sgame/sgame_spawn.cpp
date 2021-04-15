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
// File name:   sgame_spawn.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

/*
===============
idSGameSpawn::idSGameSpawn
===============
*/
idSGameSpawn::idSGameSpawn(void) {
}

/*
===============
idSGameSvcmds::~idSGameSvcmds
===============
*/
idSGameSpawn::~idSGameSpawn(void) {
}

bool idSGameSpawn::SpawnString(pointer key, pointer defaultString,
                               valueType **out) {
    sint   i;

    if(!level.spawning) {
        *out = (valueType *)defaultString;
        //idSGameMain::Error( "idSGameSpawn::SpawnString() called while not spawning" );
    }

    for(i = 0; i < level.numSpawnVars; i++) {
        if(!Q_stricmp(key, level.spawnVars[ i ][ 0 ])) {
            *out = level.spawnVars[ i ][ 1 ];
            return true;
        }
    }

    *out = (valueType *)defaultString;
    return false;
}

bool idSGameSpawn::SpawnFloat(pointer key, pointer defaultString,
                              float32 *out) {
    valueType    *s;
    bool  present;

    present = SpawnString(key, defaultString, &s);
    *out = atof(s);
    return present;
}

bool idSGameSpawn::SpawnInt(pointer key, pointer defaultString,
                            sint *out) {
    valueType      *s;
    bool  present;

    present = SpawnString(key, defaultString, &s);
    *out = atoi(s);
    return present;
}

bool idSGameSpawn::SpawnVector(pointer key, pointer defaultString,
                               float32 *out) {
    valueType    *s;
    bool  present;

    present = SpawnString(key, defaultString, &s);
    sscanf(s, "%f %f %f", &out[ 0 ], &out[ 1 ], &out[ 2 ]);
    return present;
}

bool idSGameSpawn::SpawnVector4(pointer key, pointer defaultString,
                                float32 *out) {
    valueType    *s;
    bool  present;

    present = SpawnString(key, defaultString, &s);
    sscanf(s, "%f %f %f %f", &out[ 0 ], &out[ 1 ], &out[ 2 ], &out[ 3 ]);
    return present;
}

//
// fields are needed for spawning from the entity string
//
typedef enum {
    F_INT,
    F_FLOAT,
    F_LSTRING,      // string on disk, pointer in memory, TAG_LEVEL
    F_GSTRING,      // string on disk, pointer in memory, TAG_GAME
    F_VECTOR,
    F_VECTOR4,
    F_ANGLEHACK,
    F_ENTITY,     // index on disk, pointer in memory
    F_ITEM,       // index on disk, pointer in memory
    F_CLIENT,     // index on disk, pointer in memory
    F_IGNORE
} fieldtype_t;

typedef struct {
    valueType  *name;
    sint   ofs;
    fieldtype_t type;
    sint   flags;
} field_t;

field_t fields[ ] = {
    {"classname", FOFS(classname), F_LSTRING},
    {"sound", FOFS(sound), F_LSTRING},
    {"origin", FOFS(s.origin), F_VECTOR},
    {"model", FOFS(model), F_LSTRING},
    {"model2", FOFS(model2), F_LSTRING},
    {"spawnflags", FOFS(spawnflags), F_INT},
    {"speed", FOFS(speed), F_FLOAT},
    {"target", FOFS(target), F_LSTRING},
    {"targetname", FOFS(targetname), F_LSTRING},
    {"message", FOFS(message), F_LSTRING},
    {"team", FOFS(team), F_LSTRING},
    {"wait", FOFS(wait), F_FLOAT},
    {"random", FOFS(random), F_FLOAT},
    {"count", FOFS(count), F_INT},
    {"health", FOFS(health), F_INT},
    {"light", 0, F_IGNORE},
    {"dmg", FOFS(damage), F_INT},
    {"angles", FOFS(s.angles), F_VECTOR},
    {"angle", FOFS(s.angles), F_ANGLEHACK},
    {"bounce", FOFS(physicsBounce), F_FLOAT},
    {"alpha", FOFS(pos1), F_VECTOR},
    {"radius", FOFS(pos2), F_VECTOR},
    {"acceleration", FOFS(acceleration), F_VECTOR},
    {"animation", FOFS(animation), F_VECTOR4},
    {"rotatorAngle", FOFS(rotatorAngle), F_FLOAT},
    {"targetShaderName", FOFS(targetShaderName), F_LSTRING},
    {"targetShaderNewName", FOFS(targetShaderNewName), F_LSTRING},
    {"linear", FOFS(altFire), F_INT},   //for movers to use linear movement

    {nullptr}
};

typedef struct {
    valueType  *name;
    void (*spawn)(gentity_t *ent);
} spawn_t;

spawn_t spawns[ ] = {
    // info entities don't do anything at all, but provide positional
    // information for things controlled by other processes
    { "info_player_start",        &idSGameClient::SP_info_player_start },
    { "info_player_deathmatch",   &idSGameClient::SP_info_player_deathmatch },
    { "info_player_intermission", &idSGameClient::SP_info_player_intermission },

    { "info_alien_intermission",  &idSGameClient::SP_info_alien_intermission },
    { "info_human_intermission",  &idSGameClient::SP_info_human_intermission },

    { "info_null",                &idSGameMisc::SP_info_null },
    { "info_notnull",             &idSGameMisc::SP_info_notnull },    // use target_position instead

    { "func_sound",               &idSGameMisc::SP_func_sound },
    { "func_plat",                &idSGameMover::SP_func_plat },
    { "func_button",              &idSGameMover::SP_func_button },
    { "func_door",                &idSGameMover::SP_func_door },
    { "func_door_rotating",       &idSGameMover::SP_func_door_rotating },
    { "func_door_model",          &idSGameMover::SP_func_door_model },
    { "func_static",              &idSGameMover::SP_func_static },
    { "func_rotating",            &idSGameMover::SP_func_rotating },
    { "func_bobbing",             &idSGameMover::SP_func_bobbing },
    { "func_pendulum",            &idSGameMover::SP_func_pendulum },
    { "func_train",               &idSGameMover::SP_func_train },
    { "func_group",               &idSGameMisc::SP_info_null },
    { "func_timer",               &idSGameTrigger::SP_func_timer },      // rename trigger_timer?

    // Triggers are brush objects that cause an effect when contacted
    // by a living player, usually involving firing targets.
    // While almost everything could be done with
    // a single trigger class and different targets, triggered effects
    // could not be client side predicted (push and teleport).
    { "trigger_always",           &idSGameTrigger::SP_trigger_always },
    { "trigger_multiple",         &idSGameTrigger::SP_trigger_multiple },
    { "trigger_push",             &idSGameTrigger::SP_trigger_push },
    { "trigger_teleport",         &idSGameTrigger::SP_trigger_teleport },
    { "trigger_hurt",             &idSGameTrigger::SP_trigger_hurt },
    { "trigger_stage",            &idSGameTrigger::SP_trigger_stage },
    { "trigger_win",              &idSGameTrigger::SP_trigger_win },
    { "trigger_buildable",        &idSGameTrigger::SP_trigger_buildable },
    { "trigger_class",            &idSGameTrigger::SP_trigger_class },
    { "trigger_equipment",        &idSGameTrigger::SP_trigger_equipment },
    { "trigger_gravity",          &idSGameTrigger::SP_trigger_gravity },
    { "trigger_heal",             &idSGameTrigger::SP_trigger_heal },
    { "trigger_ammo",             &idSGameTrigger::SP_trigger_ammo },

    // targets perform no action by themselves, but must be triggered
    // by another entity
    { "target_delay",             &idSGameTarget::SP_target_delay },
    { "target_speaker",           &idSGameTarget::SP_target_speaker },
    { "target_print",             &idSGameTarget::SP_target_print },
    { "target_score",             &idSGameTarget::SP_target_score },
    { "target_teleporter",        &idSGameTarget::SP_target_teleporter },
    { "target_relay",             &idSGameTarget::SP_target_relay },
    { "target_kill",              &idSGameTarget::SP_target_kill },
    { "target_position",          &idSGameTarget::SP_target_position },
    { "target_location",          &idSGameTarget::SP_target_location },
    { "target_rumble",            &idSGameTarget::SP_target_rumble },
    { "target_alien_win",         &idSGameTarget::SP_target_alien_win },
    { "target_human_win",         &idSGameTarget::SP_target_human_win },
    { "target_hurt",              &idSGameTarget::SP_target_hurt },
    { "target_push",              &idSGameTrigger::SP_target_push },

    { "light",                    &idSGameMisc::SP_light },
    { "path_corner",              &idSGameMover::SP_path_corner },

    { "misc_teleporter_dest",     &idSGameMisc::SP_misc_teleporter_dest },
    { "misc_model",               &idSGameMisc::SP_misc_model },
    { "misc_portal_surface",      &idSGameMisc::SP_misc_portal_surface },
    { "misc_portal_camera",       &idSGameMisc::SP_misc_portal_camera },

    { "misc_particle_system",     &idSGameMisc::SP_misc_particle_system },
    { "misc_anim_model",          &idSGameMisc::SP_misc_anim_model },
    { "misc_light_flare",         &idSGameMisc::SP_misc_light_flare },

    { "misc_cubemap",             &idSGameMisc::SP_misc_cubemap },

    { nullptr, 0 }
};

/*
===============
idSGameSpawn::CallSpawn

Finds the spawn function for the entity and calls it,
returning false if not found
===============
*/
bool idSGameSpawn::CallSpawn(gentity_t *ent) {
    spawn_t     *s;
    buildable_t buildable;

    if(!ent->classname) {
        idSGameMain::Printf("G_CallSpawn: nullptr classname\n");
        return false;
    }

    //check buildable spawn functions
    buildable = bggame->BuildableByEntityName(ent->classname)->number;

    if(buildable != BA_NONE) {
        // don't spawn built-in buildings if we are using a custom layout
        if(level.layout[ 0 ] && Q_stricmp(level.layout, "*BUILTIN*")) {
            return true;
        }

        if(buildable == BA_A_SPAWN || buildable == BA_H_SPAWN) {
            ent->s.angles[ YAW ] += 180.0f;
            AngleNormalize360(ent->s.angles[ YAW ]);
        }

        idSGameBuildable::SpawnBuildable(ent, buildable);
        return true;
    }

    // check normal spawn functions
    for(s = spawns; s->name; s++) {
        if(!strcmp(s->name, ent->classname)) {
            // found it
            s->spawn(ent);
            return true;
        }
    }

    idSGameMain::Printf("%s doesn't have a spawn function\n", ent->classname);
    return false;
}

/*
=============
idSGameSpawn::NewString

Builds a copy of the string, translating \n to real linefeeds
so message texts can be multi-line
=============
*/
valueType *idSGameSpawn::NewString(pointer string) {
    valueType  *newb, *new_p;
    sint   i, l;

    l = strlen(string) + 1;

    newb = (valueType *)bggame->Alloc(l);

    new_p = newb;

    // turn \n into a real linefeed
    for(i = 0 ; i < l ; i++) {
        if(string[ i ] == '\\' && i < l - 1) {
            i++;

            if(string[ i ] == 'n') {
                *new_p++ = '\n';
            } else {
                *new_p++ = '\\';
            }
        } else {
            *new_p++ = string[ i ];
        }
    }

    return newb;
}

/*
===============
idSGameSpawn::ParseField

Takes a key/value pair and sets the binary values
in a gentity
===============
*/
void idSGameSpawn::ParseField(pointer key, pointer value, gentity_t *ent) {
    field_t *f;
    uchar8 *b;
    float32 v;
    vec3_t vec;
    vec4_t vec4;

    for(f = fields; f->name; f++) {
        if(!Q_stricmp(f->name, key)) {
            // found it
            b = (uchar8 *)ent;

            switch(f->type) {
                case F_LSTRING:
                    *(valueType **)(b + f->ofs) = NewString(value);
                    break;

                case F_VECTOR:
                    sscanf(value, "%f %f %f", &vec[ 0 ], &vec[ 1 ], &vec[ 2 ]);

                    ((float32 *)(b + f->ofs))[ 0 ] = vec[ 0 ];
                    ((float32 *)(b + f->ofs))[ 1 ] = vec[ 1 ];
                    ((float32 *)(b + f->ofs))[ 2 ] = vec[ 2 ];
                    break;

                case F_VECTOR4:
                    sscanf(value, "%f %f %f %f", &vec4[ 0 ], &vec4[ 1 ], &vec4[ 2 ],
                           &vec4[ 3 ]);

                    ((float32 *)(b + f->ofs))[ 0 ] = vec4[ 0 ];
                    ((float32 *)(b + f->ofs))[ 1 ] = vec4[ 1 ];
                    ((float32 *)(b + f->ofs))[ 2 ] = vec4[ 2 ];
                    ((float32 *)(b + f->ofs))[ 3 ] = vec4[ 3 ];
                    break;

                case F_INT:
                    *(sint *)(b + f->ofs) = atoi(value);
                    break;

                case F_FLOAT:
                    *(float32 *)(b + f->ofs) = atof(value);
                    break;

                case F_ANGLEHACK:
                    v = atof(value);
                    ((float32 *)(b + f->ofs))[ 0 ] = 0;
                    ((float32 *)(b + f->ofs))[ 1 ] = v;
                    ((float32 *)(b + f->ofs))[ 2 ] = 0;
                    break;

                default:
                case F_IGNORE:
                    break;
            }

            return;
        }
    }
}

/*
===================
idSGameSpawn::SpawnGEntityFromSpawnVars

Spawn an entity and fill in all of the level fields from
level.spawnVars[], then call the class specfic spawn function
===================
*/
void idSGameSpawn::SpawnGEntityFromSpawnVars(void) {
    sint i;
    gentity_t *ent;

    // get the next free entity
    ent = idSGameUtils::Spawn();

    for(i = 0; i < level.numSpawnVars; i++) {
        ParseField(level.spawnVars[i][0], level.spawnVars[i][1], ent);
    }

    SpawnInt("notq3a", "0", &i);

    if(i) {
        idSGameUtils::FreeEntity(ent);
        return;
    }

    // move editor origin to pos
    VectorCopy(ent->s.origin, ent->s.pos.trBase);
    VectorCopy(ent->s.origin, ent->r.currentOrigin);

    // if we didn't get a classname, don't bother spawning anything
    if(!CallSpawn(ent)) {
        idSGameUtils::FreeEntity(ent);
    }
}

/*
====================
idSGameSpawn::AddSpawnVarToken
====================
*/
valueType *idSGameSpawn::AddSpawnVarToken(pointer string) {
    sint l;
    valueType  *dest;

    l = strlen(string);

    if(level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS) {
        idSGameMain::Error("G_AddSpawnVarToken: MAX_SPAWN_CHARS");
    }

    dest = level.spawnVarChars + level.numSpawnVarChars;
    ::memcpy(dest, string, l + 1);

    level.numSpawnVarChars += l + 1;

    return dest;
}

/*
====================
idSGameSpawn::ParseSpawnVars

Parses a brace bounded set of key / value pairs out of the
level's entity strings into level.spawnVars[]

This does not actually spawn an entity.
====================
*/
bool idSGameSpawn::ParseSpawnVars(void) {
    valueType keyname[ MAX_TOKEN_CHARS ];
    valueType com_token[ MAX_TOKEN_CHARS ];

    level.numSpawnVars = 0;
    level.numSpawnVarChars = 0;

    // parse the opening brace
    if(!trap_GetEntityToken(com_token, sizeof(com_token))) {
        // end of spawn string
        return false;
    }

    if(com_token[0] != '{') {
        idSGameMain::Error("idSGameSpawn::ParseSpawnVars: found %s when expecting {",
                           com_token);
    }

    // go through all the key / value pairs
    while(1) {
        // parse key
        if(!trap_GetEntityToken(keyname, sizeof(keyname))) {
            idSGameMain::Error("idSGameSpawn::ParseSpawnVars: EOF without closing brace");
        }

        if(keyname[0] == '}') {
            break;
        }

        // parse value
        if(!trap_GetEntityToken(com_token, sizeof(com_token))) {
            idSGameMain::Error("idSGameSpawn::ParseSpawnVars: EOF without closing brace");
        }

        if(com_token[0] == '}') {
            idSGameMain::Error("idSGameSpawn::ParseSpawnVars: closing brace without data");
        }

        if(level.numSpawnVars == MAX_SPAWN_VARS) {
            idSGameMain::Error("idSGameSpawn::ParseSpawnVars: MAX_SPAWN_VARS");
        }

        level.spawnVars[ level.numSpawnVars ][ 0 ] = AddSpawnVarToken(keyname);
        level.spawnVars[ level.numSpawnVars ][ 1 ] = AddSpawnVarToken(com_token);
        level.numSpawnVars++;
    }

    return true;
}

/*QUAKED worldspawn (0 0 0) ?

Every map should have exactly one worldspawn.
"music"   music wav file
"gravity" 800 is default gravity
"message" Text to print during connection process
*/
void idSGameSpawn::SP_worldspawn(void) {
    valueType *s;

    SpawnString("classname", "", &s);

    if(Q_stricmp(s, "worldspawn")) {
        idSGameMain::Error("idSGameSpawn::SP_worldspawn: The first entity isn't 'worldspawn'");
    }

    // make some data visible to connecting client
    trap_SetConfigstring(CS_GAME_VERSION, GAME_VERSION);

    trap_SetConfigstring(CS_LEVEL_START_TIME, va("%i", level.startTime));

    SpawnString("music", "", &s);
    trap_SetConfigstring(CS_MUSIC, s);

    SpawnString("message", "", &s);
    trap_SetConfigstring(CS_MESSAGE, s);          // map specific message

    trap_SetConfigstring(CS_MOTD, g_motd.string);     // message of the day

    SpawnString("gravity", "800", &s);
    trap_Cvar_Set("g_gravity", s);

    SpawnString("humanBuildPoints", DEFAULT_HUMAN_BUILDPOINTS, &s);
    trap_Cvar_Set("g_humanBuildPoints", s);

    SpawnString("humanMaxStage", DEFAULT_HUMAN_MAX_STAGE, &s);
    trap_Cvar_Set("g_humanMaxStage", s);

    //for compatibility with 1.1 maps
    if(SpawnString("humanStage2Threshold", DEFAULT_HUMAN_STAGE_THRESH, &s)) {
        trap_Cvar_Set("g_humanStageThreshold", s);
    } else {
        //proper way
        SpawnString("humanStageThreshold", DEFAULT_HUMAN_STAGE_THRESH, &s);
        trap_Cvar_Set("g_humanStageThreshold", s);
    }

    SpawnString("alienBuildPoints", DEFAULT_ALIEN_BUILDPOINTS, &s);
    trap_Cvar_Set("g_alienBuildPoints", s);

    SpawnString("alienMaxStage", DEFAULT_ALIEN_MAX_STAGE, &s);
    trap_Cvar_Set("g_alienMaxStage", s);

    if(SpawnString("alienStage2Threshold", DEFAULT_ALIEN_STAGE_THRESH, &s)) {
        trap_Cvar_Set("g_alienStageThreshold", s);
    } else {
        SpawnString("alienStage2Threshold", DEFAULT_ALIEN_STAGE_THRESH, &s);
        trap_Cvar_Set("g_alienStageThreshold", s);
    }

    SpawnString("enableDust", "0", &s);
    trap_Cvar_Set("g_enableDust", s);

    SpawnString("enableBreath", "0", &s);
    trap_Cvar_Set("g_enableBreath", s);

    SpawnString("disabledEquipment", "", &s);
    trap_Cvar_Set("g_disabledEquipment", s);

    SpawnString("disabledClasses", "", &s);
    trap_Cvar_Set("g_disabledClasses", s);

    SpawnString("disabledBuildables", "", &s);
    trap_Cvar_Set("g_disabledBuildables", s);

    g_entities[ ENTITYNUM_WORLD ].s.number = ENTITYNUM_WORLD;
    g_entities[ ENTITYNUM_WORLD ].classname = "worldspawn";

    // see if we want a warmup time
    trap_SetConfigstring(CS_WARMUP, "");

    if(g_restarted.integer) {
        trap_Cvar_Set("g_restarted", "0");
        level.warmupTime = 0;
    } else if(g_doWarmup.integer) {
        // Turn it on
        level.warmupTime = -1;
        trap_SetConfigstring(CS_WARMUP, va("%i", level.warmupTime));
        idSGameMain::LogPrintf("Warmup:\n");
    }

}
bool idSGameSpawn::ParseSpawnVarsFromFile(char **data) {
    char    keyname[MAX_TOKEN_CHARS];
    char    com_token[MAX_TOKEN_CHARS];

    level.numSpawnVars = 0;
    level.numSpawnVarChars = 0;

    Q_vsprintf_s(com_token, sizeof(com_token), sizeof(com_token), "%s",
                 COM_Parse(data));

    if(com_token[0] == 0) {
        return false;
    }

    if(com_token[0] != '{') {
        COM_ParseWarning("found '%s' when expecting {", com_token);
        return false;
    }

    while(1) {
        Q_vsprintf_s(keyname, sizeof(keyname), sizeof(keyname), "%s",
                     COM_Parse(data));

        if(!keyname[0]) {
            COM_ParseWarning("EOF without closing brace");
            return false;
        }

        if(keyname[0] == '}') {
            break;
        }

        Q_vsprintf_s(com_token, sizeof(com_token), sizeof(com_token), "%s",
                     COM_Parse(data));

        if(!com_token[0]) {
            COM_ParseWarning("EOF without closing brace");
            return false;
        }

        if(com_token[0] == '}') {
            COM_ParseWarning("closing brace without data");
            return false;
        }

        if(level.numSpawnVars == MAX_SPAWN_VARS) {
            COM_ParseWarning("MAX_SPAWN_VARS");
            return false;
        }

        level.spawnVars[level.numSpawnVars][0] = AddSpawnVarToken(keyname);
        level.spawnVars[level.numSpawnVars][1] = AddSpawnVarToken(com_token);
        level.numSpawnVars++;
    }

    return true;
}

#define MEM_CONFIG  4096
bool idSGameSpawn::SpawnEntitiesFromFile(const char *filename, bool num) {
    fileHandle_t    fileHandle;
    int     len;
    char        buf[MEM_CONFIG];
    char       *text;
    char info[1024];

    trap_GetServerinfo(info, sizeof(info));

    len = trap_FS_FOpenFile(filename, &fileHandle, FS_READ);

    if(!fileHandle) {
        Com_Printf("cant load: %s\n", filename);
        return false;
    }

    if(len <= 0) {
        Com_Printf("file %s is empty\n", filename);
        trap_FS_FCloseFile(fileHandle);
        return false;
    }

    if(len > MEM_CONFIG) {
        Com_Printf("file too large: %s is %i, max allowed is %i\n", filename, len,
                   MEM_CONFIG);
        trap_FS_FCloseFile(fileHandle);
        return false;
    }

    // xDiloc - read file
    trap_FS_Read(buf, len, fileHandle);
    buf[len] = 0;
    trap_FS_FCloseFile(fileHandle);

    COM_BeginParseSession(filename);
    text = buf;

    // xDiloc - parse
    if(num) {
        if(!ParseSpawnVarsFromFile(&text)) {
            COM_ParseWarning("SpawnEntities: no entities");
        }

        SP_worldspawn();
    }

    while(ParseSpawnVarsFromFile(&text)) {
        SpawnGEntityFromSpawnVars();
    }

    return true;
}


/*
==============
idSGameSpawn::SpawnEntitiesFromString

Parses textual entity definitions out of an entstring and spawns gentities.
==============
*/
void idSGameSpawn::SpawnEntitiesFromString(void) {
    // allow calls to idSGameSpawn::Spawn*()
    level.spawning = true;
    level.numSpawnVars = 0;

    // the worldspawn is not an actual entity, but it still
    // has a "spawn" function to perform any global setup
    // needed by a level (setting configstrings or cvars, etc)
    if(!ParseSpawnVars()) {
        idSGameMain::Error("SpawnEntities: no entities");
    }

    SP_worldspawn();

    // parse ents
    while(ParseSpawnVars()) {
        SpawnGEntityFromSpawnVars();
    }

    //Dushan - overwrite default map "worldspawn" flag with new settings/name
    SpawnEntitiesFromFile(va("maps/%s.ent", mapname.string), true);

    level.spawning =
        false;      // any future calls to G_Spawn*() will be errors
}

