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
// File name:   cgame_local.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CG_LOCAL_H__
#define __CG_LOCAL_H__

// The entire cgame module is unloaded and reloaded on each level change,
// so there is NO persistant data between levels on the client side.
// If you absolutely need something stored, it can either be kept
// by the server in the server stored userinfos, or stashed in a cvar.

#define CG_FONT_THRESHOLD   0.1
#define POWERUP_BLINKS      5
#define POWERUP_BLINK_TIME  1000
#define FADE_TIME           200
#define PULSE_TIME          200
#define DAMAGE_DEFLECT_TIME 100
#define DAMAGE_RETURN_TIME  400
#define DAMAGE_TIME         500
#define LAND_DEFLECT_TIME   150
#define LAND_RETURN_TIME    300
#define DUCK_TIME           100
#define PAIN_TWITCH_TIME    200
#define WEAPON_SELECT_TIME  1400
#define ITEM_SCALEUP_TIME   1000
#define ZOOM_TIME           150
#define ITEM_BLOB_TIME      200
#define MUZZLE_FLASH_TIME   20
#define SINK_TIME           1000    // time for fragments to sink into ground before going away
#define ATTACKER_HEAD_TIME  10000
#define REWARD_TIME         3000
#define PULSE_SCALE         1.5     // amount to scale up the icons when activating
#define MAX_STEP_CHANGE     32
#define MAX_VERTS_ON_POLY   10
#define MAX_MARK_POLYS      256
#define STAT_MINUS          10  // num frame for '-' stats digit
#define ICON_SIZE           48
#ifndef Q3MAP2
#ifdef CHAR_WIDTH
#undef CHAR_WIDTH
#endif
#define CHAR_WIDTH          32
#endif
#define CHAR_HEIGHT         48
#define TEXT_ICON_SPACE     4

// very large characters
#define GIANT_WIDTH         32
#define GIANT_HEIGHT        48

#define NUM_CROSSHAIRS      10
#define TEAM_OVERLAY_MAXNAME_WIDTH  12
#define TEAM_OVERLAY_MAXLOCATION_WIDTH  16

typedef enum
{
    FOOTSTEP_NORMAL,
    FOOTSTEP_FLESH,
    FOOTSTEP_METAL,
    FOOTSTEP_SPLASH,
    FOOTSTEP_CUSTOM,
    FOOTSTEP_NONE,
    
    FOOTSTEP_TOTAL
} footstep_t;

typedef enum
{
    IMPACTSOUND_DEFAULT,
    IMPACTSOUND_METAL,
    IMPACTSOUND_FLESH
} impactSound_t;

typedef enum
{
    JPS_OFF,
    JPS_DESCENDING,
    JPS_HOVERING,
    JPS_ASCENDING
} jetPackState_t;

//======================================================================

// when changing animation, set animationTime to frameTime + lerping time
// The current lerp will finish out, then it will lerp to the new animation
typedef struct
{
    sint oldFrame;
    sint oldFrameTime;     // time when ->oldFrame was exactly on
    
    sint frame;
    sint frameTime;        // time when ->frame will be exactly on
    
    float32 backlerp;
    
    float32 yawAngle;
    bool yawing;
    float32 pitchAngle;
    bool pitching;
    
    sint animationNumber;  // may include ANIM_TOGGLEBIT
    animation_t* animation;
    sint animationTime;    // time when the first frame of the animation will be exact
} lerpFrame_t;

//======================================================================

//attachment system
typedef enum
{
    AT_STATIC,
    AT_TAG,
    AT_CENT,
    AT_PARTICLE
} attachmentType_t;

//forward declaration for particle_t
struct particle_s;

typedef struct attachment_s
{
    attachmentType_t type;
    bool attached;
    bool staticValid;
    bool tagValid;
    bool centValid;
    bool particleValid;
    bool hasOffset;
    vec3_t offset;
    vec3_t lastValidAttachmentPoint;
    
    //AT_STATIC
    vec3_t origin;
    
    //AT_TAG
    refEntity_t re;     //FIXME: should be pointers?
    refEntity_t parent; //
    qhandle_t model;
    valueType tagName[ MAX_STRING_CHARS ];
    
    //AT_CENT
    sint centNum;
    
    //AT_PARTICLE
    struct particle_s* particle;
} attachment_t;

//======================================================================

//particle system stuff
#define MAX_PARTICLE_FILES        128

#define MAX_PS_SHADER_FRAMES      32
#define MAX_PS_MODELS             8
#define MAX_EJECTORS_PER_SYSTEM   4
#define MAX_PARTICLES_PER_EJECTOR 4

#define MAX_BASEPARTICLE_SYSTEMS  192
#define MAX_BASEPARTICLE_EJECTORS MAX_BASEPARTICLE_SYSTEMS*MAX_EJECTORS_PER_SYSTEM
#define MAX_BASEPARTICLES         MAX_BASEPARTICLE_EJECTORS*MAX_PARTICLES_PER_EJECTOR

#define MAX_PARTICLE_SYSTEMS      48
#define MAX_PARTICLE_EJECTORS     MAX_PARTICLE_SYSTEMS*MAX_EJECTORS_PER_SYSTEM
#define MAX_PARTICLES             MAX_PARTICLE_EJECTORS*5

#define PARTICLES_INFINITE        -1
#define PARTICLES_SAME_AS_INITIAL -2

//COMPILE TIME STRUCTURES
typedef enum
{
    PMT_STATIC,
    PMT_STATIC_TRANSFORM,
    PMT_TAG,
    PMT_CENT_ANGLES,
    PMT_NORMAL
} pMoveType_t;

typedef enum
{
    PMD_LINEAR,
    PMD_POINT
} pDirType_t;

typedef struct pMoveValues_u
{
    pDirType_t dirType;
    
    //PMD_LINEAR
    vec3_t dir;
    float32 dirRandAngle;
    
    //PMD_POINT
    vec3_t point;
    float32 pointRandAngle;
    
    float32 mag;
    float32 magRandFrac;
    
    float32 parentVelFrac;
    float32 parentVelFracRandFrac;
} pMoveValues_t;

typedef struct pLerpValues_s
{
    sint delay;
    float32 delayRandFrac;
    
    float32 initial;
    float32 initialRandFrac;
    
    float32 final;
    float32 finalRandFrac;
    
    float32 randFrac;
} pLerpValues_t;

