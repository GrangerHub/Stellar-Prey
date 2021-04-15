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
// File name:   sgame_admin.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __G_ADMIN_H__
#define __G_ADMIN_H__

#define MAX_ADMIN_LEVELS 32
#define MAX_ADMIN_ADMINS 1024
#define MAX_ADMIN_BANS 1024
#define MAX_ADMIN_NAMELOGS 128
#define MAX_ADMIN_NAMELOG_NAMES 5
#define MAX_ADMIN_FLAGS 64
#define MAX_ADMIN_COMMANDS 64
#define MAX_ADMIN_CMD_LEN 20
#define MAX_ADMIN_BAN_REASON 50
#define MAX_ADMIN_ADMINLOGS 128
#define MAX_ADMIN_ADMINLOG_ARGS 50

/*
 * 1 - cannot be vote kicked, vote muted
 * 2 - cannot be censored or flood protected TODO
 * 3 - never loses credits for changing teams
 * 4 - can see team chat as a spectator
 * 5 - can switch teams any time, regardless of balance
 * 6 - does not need to specify a reason for a kick/ban
 * 7 - can call a vote at any time (regardless of a vote being disabled or
 * voting limitations)
 * 8 - does not need to specify a duration for a ban
 * 9 - can run commands from team chat
 * 0 - inactivity rules do not apply to them
 * ! - admin commands cannot be used on them
 * @ - does not show up as an admin in !listplayers
 * $ - sees all information in !listplayers
 * ? - receieves and can send /a admin messages
 */
#define ADMF_IMMUNITY '1'
#define ADMF_NOCENSORFLOOD '2' /* TODO */
#define ADMF_TEAMCHANGEFREE '3'
#define ADMF_SPEC_ALLCHAT '4'
#define ADMF_FORCETEAMCHANGE '5'
#define ADMF_UNACCOUNTABLE '6'
#define ADMF_NO_VOTE_LIMIT '7'
#define ADMF_CAN_PERM_BAN '8'
#define ADMF_TEAMCHAT_CMD '9'
#define ADMF_ACTIVITY '0'
#define ADMF_IMMUTABLE '!'
#define ADMF_INCOGNITO '@'
#define ADMF_SEESFULLLISTPLAYERS '$'
#define ADMF_ADMINCHAT '?'

#define MAX_ADMIN_LISTITEMS 20
#define MAX_ADMIN_SHOWBANS 10

// important note: QVM does not seem to allow a single valueType to be a
// member of a struct at init time.  flag has been converted to valueType*
typedef struct g_admin_cmd {
    valueType *keyword;
    bool(* handler)(gentity_t *ent, sint skiparg);
    valueType *flag;
    valueType *function;  // used for !help
    valueType *syntax;  // used for !help
} g_admin_cmd_t;

typedef struct g_admin_level {
    sint level;
    valueType name[ MAX_NAME_LENGTH ];
    valueType flags[ MAX_ADMIN_FLAGS ];
} g_admin_level_t;

typedef struct g_admin_admin {
    valueType guid[ 33 ];
    valueType name[ MAX_NAME_LENGTH ];
    sint level;
    valueType flags[ MAX_ADMIN_FLAGS ];
    sint counter;
} g_admin_admin_t;

typedef struct g_admin_ban {
    valueType name[ MAX_NAME_LENGTH ];
    valueType guid[ 33 ];
    valueType ip[ 40 ];
    valueType reason[ MAX_ADMIN_BAN_REASON ];
    valueType made[ 18 ]; // big enough for strftime() %c
    sint expires;
    valueType banner[ MAX_NAME_LENGTH ];
} g_admin_ban_t;

typedef struct g_admin_command {
    valueType command[ MAX_ADMIN_CMD_LEN ];
    valueType exec[ MAX_QPATH ];
    valueType desc[ 50 ];
    sint levels[ MAX_ADMIN_LEVELS + 1 ];
} g_admin_command_t;

typedef struct g_admin_namelog {
    valueType name[ MAX_ADMIN_NAMELOG_NAMES ][MAX_NAME_LENGTH ];
    valueType ip[ 40 ];
    valueType guid[ 33 ];
    sint slot;
    bool banned;
} g_admin_namelog_t;

typedef struct g_admin_adminlog {
    valueType name[MAX_NAME_LENGTH];
    valueType command[MAX_ADMIN_CMD_LEN];
    valueType args[MAX_ADMIN_ADMINLOG_ARGS];
    sint id;
    sint time;
    sint level;
    bool success;
} g_admin_adminlog_t;

