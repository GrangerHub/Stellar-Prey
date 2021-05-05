////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2004 - 2006 Tony J.White
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// This shrubbot implementation is the original work of Tony J.White.
//
// Contains contributions from Wesley van Beelen, Chris Bajumpaa, Josh Menke,
// and Travis Maurer.
//
// The functionality of this code mimics the behaviour of the currently
// inactive project shrubet(http://www.etstats.com/shrubet/index.php?ver=2)
// by Ryan Mannion.However, shrubet was a closed - source project and
// none of it's code has been copied, only it's functionality.
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
// -------------------------------------------------------------------------------------
// File name:   sgame_admin.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

idAdminLocal adminLocal;

// big ugly global buffer for use with buffered printing of long outputs
static valueType g_bfb[ 32000 ];

// note: list ordered alphabetically
g_admin_cmd_t g_admin_cmds[ ] = {
    {
        "adjustban", &idAdminLocal::AdminAdjustBan, "b",
        "change the duration or reason of a ban.  duration is specified as "
        "numbers followed by units 'w' (weeks), 'd' (days), 'h' (hours) or "
        "'m' (minutes), or seconds if no units are specified.  if the duration is"
        " preceded by a + or -, the ban duration will be extended or shortened by"
        " the specified amount",
        "[^3ban#^7] (^5duration^7) (^5reason^7)"
    },

    {
        "admintest", &idAdminLocal::AdminAdminTest, "a",
        "display your current admin level",
        ""
    },

    {
        "allowbuild", &idAdminLocal::AdminDenyBuild, "d",
        "restore a player's ability to build",
        "[^3name|slot#^7]"
    },

    {
        "allready", &idAdminLocal::AdminAllReady, "y",
        "makes everyone ready in intermission",
        ""
    },

    {
        "ban", &idAdminLocal::AdminBan, "b",
        "ban a player by IP and GUID with an optional expiration time and reason."
        " duration is specified as numbers followed by units 'w' (weeks), 'd' "
        "(days), 'h' (hours) or 'm' (minutes), or seconds if no units are "
        "specified",
        "[^3name|slot#|IP^7] (^5duration^7) (^5reason^7)"
    },

    {
        "cancelvote", &idAdminLocal::AdminCancelVote, "c",
        "cancel a vote taking place",
        ""
    },

    {
        "denybuild", &idAdminLocal::AdminDenyBuild, "d",
        "take away a player's ability to build",
        "[^3name|slot#^7]"
    },

    {
        "help", &idAdminLocal::AdminHelp, "h",
        "display commands available to you or help on a specific command",
        "(^5command^7)"
    },

    {
        "kick", &idAdminLocal::AdminKick, "k",
        "kick a player with an optional reason",
        "[^3name|slot#^7] (^5reason^7)"
    },

    {
        "listadmins", &idAdminLocal::AdminListAdmins, "D",
        "display a list of all server admins and their levels",
        "(^5name|start admin#^7)"
    },

    {
        "listlayouts", &idAdminLocal::AdminListLayouts, "L",
        "display a list of all available layouts for a map",
        "(^5mapname^7)"
    },

    {
        "listplayers", &idAdminLocal::AdminListPlayers, "i",
        "display a list of players, their client numbers and their levels",
        ""
    },

    {
        "lock", &idAdminLocal::AdminLock, "K",
        "lock a team to prevent anyone from joining it",
        "[^3a|h^7]"
    },

    {
        "map", &idAdminLocal::AdminMap, "M",
        "load a map (and optionally force layout)",
        "[^3mapname^7] (^5layout^7)"
    },

    {
        "mute", &idAdminLocal::AdminMute, "m",
        "mute a player",
        "[^3name|slot#^7]"
    },

    {
        "namelog", &idAdminLocal::AdminNameLog, "e",
        "display a list of names used by recently connected players",
        "(^5name^7)"
    },

    {
        "nextmap", &idAdminLocal::AdminNextmap, "n",
        "go to the next map in the cycle",
        ""
    },

    {
        "passvote", &idAdminLocal::AdminPassVote, "V",
        "pass a vote currently taking place",
        ""
    },

    {
        "putteam", &idAdminLocal::AdminPutTeam, "p",
        "move a player to a specified team",
        "[^3name|slot#^7] [^3h|a|s^7]"
    },

    {
        "readconfig", &idAdminLocal::AdminReadconfig, "G",
        "reloads the admin config file and refreshes permission flags",
        ""
    },

    {
        "rename", &idAdminLocal::AdminRename, "N",
        "rename a player",
        "[^3name|slot#^7] [^3new name^7]"
    },

    {
        "restart", &idAdminLocal::AdminRestart, "r",
        "restart the current map (optionally using named layout)",
        "(^5layout^7)"
    },

    {
        "setlevel", &idAdminLocal::AdminSetlevel, "s",
        "sets the admin level of a player",
        "[^3name|slot#|admin#^7] [^3level^7]"
    },

    {
        "showbans", &idAdminLocal::AdminShowBans, "B",
        "display a (partial) list of active bans",
        "(^5start at ban#^7)"
    },

    {
        "spec999", &idAdminLocal::AdminSpec999, "P",
        "move 999 pingers to the spectator team",
        ""
    },

    {
        "time", &idAdminLocal::AdminTime, "C",
        "show the current local server time",
        ""
    },

    {
        "unban", &idAdminLocal::AdminUnBan, "b",
        "unbans a player specified by the slot as seen in showbans",
        "[^3ban#^7]"
    },

    {
        "unlock", &idAdminLocal::AdminUnlock, "K",
        "unlock a locked team",
        "[^3a|h^7]"
    },

    {
        "unmute", &idAdminLocal::AdminMute, "m",
        "unmute a muted player",
        "[^3name|slot#^7]"
    },
};

static sint adminNumCmds = sizeof(g_admin_cmds) / sizeof(
                               g_admin_cmds[ 0 ]);
static sint admin_level_maxname = 0;

g_admin_level_t *g_admin_levels[ MAX_ADMIN_LEVELS ];
g_admin_admin_t *g_admin_admins[ MAX_ADMIN_ADMINS ];
g_admin_ban_t *g_admin_bans[ MAX_ADMIN_BANS ];
g_admin_command_t *g_admin_commands[ MAX_ADMIN_COMMANDS ];
g_admin_namelog_t *g_admin_namelog[ MAX_ADMIN_NAMELOGS ];

/*
===============
idAdminLocal::AdminPermission
===============
*/
bool idAdminLocal::AdminPermission(gentity_t *ent, valueType flag) {
    sint i, l = 0;
    valueType *flags;

    // console always wins
    if(!ent) {
        return true;
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(!Q_stricmp(ent->client->pers.guid, g_admin_admins[ i ]->guid)) {
            flags = g_admin_admins[ i ]->flags;

            while(*flags) {
                if(*flags == flag) {
                    return true;
                } else if(*flags == '-') {
                    while(*flags++) {
                        if(*flags == flag) {
                            return false;
                        }

                        if(*flags == '+') {
                            break;
                        }
                    }
                } else if(*flags == '*') {
                    while(*flags++) {
                        if(*flags == flag) {
                            return false;
                        }
                    }

                    // flags with significance only for individuals (
                    // like ADMF_INCOGNITO and ADMF_IMMUTABLE are NOT covered
                    // by the '*' wildcard.  They must be specified manually.
                    return (flag != ADMF_INCOGNITO && flag != ADMF_IMMUTABLE);
                }

                flags++;
            }

            l = g_admin_admins[ i ]->level;
        }
    }

    for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
        if(g_admin_levels[ i ]->level == l) {
            flags = g_admin_levels[ i ]->flags;

            while(*flags) {
                if(*flags == flag) {
                    return true;
                }

                if(*flags == '*') {
                    while(*flags++) {
                        if(*flags == flag) {
                            return false;
                        }
                    }

                    // flags with significance only for individuals (
                    // like ADMF_INCOGNITO and ADMF_IMMUTABLE are NOT covered
                    // by the '*' wildcard.  They must be specified manually.
                    return (flag != ADMF_INCOGNITO && flag != ADMF_IMMUTABLE);
                }

                flags++;
            }
        }
    }

    return false;
}

/*
===============
idAdminLocal::AdminNameCheck
===============
*/
bool idAdminLocal::AdminNameCheck(gentity_t *ent, valueType *name,
                                  valueType *err, sint len) {
    sint i;
    gclient_t *client;
    valueType testName[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType name2[ MAX_COLORFUL_NAME_LENGTH ] = {""};

    idSGameCmds::SanitiseString(name, name2, sizeof(name2));

    if(!Q_stricmp(name2, "UnnamedPlayer")) {
        return true;
    }

    for(i = 0; i < level.maxclients; i++) {
        client = &level.clients[ i ];

        if(client->pers.connected == CON_DISCONNECTED) {
            continue;
        }

        // can rename ones self to the same name using different colors
        if(i == (ent - g_entities)) {
            continue;
        }

        idSGameCmds::SanitiseString(client->pers.netname, testName,
                                    sizeof(testName));

        if(!Q_stricmp(name2, testName)) {
            Q_vsprintf_s(err, len, len, "The name '%s^7' is already in use", name);
            return false;
        }
    }

    if(!g_adminNameProtect.integer) {
        return true;
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(g_admin_admins[i]->level < 1) {
            continue;
        }

        idSGameCmds::SanitiseString(g_admin_admins[ i ]->name, testName,
                                    sizeof(testName));

        if(!Q_stricmp(name2, testName) &&
                Q_stricmp(ent->client->pers.guid, g_admin_admins[ i ]->guid)) {
            Q_vsprintf_s(err, len, len,
                         "The name '%s^7' belongs to an admin, please use another name", name);
            return false;
        }
    }

    return true;
}

/*
===============
idAdminLocal::SetClientSound
===============
*/
bool idAdminLocal::AdminHigherGuid(valueType *admin_guid,
                                   valueType *victim_guid) {
    sint i, alevel = 0;

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(!Q_stricmp(admin_guid, g_admin_admins[ i ]->guid)) {
            alevel = g_admin_admins[ i ]->level;
            break;
        }
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(!Q_stricmp(victim_guid, g_admin_admins[ i ]->guid)) {
            if(alevel < g_admin_admins[i]->level) {
                return false;
            }

            return !strstr(g_admin_admins[ i ]->flags, va("%c", ADMF_IMMUTABLE));
        }
    }

    return true;
}

/*
===============
idAdminLocal::AdminHigher
===============
*/
bool idAdminLocal::AdminHigher(gentity_t *admin, gentity_t *victim) {
    // console always wins
    if(!admin) {
        return true;
    }

    // just in case
    if(!victim) {
        return true;
    }

    return AdminHigherGuid(admin->client->pers.guid,
                           victim->client->pers.guid);
}

/*
===============
idAdminLocal::SetClientSound
===============
*/
void idAdminLocal::AdminWriteConfigString(valueType *s, fileHandle_t f) {
    valueType buf[ MAX_STRING_CHARS ];

    buf[ 0 ] = '\0';

    if(s[ 0 ]) {
        //Q_strcat(buf, sizeof(buf), s);
        Q_strncpyz(buf, s, sizeof(buf));
        trap_FS_Write(buf, strlen(buf), f);
    }

    trap_FS_Write("\n", 1, f);
}

/*
===============
idAdminLocal::AdminWriteConfigInt
===============
*/
void idAdminLocal::AdminWriteConfigInt(sint v, fileHandle_t f) {
    valueType buf[ 32 ];

    Q_vsprintf_s(buf, sizeof(buf), sizeof(buf), "%d", v);

    trap_FS_Write(buf, strlen(buf), f);
    trap_FS_Write("\n", 1, f);
}