//particle template
typedef struct baseParticle_s
{
    vec3_t displacement;
    float32 randDisplacement;
    float32 normalDisplacement;
    
    pMoveType_t velMoveType;
    pMoveValues_t velMoveValues;
    
    pMoveType_t accMoveType;
    pMoveValues_t accMoveValues;
    
    sint lifeTime;
    float32 lifeTimeRandFrac;
    
    float32 bounceFrac;
    float32 bounceFracRandFrac;
    bool bounceCull;
    
    valueType bounceMarkName[ MAX_QPATH ];
    qhandle_t bounceMark;
    float32 bounceMarkRadius;
    float32 bounceMarkRadiusRandFrac;
    float32 bounceMarkCount;
    float32 bounceMarkCountRandFrac;
    
    valueType bounceSoundName[ MAX_QPATH ];
    qhandle_t bounceSound;
    float32 bounceSoundCount;
    float32 bounceSoundCountRandFrac;
    
    pLerpValues_t radius;
    sint physicsRadius;
    pLerpValues_t alpha;
    pLerpValues_t rotation;
    
    bool dynamicLight;
    pLerpValues_t dLightRadius;
    uchar8 dLightColor[ 3 ];
    
    sint colorDelay;
    float32 colorDelayRandFrac;
    uchar8 initialColor[ 3 ];
    uchar8 finalColor[ 3 ];
    
    valueType childSystemName[ MAX_QPATH ];
    qhandle_t childSystemHandle;
    
    valueType onDeathSystemName[ MAX_QPATH ];
    qhandle_t onDeathSystemHandle;
    
    valueType childTrailSystemName[ MAX_QPATH ];
    qhandle_t childTrailSystemHandle;
    
    //particle invariant stuff
    valueType shaderNames[ MAX_PS_SHADER_FRAMES ][ MAX_QPATH ];
    qhandle_t shaders[ MAX_PS_SHADER_FRAMES ];
    sint numFrames;
    float32 framerate;
    
    valueType modelNames[ MAX_PS_MODELS ][ MAX_QPATH ];
    qhandle_t models[ MAX_PS_MODELS ];
    sint numModels;
    animation_t modelAnimation;
    
    bool overdrawProtection;
    bool realLight;
    bool cullOnStartSolid;
    
    float32 scaleWithCharge;
} baseParticle_t;

//ejector template
typedef struct baseParticleEjector_s
{
    baseParticle_t* particles[ MAX_PARTICLES_PER_EJECTOR ];
    sint numParticles;
    
    pLerpValues_t eject;          //zero period indicates creation of all particles at once
    
    sint totalParticles;         //can be infinite
    float32 totalParticlesRandFrac;
} baseParticleEjector_t;

//particle system template
typedef struct baseParticleSystem_s
{
    valueType name[ MAX_QPATH ];
    baseParticleEjector_t* ejectors[ MAX_EJECTORS_PER_SYSTEM ];
    sint numEjectors;
    
    bool thirdPersonOnly;
    bool registered; //whether or not the assets for this particle have been loaded
} baseParticleSystem_t;

//RUN TIME STRUCTURES
typedef struct particleSystem_s
{
    baseParticleSystem_t* _class;
    
    attachment_t attachment;
    
    bool valid;
    bool lazyRemove; //mark this system for later removal
    
    //for PMT_NORMAL
    bool normalValid;
    vec3_t normal;
    
    sint charge;
} particleSystem_t;

typedef struct particleEjector_s
{
    baseParticleEjector_t* _class;
    particleSystem_t* parent;
    
    pLerpValues_t ejectPeriod;
    
    sint count;
    sint totalParticles;
    
    sint nextEjectionTime;
    
    bool valid;
} particleEjector_t;


//used for actual particle evaluation
typedef struct particle_s
{
    baseParticle_t* _class;
    particleEjector_t* parent;
    
    sint birthTime;
    sint lifeTime;
    
    float32 bounceMarkRadius;
    sint bounceMarkCount;
    sint bounceSoundCount;
    bool atRest;
    
    vec3_t origin;
    vec3_t velocity;
    
    pMoveType_t accMoveType;
    pMoveValues_t accMoveValues;
    
    sint lastEvalTime;
    
    sint nextChildTime;
    
    pLerpValues_t radius;
    pLerpValues_t alpha;
    pLerpValues_t rotation;
    
    pLerpValues_t dLightRadius;
    
    sint colorDelay;
    
    qhandle_t model;
    lerpFrame_t lf;
    vec3_t lastAxis[ 3 ];
    
    bool valid;
    sint frameWhenInvalidated;
    
    sint sortKey;
} particle_t;

//======================================================================

//trail system stuff
#define MAX_TRAIL_FILES           128

#define MAX_BEAMS_PER_SYSTEM      4

#define MAX_BASETRAIL_SYSTEMS     64
#define MAX_BASETRAIL_BEAMS       MAX_BASETRAIL_SYSTEMS*MAX_BEAMS_PER_SYSTEM

#define MAX_TRAIL_SYSTEMS         32
#define MAX_TRAIL_BEAMS           MAX_TRAIL_SYSTEMS*MAX_BEAMS_PER_SYSTEM
#define MAX_TRAIL_BEAM_NODES      128

#define MAX_TRAIL_BEAM_JITTERS    4

typedef enum
{
    TBTT_STRETCH,
    TBTT_REPEAT
} trailBeamTextureType_t;

typedef struct baseTrailJitter_s
{
    float32 magnitude;
    sint period;
} baseTrailJitter_t;

//beam template
typedef struct baseTrailBeam_s
{
    sint numSegments;
    float32 frontWidth;
    float32 backWidth;
    float32 frontAlpha;
    float32 backAlpha;
    uchar8 frontColor[ 3 ];
    uchar8 backColor[ 3 ];
    
    // the time it takes for a segment to vanish (single attached only)
    sint segmentTime;
    
    // the time it takes for a beam to fade out (double attached only)
    sint fadeOutTime;
    
    valueType shaderName[ MAX_QPATH ];
    qhandle_t shader;
    
    trailBeamTextureType_t textureType;
    
    //TBTT_STRETCH
    float32 frontTextureCoord;
    float32 backTextureCoord;
    
    //TBTT_REPEAT
    float32 repeatLength;
    bool clampToBack;
    
    bool realLight;
    
    sint numJitters;
    baseTrailJitter_t jitters[ MAX_TRAIL_BEAM_JITTERS ];
    bool jitterAttachments;
} baseTrailBeam_t;