//
// idAdminLocal
//
class idAdminLocal {
public:
    bool AdminBanCheck(valueType *userinfo, valueType *reason, sint rlen);
    bool AdminCmdCheck(gentity_t *ent, bool say);
    static bool AdminReadconfig(gentity_t *ent, sint skiparg);
    void AdminWriteConfig(void);
    bool AdminPermission(gentity_t *ent, valueType flag);
    bool AdminNameCheck(gentity_t *ent, valueType *name, valueType *err,
                        sint len);
    void AdminNamelogUpdate(gclient_t *ent, bool disconnect);
    g_admin_admin_t *Admin(gentity_t *ent);
    sint AdminParseTime(pointer time);

    static bool AdminTime(gentity_t *ent, sint skiparg);
    static bool AdminSetlevel(gentity_t *ent, sint skiparg);
    static bool AdminKick(gentity_t *ent, sint skiparg);
    static bool AdminAdjustBan(gentity_t *ent, sint skiparg);
    static bool AdminBan(gentity_t *ent, sint skiparg);
    static bool AdminUnBan(gentity_t *ent, sint skiparg);
    static bool AdminPutTeam(gentity_t *ent, sint skiparg);
    static bool AdminListAdmins(gentity_t *ent, sint skiparg);
    static bool AdminListLayouts(gentity_t *ent, sint skiparg);
    static bool AdminListPlayers(gentity_t *ent, sint skiparg);
    static bool AdminMap(gentity_t *ent, sint skiparg);
    static bool AdminMute(gentity_t *ent, sint skiparg);
    static bool AdminDenyBuild(gentity_t *ent, sint skiparg);
    static bool AdminShowBans(gentity_t *ent, sint skiparg);
    static bool AdminHelp(gentity_t *ent, sint skiparg);
    static bool AdminAdminTest(gentity_t *ent, sint skiparg);
    static bool AdminAllReady(gentity_t *ent, sint skiparg);
    static bool AdminCancelVote(gentity_t *ent, sint skiparg);
    static bool AdminPassVote(gentity_t *ent, sint skiparg);
    static bool AdminSpec999(gentity_t *ent, sint skiparg);
    static bool AdminRename(gentity_t *ent, sint skiparg);
    static bool AdminRestart(gentity_t *ent, sint skiparg);
    static bool AdminNextmap(gentity_t *ent, sint skiparg);
    static bool AdminNameLog(gentity_t *ent, sint skiparg);
    static bool AdminLock(gentity_t *ent, sint skiparg);
    static bool AdminUnlock(gentity_t *ent, sint skiparg);
    void AdminPrint(gentity_t *ent, valueType *m);
    void AdminBufferPrint(gentity_t *ent, valueType *m);
    void AdminBufferBegin(void);
    void AdminBufferEnd(gentity_t *ent);
    void AdminDuration(sint secs, valueType *duration, sint dursize);
    void AdminCleanup(void);
    void AdminNamelogCleanup(void);
    bool AdminCreateBan(gentity_t *ent, valueType *netname, valueType *guid,
                        valueType *ip, sint seconds, valueType *reason);
    bool AdminHigherGuid(valueType *admin_guid, valueType *victim_guid);
    bool AdminHigher(gentity_t *admin, gentity_t *victim);
    void AdminWriteConfigString(valueType *s, fileHandle_t f);
    void AdminWriteConfigInt(sint v, fileHandle_t f);
    void AdminReadConfigString(valueType **cnf, valueType *s, sint size);
    void AdminReadConfigInt(valueType **cnf, sint *v);
    void AdminDefaultLevels(void);
    bool AdminCommandPermission(gentity_t *ent, valueType *command);
    void AdminLog(gentity_t *admin, valueType *cmd, sint skiparg);
    sint AdminListAdmins(gentity_t *ent, sint start, valueType *search);

public:
#define AP(x) trap_SendServerCommand(-1, x)
#define CP(x) trap_SendServerCommand(ent-g_entities, x)
#define CPx(x, y) trap_SendServerCommand(x, y)
#define ADMP(x) AdminPrint(ent, x)
#define ADMBP(x) AdminBufferPrint(ent, x)
#define ADMBP_begin() AdminBufferBegin()
#define ADMBP_end() AdminBufferEnd(ent)
};


extern idAdminLocal adminLocal;

#endif //!_G_ADMIN_H__ 