/*
===============
idAdminLocal::SetClientSound
===============
*/
void idAdminLocal::AdminWriteConfig(void) {
    fileHandle_t f;
    sint len, i, j, t;
    valueType levels[ MAX_STRING_CHARS ] = {""};

    if(!g_admin.string[ 0 ]) {
        idSGameMain::Printf(S_COLOR_YELLOW
                            "WARNING: g_admin is not set. configuration will not be saved to a file.\n");
        return;
    }

    t = trap_RealTime(nullptr);

    len = trap_FS_FOpenFile(g_admin.string, &f, FS_WRITE);

    if(len < 0) {
        idSGameMain::Printf("admin_writeconfig: could not open g_admin file \"%s\"\n",
                            g_admin.string);
        return;
    }

    for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
        trap_FS_Write("[level]\n", 8, f);
        trap_FS_Write("level   = ", 10, f);
        AdminWriteConfigInt(g_admin_levels[ i ]->level, f);
        trap_FS_Write("name    = ", 10, f);
        AdminWriteConfigString(g_admin_levels[ i ]->name, f);
        trap_FS_Write("flags   = ", 10, f);
        AdminWriteConfigString(g_admin_levels[ i ]->flags, f);
        trap_FS_Write("\n", 1, f);
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        // don't write level 0 users
        if(g_admin_admins[i]->level == 0) {
            continue;
        }

        trap_FS_Write("[admin]\n", 8, f);
        trap_FS_Write("name    = ", 10, f);
        AdminWriteConfigString(g_admin_admins[ i ]->name, f);
        trap_FS_Write("guid    = ", 10, f);
        AdminWriteConfigString(g_admin_admins[ i ]->guid, f);
        trap_FS_Write("level   = ", 10, f);
        AdminWriteConfigInt(g_admin_admins[ i ]->level, f);
        trap_FS_Write("flags   = ", 10, f);
        AdminWriteConfigString(g_admin_admins[ i ]->flags, f);
        trap_FS_Write("counter = ", 10, f);
        AdminWriteConfigInt(g_admin_admins[i]->counter, f);
        trap_FS_Write("\n", 1, f);
    }

    for(i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++) {
        // don't write expired bans
        // if expires is 0, then it's a perm ban
        if(g_admin_bans[i]->expires != 0 && (g_admin_bans[i]->expires - t) < 1) {
            continue;
        }

        trap_FS_Write("[ban]\n", 6, f);
        trap_FS_Write("name    = ", 10, f);
        AdminWriteConfigString(g_admin_bans[ i ]->name, f);
        trap_FS_Write("guid    = ", 10, f);
        AdminWriteConfigString(g_admin_bans[ i ]->guid, f);
        trap_FS_Write("ip      = ", 10, f);
        AdminWriteConfigString(g_admin_bans[ i ]->ip, f);
        trap_FS_Write("reason  = ", 10, f);
        AdminWriteConfigString(g_admin_bans[ i ]->reason, f);
        trap_FS_Write("made    = ", 10, f);
        AdminWriteConfigString(g_admin_bans[ i ]->made, f);
        trap_FS_Write("expires = ", 10, f);
        AdminWriteConfigInt(g_admin_bans[ i ]->expires, f);
        trap_FS_Write("banner  = ", 10, f);
        AdminWriteConfigString(g_admin_bans[ i ]->banner, f);
        trap_FS_Write("\n", 1, f);
    }

    for(i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++) {
        levels[ 0 ] = '\0';
        trap_FS_Write("[command]\n", 10, f);
        trap_FS_Write("command = ", 10, f);
        AdminWriteConfigString(g_admin_commands[ i ]->command, f);
        trap_FS_Write("exec    = ", 10, f);
        AdminWriteConfigString(g_admin_commands[ i ]->exec, f);
        trap_FS_Write("desc    = ", 10, f);
        AdminWriteConfigString(g_admin_commands[ i ]->desc, f);
        trap_FS_Write("levels  = ", 10, f);

        for(j = 0; g_admin_commands[ i ]->levels[ j ] != -1; j++) {
            Q_strcat(levels, sizeof(levels), va("%i ",
                                                g_admin_commands[ i ]->levels[ j ]));
        }

        AdminWriteConfigString(levels, f);

        trap_FS_Write("\n", 1, f);
    }

    trap_FS_FCloseFile(f);
}

/*
===============
idAdminLocal::AdminReadConfigString
===============
*/
void idAdminLocal::AdminReadConfigString(valueType **cnf, valueType *s,
        sint size) {
    valueType *t;

    //COM_MatchToken(cnf, "=");
    s[ 0 ] = '\0';
    t = COM_ParseExt(cnf, false);

    if(strcmp(t, "=")) {
        COM_ParseWarning("expected '=' before \"%s\"", t);
        Q_strncpyz(s, t, size);
    }

    while(1) {
        t = COM_ParseExt(cnf, false);

        if(!*t) {
            break;
        }

        if(strlen(t) + strlen(s) >= size) {
            break;
        }

        if(*s) {
            Q_strcat(s, size, " ");
        }

        Q_strcat(s, size, t);
    }
}

/*
===============
idAdminLocal::AdminReadConfigInt
===============
*/
void idAdminLocal::AdminReadConfigInt(valueType **cnf, sint *v) {
    valueType *t;

    //COM_MatchToken(cnf, "=");
    t = COM_ParseExt(cnf, false);

    if(!strcmp(t, "=")) {
        t = COM_ParseExt(cnf, false);
    } else {
        COM_ParseWarning("expected '=' before \"%s\"", t);
    }

    *v = atoi(t);
}


/*
===============
idAdminLocal::AdminDefaultLevels

if we can't parse any levels from readconfig, set up default
ones to make new installs easier for admins
===============
*/
void idAdminLocal::AdminDefaultLevels(void) {
    g_admin_level_t *l;
    sint i;

    for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
        bggame->Free(g_admin_levels[ i ]);
        g_admin_levels[ i ] = nullptr;
    }

    for(i = 0; i <= 5; i++) {
        l = (g_admin_level_t *)bggame->Alloc(sizeof(g_admin_level_t));
        l->level = i;
        *l->name = '\0';
        *l->flags = '\0';
        g_admin_levels[ i ] = l;
    }

    Q_strncpyz(g_admin_levels[ 0 ]->name, "^4Unknown Player", sizeof(l->name));
    Q_strncpyz(g_admin_levels[ 0 ]->flags, "iahC", sizeof(l->flags));

    Q_strncpyz(g_admin_levels[ 1 ]->name, "^5Server Regular", sizeof(l->name));
    Q_strncpyz(g_admin_levels[ 1 ]->flags, "iahC", sizeof(l->flags));

    Q_strncpyz(g_admin_levels[ 2 ]->name, "^6Team Manager", sizeof(l->name));
    Q_strncpyz(g_admin_levels[ 2 ]->flags, "iahCpP", sizeof(l->flags));

    Q_strncpyz(g_admin_levels[ 3 ]->name, "^2Junior Admin", sizeof(l->name));
    Q_strncpyz(g_admin_levels[ 3 ]->flags, "iahCpPkm$?", sizeof(l->flags));

    Q_strncpyz(g_admin_levels[ 4 ]->name, "^3Senior Admin", sizeof(l->name));
    Q_strncpyz(g_admin_levels[ 4 ]->flags, "iahCpPkmBbe$?", sizeof(l->flags));

    Q_strncpyz(g_admin_levels[ 5 ]->name, "^1Server Operator",
               sizeof(l->name));
    Q_strncpyz(g_admin_levels[ 5 ]->flags, "*", sizeof(l->flags));

    admin_level_maxname = 15;
}

/*
===============
idAdminLocal::AdminAdmin

return the admin struct for a player entity.
===============
*/
g_admin_admin_t *idAdminLocal::Admin(gentity_t *ent) {
    sint i;

    if(!ent) {
        return nullptr;
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(!Q_stricmp(g_admin_admins[ i ]->guid, ent->client->pers.guid)) {
            return g_admin_admins[i];
        }
    }

    return nullptr;
}

/*
===============
idAdminLocal::AdminCommandPermission
===============
*/
bool idAdminLocal::AdminCommandPermission(gentity_t *ent,
        valueType *command) {
    sint i, j, level;

    if(!ent) {
        return true;
    }

    level = ent->client->pers.adminLevel;

    for(i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++) {
        if(!Q_stricmp(command, g_admin_commands[ i ]->command)) {
            for(j = 0; g_admin_commands[ i ]->levels[ j ] != -1; j++) {
                if(g_admin_commands[ i ]->levels[ j ] == level) {
                    return true;
                }
            }
        }
    }

    return false;
}

/*
===============
idAdminLocal::AdminLog
===============
*/
void idAdminLocal::AdminLog(gentity_t *admin, valueType *cmd,
                            sint skiparg) {
    fileHandle_t f;
    sint len, i, j, min, tens, sec, pids[MAX_CLIENTS];
    valueType string[ MAX_STRING_CHARS ];
    g_admin_admin_t *a;
    g_admin_level_t *l;
    valueType flags[ MAX_ADMIN_FLAGS * 2 ];
    gentity_t *victim = nullptr;
    valueType name[ MAX_COLORFUL_NAME_LENGTH ];

    if(!g_adminLog.string[0]) {
        return;
    }

    len = trap_FS_FOpenFile(g_adminLog.string, &f, FS_APPEND);

    if(len < 0) {
        idSGameMain::Printf("idAdminLocal::AdminLog: error could not open %s\n",
                            g_adminLog.string);
        return;
    }

    sec = level.time / 1000;
    min = sec / 60;
    sec -= min * 60;
    tens = sec / 10;
    sec -= tens * 10;

    *flags = '\0';

    if(admin) {
        for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
            if(!Q_stricmp(g_admin_admins[ i ]->guid, admin->client->pers.guid)) {
                a = g_admin_admins[ i ];
                Q_strncpyz(flags, a->flags, sizeof(flags));

                for(j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++) {
                    if(g_admin_levels[ j ]->level == a->level) {
                        l = g_admin_levels[ j ];
                        Q_strcat(flags, sizeof(flags), l->flags);
                        break;
                    }
                }

                break;
            }
        }
    }

    if(idSGameCmds::SayArgc() > 1 + skiparg) {
        idSGameCmds::SayArgv(1 + skiparg, name, sizeof(name));

        if(idSGameCmds::ClientNumbersFromString(name, pids, MAX_CLIENTS,
                                                false) == 1) {
            victim = &g_entities[ pids[ 0 ] ];
        }
    }

    if(victim && Q_stricmp(cmd, "attempted")) {
        Q_vsprintf_s(string, sizeof(string), sizeof(string),
                     "%3i:%i%i: %i: %s: %s: %s: %s: %s: %s: \"%s\"\n",
                     min, tens, sec,
                     (admin) ? admin->s.clientNum : -1,
                     (admin) ? admin->client->pers.guid
                     : "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                     (admin) ? admin->client->pers.netname : "console",
                     flags, cmd,
                     victim->client->pers.guid,
                     victim->client->pers.netname,
                     idSGameCmds::SayConcatArgs(2 + skiparg));
    } else {
        Q_vsprintf_s(string, sizeof(string), sizeof(string),
                     "%3i:%i%i: %i: %s: %s: %s: %s: \"%s\"\n",
                     min, tens, sec,
                     (admin) ? admin->s.clientNum : -1,
                     (admin) ? admin->client->pers.guid
                     : "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                     (admin) ? admin->client->pers.netname : "console",
                     flags, cmd,
                     idSGameCmds::SayConcatArgs(1 + skiparg));
    }

    trap_FS_Write(string, strlen(string), f);
    trap_FS_FCloseFile(f);
}