//trail system template
typedef struct baseTrailSystem_s
{
    valueType name[ MAX_QPATH ];
    baseTrailBeam_t* beams[ MAX_BEAMS_PER_SYSTEM ];
    sint numBeams;
    
    sint lifeTime;
    bool thirdPersonOnly;
    bool registered; //whether or not the assets for this trail have been loaded
} baseTrailSystem_t;

typedef struct trailSystem_s
{
    baseTrailSystem_t* _class;
    
    attachment_t frontAttachment;
    attachment_t backAttachment;
    
    sint birthTime;
    sint destroyTime;
    bool valid;
} trailSystem_t;

typedef struct trailBeamNode_s
{
    vec3_t refPosition;
    vec3_t position;
    
    sint timeLeft;
    
    float32 textureCoord;
    float32 halfWidth;
    uchar8 alpha;
    uchar8 color[ 3 ];
    
    vec2_t jitters[ MAX_TRAIL_BEAM_JITTERS ];
    
    struct trailBeamNode_s* prev;
    struct trailBeamNode_s* next;
    
    bool used;
} trailBeamNode_t;

typedef struct trailBeam_s
{
    baseTrailBeam_t* _class;
    trailSystem_t* parent;
    
    trailBeamNode_t nodePool[ MAX_TRAIL_BEAM_NODES ];
    trailBeamNode_t* nodes;
    
    sint lastEvalTime;
    
    bool valid;
    
    sint nextJitterTimes[ MAX_TRAIL_BEAM_JITTERS ];
} trailBeam_t;

//======================================================================

// player entities need to track more information
// than any other type of entity.

// note that not every player entity is a client entity,
// because corpses after respawn are outside the normal
// client numbering range

// smoothing of view and model for WW transitions
#define MAXSMOOTHS 32

typedef struct
{
    float32 time;
    float32 timeMod;
    
    vec3_t rotAxis;
    float32 rotAngle;
} smooth_t;


typedef struct
{
    lerpFrame_t legs, torso, nonseg, weapon;
    sint painTime;
    sint painDirection;  // flip from 0 to 1
    
    bool squadMarked;    // player has been marked as a squadmember
    
    // machinegun spinning
    float32 barrelAngle;
    sint barrelTime;
    bool barrelSpinning;
    
    refEntity_t bodyRefEnt, headRefEnt, gunRefEnt;
    vec3_t lastNormal;
    vec3_t lastAxis[ 3 ];
    smooth_t sList[ MAXSMOOTHS ];
} playerEntity_t;

typedef struct lightFlareStatus_s
{
    float32 lastRadius;    //caching of likely flare radius
    float32 lastRatio;     //caching of likely flare ratio
    sint lastTime;      //last time flare was visible/occluded
    bool status;        //flare is visble?
} lightFlareStatus_t;

typedef struct buildableStatus_s
{
    sint lastTime;      // Last time status was visible
    bool visible;       // Status is visble?
} buildableStatus_t;

typedef struct buildableCache_s
{
    vec3_t cachedOrigin;   // If either the cached entity origin or the
    vec3_t cachedNormal;   // cached surfNormal change the cache is invalid
    vec3_t axis[ 3 ];
    vec3_t origin;
} buildableCache_t;

//=================================================

// centity_t have a direct corespondence with gentity_t in the game, but
// only the entityState_t is directly communicated to the cgame
typedef struct centity_s
{
    entityState_t currentState; // from cg.frame
    entityState_t nextState; // from cg.nextFrame, if available
    bool interpolate; // true if next is valid to interpolate to
    bool currentValid; // true if cg.frame holds this entity
    
    sint muzzleFlashTime;  // move to playerEntity?
    sint muzzleFlashTime2; // move to playerEntity?
    sint muzzleFlashTime3; // move to playerEntity?
    sint previousEvent;
    sint teleportFlag;
    
    sint trailTime; // so missile trails can handle dropped initial packets
    sint dustTrailTime;
    sint miscTime;
    sint soundTime;
    sint snapShotTime; // last time this entity was found in a snapshot
    
    playerEntity_t pe;
    
    sint errorTime;        // decay the error from this time
    vec3_t errorOrigin;
    vec3_t errorAngles;
    
    bool extrapolated;     // false if origin / angles is an interpolation
    vec3_t rawOrigin;
    vec3_t rawAngles;
    
    vec3_t beamEnd;
    
    // exact interpolated position of entity on this frame
    vec3_t lerpOrigin;
    vec3_t lerpAngles;
    
    lerpFrame_t lerpFrame;
    
    buildableAnimNumber_t buildableAnim;    //persistant anim number
    buildableAnimNumber_t oldBuildableAnim; //to detect when new anims are set
    particleSystem_t* buildablePS;     //handles things like smoke/blood when heavily damaged
    particleSystem_t* buildableHitPS;  //handles when a buildable is hit
    buildableStatus_t buildableStatus;
    buildableCache_t buildableCache;   // so we don't recalculate things
    float32 lastBuildableHealthScale;
    sint lastBuildableDamageSoundTime;
    
    lightFlareStatus_t lfs;
    
    bool doorState;
    
    bool animInit;
    bool animPlaying;
    bool animLastState;
    
    particleSystem_t* muzzlePS;
    bool muzzlePsTrigger;
    
    particleSystem_t* jetPackPS;
    jetPackState_t jetPackState;
    
    particleSystem_t* poisonCloudedPS;
    
    particleSystem_t* entityPS;
    bool entityPSMissing;
    
    trailSystem_t* level2ZapTS[ 3 ];
    sint level2ZapTime;
    
    trailSystem_t* muzzleTS; //used for the tesla and reactor
    sint muzzleTSDeathTime;
    
    bool valid;
    bool oldValid;
    struct centity_s* nextLocation;
} centity_t;


//======================================================================

typedef struct markPoly_s
{
    struct markPoly_s* prevMark, *nextMark;
    sint time;
    qhandle_t markShader;
    bool alphaFade; // fade alpha instead of rgb
    float32 color[ 4 ];
    poly_t poly;
    polyVert_t verts[ MAX_VERTS_ON_POLY ];
} markPoly_t;

//======================================================================


typedef struct
{
    sint client;
    sint score;
    sint ping;
    sint time;
    sint team;
    weapon_t weapon;
    upgrade_t upgrade;
} score_t;

// each client has an associated clientInfo_t
// that contains media references necessary to present the
// client model and other color coded effects
// this is regenerated each time a client's configstring changes,
// usually as a result of a userinfo (name, model, etc) change
#define MAX_CUSTOM_SOUNDS 32
typedef struct
{
    bool infoValid;
    
    sint clientNum;
    
    valueType name[ MAX_QPATH ];
    team_t team;
    
    sint botSkill; // 0 = not bot, 1-5 = bot
    
    vec3_t color1;
    vec3_t color2;
    
    sint score;                      // updated by score servercmds
    sint location;                   // location index for team mode
    sint health;                     // you only get this info about your teammates
    sint armor;
    sint curWeapon;
    sint handicap;
    
    // when clientinfo is changed, the loading of models/skins/sounds
    // can be deferred until you are dead, to prevent hitches in
    // gameplay
    valueType modelName[ MAX_QPATH ];
    valueType skinName[ MAX_QPATH ];
    
    bool newAnims;                   // true if using the new mission pack animations
    bool fixedlegs;                  // true if legs yaw is always the same as torso yaw
    bool fixedtorso;                 // true if torso never changes yaw
    bool nonsegmented;               // true if model is Q2 style nonsegmented
    
    vec3_t headOffset;                 // move head in icon views
    footstep_t footsteps;
    gender_t gender;                     // from model
    
    qhandle_t legsModel;
    qhandle_t legsSkin;
    
    qhandle_t torsoModel;
    qhandle_t torsoSkin;
    
    qhandle_t headModel;
    qhandle_t headSkin;
    
    qhandle_t nonSegModel;                //non-segmented model system
    qhandle_t nonSegSkin;                 //non-segmented model system
    
    qhandle_t modelIcon;
    
    animation_t animations[ MAX_PLAYER_TOTALANIMATIONS ];
    
    sfxHandle_t sounds[ MAX_CUSTOM_SOUNDS ];
    
    sfxHandle_t customFootsteps[ 4 ];
    sfxHandle_t customMetalFootsteps[ 4 ];
    
    valueType voice[ MAX_VOICE_NAME_LEN ];
    sint voiceTime;
} clientInfo_t;


typedef struct weaponInfoMode_s
{
    float32 flashDlight;
    vec3_t flashDlightColor;
    sfxHandle_t flashSound[ 4 ];  // fast firing weapons randomly choose
    bool continuousFlash;
    
    qhandle_t missileModel;
    sfxHandle_t missileSound;
    float32 missileDlight;
    vec3_t missileDlightColor;
    sint missileRenderfx;
    bool usesSpriteMissle;
    qhandle_t missileSprite;
    sint missileSpriteSize;
    float32 missileSpriteCharge;
    qhandle_t missileParticleSystem;
    qhandle_t missileTrailSystem;
    bool missileRotates;
    bool missileAnimates;
    sint missileAnimStartFrame;
    sint missileAnimNumFrames;
    sint missileAnimFrameRate;
    sint missileAnimLooping;
    
    sfxHandle_t firingSound;
    bool loopFireSound;
    
    qhandle_t muzzleParticleSystem;
    
    bool alwaysImpact;
    qhandle_t impactParticleSystem;
    qhandle_t impactMark;
    qhandle_t impactMarkSize;
    sfxHandle_t impactSound[ 4 ]; //random impact sound
    sfxHandle_t impactFleshSound[ 4 ]; //random impact sound
} weaponInfoMode_t;

// each WP_* weapon enum has an associated weaponInfo_t
// that contains media references necessary to present the
// weapon and its effects
typedef struct weaponInfo_s
{
    bool registered;
    valueType* humanName;
    
    qhandle_t handsModel;       // the hands don't actually draw, they just position the weapon
    qhandle_t weaponModel;
    qhandle_t barrelModel;
    qhandle_t flashModel;
    
    qhandle_t weaponModel3rdPerson;
    qhandle_t barrelModel3rdPerson;
    qhandle_t flashModel3rdPerson;
    
    animation_t animations[ MAX_WEAPON_ANIMATIONS ];
    bool noDrift;
    
    vec3_t weaponMidpoint;   // so it will rotate centered instead of by tag
    
    qhandle_t weaponIcon;
    qhandle_t ammoIcon;
    
    qhandle_t crossHair;
    sint crossHairSize;
    
    sfxHandle_t readySound;
    
    bool disableIn3rdPerson;
    
    weaponInfoMode_t wim[ WPM_NUM_WEAPONMODES ];
} weaponInfo_t;

typedef struct upgradeInfo_s
{
    bool registered;
    valueType* humanName;
    
    qhandle_t upgradeIcon;
} upgradeInfo_t;

typedef struct
{
    bool looped;
    bool enabled;
    
    sfxHandle_t sound;
} sound_t;

typedef struct
{
    qhandle_t models[ MAX_BUILDABLE_MODELS ];
    animation_t animations[ MAX_BUILDABLE_ANIMATIONS ];
    
    //same number of sounds as animations
    sound_t sounds[ MAX_BUILDABLE_ANIMATIONS ];
} buildableInfo_t;

#define MAX_REWARDSTACK   10
#define MAX_SOUNDBUFFER   20

//======================================================================

typedef struct
{
    vec3_t alienBuildablePos[ MAX_GENTITIES ];
    sint alienBuildableTimes[ MAX_GENTITIES ];
    sint numAlienBuildables;
    
    vec3_t humanBuildablePos[ MAX_GENTITIES ];
    sint numHumanBuildables;
    
    vec3_t alienClientPos[ MAX_CLIENTS ];
    sint numAlienClients;
    
    vec3_t humanClientPos[ MAX_CLIENTS ];
    sint numHumanClients;
    
    sint lastUpdateTime;
    vec3_t origin;
    vec3_t vangles;
} entityPos_t;

typedef struct
{
    sint time;
    sint length;
} consoleLine_t;

#define MAX_CONSOLE_TEXT  8192
#define MAX_CONSOLE_LINES 32

// all cg.stepTime, cg.duckTime, cg.landTime, etc are set to cg.time when the action
// occurs, and they will have visible effects for #define STEP_TIME or whatever msec after

#define MAX_PREDICTED_EVENTS  16

#define NUM_SAVED_STATES ( CMD_BACKUP + 2 )

// After this many msec the crosshair name fades out completely
#define CROSSHAIR_CLIENT_TIMEOUT 1000