/*
===============
idAdminLocal::AdminListAdmins
===============
*/
sint idAdminLocal::AdminListAdmins(gentity_t *ent, sint start,
                                   valueType *search) {
    sint drawn = 0;
    valueType guid_stub[9];
    valueType name[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType name2[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType lname[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    sint i, j;
    gentity_t *vic;
    sint l = 0;
    bool dup = false;

    adminLocal.ADMBP_begin();

    // print out all connected players regardless of level if name searching
    for(i = 0; i < level.maxclients && search[ 0 ]; i++) {
        vic = &g_entities[ i ];

        if(vic->client && vic->client->pers.connected != CON_CONNECTED) {
            continue;
        }

        l = vic->client->pers.adminLevel;

        idSGameCmds::SanitiseString(vic->client->pers.netname, name, sizeof(name));

        if(!strstr(name, search)) {
            continue;
        }

        for(j = 0; j < 8; j++) {
            guid_stub[j] = vic->client->pers.guid[j + 24];
        }

        guid_stub[ j ] = '\0';

        lname[ 0 ] = '\0';

        for(j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++) {
            if(g_admin_levels[ j ]->level == l) {
                sint k, colorlen;

                for(colorlen = k = 0; g_admin_levels[j]->name[k]; k++) {
                    if(Q_IsColorString(&g_admin_levels[j]->name[k])) {
                        colorlen +=
                            Q_ColorStringLength(&g_admin_levels[j]->name[k]);
                    } else if(Q_IsColorEscapeEscape(&g_admin_levels[j]->name[k])) {
                        colorlen++;
                    }
                }

                Q_vsprintf_s(lname, sizeof(lname), sizeof(lname), "%*s",
                             admin_level_maxname + colorlen, g_admin_levels[ j ]->name);
                break;
            }
        }

        adminLocal.ADMBP(va("%4i %4i %s^7 (*%s) %s^7\n", i, l, lname, guid_stub,
                            vic->client->pers.netname));
        drawn++;
    }

    for(i = start; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ] &&
            drawn < MAX_ADMIN_LISTITEMS; i++) {
        if(search[ 0 ]) {
            idSGameCmds::SanitiseString(g_admin_admins[ i ]->name, name, sizeof(name));

            if(!strstr(name, search)) {
                continue;
            }

            // verify we don't have the same guid/name pair in connected players
            // since we don't want to draw the same player twice
            dup = false;

            for(j = 0; j < level.maxclients; j++) {
                vic = &g_entities[ j ];

                if(!vic->client || vic->client->pers.connected != CON_CONNECTED) {
                    continue;
                }

                idSGameCmds::SanitiseString(vic->client->pers.netname, name2,
                                            sizeof(name2));

                if(!Q_stricmp(vic->client->pers.guid, g_admin_admins[ i ]->guid) &&
                        strstr(name2, search)) {
                    dup = true;
                    break;
                }
            }

            if(dup) {
                continue;
            }
        }

        for(j = 0; j < 8; j++) {
            guid_stub[j] = g_admin_admins[i]->guid[j + 24];
        }

        guid_stub[ j ] = '\0';

        lname[ 0 ] = '\0';

        for(j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++) {
            if(g_admin_levels[ j ]->level == g_admin_admins[ i ]->level) {
                sint k, colorlen;

                for(colorlen = k = 0; g_admin_levels[ j ]->name[ k ]; k++)
                    if(Q_IsColorString(&g_admin_levels[ j ]->name[ k ])) {
                        colorlen +=
                            Q_ColorStringLength(&g_admin_levels[j]->name[k]);
                    } else if(Q_IsColorEscapeEscape(&g_admin_levels[j]->name[k])) {
                        colorlen++;
                    }

                Q_vsprintf_s(lname, sizeof(lname), sizeof(lname), "%*s",
                             admin_level_maxname + colorlen,
                             g_admin_levels[ j ]->name);
                break;
            }
        }

        adminLocal.ADMBP(va("%4i %4i %s^7 (*%s) %s^7\n",
                            (i + MAX_CLIENTS),
                            g_admin_admins[ i ]->level,
                            lname,
                            guid_stub,
                            g_admin_admins[ i ]->name));
        drawn++;
    }

    adminLocal.ADMBP_end();

    return drawn;
}

/*
===============
idAdminLocal::AdminDuration
===============
*/
void idAdminLocal::AdminDuration(sint secs, valueType *duration,
                                 sint dursize) {

    if(secs > (60 * 60 * 24 * 365 * 50) || secs < 0) {
        Q_strncpyz(duration, "PERMANENT", dursize);
    } else if(secs >= (60 * 60 * 24 * 365)) {
        Q_vsprintf_s(duration, dursize, dursize, "%1.1f years",
                     (secs / (60 * 60 * 24 * 365.0f)));
    } else if(secs >= (60 * 60 * 24 * 90)) {
        Q_vsprintf_s(duration, dursize, dursize, "%1.1f weeks",
                     (secs / (60 * 60 * 24 * 7.0f)));
    } else if(secs >= (60 * 60 * 24)) {
        Q_vsprintf_s(duration, dursize, dursize, "%1.1f days",
                     (secs / (60 * 60 * 24.0f)));
    } else if(secs >= (60 * 60)) {
        Q_vsprintf_s(duration, dursize, dursize, "%1.1f hours",
                     (secs / (60 * 60.0f)));
    } else if(secs >= 60) {
        Q_vsprintf_s(duration, dursize, dursize, "%1.1f minutes", (secs / 60.0f));
    } else {
        Q_vsprintf_s(duration, dursize, dursize, "%i seconds", secs);
    }
}

/*
===============
idAdminLocal::AdminBanCheck
===============
*/
bool idAdminLocal::AdminBanCheck(valueType *userinfo, valueType *reason,
                                 sint rlen) {
    valueType *guid, *ip;
    sint i, t;

    *reason = '\0';
    t = trap_RealTime(nullptr);

    if(!*userinfo) {
        return false;
    }

    ip = Info_ValueForKey(userinfo, "ip");

    if(!*ip) {
        return false;
    }

    guid = Info_ValueForKey(userinfo, "cl_guid");

    for(i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++) {
        // 0 is for perm ban
        if(g_admin_bans[i]->expires != 0 && (g_admin_bans[i]->expires - t) < 1) {
            continue;
        }

        if(strstr(ip, g_admin_bans[ i ]->ip)) {
            valueType duration[ 32 ];
            AdminDuration((g_admin_bans[ i ]->expires - t), duration,
                          sizeof(duration));
            Q_vsprintf_s(reason, rlen, rlen,
                         "You have been banned by %s^7 reason: %s^7 expires: %s",
                         g_admin_bans[ i ]->banner, g_admin_bans[ i ]->reason, duration);

            idSGameMain::Printf("Banned player tried to connect from IP %s\n", ip);

            return true;
        }

        if(*guid && !Q_stricmp(g_admin_bans[ i ]->guid, guid)) {
            valueType duration[ 32 ];
            AdminDuration((g_admin_bans[ i ]->expires - t), duration,
                          sizeof(duration));
            Q_vsprintf_s(reason, rlen, rlen,
                         "You have been banned by %s^7 reason: %s^7 expires: %s",
                         g_admin_bans[ i ]->banner, g_admin_bans[ i ]->reason, duration);
            idSGameMain::Printf("Banned player tried to connect with GUID %s\n", guid);
            return true;
        }
    }

    return false;
}

/*
===============
idAdminLocal::AdminCmdCheck
===============
*/
bool idAdminLocal::AdminCmdCheck(gentity_t *ent, bool say) {
    sint i, skip = 0;
    valueType command[ MAX_ADMIN_CMD_LEN ];
    valueType *cmd;

    command[ 0 ] = '\0';
    idSGameCmds::SayArgv(0, command, sizeof(command));

    if(!command[0]) {
        return false;
    }

    if(!Q_stricmp(command, "say") || (!Q_stricmp(command, "say_team") &&
                                      AdminPermission(ent, ADMF_TEAMCHAT_CMD))) {
        skip = 1;
        idSGameCmds::SayArgv(1, command, sizeof(command));
    }

    if(command[ 0 ] == '!') {
        cmd = &command[ 1 ];
    } else {
        return false;
    }

    for(i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++) {
        if(Q_stricmp(cmd, g_admin_commands[ i ]->command)) {
            continue;
        }

        if(AdminCommandPermission(ent, cmd)) {
            // flooding say will have already been accounted for in ClientCommand
            if(!say && idSGameCmds::FloodLimited(ent)) {
                return true;
            }

            trap_SendConsoleCommand(EXEC_APPEND, g_admin_commands[ i ]->exec);
            AdminLog(ent, cmd, skip);
        } else {
            ADMP(va("^3!%s: ^7permission denied\n", g_admin_commands[ i ]->command));
            AdminLog(ent, "attempted", skip - 1);
        }

        return true;
    }

    for(i = 0; i < adminNumCmds; i++) {
        if(Q_stricmp(cmd, g_admin_cmds[i].keyword)) {
            continue;
        }

        if(AdminPermission(ent, g_admin_cmds[ i ].flag[ 0 ])) {
            // flooding say will have already been accounted for in ClientCommand
            if(!say && idSGameCmds::FloodLimited(ent)) {
                return true;
            }

            g_admin_cmds[ i ].handler(ent, skip);
            AdminLog(ent, cmd, skip);
        } else {
            ADMP(va("^3!%s: ^7permission denied\n", g_admin_cmds[ i ].keyword));
            AdminLog(ent, "attempted", skip - 1);
        }

        return true;
    }

    return false;
}

/*
===============
idAdminLocal::AdminNamelogCleanup
===============
*/
void idAdminLocal::AdminNamelogCleanup(void) {
    sint i;

    for(i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++) {
        bggame->Free(g_admin_namelog[ i ]);
        g_admin_namelog[ i ] = nullptr;
    }
}

/*
===============
idAdminLocal::AdminNamelogUpdate
===============
*/
void idAdminLocal::AdminNamelogUpdate(gclient_t *client, bool disconnect) {
    sint i, j;
    g_admin_namelog_t *namelog;
    valueType n1[ MAX_COLORFUL_NAME_LENGTH ];
    valueType n2[ MAX_COLORFUL_NAME_LENGTH ];
    sint clientNum = (client - level.clients);

    idSGameCmds::SanitiseString(client->pers.netname, n1, sizeof(n1));

    for(i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++) {
        if(disconnect && g_admin_namelog[i]->slot != clientNum) {
            continue;
        }

        if(!disconnect && !(g_admin_namelog[ i ]->slot == clientNum ||
                            g_admin_namelog[ i ]->slot == -1)) {
            continue;
        }

        if(!Q_stricmp(client->pers.ip, g_admin_namelog[ i ]->ip) &&
                !Q_stricmp(client->pers.guid, g_admin_namelog[ i ]->guid)) {
            for(j = 0; j < MAX_ADMIN_NAMELOG_NAMES &&
                    g_admin_namelog[ i ]->name[ j ][ 0 ]; j++) {
                idSGameCmds::SanitiseString(g_admin_namelog[ i ]->name[ j ], n2,
                                            sizeof(n2));

                if(!Q_stricmp(n1, n2)) {
                    break;
                }
            }

            if(j == MAX_ADMIN_NAMELOG_NAMES) {
                j = MAX_ADMIN_NAMELOG_NAMES - 1;
            }

            Q_strncpyz(g_admin_namelog[ i ]->name[ j ], client->pers.netname,
                       sizeof(g_admin_namelog[ i ]->name[ j ]));
            g_admin_namelog[ i ]->slot = (disconnect) ? -1 : clientNum;

            // if this player is connecting, they are no longer banned
            if(!disconnect) {
                g_admin_namelog[i]->banned = false;
            }

            return;
        }
    }

    if(i >= MAX_ADMIN_NAMELOGS) {
        idSGameMain::Printf("idAdminLocal::AdminNamelogUpdate: warning, g_admin_namelogs overflow\n");
        return;
    }

    namelog = (g_admin_namelog_t *)bggame->Alloc(sizeof(g_admin_namelog_t));
    ::memset(namelog, 0, sizeof(namelog));

    for(j = 0; j < MAX_ADMIN_NAMELOG_NAMES; j++) {
        namelog->name[j][0] = '\0';
    }

    Q_strncpyz(namelog->ip, client->pers.ip, sizeof(namelog->ip));
    Q_strncpyz(namelog->guid, client->pers.guid, sizeof(namelog->guid));
    Q_strncpyz(namelog->name[ 0 ], client->pers.netname,
               sizeof(namelog->name[ 0 ]));

    namelog->slot = (disconnect) ? -1 : clientNum;
    g_admin_namelog[ i ] = namelog;
}

/*
===============
idAdminLocal::AdminReadConfig
===============
*/
bool idAdminLocal::AdminReadconfig(gentity_t *ent, sint skiparg) {
    g_admin_level_t *l = nullptr;
    g_admin_admin_t *a = nullptr;
    g_admin_ban_t *b = nullptr;
    g_admin_command_t *c = nullptr;
    sint lc = 0, ac = 0, bc = 0, cc = 0;
    fileHandle_t f;
    sint len;
    valueType *cnf, *cnf2;
    valueType *t;
    bool level_open, admin_open, ban_open, command_open;
    sint i;

    adminLocal.AdminCleanup();

    if(!g_admin.string[ 0 ]) {
        adminLocal.ADMP("^3!readconfig: g_admin is not set, not loading configuration from a file\n");
        adminLocal.AdminDefaultLevels();
        return false;
    }

    len = trap_FS_FOpenFile(g_admin.string, &f, FS_READ);

    if(len < 0) {
        idSGameMain::Printf("^3!readconfig: ^7could not open admin config file %s\n",
                            g_admin.string);
        adminLocal.AdminDefaultLevels();
        return false;
    }

    cnf = (valueType *)bggame->Alloc(len + 1);
    cnf2 = cnf;
    trap_FS_Read(cnf, len, f);
    *(cnf + len) = '\0';
    trap_FS_FCloseFile(f);

    admin_level_maxname = 0;

    level_open = admin_open = ban_open = command_open = false;
    COM_BeginParseSession(g_admin.string);

    while(1) {
        t = COM_Parse(&cnf);

        if(!*t) {
            break;
        }

        if(!Q_stricmp(t, "[level]")) {
            if(lc >= MAX_ADMIN_LEVELS) {
                return false;
            }

            l = (g_admin_level_t *)bggame->Alloc(sizeof(g_admin_level_t));
            g_admin_levels[ lc++ ] = l;
            level_open = true;
            admin_open = ban_open = command_open = false;
        } else if(!Q_stricmp(t, "[admin]")) {
            if(ac >= MAX_ADMIN_ADMINS) {
                return false;
            }

            a = (g_admin_admin_t *)bggame->Alloc(sizeof(g_admin_admin_t));
            g_admin_admins[ ac++ ] = a;
            admin_open = true;
            level_open = ban_open = command_open = false;
        } else if(!Q_stricmp(t, "[ban]")) {
            if(bc >= MAX_ADMIN_BANS) {
                return false;
            }

            b = (g_admin_ban_t *)bggame->Alloc(sizeof(g_admin_ban_t));
            g_admin_bans[ bc++ ] = b;
            ban_open = true;
            level_open = admin_open = command_open = false;
        } else if(!Q_stricmp(t, "[command]")) {
            if(cc >= MAX_ADMIN_COMMANDS) {
                return false;
            }

            c = (g_admin_command_t *)bggame->Alloc(sizeof(g_admin_command_t));
            g_admin_commands[ cc++ ] = c;
            c->levels[ 0 ] = -1;
            command_open = true;
            level_open = admin_open = ban_open = false;
        } else if(level_open) {
            if(!Q_stricmp(t, "level")) {
                adminLocal.AdminReadConfigInt(&cnf, &l->level);
            } else if(!Q_stricmp(t, "name")) {
                adminLocal.AdminReadConfigString(&cnf, l->name, sizeof(l->name));
            } else if(!Q_stricmp(t, "flags")) {
                adminLocal.AdminReadConfigString(&cnf, l->flags, sizeof(l->flags));
            } else {
                COM_ParseError("[level] unrecognized token \"%s\"", t);
            }
        } else if(admin_open) {
            if(!Q_stricmp(t, "name")) {
                adminLocal.AdminReadConfigString(&cnf, a->name, sizeof(a->name));
            } else if(!Q_stricmp(t, "guid")) {
                adminLocal.AdminReadConfigString(&cnf, a->guid, sizeof(a->guid));
            } else if(!Q_stricmp(t, "level")) {
                adminLocal.AdminReadConfigInt(&cnf, &a->level);
            } else if(!Q_stricmp(t, "flags")) {
                adminLocal.AdminReadConfigString(&cnf, a->flags, sizeof(a->flags));
            } else if(!Q_stricmp(t, "counter")) {
                adminLocal.AdminReadConfigInt(&cnf, &a->counter);
            } else {
                COM_ParseError("[admin] unrecognized token \"%s\"", t);
            }
        } else if(ban_open) {
            if(!Q_stricmp(t, "name")) {
                adminLocal.AdminReadConfigString(&cnf, b->name, sizeof(b->name));
            } else if(!Q_stricmp(t, "guid")) {
                adminLocal.AdminReadConfigString(&cnf, b->guid, sizeof(b->guid));
            } else if(!Q_stricmp(t, "ip")) {
                adminLocal.AdminReadConfigString(&cnf, b->ip, sizeof(b->ip));
            } else if(!Q_stricmp(t, "reason")) {
                adminLocal.AdminReadConfigString(&cnf, b->reason, sizeof(b->reason));
            } else if(!Q_stricmp(t, "made")) {
                adminLocal.AdminReadConfigString(&cnf, b->made, sizeof(b->made));
            } else if(!Q_stricmp(t, "expires")) {
                adminLocal.AdminReadConfigInt(&cnf, &b->expires);
            } else if(!Q_stricmp(t, "banner")) {
                adminLocal.AdminReadConfigString(&cnf, b->banner, sizeof(b->banner));
            } else {
                COM_ParseError("[ban] unrecognized token \"%s\"", t);
            }
        } else if(command_open) {
            if(!Q_stricmp(t, "command")) {
                adminLocal.AdminReadConfigString(&cnf, c->command, sizeof(c->command));
            } else if(!Q_stricmp(t, "exec")) {
                adminLocal.AdminReadConfigString(&cnf, c->exec, sizeof(c->exec));
            } else if(!Q_stricmp(t, "desc")) {
                adminLocal.AdminReadConfigString(&cnf, c->desc, sizeof(c->desc));
            } else if(!Q_stricmp(t, "levels")) {
                valueType levels[ MAX_STRING_CHARS ] = {""};
                valueType *level = levels, * lp;
                sint cmdlevel = 0;

                adminLocal.AdminReadConfigString(&cnf, levels, sizeof(levels));

                while(cmdlevel < MAX_ADMIN_LEVELS) {
                    lp = COM_Parse(&level);

                    if(!*lp) {
                        break;
                    }

                    c->levels[ cmdlevel++ ] = atoi(lp);
                }

                // ensure the list is -1 terminated
                c->levels[ cmdlevel ] = -1;
            } else {
                COM_ParseError("[command] unrecognized token \"%s\"", t);
            }
        } else {
            COM_ParseError("unexpected token \"%s\"", t);
        }
    }

    bggame->Free(cnf2);
    adminLocal.ADMP(
        va("^3!readconfig: ^7loaded %d levels, %d admins, %d bans, %d commands\n",
           lc, ac, bc, cc));

    if(lc == 0) {
        adminLocal.AdminDefaultLevels();
    } else {
        // max printable name length for formatting
        for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
            len = Q_PrintStrlen(l->name);

            if(len > admin_level_maxname) {
                admin_level_maxname = len;
            }
        }
    }

    // reset adminLevel
    for(i = 0; i < level.maxclients; i++) {
        if(level.clients[i].pers.connected != CON_DISCONNECTED) {
            level.clients[i].pers.admin = adminLocal.Admin(&g_entities[i]);
        }
    }

    return true;
}

/*
===============
idGameLocal::SetClientSound
===============
*/
bool idAdminLocal::AdminTime(gentity_t *ent, sint skiparg) {
    qtime_t qt;

    trap_RealTime(&qt);
    adminLocal.ADMP(va("^3!time: ^7local time is %02i:%02i:%02i\n", qt.tm_hour,
                       qt.tm_min, qt.tm_sec));
    return true;
}

/*
===============
idGameLocal::SetClientSound
===============
*/
bool idAdminLocal::AdminSetlevel(gentity_t *ent, sint skiparg) {
    valueType name[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType lstr[ 11 ]; // 10 is max strlen() for 32-bit sint
    valueType adminname[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType testname[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType guid[ 33 ];
    sint l, i, matches = 0, id = -1;
    gentity_t *vic = nullptr;
    bool updated = false;
    g_admin_admin_t *a;
    bool found = false;
    bool numeric = true;

    if(idSGameCmds::SayArgc() < 3 + skiparg) {
        adminLocal.ADMP("^3!setlevel: ^7usage: !setlevel [name|slot#] [level]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, testname, sizeof(testname));
    idSGameCmds::SayArgv(2 + skiparg, lstr, sizeof(lstr));

    l = atoi(lstr);
    idSGameCmds::SanitiseString(testname, name, sizeof(name));

    for(i = 0; i < sizeof(name) && name[ i ]; i++) {
        if(!isdigit(name[ i ])) {
            numeric = false;
            break;
        }
    }

    if(numeric) {
        id = atoi(name);
    }

    if(ent && l > ent->client->pers.adminLevel) {
        adminLocal.ADMP("^3!setlevel: ^7you may not use !setlevel to set a level higher than your current level\n");
        return false;
    }

    // if admin is activated for the first time on a running server, we need
    // to ensure at least the default levels get created
    if(!ent && !g_admin_levels[0]) {
        adminLocal.AdminReadconfig(nullptr, 0);
    }

    for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
        if(g_admin_levels[ i ]->level == l) {
            found = true;
            break;
        }
    }

    if(!found) {
        adminLocal.ADMP("^3!setlevel: ^7level is not defined\n");
        return false;
    }

    if(numeric && id >= 0 && id < level.maxclients) {
        vic = &g_entities[id];
    }

    if(vic && vic->client && vic->client->pers.connected != CON_DISCONNECTED) {
        Q_strncpyz(adminname, vic->client->pers.netname, sizeof(adminname));
        Q_strncpyz(guid, vic->client->pers.guid, sizeof(guid));
        matches = 1;
    } else if(numeric && id >= MAX_CLIENTS &&
              id < MAX_CLIENTS + MAX_ADMIN_ADMINS &&
              g_admin_admins[ id - MAX_CLIENTS ]) {
        Q_strncpyz(adminname, g_admin_admins[ id - MAX_CLIENTS ]->name,
                   sizeof(adminname));
        Q_strncpyz(guid, g_admin_admins[ id - MAX_CLIENTS ]->guid, sizeof(guid));
        matches = 1;
    } else {
        for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ] &&
                matches < 2; i++) {
            idSGameCmds::SanitiseString(g_admin_admins[ i ]->name, testname,
                                        sizeof(testname));

            if(strstr(testname, name)) {
                Q_strncpyz(adminname, g_admin_admins[ i ]->name, sizeof(adminname));
                Q_strncpyz(guid, g_admin_admins[ i ]->guid, sizeof(guid));
                matches++;
            }
        }

        for(i = 0; i < level.maxclients && matches < 2; i++) {
            if(level.clients[i].pers.connected == CON_DISCONNECTED) {
                continue;
            }

            if(matches && !Q_stricmp(level.clients[ i ].pers.guid, guid)) {
                vic = &g_entities[ i ];
                continue;
            }

            idSGameCmds::SanitiseString(level.clients[ i ].pers.netname, testname,
                                        sizeof(testname));

            if(strstr(testname, name)) {
                vic = &g_entities[ i ];
                matches++;
                Q_strncpyz(guid, vic->client->pers.guid, sizeof(guid));
            }
        }

        if(vic) {
            Q_strncpyz(adminname, vic->client->pers.netname, sizeof(adminname));
        }
    }

    if(matches == 0) {
        adminLocal.ADMP("^3!setlevel:^7 no match.  use !listplayers or !listadmins to find an appropriate number to use instead of name.\n");
        return false;
    }

    if(matches > 1) {
        adminLocal.ADMP("^3!setlevel:^7 more than one match.  Use the admin number instead:\n");
        adminLocal.AdminListAdmins(ent, 0, name);
        return false;
    }

    if(!Q_stricmp(guid, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")) {
        adminLocal.ADMP(va("^3!setlevel: ^7%s does not have a valid GUID\n",
                           adminname));
        return false;
    }

    if(ent && !adminLocal.AdminHigherGuid(ent->client->pers.guid, guid)) {
        adminLocal.ADMP("^3!setlevel: ^7sorry, but your intended victim has a higher admin level than you\n");
        return false;
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(!Q_stricmp(g_admin_admins[ i ]->guid, guid)) {
            a = g_admin_admins[i];
            a->level = l;
            Q_strncpyz(a->name, adminname, sizeof(a->name));
            updated = true;
        }
    }

    if(!updated) {
        if(i == MAX_ADMIN_ADMINS) {
            adminLocal.ADMP("^3!setlevel: ^7too many admins\n");
            return false;
        }

        a = (g_admin_admin_t *)bggame->Alloc(sizeof(g_admin_admin_t));
        a->level = l;
        Q_strncpyz(a->name, adminname, sizeof(a->name));
        Q_strncpyz(a->guid, guid, sizeof(a->guid));
        *a->flags = '\0';
        a->counter = -1;
        g_admin_admins[ i ] = a;
    }

    AP(va("print \"^3!setlevel: ^7%s^7 was given level %d admin rights by %s\n\"",
          adminname, l, (ent) ? ent->client->pers.netname : "console"));

    if(vic) {
        vic->client->pers.admin = (l ? a : nullptr);
        vic->client->pers.adminLevel = l;
    }

    if(!g_admin.string[0]) {
        adminLocal.ADMP("^3!setlevel: ^7WARNING g_admin not set, not saving admin record to a file\n");
    } else {
        adminLocal.AdminWriteConfig();
    }

    return true;
}

/*
===============
idAdminLocal::AdminCreateBan
===============
*/
bool idAdminLocal::AdminCreateBan(gentity_t *ent, valueType *netname,
                                  valueType *guid, valueType *ip, sint seconds, valueType *reason) {
    g_admin_ban_t *b = nullptr;
    qtime_t qt;
    sint t, i;

    t = trap_RealTime(&qt);
    b = (g_admin_ban_t *)bggame->Alloc(sizeof(g_admin_ban_t));

    if(!b) {
        return false;
    }

    Q_strncpyz(b->name, netname, sizeof(b->name));
    Q_strncpyz(b->guid, guid, sizeof(b->guid));
    Q_strncpyz(b->ip, ip, sizeof(b->ip));

    //strftime( b->made, sizeof( b->made ), "%m/%d/%y %H:%M:%S", lt );
    Q_vsprintf_s(b->made, sizeof(b->made), sizeof(b->made),
                 "%02i/%02i/%02i %02i:%02i:%02i", qt.tm_mon + 1, qt.tm_mday,
                 qt.tm_year % 100, qt.tm_hour, qt.tm_min, qt.tm_sec);

    if(ent) {
        Q_strncpyz(b->banner, ent->client->pers.netname, sizeof(b->banner));
    } else {
        Q_strncpyz(b->banner, "console", sizeof(b->banner));
    }

    if(!seconds) {
        b->expires = 0;
    } else {
        b->expires = t + seconds;
    }

    if(!*reason) {
        Q_strncpyz(b->reason, "banned by admin", sizeof(b->reason));
    } else {
        Q_strncpyz(b->reason, reason, sizeof(b->reason));
    }

    for(i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++)
        ;

    if(i == MAX_ADMIN_BANS) {
        ADMP("^3!ban: ^7too many bans\n");
        bggame->Free(b);
        return false;
    }

    g_admin_bans[ i ] = b;
    return true;
}

/*
===============
idAdminLocal::AdminParseTime
===============
*/
sint idAdminLocal::AdminParseTime(pointer time) {
    sint seconds = 0, num = 0;

    while(*time) {
        if(!isdigit(*time)) {
            return -1;
        }

        while(isdigit(*time)) {
            num = num * 10 + *time++ - '0';
        }

        if(!*time) {
            break;
        }

        switch(*time++) {
            case 'w':
                num *= 7;

            case 'd':
                num *= 24;

            case 'h':
                num *= 60;

            case 'm':
                num *= 60;

            case 's':
                break;

            default:
                return -1;
        }

        seconds += num;
        num = 0;
    }

    if(num) {
        seconds += num;
    }

    return seconds;
}

/*
===============
idAdminLocal::AdminKick
===============
*/
bool idAdminLocal::AdminKick(gentity_t *ent, sint skiparg) {
    sint pid, minargc;
    valueType name[ MAX_COLORFUL_NAME_LENGTH ],
              err[ MAX_STRING_CHARS ];
    const valueType *reason;
    gentity_t *vic;

    minargc = 3 + skiparg;

    if(adminLocal.AdminPermission(ent, ADMF_UNACCOUNTABLE)) {
        minargc = 2 + skiparg;
    }

    if(idSGameCmds::SayArgc() < minargc) {
        adminLocal.ADMP("^3!kick: ^7usage: !kick [name] [reason]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, name, sizeof(name));
    reason = idSGameCmds::SayConcatArgs(2 + skiparg);


    if((pid = idSGameCmds::ClientNumberFromString(name, err,
              sizeof(err))) == -1) {
        adminLocal.ADMP(va("^3kick: ^7%s", err));
        return false;
    }

    vic = &g_entities[ pid ];

    if(!adminLocal.AdminHigher(ent, vic)) {
        adminLocal.ADMP("^3!kick: ^7sorry, but your intended victim has a higher admin level than you\n");
        return false;
    }

    if(vic->client->pers.localClient) {
        adminLocal.ADMP("^3!kick: ^7disconnecting the host would end the game\n");
        return false;
    }

    adminLocal.AdminCreateBan(ent, vic->client->pers.netname,
                              vic->client->pers.guid, vic->client->pers.ip,
                              adminLocal.AdminParseTime(va("1s%s", g_adminTempBan.string)),
                              (*reason) ? reason : "kicked by admin");

    if(g_admin.string[0]) {
        adminLocal.AdminWriteConfig();
    }

    trap_SendServerCommand(pid,
                           va("disconnect \"You have been kicked.\n%s^7\nreason:\n%s\"",
                              (ent) ? va("admin:\n%s", ent->client->pers.netname) : "",
                              (*reason) ? reason : "kicked by admin"));

    trap_DropClient(pid, va("has been kicked%s^7. reason: %s",
                            (ent) ? va(" by %s", ent->client->pers.netname) : "",
                            (*reason) ? reason : "kicked by admin"), 0);

    return true;
}

/*
===============
idAdminLocal::AdminBan
===============
*/
bool idAdminLocal::AdminBan(gentity_t *ent, sint skiparg) {
    sint seconds;
    valueType search[ MAX_COLORFUL_NAME_LENGTH ];
    valueType secs[ MAX_TOKEN_CHARS ];
    valueType *reason;
    sint minargc;
    valueType duration[ 32 ];
    sint logmatch = -1, logmatches = 0;
    sint i, j;
    bool exactmatch = false;
    valueType n2[ MAX_COLORFUL_NAME_LENGTH ];
    valueType s2[ MAX_COLORFUL_NAME_LENGTH ];
    valueType guid_stub[ 9 ];

    if(adminLocal.AdminPermission(ent, ADMF_CAN_PERM_BAN) &&
            adminLocal.AdminPermission(ent, ADMF_UNACCOUNTABLE)) {
        minargc = 2 + skiparg;
    } else if(adminLocal.AdminPermission(ent, ADMF_CAN_PERM_BAN) ||
              adminLocal.AdminPermission(ent, ADMF_UNACCOUNTABLE)) {
        minargc = 3 + skiparg;
    } else {
        minargc = 4 + skiparg;
    }

    if(idSGameCmds::SayArgc() < minargc) {
        adminLocal.ADMP("^3!ban: ^7usage: !ban [name|slot|ip] [duration] [reason]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, search, sizeof(search));
    idSGameCmds::SanitiseString(search, s2, sizeof(s2));
    idSGameCmds::SayArgv(2 + skiparg, secs, sizeof(secs));

    seconds = adminLocal.AdminParseTime(secs);

    if(seconds <= 0) {
        if(adminLocal.AdminPermission(ent, ADMF_CAN_PERM_BAN)) {
            seconds = 0;
        } else {
            adminLocal.ADMP("^3!ban: ^7you may not issue permanent bans\n");
            return false;
        }

        reason = idSGameCmds::SayConcatArgs(2 + skiparg);
    } else {
        reason = idSGameCmds::SayConcatArgs(3 + skiparg);
    }

    for(i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++) {
        // skip players in the namelog who have already been banned
        if(g_admin_namelog[i]->banned) {
            continue;
        }

        // skip disconnected players when banning on slot number
        if(g_admin_namelog[i]->slot == -1) {
            continue;
        }

        if(!Q_stricmp(va("%d", g_admin_namelog[ i ]->slot), search)) {
            logmatches = 1;
            logmatch = i;
            exactmatch = true;
            break;
        }
    }

    for(i = 0; !exactmatch && i < MAX_ADMIN_NAMELOGS &&
            g_admin_namelog[ i ]; i++) {
        // skip players in the namelog who have already been banned
        if(g_admin_namelog[i]->banned) {
            continue;
        }

        if(!Q_stricmp(g_admin_namelog[ i ]->ip, search)) {
            logmatches = 1;
            logmatch = i;
            exactmatch = true;
            break;
        }

        for(j = 0; j < MAX_ADMIN_NAMELOG_NAMES &&
                g_admin_namelog[ i ]->name[ j ][ 0 ]; j++) {
            idSGameCmds::SanitiseString(g_admin_namelog[ i ]->name[ j ], n2,
                                        sizeof(n2));

            if(strstr(n2, s2)) {
                if(logmatch != i) {
                    logmatches++;
                }

                logmatch = i;
            }
        }
    }

    if(!logmatches) {
        adminLocal.ADMP("^3!ban: ^7no player found by that name, IP, or slot number\n");
        return false;
    }

    if(logmatches > 1) {
        adminLocal.ADMBP_begin();
        adminLocal.ADMBP("^3!ban: ^7multiple recent clients match name, use IP or slot#:\n");

        for(i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++) {
            for(j = 0; j < 8; j++) {
                guid_stub[ j ] = g_admin_namelog[ i ]->guid[ j + 24 ];
            }

            guid_stub[ j ] = '\0';

            for(j = 0; j < MAX_ADMIN_NAMELOG_NAMES &&
                    g_admin_namelog[ i ]->name[ j ][ 0 ]; j++) {
                idSGameCmds::SanitiseString(g_admin_namelog[ i ]->name[ j ], n2,
                                            sizeof(n2));

                if(strstr(n2, s2)) {
                    if(g_admin_namelog[i]->slot > -1) {
                        adminLocal.ADMBP("^3");
                    }

                    adminLocal.ADMBP(va("%-2s (*%s) %15s ^7'%s^7'\n",
                                        (g_admin_namelog[ i ]->slot > -1) ? va("%d",
                                                g_admin_namelog[ i ]->slot) : "-",
                                        guid_stub, g_admin_namelog[ i ]->ip, g_admin_namelog[ i ]->name[ j ]));
                }
            }
        }

        adminLocal.ADMBP_end();
        return false;
    }

    if(ent && !adminLocal.AdminHigherGuid(ent->client->pers.guid,
                                          g_admin_namelog[ logmatch ]->guid)) {
        adminLocal.ADMP("^3!ban: ^7sorry, but your intended victim has a higher admin level than you\n");
        return false;
    }

    if(!strcmp(g_admin_namelog[ logmatch ]->ip, "localhost")) {
        adminLocal.ADMP("^3!ban: ^7disconnecting the host would end the game\n");
        return false;
    }

    adminLocal.AdminDuration((seconds) ? seconds : -1, duration,
                             sizeof(duration));
    adminLocal.AdminCreateBan(ent, g_admin_namelog[ logmatch ]->name[ 0 ],
                              g_admin_namelog[ logmatch ]->guid, g_admin_namelog[ logmatch ]->ip,
                              seconds, reason);

    g_admin_namelog[ logmatch ]->banned = true;

    if(!g_admin.string[0]) {
        adminLocal.ADMP("^3!ban: ^7WARNING g_admin not set, not saving ban to a file\n");
    } else {
        adminLocal.AdminWriteConfig();
    }

    if(g_admin_namelog[ logmatch ]->slot == -1) {
        // client is already disconnected so stop here
        AP(va("print \"^3!ban:^7 %s^7 has been banned by %s^7, duration: %s, reason: %s\n\"",
              g_admin_namelog[ logmatch ]->name[ 0 ],
              (ent) ? ent->client->pers.netname : "console", duration,
              (*reason) ? reason : "banned by admin"));
        return true;
    }

    trap_SendServerCommand(g_admin_namelog[ logmatch ]->slot,
                           va("disconnect \"You have been banned.\n admin:\n%s^7\nduration:\n%s\nreason:\n%s\"",
                              (ent) ? ent->client->pers.netname : "console", duration,
                              (*reason) ? reason : "banned by admin"));

    trap_DropClient(g_admin_namelog[ logmatch ]->slot,
                    va("has been banned by %s^7, duration: %s, reason: %s",
                       (ent) ? ent->client->pers.netname : "console",
                       duration, (*reason) ? reason : "banned by admin"), 0);
    return true;
}

/*
===============
idAdminLocal::AdminUnBan
===============
*/
bool idAdminLocal::AdminUnBan(gentity_t *ent, sint skiparg) {
    sint bnum;
    valueType bs[ 5 ];

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        adminLocal.ADMP("^3!unban: ^7usage: !unban [ban#]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, bs, sizeof(bs));

    bnum = atoi(bs);

    if(bnum < 1 || bnum > MAX_ADMIN_BANS || !g_admin_bans[ bnum - 1 ]) {
        adminLocal.ADMP("^3!unban: ^7invalid ban#\n");
        return false;
    }

    if(g_admin_bans[ bnum - 1 ]->expires == 0 &&
            !adminLocal.AdminPermission(ent, ADMF_CAN_PERM_BAN)) {
        adminLocal.ADMP("^3!unban: ^7you cannot remove permanent bans\n");
        return false;
    }

    g_admin_bans[ bnum - 1 ]->expires = trap_RealTime(nullptr);

    AP(va("print \"^3!unban: ^7ban #%d for %s^7 has been removed by %s\n\"",
          bnum, g_admin_bans[ bnum - 1 ]->name,
          (ent) ? ent->client->pers.netname : "console"));

    if(g_admin.string[0]) {
        adminLocal.AdminWriteConfig();
    }

    return true;
}

/*
===============
idGameLocal::SetClientSound
===============
*/
bool idAdminLocal::AdminAdjustBan(gentity_t *ent, sint skiparg) {
    sint bnum, length, expires, time = trap_RealTime(nullptr);
    valueType duration[ 32 ] = {""};
    valueType *reason;
    valueType bs[ 5 ];
    valueType secs[ MAX_TOKEN_CHARS ];
    valueType mode = '\0';
    g_admin_ban_t *ban;

    if(idSGameCmds::SayArgc() < 3 + skiparg) {
        adminLocal.ADMP("^3!adjustban: ^7usage: !adjustban [ban#] [duration] [reason]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, bs, sizeof(bs));

    bnum = atoi(bs);

    if(bnum < 1 || bnum > MAX_ADMIN_BANS || !g_admin_bans[ bnum - 1 ]) {
        adminLocal.ADMP("^3!adjustban: ^7invalid ban#\n");
        return false;
    }

    ban = g_admin_bans[ bnum - 1 ];

    if(ban->expires == 0 &&
            !adminLocal.AdminPermission(ent, ADMF_CAN_PERM_BAN)) {
        adminLocal.ADMP("^3!adjustban: ^7you cannot modify permanent bans\n");
        return false;
    }

    idSGameCmds::SayArgv(2 + skiparg, secs, sizeof(secs));

    if(secs[0] == '+' || secs[0] == '-') {
        mode = secs[0];
    }

    length = adminLocal.AdminParseTime(&secs[ mode ? 1 : 0 ]);

    if(length < 0) {
        skiparg--;
    } else {
        if(length) {
            if(ban->expires == 0 && mode) {
                adminLocal.ADMP("^3!adjustban: ^7new duration must be explicit\n");
                return false;
            }

            if(mode == '+') {
                expires = ban->expires + length;
            } else if(mode == '-') {
                expires = ban->expires - length;
            } else {
                expires = time + length;
            }

            if(expires <= time) {
                adminLocal.ADMP("^3!adjustban: ^7ban duration must be positive\n");
                return false;
            }
        } else if(adminLocal.AdminPermission(ent, ADMF_CAN_PERM_BAN)) {
            expires = 0;
        } else {
            adminLocal.ADMP("^3!adjustban: ^7ban duration must be positive\n");
            return false;
        }

        ban->expires = expires;
        adminLocal.AdminDuration((expires) ? expires - time : -1, duration,
                                 sizeof(duration));
    }

    reason = idSGameCmds::SayConcatArgs(3 + skiparg);

    if(*reason) {
        Q_strncpyz(ban->reason, reason, sizeof(ban->reason));
    }

    AP(va("print \"^3!adjustban: ^7ban #%d for %s^7 has been updated by %s^7 %s%s%s%s%s\n\"",
          bnum, ban->name,
          (ent) ? ent->client->pers.netname : "console",
          (length >= 0) ? "duration: " : "",
          duration, (length >= 0 &&
                     *reason) ? ", " : "", (*reason) ? "reason: " : "", reason));

    if(ent) {
        Q_strncpyz(ban->banner, ent->client->pers.netname, sizeof(ban->banner));
    }

    if(g_admin.string[0]) {
        adminLocal.AdminWriteConfig();
    }

    return true;
}

/*
===============
idAdminLocal::AdminPutTeam
===============
*/
bool idAdminLocal::AdminPutTeam(gentity_t *ent, sint skiparg) {
    sint pid;
    valueType name[ MAX_COLORFUL_NAME_LENGTH ], team[ 7 ],
              err[ MAX_STRING_CHARS ];
    gentity_t *vic;
    team_t teamnum = TEAM_NONE;

    idSGameCmds::SayArgv(1 + skiparg, name, sizeof(name));
    idSGameCmds::SayArgv(2 + skiparg, team, sizeof(team));

    if(idSGameCmds::SayArgc() < 3 + skiparg) {
        adminLocal.ADMP("^3!putteam: ^7usage: !putteam [name] [h|a|s]\n");
        return false;
    }


    if((pid = idSGameCmds::ClientNumberFromString(name, err,
              sizeof(err))) == -1) {
        adminLocal.ADMP(va("^3!putteam: ^7%s\n", err));
        return false;
    }

    if(!adminLocal.AdminHigher(ent, &g_entities[ pid ])) {
        adminLocal.ADMP("^3!putteam: ^7sorry, but your intended victim has a higher admin level than you\n");
        return false;
    }

    vic = &g_entities[ pid ];
    teamnum = idSGameTeam::TeamFromString(team);

    if(teamnum == NUM_TEAMS) {
        adminLocal.ADMP(va("^3!putteam: ^7unknown team %s\n", team));
        return false;
    }

    if(vic->client->pers.teamSelection == teamnum) {
        return false;
    }

    idSGameTeam::ChangeTeam(vic, teamnum);

    AP(va("print \"^3!putteam: ^7%s^7 put %s^7 on to the %s team\n\"",
          (ent) ? ent->client->pers.netname : "console", vic->client->pers.netname,
          bggame->TeamName(teamnum)));
    return true;
}

/*
===============
idAdminLocal::AdminMap
===============
*/
bool idAdminLocal::AdminMap(gentity_t *ent, sint skiparg) {
    valueType map[ MAX_QPATH ];
    valueType layout[ MAX_QPATH ] = { "" };

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        adminLocal.ADMP("^3!map: ^7usage: !map [map] (layout)\n");
        return false;
    }

    idSGameCmds::SayArgv(skiparg + 1, map, sizeof(map));

    if(!trap_FS_FOpenFile(va("maps/%s.bsp", map), nullptr, FS_READ)) {
        adminLocal.ADMP(va("^3!map: ^7invalid map name '%s'\n", map));
        return false;
    }

    if(idSGameCmds::SayArgc() > 2 + skiparg) {
        idSGameCmds::SayArgv(skiparg + 2, layout, sizeof(layout));

        if(!Q_stricmp(layout, "*BUILTIN*") ||
                trap_FS_FOpenFile(va("layouts/%s/%s.dat", map, layout), nullptr,
                                  FS_READ) > 0) {
            trap_Cvar_Set("g_layouts", layout);
        } else {
            adminLocal.ADMP(va("^3!map: ^7invalid layout name '%s'\n", layout));
            return false;
        }
    }

    trap_SendConsoleCommand(EXEC_APPEND, va("map %s", map));

    level.restarted = true;

    AP(va("print \"^3!map: ^7map '%s' started by %s %s\n\"", map,
          (ent) ? ent->client->pers.netname : "console",
          (layout[ 0 ]) ? va("(forcing layout '%s')", layout) : ""));

    return true;
}

/*
===============
idGameLocal::SetClientSound
===============
*/
bool idAdminLocal::AdminMute(gentity_t *ent, sint skiparg) {
    sint pid;
    valueType name[ MAX_COLORFUL_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
    valueType command[ MAX_ADMIN_CMD_LEN ], *cmd;
    gentity_t *vic;

    idSGameCmds::SayArgv(skiparg, command, sizeof(command));

    cmd = command;

    if(cmd && *cmd == '!') {
        cmd++;
    }

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        adminLocal.ADMP(va("^3!%s: ^7usage: !%s [name|slot#]\n", cmd, cmd));
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, name, sizeof(name));


    if((pid = idSGameCmds::ClientNumberFromString(name, err,
              sizeof(err))) == -1) {
        adminLocal.ADMP(va("^3!%s: ^7%s\n", cmd, err));
        return false;
    }

    if(!adminLocal.AdminHigher(ent, &g_entities[ pid ])) {
        adminLocal.ADMP(
            va("^3!%s: ^7sorry, but your intended victim has a higher admin level than you\n",
               cmd));
        return false;
    }

    vic = &g_entities[ pid ];

    if(vic->client->pers.muted == true) {
        if(!Q_stricmp(cmd, "mute")) {
            adminLocal.ADMP("^3!mute: ^7player is already muted\n");
            return true;
        }

        vic->client->pers.muted = false;

        CPx(pid, "cp \"^1You have been unmuted\"");
        AP(va("print \"^3!unmute: ^7%s^7 has been unmuted by %s\n\"",
              vic->client->pers.netname, (ent) ? ent->client->pers.netname : "console"));
    } else {
        if(!Q_stricmp(cmd, "unmute")) {
            adminLocal.ADMP("^3!unmute: ^7player is not currently muted\n");
            return true;
        }

        vic->client->pers.muted = true;

        CPx(pid, "cp \"^1You've been muted\"");
        AP(va("print \"^3!mute: ^7%s^7 has been muted by ^7%s\n\"",
              vic->client->pers.netname, (ent) ? ent->client->pers.netname : "console"));
    }

    return true;
}

/*
===============
idAdminLocal::AdminDenyBuild
===============
*/
bool idAdminLocal::AdminDenyBuild(gentity_t *ent, sint skiparg) {
    sint pid;
    valueType name[ MAX_COLORFUL_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
    valueType command[ MAX_ADMIN_CMD_LEN ], *cmd;
    gentity_t *vic;

    idSGameCmds::SayArgv(skiparg, command, sizeof(command));
    cmd = command;

    if(cmd && *cmd == '!') {
        cmd++;
    }

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        adminLocal.ADMP(va("^3!%s: ^7usage: !%s [name|slot#]\n", cmd, cmd));
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, name, sizeof(name));

    if((pid = idSGameCmds::ClientNumberFromString(name, err,
              sizeof(err))) == -1) {
        adminLocal.ADMP(va("^3!%s: ^7%s\n", cmd, err));
        return false;
    }

    if(!adminLocal.AdminHigher(ent, &g_entities[ pid ])) {
        adminLocal.ADMP(
            va("^3!%s: ^7sorry, but your intended victim has a higher admin level than you\n",
               cmd));
        return false;
    }

    vic = &g_entities[ pid ];

    if(vic->client->pers.denyBuild) {
        if(!Q_stricmp(cmd, "denybuild")) {
            adminLocal.ADMP("^3!denybuild: ^7player already has no building rights\n");
            return true;
        }

        vic->client->pers.denyBuild = false;

        CPx(pid, "cp \"^1You've regained your building rights\"");
        AP(va("print \"^3!allowbuild: ^7building rights for ^7%s^7 restored by %s\n\"",
              vic->client->pers.netname, (ent) ? ent->client->pers.netname : "console"));
    } else {
        if(!Q_stricmp(cmd, "allowbuild")) {
            adminLocal.ADMP("^3!allowbuild: ^7player already has building rights\n");
            return true;
        }

        vic->client->pers.denyBuild = true;
        vic->client->ps.stats[ STAT_BUILDABLE ] = BA_NONE;
        CPx(pid, "cp \"^1You've lost your building rights\"");
        AP(va("print \"^3!denybuild: ^7building rights for ^7%s^7 revoked by ^7%s\n\"",
              vic->client->pers.netname, (ent) ? ent->client->pers.netname : "console"));
    }

    return true;
}

/*
===============
idAdminLocal::AdminListAdmins
===============
*/
bool idAdminLocal::AdminListAdmins(gentity_t *ent, sint skiparg) {
    sint i, found = 0;
    valueType search[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType s[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    sint start = 0;
    bool numeric = true;
    sint drawn = 0;

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(g_admin_admins[i]->level == 0) {
            continue;
        }

        found++;
    }

    if(!found) {
        adminLocal.ADMP("^3!listadmins: ^7no admins defined\n");
        return false;
    }

    if(idSGameCmds::SayArgc() == 2 + skiparg) {
        idSGameCmds::SayArgv(1 + skiparg, s, sizeof(s));

        for(i = 0; i < sizeof(s) && s[ i ]; i++) {
            if(isdigit(s[i])) {
                continue;
            }

            numeric = false;
        }

        if(numeric) {
            start = atoi(s);

            if(start > 0) {
                start -= 1;
            } else if(start < 0) {
                start = found + start;
            }
        } else {
            idSGameCmds::SanitiseString(s, search, sizeof(search));
        }
    }

    if(start >= found || start < 0) {
        start = 0;
    }

    if(start >= found) {
        adminLocal.ADMP(va("^3!listadmins: ^7listing %d admins\n", found));
        return false;
    }

    drawn = adminLocal.AdminListAdmins(ent, start, search);

    if(search[ 0 ]) {
        adminLocal.ADMP(va("^3!listadmins:^7 found %d admins matching '%s^7'\n",
                           drawn, search));
    } else {
        adminLocal.ADMBP_begin();
        adminLocal.ADMBP(va("^3!listadmins:^7 showing admin %d - %d of %d.  ",
                            (found) ? (start + 1) : 0, ((start + MAX_ADMIN_LISTITEMS) > found) ?
                            found : (start + MAX_ADMIN_LISTITEMS), found));

        if((start + MAX_ADMIN_LISTITEMS) < found) {
            adminLocal.ADMBP(va("run '!listadmins %d' to see more",
                                (start + MAX_ADMIN_LISTITEMS + 1)));
        }

        adminLocal.ADMBP("\n");
        adminLocal.ADMBP_end();
    }

    return true;
}

/*
===============
idAdminLocal::AdminListLayouts
===============
*/
bool idAdminLocal::AdminListLayouts(gentity_t *ent, sint skiparg) {
    valueType list[ MAX_CVAR_VALUE_STRING ];
    valueType map[ MAX_QPATH ];
    sint count = 0, i = 0;
    valueType *s;
    valueType layout[ MAX_QPATH ] = { "" };

    if(idSGameCmds::SayArgc() == 2 + skiparg) {
        idSGameCmds::SayArgv(1 + skiparg, map, sizeof(map));
    } else {
        trap_Cvar_VariableStringBuffer("mapname", map, sizeof(map));
    }

    count = idSGameBuildable::LayoutList(map, list, sizeof(list));

    adminLocal.ADMBP_begin();
    adminLocal.ADMBP(va("^3!listlayouts:^7 %d layouts found for '%s':\n",
                        count, map));
    s = &list[ 0 ];

    while(*s) {
        if(*s == ' ') {
            adminLocal.ADMBP(va(" %s\n", layout));
            layout[ 0 ] = '\0';
            i = 0;
        } else if(i < sizeof(layout) - 2) {
            layout[ i++ ] = *s;
            layout[ i ] = '\0';
        }

        s++;
    }

    if(layout[0]) {
        adminLocal.ADMBP(va(" %s\n", layout));
    }

    adminLocal.ADMBP_end();
    return true;
}

/*
===============
idGameLocal::SetClientSound
===============
*/
bool idAdminLocal::AdminListPlayers(gentity_t *ent, sint skiparg) {
    sint i, j, l;
    gclient_t *p;
    valueType c[ 3 ], t[ 2 ]; // color and team letter
    valueType n[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType n2[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType n3[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType lname[ MAX_COLORFUL_NAME_LENGTH ];
    valueType guid_stub[ 9 ];
    valueType muted[ 2 ], denied[ 2 ];

    adminLocal.ADMBP_begin();
    adminLocal.ADMBP(va("^3!listplayers: ^7%d players connected:\n",
                        level.numConnectedClients));

    for(i = 0; i < level.maxclients; i++) {
        p = &level.clients[ i ];
        Q_strncpyz(t, "S", sizeof(t));
        Q_strncpyz(c, S_COLOR_YELLOW, sizeof(c));

        if(p->pers.teamSelection == TEAM_HUMANS) {
            Q_strncpyz(t, "H", sizeof(t));
            Q_strncpyz(c, S_COLOR_BLUE, sizeof(c));
        } else if(p->pers.teamSelection == TEAM_ALIENS) {
            Q_strncpyz(t, "A", sizeof(t));
            Q_strncpyz(c, S_COLOR_RED, sizeof(c));
        }

        if(p->pers.connected == CON_CONNECTING) {
            Q_strncpyz(t, "C", sizeof(t));
            Q_strncpyz(c, S_COLOR_CYAN, sizeof(c));
        } else if(p->pers.connected != CON_CONNECTED) {
            continue;
        }

        for(j = 0; j < 8; j++) {
            guid_stub[j] = p->pers.guid[j + 24];
        }

        guid_stub[ j ] = '\0';

        muted[ 0 ] = '\0';

        if(p->pers.muted) {
            Q_strncpyz(muted, "M", sizeof(muted));
        }

        denied[ 0 ] = '\0';

        if(p->pers.denyBuild) {
            Q_strncpyz(denied, "B", sizeof(denied));
        }

        l = 0;
        idSGameCmds::SanitiseString(p->pers.netname, n2, sizeof(n2));

        n[ 0 ] = '\0';

        for(j = 0; j < MAX_ADMIN_ADMINS && g_admin_admins[ j ]; j++) {
            if(!Q_stricmp(g_admin_admins[ j ]->guid, p->pers.guid)) {
                // don't gather aka or level info if the admin is incognito
                if(ent && adminLocal.AdminPermission(&g_entities[ i ], ADMF_INCOGNITO)) {
                    break;
                }

                l = g_admin_admins[ j ]->level;

                idSGameCmds::SanitiseString(g_admin_admins[ j ]->name, n3, sizeof(n3));

                if(Q_stricmp(n2, n3)) {
                    Q_strncpyz(n, g_admin_admins[ j ]->name, sizeof(n));
                }

                break;
            }
        }

        lname[ 0 ] = '\0';

        for(j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++) {
            if(g_admin_levels[ j ]->level == l) {
                sint k, colorlen;

                for(colorlen = k = 0; g_admin_levels[j]->name[k]; k++) {
                    if(Q_IsColorString(&g_admin_levels[j]->name[k])) {
                        colorlen += Q_ColorStringLength(&g_admin_levels[j]->name[k]);
                    } else if(Q_IsColorEscapeEscape(&g_admin_levels[j]->name[k])) {
                        colorlen++;
                    }
                }

                Q_vsprintf_s(lname, sizeof(lname), sizeof(lname), "%*s",
                             admin_level_maxname + colorlen, g_admin_levels[j]->name);

                break;
            }

        }

        if(adminLocal.AdminPermission(ent, ADMF_SEESFULLLISTPLAYERS)) {
            adminLocal.ADMBP(
                va("%2i %s%s^7 %-2i %s^7 (*%s) ^1%1s%1s^7 %s^7 %s%s^7%s\n", i, c, t, l,
                   lname, guid_stub, muted, denied, p->pers.netname,
                   (*n) ? "(a.k.a. " : "", n, (*n) ? ")" : ""));
        } else {
            adminLocal.ADMBP(va("%2i %s%s^7 ^1%1s%1s^7 %s^7\n", i, c, t, muted, denied,
                                p->pers.netname));
        }
    }

    adminLocal.ADMBP_end();

    return true;
}

/*
===============
idAdminLocal::AdminShowBans
===============
*/
bool idAdminLocal::AdminShowBans(gentity_t *ent, sint skiparg) {
    sint i, found = 0, max = -1, count, t, j, k;
    valueType duration[ 32 ];
    sint max_name = 1, max_banner = 1, colorlen;
    sint len, secs, start = 0;
    valueType skip[ 11 ];
    valueType date[ 11 ];
    valueType *made;
    valueType n1[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType n2[ MAX_COLORFUL_NAME_LENGTH ] = {""};

    t = trap_RealTime(nullptr);

    for(i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++) {
        if(g_admin_bans[ i ]->expires != 0 &&
                (g_admin_bans[ i ]->expires - t) < 1) {
            continue;
        }

        found++;
        max = i;
    }

    if(max < 0) {
        adminLocal.ADMP("^3!showbans: ^7no bans to display\n");
        return false;
    }

    if(idSGameCmds::SayArgc() == 2 + skiparg) {
        idSGameCmds::SayArgv(1 + skiparg, skip, sizeof(skip));
        start = atoi(skip);

        // showbans 1 means start with ban 0
        if(start > 0) {
            start--;
        } else if(start < 0) {
            for(i = max, count = 0; i >= 0 && count < -start; i--) {
                if(g_admin_bans[i]->expires == 0 || (g_admin_bans[i]->expires - t) > 0) {
                    count++;
                }
            }

            start = i + 1;
        }
    }

    if(start < 0) {
        start = 0;
    }

    if(start > max) {
        adminLocal.ADMP(va("^3!showbans: ^7%d is the last valid ban\n", max + 1));
        return false;
    }

    for(i = start, count = 0; i <= max && count < MAX_ADMIN_SHOWBANS; i++) {
        if(g_admin_bans[i]->expires != 0 && (g_admin_bans[i]->expires - t) < 1) {
            continue;
        }

        count++;

        len = Q_PrintStrlen(g_admin_bans[ i ]->name);

        if(len > max_name) {
            max_name = len;
        }

        len = Q_PrintStrlen(g_admin_bans[ i ]->banner);

        if(len > max_banner) {
            max_banner = len;
        }
    }

    adminLocal.ADMBP_begin();

    for(i = start, count = 0; i <= max && count < MAX_ADMIN_SHOWBANS; i++) {
        if(g_admin_bans[i]->expires != 0 && (g_admin_bans[i]->expires - t) < 1) {
            continue;
        }

        count++;

        // only print out the the date part of made
        date[ 0 ] = '\0';
        made = g_admin_bans[ i ]->made;

        for(j = 0; made && *made; j++) {
            if((j + 1) >= sizeof(date)) {
                break;
            }

            if(*made == ' ') {
                break;
            }

            date[ j ] = *made;
            date[ j + 1 ] = '\0';
            made++;
        }

        secs = (g_admin_bans[ i ]->expires - t);
        adminLocal.AdminDuration(secs, duration, sizeof(duration));

        for(colorlen = k = 0; g_admin_bans[i]->name[k]; k++) {
            if(Q_IsColorString(&g_admin_bans[i]->name[k])) {
                colorlen += Q_ColorStringLength(&g_admin_bans[i]->name[k]);
            } else if(Q_IsColorEscapeEscape(&g_admin_bans[i]->name[k])) {
                colorlen++;
            }
        }

        Q_vsprintf_s(n1, sizeof(n1), sizeof(n1), "%*s", max_name + colorlen,
                     g_admin_bans[ i ]->name);

        for(colorlen = k = 0; g_admin_bans[i]->banner[k]; k++) {
            if(Q_IsColorString(&g_admin_bans[i]->banner[k])) {
                colorlen += Q_ColorStringLength(&g_admin_bans[i]->banner[k]);
            } else if(Q_IsColorEscapeEscape(&g_admin_bans[i]->banner[k])) {
                colorlen++;
            }
        }

        Q_vsprintf_s(n2, sizeof(n2), sizeof(n2), "%*s", max_banner + colorlen,
                     g_admin_bans[ i ]->banner);

        adminLocal.ADMBP(va("%4i %s^7 %-15s %-8s %s^7 %-10s\n     \\__ %s\n",
                            (i + 1), n1, g_admin_bans[ i ]->ip, date, n2, duration,
                            g_admin_bans[ i ]->reason));
    }

    adminLocal.ADMBP(
        va("^3!showbans:^7 showing bans %d - %d of %d (%d total).",
           (found) ? (start + 1) : 0, i, max + 1, found));

    if(i <= max) {
        adminLocal.ADMBP(va("  run !showbans %d to see more", i + 1));
    }

    adminLocal.ADMBP("\n");
    adminLocal.ADMBP_end();

    return true;
}

/*
===============
idAdminLocal::AdminHelp
===============
*/
bool idAdminLocal::AdminHelp(gentity_t *ent, sint skiparg) {
    sint i;

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        sint j = 0;
        sint count = 0;

        adminLocal.ADMBP_begin();

        for(i = 0; i < adminNumCmds; i++) {
            if(adminLocal.AdminPermission(ent, g_admin_cmds[ i ].flag[ 0 ])) {
                adminLocal.ADMBP(va("^3!%-12s", g_admin_cmds[ i ].keyword));
                j++;
                count++;
            }

            // show 6 commands per line
            if(j == 6) {
                adminLocal.ADMBP("\n");
                j = 0;
            }
        }

        for(i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++) {
            if(!adminLocal.AdminCommandPermission(ent, g_admin_commands[i]->command)) {
                continue;
            }

            adminLocal.ADMBP(va("^3!%-12s", g_admin_commands[ i ]->command));

            j++;
            count++;

            // show 6 commands per line
            if(j == 6) {
                adminLocal.ADMBP("\n");
                j = 0;
            }
        }

        if(count) {
            adminLocal.ADMBP("\n");
        }

        adminLocal.ADMBP(va("^3!help: ^7%i available commands\n", count));
        adminLocal.ADMBP("run !help [^3command^7] for help with a specific command.\n");
        adminLocal.ADMBP_end();

        return true;
    } else {
        //!help param
        valueType param[ MAX_ADMIN_CMD_LEN ];
        valueType *cmd;

        idSGameCmds::SayArgv(1 + skiparg, param, sizeof(param));
        cmd = (param[0] == '!') ? &param[1] : &param[0];
        adminLocal.ADMBP_begin();

        for(i = 0; i < adminNumCmds; i++) {
            if(!Q_stricmp(cmd, g_admin_cmds[ i ].keyword)) {
                if(!adminLocal.AdminPermission(ent, g_admin_cmds[ i ].flag[ 0 ])) {
                    adminLocal.ADMBP(va("^3!help: ^7you have no permission to use '%s'\n",
                                        g_admin_cmds[ i ].keyword));
                    adminLocal.ADMBP_end();
                    return false;
                }

                adminLocal.ADMBP(va("^3!help: ^7help for '!%s':\n",
                                    g_admin_cmds[ i ].keyword));
                adminLocal.ADMBP(va(" ^3Function: ^7%s\n", g_admin_cmds[ i ].function));
                adminLocal.ADMBP(va(" ^3Syntax: ^7!%s %s\n", g_admin_cmds[ i ].keyword,
                                    g_admin_cmds[ i ].syntax));
                adminLocal.ADMBP(va(" ^3Flag: ^7'%c'\n", g_admin_cmds[ i ].flag[ 0 ]));
                adminLocal.ADMBP_end();
                return true;
            }
        }

        for(i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++) {
            if(!Q_stricmp(cmd, g_admin_commands[ i ]->command)) {
                if(!adminLocal.AdminCommandPermission(ent,
                                                      g_admin_commands[ i ]->command)) {
                    adminLocal.ADMBP(va("^3!help: ^7you have no permission to use '%s'\n",
                                        g_admin_commands[ i ]->command));
                    adminLocal.ADMBP_end();
                    return false;
                }

                adminLocal.ADMBP(va("^3!help: ^7help for '%s':\n",
                                    g_admin_commands[ i ]->command));
                adminLocal.ADMBP(va(" ^3Description: ^7%s\n",
                                    g_admin_commands[ i ]->desc));
                adminLocal.ADMBP(va(" ^3Syntax: ^7!%s\n", g_admin_commands[ i ]->command));
                adminLocal.ADMBP_end();
                return true;
            }
        }

        adminLocal.ADMBP(va("^3!help: ^7no help found for '%s'\n", cmd));
        adminLocal.ADMBP_end();
        return false;
    }
}

/*
===============
idAdminLocal::AdminAdminTest
===============
*/
bool idAdminLocal::AdminAdminTest(gentity_t *ent, sint skiparg) {
    sint i, l = 0;
    bool found = false;
    bool lname = false;

    if(!ent) {
        adminLocal.ADMP("^3!admintest: ^7you are on the console.\n");
        return true;
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        if(!Q_stricmp(g_admin_admins[ i ]->guid, ent->client->pers.guid)) {
            found = true;
            break;
        }
    }

    if(found) {
        l = g_admin_admins[ i ]->level;

        for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
            if(g_admin_levels[i]->level != l) {
                continue;
            }

            if(*g_admin_levels[ i ]->name) {
                lname = true;
                break;
            }
        }
    }

    AP(va("print \"^3!admintest: ^7%s^7 is a level %d admin %s%s^7%s\n\"",
          ent->client->pers.netname, l,
          (lname) ? "(" : "", (lname) ? g_admin_levels[ i ]->name : "",
          (lname) ? ")" : ""));

    return true;
}

/*
===============
idAdminLocal::AdminAllReady
===============
*/
bool idAdminLocal::AdminAllReady(gentity_t *ent, sint skiparg) {
    sint i = 0;
    gclient_t *cl;

    if(!level.intermissiontime) {
        adminLocal.ADMP("^3!allready: ^7this command is only valid during intermission\n");
        return false;
    }

    for(i = 0; i < g_maxclients.integer; i++) {
        cl = level.clients + i;

        if(cl->pers.connected != CON_CONNECTED) {
            continue;
        }

        if(cl->pers.teamSelection == TEAM_NONE) {
            continue;
        }

        cl->readyToExit = 1;
    }

    AP(va("print \"^3!allready:^7 %s^7 says everyone is READY now\n\"",
          (ent) ? ent->client->pers.netname : "console"));

    return true;
}

/*
===============
idAdminLocal::AdminCancelVote
===============
*/
bool idAdminLocal::AdminCancelVote(gentity_t *ent, sint skiparg) {

    if(!level.voteTime && !level.teamVoteTime[ 0 ] &&
            !level.teamVoteTime[ 1 ]) {
        adminLocal.ADMP("^3!cancelvote: ^7no vote in progress\n");
        return false;
    }

    level.voteNo = level.numConnectedClients;
    level.voteYes = 0;

    idSGameMain::CheckVote();

    level.teamVoteNo[ 0 ] = level.numConnectedClients;
    level.teamVoteYes[ 0 ] = 0;

    idSGameMain::CheckTeamVote(TEAM_HUMANS);

    level.teamVoteNo[ 1 ] = level.numConnectedClients;
    level.teamVoteYes[ 1 ] = 0;

    idSGameMain::CheckTeamVote(TEAM_ALIENS);

    AP(va("print \"^3!cancelvote: ^7%s^7 decided that everyone voted No\n\"",
          (ent) ? ent->client->pers.netname : "console"));

    return true;
}

/*
===============
idAdminLocal::AdminPassVote
===============
*/
bool idAdminLocal::AdminPassVote(gentity_t *ent, sint skiparg) {
    if(!level.voteTime && !level.teamVoteTime[ 0 ] &&
            !level.teamVoteTime[ 1 ]) {
        adminLocal.ADMP("^3!passvote: ^7no vote in progress\n");
        return false;
    }

    level.voteYes = level.numConnectedClients;
    level.voteNo = 0;

    idSGameMain::CheckVote();

    level.teamVoteYes[ 0 ] = level.numConnectedClients;
    level.teamVoteNo[ 0 ] = 0;

    idSGameMain::CheckTeamVote(TEAM_HUMANS);

    level.teamVoteYes[ 1 ] = level.numConnectedClients;
    level.teamVoteNo[ 1 ] = 0;

    idSGameMain::CheckTeamVote(TEAM_ALIENS);

    AP(va("print \"^3!passvote: ^7%s^7 decided that everyone voted Yes\n\"",
          (ent) ? ent->client->pers.netname : "console"));

    return true;
}

/*
===============
idAdminLocal::AdminSpec999
===============
*/
bool idAdminLocal::AdminSpec999(gentity_t *ent, sint skiparg) {
    sint i;
    gentity_t *vic;

    for(i = 0; i < level.maxclients; i++) {
        vic = &g_entities[ i ];

        if(!vic->client) {
            continue;
        }

        if(vic->client->pers.connected != CON_CONNECTED) {
            continue;
        }

        if(vic->client->pers.teamSelection == TEAM_NONE) {
            continue;
        }

        if(vic->client->ps.ping == 999) {
            idSGameTeam::ChangeTeam(vic, TEAM_NONE);
            AP(va("print \"^3!spec999: ^7%s^7 moved ^7%s^7 to spectators\n\"",
                  (ent) ? ent->client->pers.netname : "console", vic->client->pers.netname));
        }
    }

    return true;
}

/*
===============
idAdminLocal::AdminRename
===============
*/
bool idAdminLocal::AdminRename(gentity_t *ent, sint skiparg) {
    sint pid;
    valueType name[ MAX_COLORFUL_NAME_LENGTH ];
    valueType newname[ MAX_COLORFUL_NAME_LENGTH ];
    valueType oldname[ MAX_COLORFUL_NAME_LENGTH ];
    valueType err[ MAX_STRING_CHARS ];
    valueType userinfo[ MAX_INFO_STRING ];
    valueType *s;
    gentity_t *victim = nullptr;

    if(idSGameCmds::SayArgc() < 3 + skiparg) {
        adminLocal.ADMP("^3!rename: ^7usage: !rename [name] [newname]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, name, sizeof(name));

    s = idSGameCmds::SayConcatArgs(2 + skiparg);
    Q_strncpyz(newname, s, sizeof(newname));

    if((pid = idSGameCmds::ClientNumberFromString(name, err,
              sizeof(err))) == -1) {
        adminLocal.ADMP(va("^3!rename: ^7%s\n", err));
        return false;
    }

    victim = &g_entities[ pid ];

    if(!adminLocal.AdminHigher(ent, victim)) {
        adminLocal.ADMP("^3!rename: ^7sorry, but your intended victim has a higher admin level than you\n");
        return false;
    }

    if(!adminLocal.AdminNameCheck(victim, newname, err, sizeof(err))) {
        adminLocal.ADMP(va("^3!rename: ^7%s\n", err));
        return false;
    }

    level.clients[ pid ].pers.nameChanges--;
    level.clients[ pid ].pers.nameChangeTime = 0;

    trap_GetUserinfo(pid, userinfo, sizeof(userinfo));

    s = Info_ValueForKey(userinfo, "name");
    Q_strncpyz(oldname, s, sizeof(oldname));

    Info_SetValueForKey(userinfo, "name", newname);
    trap_SetUserinfo(pid, userinfo);

    sgameLocal.ClientUserinfoChanged(pid);

    AP(va("print \"^3!rename: ^7%s^7 has been renamed to %s^7 by %s\n\"",
          oldname, newname, (ent) ? ent->client->pers.netname : "console"));

    return true;
}

/*
===============
idAdminLocal::AdminRestart
===============
*/
bool idAdminLocal::AdminRestart(gentity_t *ent, sint skiparg) {
    valueType layout[MAX_CVAR_VALUE_STRING] = { "" };
    valueType teampref[MAX_STRING_CHARS] = { "" };
    sint i;
    gclient_t *cl;

    if(trap_Argc() > 1) {
        valueType map[MAX_QPATH];

        trap_Cvar_VariableStringBuffer("mapname", map, sizeof(map));
        trap_Argv(skiparg, layout, sizeof(layout));

        // Figure out which argument is which
        if(Q_stricmp(layout, "keepteams") && Q_stricmp(layout, "keepteamslock") &&
                Q_stricmp(layout, "switchteams") && Q_stricmp(layout, "switchteamslock")) {
            if(!Q_stricmp(layout, "*BUILTIN*") ||
                    trap_FS_FOpenFile(va("layouts/%s/%s.dat", map, layout), nullptr,
                                      FS_READ) > 0) {
                trap_Cvar_Set("g_layouts", layout);
            } else {
                adminLocal.ADMP(va("^3restart: ^7layout '%s' does not exist\n", layout));
                return false;
            }
        } else {
            layout[0] = '\0';
            trap_Argv(skiparg, teampref, sizeof(teampref));
        }
    }

    if(trap_Argc() > 2) {
        trap_Argv(2, teampref, sizeof(teampref));
    }

    if(!Q_stricmpn(teampref, "keepteams", 9)) {
        for(i = 0; i < g_maxclients.integer; i++) {
            cl = level.clients + i;

            if(cl->pers.connected != CON_CONNECTED) {
                continue;
            }

            if(cl->pers.teamSelection == TEAM_NONE) {
                continue;
            }

            cl->sess.restartTeam = cl->pers.teamSelection;
        }
    } else if(!Q_stricmpn(teampref, "switchteams", 11)) {
        for(i = 0; i < g_maxclients.integer; i++) {
            cl = level.clients + i;

            if(cl->pers.connected != CON_CONNECTED) {
                continue;
            }

            if(cl->pers.teamSelection == TEAM_HUMANS) {
                cl->sess.restartTeam = TEAM_ALIENS;
            } else if(cl->pers.teamSelection == TEAM_ALIENS) {
                cl->sess.restartTeam = TEAM_HUMANS;
            }
        }
    }

    if(!Q_stricmp(teampref, "switchteamslock") ||
            !Q_stricmp(teampref, "keepteamslock")) {
        trap_Cvar_Set("g_lockTeamsAtStart", "1");
    }

    trap_SendConsoleCommand(EXEC_APPEND, "map_restart");

    AP(va("print \"^3restart: ^7map restarted by %s %s %s\n\"",
          (ent) ? ent->client->pers.netname : "console",
          (layout[0]) ? va("^7(forcing layout '%s^7')", layout) : "",
          (teampref[0]) ? va("^7(with teams option: '%s^7')", teampref) : ""));

    return true;
}

/*
===============
idAdminLocal::AdminNextmap
===============
*/
bool idAdminLocal::AdminNextmap(gentity_t *ent, sint skiparg) {
    AP(va("print \"^3!nextmap: ^7%s^7 decided to load the next map\n\"",
          (ent) ? ent->client->pers.netname : "console"));

    level.lastWin = TEAM_NONE;

    trap_SetConfigstring(CS_WINNER, "Evacuation");

    idSGameMain::LogExit(va("nextmap was run by %s",
                            (ent) ? ent->client->pers.netname : "console"));

    return true;
}

/*
===============
idAdminLocal::AdminNameLog
===============
*/
bool idAdminLocal::AdminNameLog(gentity_t *ent, sint skiparg) {
    sint i, j;
    valueType search[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType s2[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType n2[ MAX_COLORFUL_NAME_LENGTH ] = {""};
    valueType guid_stub[ 9 ];
    bool found = false;
    sint printed = 0;

    if(idSGameCmds::SayArgc() > 1 + skiparg) {
        idSGameCmds::SayArgv(1 + skiparg, search, sizeof(search));
        idSGameCmds::SanitiseString(search, s2, sizeof(s2));
    }

    adminLocal.ADMBP_begin();

    for(i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++) {
        if(search[ 0 ]) {
            found = false;

            for(j = 0; j < MAX_ADMIN_NAMELOG_NAMES &&
                    g_admin_namelog[ i ]->name[ j ][ 0 ]; j++) {
                idSGameCmds::SanitiseString(g_admin_namelog[ i ]->name[ j ], n2,
                                            sizeof(n2));

                if(strstr(n2, s2)) {
                    found = true;
                    break;
                }
            }

            if(!found) {
                continue;
            }
        }

        printed++;

        for(j = 0; j < 8; j++) {
            guid_stub[j] = g_admin_namelog[i]->guid[j + 24];
        }

        guid_stub[ j ] = '\0';

        if(g_admin_namelog[i]->slot > -1) {
            adminLocal.ADMBP("^3");
        }

        adminLocal.ADMBP(va("%-2s (*%s) %15s^7",
                            (g_admin_namelog[ i ]->slot > -1) ?
                            va("%d", g_admin_namelog[ i ]->slot) : "-", guid_stub,
                            g_admin_namelog[ i ]->ip));

        for(j = 0; j < MAX_ADMIN_NAMELOG_NAMES &&
                g_admin_namelog[ i ]->name[ j ][ 0 ]; j++) {
            adminLocal.ADMBP(va(" '%s^7'", g_admin_namelog[ i ]->name[ j ]));
        }

        adminLocal.ADMBP("\n");
    }

    adminLocal.ADMBP(va("^3!namelog:^7 %d recent clients found\n", printed));
    adminLocal.ADMBP_end();

    return true;
}

/*
===============
idAdminLocal::AdminLock
===============
*/
bool idAdminLocal::AdminLock(gentity_t *ent, sint skiparg) {
    valueType teamName[2] = {""};
    team_t team;

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        adminLocal.ADMP("^3!lock: ^7usage: !lock [a|h]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, teamName, sizeof(teamName));
    team = idSGameTeam::TeamFromString(teamName);

    if(team == TEAM_ALIENS) {
        if(level.alienTeamLocked) {
            adminLocal.ADMP("^3!lock: ^7the alien team is already locked\n");
            return false;
        }

        level.alienTeamLocked = true;
    } else if(team == TEAM_HUMANS) {
        if(level.humanTeamLocked) {
            adminLocal.ADMP("^3!lock: ^7the human team is already locked\n");
            return false;
        }

        level.humanTeamLocked = true;
    } else {
        adminLocal.ADMP(va("^3!lock: ^7invalid team\"%c\"\n", teamName[0]));
        return false;
    }

    AP(va("print \"^3!lock: ^7the %s team has been locked by %s\n\"",
          bggame->TeamName(team), (ent) ? ent->client->pers.netname : "console"));
    return true;
}

/*
===============
idAdminLocal::AdminUnlock
===============
*/
bool idAdminLocal::AdminUnlock(gentity_t *ent, sint skiparg) {
    valueType teamName[2] = {""};
    team_t team;

    if(idSGameCmds::SayArgc() < 2 + skiparg) {
        adminLocal.ADMP("^3!unlock: ^7usage: !unlock [a|h]\n");
        return false;
    }

    idSGameCmds::SayArgv(1 + skiparg, teamName, sizeof(teamName));
    team = idSGameTeam::TeamFromString(teamName);

    if(team == TEAM_ALIENS) {
        if(!level.alienTeamLocked) {
            adminLocal.ADMP("^3!unlock: ^7the alien team is not currently locked\n");
            return false;
        }

        level.alienTeamLocked = false;
    } else if(team == TEAM_HUMANS) {
        if(!level.humanTeamLocked) {
            adminLocal.ADMP("^3!unlock: ^7the human team is not currently locked\n");
            return false;
        }

        level.humanTeamLocked = false;
    } else {
        adminLocal.ADMP(va("^3!unlock: ^7invalid team\"%c\"\n", teamName[0]));
        return false;
    }

    AP(va("print \"^3!unlock: ^7the %s team has been unlocked by %s\n\"",
          bggame->TeamName(team), (ent) ? ent->client->pers.netname : "console"));

    return true;
}

/*
================
idAdminLocal::AdminPrint

This function facilitates the ADMP define.  ADMP() is similar to CP except
that it prints the message to the server console if ent is not defined.
================
*/
void idAdminLocal::AdminPrint(gentity_t *ent, valueType *m) {
    if(ent) {
        trap_SendServerCommand(ent - level.gentities, va("print \"%s\"", m));
    } else {
        valueType m2[ MAX_STRING_CHARS ];

        if(!trap_Cvar_VariableIntegerValue("com_ansiColor")) {
            idSGameCmds::DecolorString(m, m2, sizeof(m2));
            trap_Print(m2);
        } else {
            trap_Print(m);
        }
    }
}

/*
===============
idAdminLocal::AdminBufferBegin
===============
*/
void idAdminLocal::AdminBufferBegin(void) {
    g_bfb[ 0 ] = '\0';
}

/*
===============
idAdminLocal::AdminBufferEnd
===============
*/
void idAdminLocal::AdminBufferEnd(gentity_t *ent) {
    ADMP(g_bfb);
}

/*
===============
idAdminLocal::AdminBufferPrint
===============
*/
void idAdminLocal::AdminBufferPrint(gentity_t *ent, valueType *m) {
    // 1022 - strlen("print 64 \"\"") - 1
    if(strlen(m) + strlen(g_bfb) >= 1009) {
        adminLocal.ADMP(g_bfb);
        g_bfb[ 0 ] = '\0';
    }

    Q_strcat(g_bfb, sizeof(g_bfb), m);
}

/*
===============
idAdminLocal::AdminCleanup
===============
*/
void idAdminLocal::AdminCleanup(void) {
    sint i = 0;

    for(i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++) {
        bggame->Free(g_admin_levels[ i ]);
        g_admin_levels[ i ] = nullptr;
    }

    for(i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++) {
        bggame->Free(g_admin_admins[ i ]);
        g_admin_admins[ i ] = nullptr;
    }

    for(i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++) {
        bggame->Free(g_admin_bans[ i ]);
        g_admin_bans[ i ] = nullptr;
    }

    for(i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++) {
        bggame->Free(g_admin_commands[ i ]);
        g_admin_commands[ i ] = nullptr;
    }
}