typedef struct
{
    sint clientFrame; // incremented each frame
    
    sint clientNum;
    
    bool demoPlayback;
    bool levelShot; // taking a level menu screenshot
    sint deferredPlayerLoading;
    bool loading; // don't defer players at initial startup
    bool intermissionStarted; // don't play voice rewards, because game will end shortly
    
    // there are only one or two snapshot_t that are relevent at a time
    sint latestSnapshotNum; // the number of snapshots the client system has received
    sint latestSnapshotTime; // the time from latestSnapshotNum, so we don't need to read the snapshot yet
    
    snapshot_t* snap; // cg.snap->serverTime <= cg.time
    snapshot_t* nextSnap; // cg.nextSnap->serverTime > cg.time, or nullptr
    snapshot_t activeSnapshots[ 2 ];
    
    float32 frameInterpolation; // (float32)( cg.time - cg.frame->serverTime ) /
    // (cg.nextFrame->serverTime - cg.frame->serverTime)
    
    bool thisFrameTeleport;
    bool nextFrameTeleport;
    
    sint frametime; // cg.time - cg.oldTime
    
    sint time; // this is the time value that the client
    // is rendering at.
    sint oldTime; // time at last frame, used for missile trails and prediction checking
    
    sint physicsTime; // either cg.snap->time or cg.nextSnap->time
    
    sint timelimitWarnings; // 5 min, 1 min, overtime
    sint fraglimitWarnings;
    
    bool mapRestart; // set on a map restart to set back the weapon
    
    bool renderingThirdPerson; // during deaths, chasecams, etc
    
    // prediction state
    bool hyperspace;  // true if prediction has hit a trigger_teleport
    playerState_t predictedPlayerState;
    pmoveExt_t pmext;
    centity_t predictedPlayerEntity;
    bool validPPS; // clear until the first call to CG_PredictPlayerState
    sint predictedErrorTime;
    vec3_t predictedError;
    
    sint eventSequence;
    sint predictableEvents[MAX_PREDICTED_EVENTS];
    
    float32 stepChange; // for stair up smoothing
    sint stepTime;
    
    float32 duckChange; // for duck viewheight smoothing
    sint duckTime;
    
    float32 landChange; // for landing hard
    sint landTime;
    
    // input state sent to server
    sint weaponSelect;
    
    // auto rotating items
    vec3_t autoAngles;
    vec3_t autoAxis[ 3 ];
    vec3_t autoAnglesFast;
    vec3_t autoAxisFast[ 3 ];
    
    // view rendering
    refdef_t refdef;
    vec3_t refdefViewAngles; // will be converted to refdef.viewaxis
    vec3_t refdefViewAnglesWeapon;
    
    // zoom key
    bool zoomed;
    sint zoomTime;
    float32 zoomSensitivity;
    
    sint	identifyClientRequest;
    
    // information screen text during loading
    valueType infoScreenText[ MAX_STRING_CHARS ];
    
    // scoreboard
    sint scoresRequestTime;
    sint numScores;
    sint selectedScore;
    sint teamScores[ 2 ];
    score_t scores[MAX_CLIENTS];
    bool showScores;
    bool scoreBoardShowing;
    sint scoreFadeTime;
    valueType killerName[ MAX_NAME_LENGTH ];
    valueType spectatorList[ MAX_STRING_CHARS ]; // list of names
    sint spectatorLen; // length of list
    float32 spectatorWidth; // width in device units
    sint spectatorTime; // next time to offset
    sint spectatorPaintX; // current paint x
    sint spectatorPaintX2; // current paint x
    sint spectatorOffset; // current offset from start
    sint spectatorPaintLen; // current offset from start
    
    // centerprinting
    sint centerPrintTime;
    sint centerPrintCharWidth;
    sint centerPrintY;
    valueType centerPrint[ 1024 ];
    sint centerPrintLines;
    
    // low ammo warning state
    sint lowAmmoWarning; // 1 = low, 2 = empty
    
    // kill timers for carnage reward
    sint lastKillTime;
    
    // crosshair client ID
    sint crosshairBuildable;
    sint crosshairClientNum;
    sint crosshairClientTime;
    
    // powerup active flashing
    sint powerupActive;
    sint powerupTime;
    
    // attacking player
    sint attackerTime;
    sint voiceTime;
    
    // reward medals
    sint rewardStack;
    sint rewardTime;
    sint rewardCount[ MAX_REWARDSTACK ];
    qhandle_t rewardShader[ MAX_REWARDSTACK ];
    qhandle_t rewardSound[ MAX_REWARDSTACK ];
    
    // sound buffer mainly for announcer sounds
    sint soundBufferIn;
    sint soundBufferOut;
    sint soundTime;
    qhandle_t soundBuffer[ MAX_SOUNDBUFFER ];
    
    // for voice chat buffer
    sint voiceChatTime;
    sint voiceChatBufferIn;
    sint voiceChatBufferOut;
    
    // warmup countdown
    sint warmup;
    sint warmupCount;
    
    //==========================
    
    sint itemPickup;
    sint itemPickupTime;
    sint itemPickupBlendTime; // the pulse around the crosshair is timed seperately
    
    sint weaponSelectTime;
    sint feedbackAnimation;
    sint feedbackAnimationType;
    
    // blend blobs
    float32 damageTime;
    float32 damageX;
    float32 damageY;
    float32 damageValue;
    
    // status bar head
    float32 headYaw;
    float32 headEndPitch;
    float32 headEndYaw;
    sint headEndTime;
    float32 headStartPitch;
    float32 headStartYaw;
    sint headStartTime;
    
    // view movement
    float32 v_dmg_time;
    float32 v_dmg_pitch;
    float32 v_dmg_roll;
    
    vec3_t kick_angles; // weapon kicks
    vec3_t kick_origin;
    
    bool chaseFollow;
    
    // temp working variables for player view
    float32 bobfracsin;
    sint bobcycle;
    float32 xyspeed;
    sint nextOrbitTime;
    
    // development tool
    refEntity_t testModelEntity;
    refEntity_t testModelBarrelEntity;
    valueType testModelName[MAX_QPATH];
    valueType testModelBarrelName[MAX_QPATH];
    bool testGun;
    
    sint spawnTime; // fovwarp
    sint weapon1Time; // time when BUTTON_ATTACK went t->f f->t
    sint weapon2Time; // time when BUTTON_ATTACK2 went t->f f->t
    sint weapon3Time; // time when BUTTON_USE_HOLDABLE went t->f f->t
    bool weapon1Firing;
    bool weapon2Firing;
    bool weapon3Firing;
    
    sint poisonedTime;
    
    vec3_t lastNormal; // view smoothage
    vec3_t lastVangles; // view smoothage
    smooth_t sList[ MAXSMOOTHS ]; // WW smoothing
    
    sint forwardMoveTime; // for struggling
    sint rightMoveTime;
    sint upMoveTime;
    
    float32 charModelFraction; // loading percentages
    float32 mediaFraction;
    float32 buildablesFraction;
    
    sint lastBuildAttempt;
    sint lastEvolveAttempt;
    
    valueType consoleText[ MAX_CONSOLE_TEXT ];
    consoleLine_t consoleLines[ MAX_CONSOLE_LINES ];
    sint numConsoleLines;
    
    particleSystem_t* poisonCloudPS;
    particleSystem_t* poisonCloudedPS;
    
    float32 painBlendValue;
    float32 painBlendTarget;
    float32 healBlendValue;
    sint lastHealth;
    
    sint lastPredictedCommand;
    sint lastServerTime;
    playerState_t savedPmoveStates[ NUM_SAVED_STATES ];
    sint stateHead, stateTail;
    sint ping;
    
    float32 chargeMeterAlpha;
    float32 chargeMeterValue;
    qhandle_t lastHealthCross;
    float32 healthCrossFade;
    
    sint nextWeaponClickTime;
    
    centity_t* locationHead;
    
    bool serverRespawning;
} cg_t;


// all of the model, shader, and sound references that are
// loaded at gamestate time are stored in cgMedia_t
// Other media that can be tied to clients, weapons, or items are
// stored in the clientInfo_t, itemInfo_t, weaponInfo_t, and powerupInfo_t
typedef struct
{
    qhandle_t charsetShader;
    qhandle_t whiteShader;
    qhandle_t outlineShader;
    
    qhandle_t level2ZapTS;
    
    qhandle_t balloonShader;
    qhandle_t connectionShader;
    
    qhandle_t viewBloodShader;
    qhandle_t tracerShader;
    qhandle_t crosshairShader[ WP_NUM_WEAPONS ];
    qhandle_t backTileShader;
    
    qhandle_t creepShader;
    
    qhandle_t scannerShader;
    qhandle_t scannerBlipShader;
    qhandle_t scannerLineShader;
    
    
    qhandle_t numberShaders[ 11 ];
    
    qhandle_t shadowMarkShader;
    qhandle_t wakeMarkShader;
    
    // buildable shaders
    qhandle_t greenBuildShader;
    qhandle_t redBuildShader;
    qhandle_t humanSpawningShader;
    
    // disconnect
    qhandle_t disconnectPS;
    qhandle_t disconnectSound;
    
    // sounds
    sfxHandle_t tracerSound;
    sfxHandle_t weaponEmptyClick;
    sfxHandle_t selectSound;
    sfxHandle_t footsteps[ FOOTSTEP_TOTAL ][ 4 ];
    sfxHandle_t talkSound;
    sfxHandle_t alienTalkSound;
    sfxHandle_t humanTalkSound;
    sfxHandle_t landSound;
    sfxHandle_t fallSound;
    sfxHandle_t turretSpinupSound;
    
    sfxHandle_t hardBounceSound1;
    sfxHandle_t hardBounceSound2;
    
    sfxHandle_t voteNow;
    sfxHandle_t votePassed;
    sfxHandle_t voteFailed;
    
    sfxHandle_t watrInSound;
    sfxHandle_t watrOutSound;
    sfxHandle_t watrUnSound;
    
    sfxHandle_t jetpackDescendSound;
    sfxHandle_t jetpackIdleSound;
    sfxHandle_t jetpackAscendSound;
    
    qhandle_t jetPackDescendPS;
    qhandle_t jetPackHoverPS;
    qhandle_t jetPackAscendPS;
    
    sfxHandle_t medkitUseSound;
    
    sfxHandle_t alienStageTransition;
    sfxHandle_t humanStageTransition;
    
    sfxHandle_t alienOvermindAttack;
    sfxHandle_t alienOvermindDying;
    sfxHandle_t alienOvermindSpawns;
    
    sfxHandle_t alienBuildableExplosion;
    sfxHandle_t alienBuildableDamage;
    sfxHandle_t alienBuildablePrebuild;
    sfxHandle_t humanBuildableExplosion;
    sfxHandle_t humanBuildablePrebuild;
    sfxHandle_t humanBuildableDamage[ 4 ];
    
    sfxHandle_t alienL1Grab;
    sfxHandle_t alienL4ChargePrepare;
    sfxHandle_t alienL4ChargeStart;
    
    qhandle_t cursor;
    qhandle_t selectCursor;
    qhandle_t sizeCursor;
    
    //light armour
    qhandle_t larmourHeadSkin;
    qhandle_t larmourLegsSkin;
    qhandle_t larmourTorsoSkin;
    
    qhandle_t jetpackModel;
    qhandle_t jetpackFlashModel;
    qhandle_t battpackModel;
    
    sfxHandle_t repeaterUseSound;
    
    sfxHandle_t buildableRepairSound;
    sfxHandle_t buildableRepairedSound;
    
    qhandle_t poisonCloudPS;
    qhandle_t poisonCloudedPS;
    qhandle_t alienEvolvePS;
    qhandle_t alienAcidTubePS;
    
    sfxHandle_t alienEvolveSound;
    
    qhandle_t humanBuildableDamagedPS;
    qhandle_t humanBuildableHitSmallPS;
    qhandle_t humanBuildableHitLargePS;
    qhandle_t humanBuildableDestroyedPS;
    qhandle_t alienBuildableDamagedPS;
    qhandle_t alienBuildableHitSmallPS;
    qhandle_t alienBuildableHitLargePS;
    qhandle_t alienBuildableDestroyedPS;
    
    qhandle_t alienBleedPS;
    qhandle_t humanBleedPS;
    
    qhandle_t alienAttackFeedbackShaders[11];
    qhandle_t alienAttackFeedbackShadersFlipped[11];
    qhandle_t alienRangedAttackFeedbackShaders[11];
    
    qhandle_t teslaZapTS;
    qhandle_t massDriverTS;
    
    sfxHandle_t lCannonWarningSound;
    sfxHandle_t lCannonWarningSound2;
    
    qhandle_t buildWeaponTimerPie[ 8 ];
    qhandle_t upgradeClassIconShader;
    qhandle_t healthCross;
    qhandle_t healthCross2X;
    qhandle_t healthCross3X;
    qhandle_t healthCrossMedkit;
    qhandle_t healthCrossPoisoned;
    
    qhandle_t squadMarkerH;
    qhandle_t squadMarkerV;
} cgMedia_t;

typedef struct
{
    qhandle_t frameShader;
    qhandle_t overlayShader;
    qhandle_t noPowerShader;
    qhandle_t markedShader;
    vec4_t healthSevereColor;
    vec4_t healthHighColor;
    vec4_t healthElevatedColor;
    vec4_t healthGuardedColor;
    vec4_t healthLowColor;
    sint frameHeight;
    sint frameWidth;
    sint healthPadding;
    sint overlayHeight;
    sint overlayWidth;
    float32 verticalMargin;
    float32 horizontalMargin;
    vec4_t foreColor;
    vec4_t backColor;
    bool loaded;
} buildStat_t;

// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournement restart is done, allowing
// all clients to begin playing instantly
typedef struct
{
    gameState_t gameState; // gamestate from server
    vidconfig_t glconfig; // rendering configuration
    float32 screenXScale; // derived from glconfig
    float32 screenYScale;
    float32 screenXBias;
    
    sint serverCommandSequence; // reliable command stream counter
    sint processedSnapshotNum; // the number of snapshots cgame has requested
    
    bool localServer; // detected on startup by checking sv_running
    
    // parsed from serverinfo
    sint timelimit;
    sint maxclients;
    valueType mapname[ MAX_QPATH ];
    bool markDeconstruct; // Whether or not buildables are marked
    
    sint voteTime;
    sint voteYes;
    sint voteNo;
    bool voteModified; // beep whenever changed
    valueType voteString[ MAX_STRING_TOKENS ];
    
    sint teamVoteTime[ 2 ];
    sint teamVoteYes[ 2 ];
    sint teamVoteNo[ 2 ];
    bool teamVoteModified[ 2 ]; // beep whenever changed
    valueType teamVoteString[ 2 ][ MAX_STRING_TOKENS ];
    
    sint levelStartTime;
    
    sint scores1, scores2; // from configstrings
    
    bool newHud;
    
    sint alienBuildPoints;
    sint alienBuildPointsTotal;
    sint humanBuildPoints;
    sint humanBuildPointsTotal;
    sint humanBuildPointsPowered;
    
    sint alienStage;
    sint humanStage;
    sint alienCredits;
    sint humanCredits;
    sint alienNextStageThreshold;
    sint humanNextStageThreshold;
    
    // locally derived information from gamestate
    qhandle_t gameModels[ MAX_MODELS ];
    qhandle_t gameShaders[ MAX_GAME_SHADERS ];
    qhandle_t gameParticleSystems[ MAX_GAME_PARTICLE_SYSTEMS ];
    sfxHandle_t gameSounds[ MAX_SOUNDS ];
    
    sint numInlineModels;
    qhandle_t inlineDrawModel[ MAX_MODELS ];
    vec3_t inlineModelMidpoints[ MAX_MODELS ];
    
    clientInfo_t clientinfo[ MAX_CLIENTS ];
    
    // corpse info
    clientInfo_t corpseinfo[ MAX_CLIENTS ];
    
    sint cursorX;
    sint cursorY;
    bool eventHandling;
    bool mouseCaptured;
    bool sizingHud;
    void* capturedItem;
    qhandle_t activeCursor;
    
    buildStat_t alienBuildStat;
    buildStat_t humanBuildStat;
    
    // media
    cgMedia_t media;
    
    voice_t* voices;
    clientList_t ignoreList;
} cgs_t;

typedef struct
{
    valueType* cmd;
    void ( *function )( void );
    valueType* cmdDesc;
    void( *completer )( void );
} consoleCommand_t;

//==============================================================================

extern cgs_t cgs;
extern cg_t cg;
extern centity_t cg_entities[ MAX_GENTITIES ];
extern displayContextDef_t cgDC;

extern weaponInfo_t cg_weapons[ 32 ];
extern upgradeInfo_t cg_upgrades[ 32 ];

extern buildableInfo_t cg_buildables[ BA_NUM_BUILDABLES ];

extern markPoly_t cg_markPolys[ MAX_MARK_POLYS ];

extern vmConvar_t cg_version;
extern vmConvar_t cg_teslaTrailTime;
extern vmConvar_t cg_centertime;
extern vmConvar_t cg_runpitch;
extern vmConvar_t cg_runroll;
extern vmConvar_t cg_swingSpeed;
extern vmConvar_t cg_shadows;
extern vmConvar_t cg_drawTimer;
extern vmConvar_t cg_drawClock;
extern vmConvar_t cg_drawFPS;
extern vmConvar_t cg_drawSpeed;
extern vmConvar_t cg_drawDemoState;
extern vmConvar_t cg_drawSnapshot;
extern vmConvar_t cg_drawChargeBar;
extern vmConvar_t cg_drawCrosshair;
extern vmConvar_t cg_drawCrosshairNames;
extern vmConvar_t cg_crosshairSize;
extern vmConvar_t cg_drawAmmoStack;
extern vmConvar_t cg_draw2D;
extern vmConvar_t cg_drawStatus;
extern vmConvar_t cg_animSpeed;
extern vmConvar_t cg_debugAnim;
extern vmConvar_t cg_debugAlloc;
extern vmConvar_t cg_debugPosition;
extern vmConvar_t cg_debugEvents;
extern vmConvar_t cg_errorDecay;
extern vmConvar_t cg_nopredict;
extern vmConvar_t cg_debugMove;
extern vmConvar_t cg_noPlayerAnims;
extern vmConvar_t cg_showmiss;
extern vmConvar_t cg_footsteps;
extern vmConvar_t cg_addMarks;
extern vmConvar_t cg_brassTime;
extern vmConvar_t cg_viewsize;
extern vmConvar_t cg_drawGun;
extern vmConvar_t cg_gun_frame;
extern vmConvar_t cg_gun_x;
extern vmConvar_t cg_gun_y;
extern vmConvar_t cg_gun_z;
extern vmConvar_t cg_tracerChance;
extern vmConvar_t cg_tracerWidth;
extern vmConvar_t cg_tracerLength;
extern vmConvar_t cg_autoswitch;
extern vmConvar_t cg_thirdPerson;
extern vmConvar_t cg_thirdPersonShoulderViewMode;
extern vmConvar_t cg_thirdPersonPitchFollow;
extern vmConvar_t cg_thirdPersonRange;
extern vmConvar_t cg_shoulderViewOverride;
extern vmConvar_t cg_shoulderViewUp;
extern vmConvar_t cg_shoulderViewRight;
extern vmConvar_t cg_shoulderViewForward;
extern vmConvar_t cg_stereoSeparation;
extern vmConvar_t cg_lagometer;
extern vmConvar_t cg_synchronousClients;
extern vmConvar_t cg_stats;
extern vmConvar_t cg_paused;
extern vmConvar_t cg_blood;
extern vmConvar_t cg_drawFriend;
extern vmConvar_t cg_teamChatsOnly;
extern vmConvar_t cg_noVoiceChats;
extern vmConvar_t cg_noVoiceText;
extern vmConvar_t cg_hudFiles;
extern vmConvar_t cg_smoothClients;
extern vmConvar_t pmove_fixed;
extern vmConvar_t pmove_msec;
extern vmConvar_t cg_cameraMode;
extern vmConvar_t cg_timescaleFadeEnd;
extern vmConvar_t cg_timescaleFadeSpeed;
extern vmConvar_t cg_timescale;
extern vmConvar_t cg_noTaunt;
extern vmConvar_t cg_drawSurfNormal;
extern vmConvar_t cg_drawBBOX;
extern vmConvar_t cg_wwSmoothTime;
extern vmConvar_t cg_flySpeed;
extern vmConvar_t cg_depthSortParticles;
extern vmConvar_t cg_bounceParticles;
extern vmConvar_t cg_consoleLatency;
extern vmConvar_t cg_lightFlare;
extern vmConvar_t cg_debugParticles;
extern vmConvar_t cg_debugTrails;
extern vmConvar_t cg_debugPVS;
extern vmConvar_t cg_disableWarningDialogs;
extern vmConvar_t cg_disableUpgradeDialogs;
extern vmConvar_t cg_disableBuildDialogs;
extern vmConvar_t cg_disableCommandDialogs;
extern vmConvar_t cg_disableScannerPlane;
extern vmConvar_t cg_tutorial;
extern vmConvar_t cg_painBlendUpRate;
extern vmConvar_t cg_painBlendDownRate;
extern vmConvar_t cg_painBlendMax;
extern vmConvar_t cg_painBlendScale;
extern vmConvar_t cg_painBlendZoom;
extern vmConvar_t cg_stickySpec;
extern vmConvar_t cg_alwaysSprint;
extern vmConvar_t cg_unlagged;
extern vmConvar_t cg_debugVoices;
extern vmConvar_t ui_currentClass;
extern vmConvar_t ui_carriage;
extern vmConvar_t ui_stage;
extern vmConvar_t ui_dialog;
extern vmConvar_t ui_voteActive;
extern vmConvar_t ui_alienTeamVoteActive;
extern vmConvar_t ui_humanTeamVoteActive;
extern vmConvar_t cg_debugRandom;
extern vmConvar_t cg_optimizePrediction;
extern vmConvar_t cg_projectileNudge;
extern vmConvar_t cg_drawBuildableStatus;
extern vmConvar_t cg_hideHealthyBuildableStatus;
extern vmConvar_t cg_drawTeamStatus;
extern vmConvar_t cg_hideHealthyTeamStatus;
extern vmConvar_t cg_drawAlienFeedback;
extern vmConvar_t cg_voice;
extern vmConvar_t cg_suppressWAnimWarnings;
extern vmConvar_t cg_emoticons;
extern vmConvar_t cg_predictItems;

#define MAGIC_TRACE_HACK -2

// cg_drawCrosshair settings
#define CROSSHAIR_ALWAYSOFF 0
#define CROSSHAIR_RANGEDONLY 1
#define CROSSHAIR_ALWAYSON 2

// menu types for cg_disable*Dialogs
typedef enum
{
    DT_INTERACTIVE, // team, class, armoury
    DT_ARMOURYEVOLVE, // Insufficient funds et al
    DT_BUILD, // build errors
    DT_COMMAND, // You must be living/human/spec etc.
} cgameDialogType_t;

//
// idCGameLocal
//
class idCGameLocal : public idCGame
{
public:
    idCGameLocal();
    ~idCGameLocal();
    
    virtual void Init( sint serverMessageNum, sint serverCommandSequence, sint clientNum, bool demoPlayback );
    // called when the level loads or when the renderer is restarted
    // all media should be registered at this time
    // cgame will display loading status by calling SCR_Update, which
    // will call CG_DrawInformation during the loading process
    // reliableCommandSequence will be 0 on fresh loads, but higher for
    // demos, tourney restarts, or vid_restarts
    
    virtual void Shutdown();
    // oportunity to flush and close any open files
    
    virtual bool ConsoleCommand();
    // a console command has been issued locally that is not recognized by the
    // main game system.
    // use Cmd_Argc() / Cmd_Argv() to read the command, return false if the
    // command is not known to the game
    
    virtual void DrawActiveFrame( sint serverTime, stereoFrame_t stereoView, bool demoPlayback );
    // Generates and draws a game scene and status information at the given time.
    // If demoPlayback is set, local movement prediction will not be enabled
    
    virtual void ConsoleText( void );
    //	pass text that has been printed to the console to cgame
    //	use Cmd_Argc() / Cmd_Argv() to read it
    
    virtual sint CrosshairPlayer( void );
    
    virtual sint LastAttacker( void );
    
    virtual void KeyEvent( sint key, bool down );
    
    virtual void MouseEvent( sint dx, sint dy );
    
    virtual void EventHandling( sint type, bool fForced );
    
    virtual bool GetTag( sint clientNum, valueType* tagname, orientation_t* _or );
    
    virtual bool CheckExecKey( sint key );
    
    virtual bool WantsBindKeys( void );
    
    virtual void CompleteCommand( sint argNum );
    // will callback on all availible completions
    // use Cmd_Argc() / Cmd_Argv() to read the command
public:
    static void Printf( pointer msg, ... );
    static void Error( pointer msg, ... );
};

extern idCGameLocal cgameLocal;

#endif //!__CG_LOCAL_H__//
