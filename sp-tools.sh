#!/bin/bash

# Make sure this script is located in the root of the development environment repo.
# Don't mess with the variables in this script, instead use the generated config.sh
# to adjust the configurable variables.


set -e
function On_Error () {
	printf "Critical error at $(caller): ${BASH_COMMAND}\n"
	exit 1
}
trap On_Error ERR


SCRIPT=`readlink -f ${BASH_SOURCE[0]}`
SCRIPTPATH=`dirname "$SCRIPT"`
SCRIPTNAME=`basename "$0"`
CURRENTPATH=$(pwd)

BASEPATH="$SCRIPTPATH/basepath"
HOMEPATH="$SCRIPTPATH/homepath"

BASEGAME="main"

LOGPATH="$SCRIPTPATH/chat-logs"
SCRUBBED_LOGPATH="$LOGPATH/scrubbed"

SOURCEPATH="$SCRIPTPATH/source"
GAMELOGICPATH="$SOURCEPATH/game-logic"
ENGINEPATH="$SOURCEPATH/OpenWolf-Engine"

RELEASEPATH="$SCRIPTPATH/release"

case $OSTYPE in
  win32)
    PLATFORM="windows"
    ;;

  msys)
    PLATFORM="windows"
    ;;

  darwin)
    PLATFORM="macosx"
    ;;

  bsd)
    PLATFORM="bsd"
    ;;

  freebsd)
    PLATFORM="bsd"
    ;;

  *)
    PLATFORM="unix"
    ;;
esac

BINPATH="$BASEPATH/bin/$PLATFORM"

if [ $HOSTTYPE = "x86_64" ]; then
  BUILD_ARCH="AMD64"
else
	BUILD_ARCH=$HOSTTYPE
fi

Set_Config_To_Defaults() {
  if [ -f $SCRIPTPATH/config.sh ]; then
    rm $SCRIPTPATH/config.sh
  fi

  printf "Saving default values to config.sh\n\n"

  printf "#!/bin/bash\n\n" > $SCRIPTPATH/config.sh
  printf "GAME_APP_NAME=\"Stellar Prey Test\"\n" >> $SCRIPTPATH/config.sh
  printf "GAME_APP_VERSION=\"0.0.1\"\n" >> $SCRIPTPATH/config.sh
  printf "GAME_APP_STAGE=\"alpha\"\n\n" >> $SCRIPTPATH/config.sh

  printf "FS_GAME=\"$BASEGAME\"\n\n" >> $SCRIPTPATH/config.sh
  printf "DOWNLOAD_URL=\"http://dl.lonestellar.site.nfoservers.com/\"\n\n" >> $SCRIPTPATH/config.sh

  printf "SSH_REMOTE_HOST=\"remote_host\"\n" >> $SCRIPTPATH/config.sh
  printf "SSH_REMOTE_SERVER_PATH=\"/home/username/Stellar-Prey\"\n\n" >> $SCRIPTPATH/config.sh

  printf "TMUX_SESSION_NAME=\"dedicated\"\n" >> $SCRIPTPATH/config.sh

  printf "LOGDAYS=90\n\n" >> $SCRIPTPATH/config.sh

  printf "#Build type options: Debug, Release, RelWithDebInfo, MinSizeRel\n" >> $SCRIPTPATH/config.sh
  printf "CMAKE_BUILD_TYPE=\"Debug\"\n\n" >> $SCRIPTPATH/config.sh

  printf "BUILD_CLIENT=\"ON\"\n" >> $SCRIPTPATH/config.sh
  printf "BUILD_SERVER=\"ON\"\n" >> $SCRIPTPATH/config.sh
  printf "BUILD_AUTOUPDATE_SERVER=\"ON\"\n" >> $SCRIPTPATH/config.sh
  printf "BUILD_MASTER_SERVER=\"ON\"\n" >> $SCRIPTPATH/config.sh
  printf "BUILD_AUTH_SERVER=\"ON\"\n" >> $SCRIPTPATH/config.sh
  printf "BUILD_COMMUNITY_SERVER=\"ON\"\n" >> $SCRIPTPATH/config.sh
}

#Caches the entire config file
Cache_Config_File() {
  if [ -f $SCRIPTPATH/.cache-config.sh ]; then
    rm $SCRIPTPATH/.cache-config.sh
  fi

  printf "#!/bin/bash\n\n" > $SCRIPTPATH/.cache-config.sh
  printf "#Don't manually modify this file\n\n" > $SCRIPTPATH/.cache-config.sh

  printf "CACHED_GAME_APP_NAME=\"$GAME_APP_NAME\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_GAME_APP_VERSION=\"$GAME_APP_VERSION\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_GAME_APP_STAGE=\"$GAME_APP_STAGE\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_FS_GAME=\"$FS_GAME\"\n\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_DOWNLOAD_URL=\"$DOWNLOAD_URL\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_SSH_REMOTE_HOST=\"$SSH_REMOTE_HOST\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_SSH_REMOTE_SERVER_PATH=\"$SSH_REMOTE_SERVER_PATH\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_TMUX_SESSION_NAME=\"$TMUX_SESSION_NAME\"\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_LOGDAYS=\"$LOGDAYS\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "#Build type options: Debug, Release, RelWithDebInfo, MinSizeRel" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_CMAKE_BUILD_TYPE=\"$CMAKE_BUILD_TYPE\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_BUILD_CLIENT=\"$BUILD_CLIENT\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_SERVER=\"$BUILD_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_AUTOUPDATE_SERVER=\"$BUILD_AUTOUPDATE_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_MASTER_SERVER=\"$BUILD_MASTER_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_AUTH_SERVER=\"$BUILD_AUTH_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_COMMUNITY_SERVER=\"$BUILD_COMMUNITY_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
}

# create a default config file if a config file doesn't exist
if [ ! -f $SCRIPTPATH/config.sh ]; then
  echo "config.sh not found"

  Set_Config_To_Defaults
  if [ $# -gt 0 ] && [ $1 = "default_config" ]; then
    source $SCRIPTPATH/config.sh

    # cache the config file if the config file hasn't been cached before
    if [ ! -f $SCRIPTPATH/.cache-config.sh ]; then
      Cache_Config_File
      CACHE_CONFIG_FILE_CREATED=1
    else
      CACHE_CONFIG_FILE_CREATED=0
    fi

    exit 0
  fi
fi

source $SCRIPTPATH/config.sh

GAME_APP_NAME_UPPER=$(echo $GAME_APP_NAME  | tr -d ' ')

#Used to save only directly changed cached variables
Save_Cached_Variables() {
  if [ -f $SCRIPTPATH/.cache-config.sh ]; then
    rm $SCRIPTPATH/.cache-config.sh
  fi

  printf "#!/bin/bash\n\n" > $SCRIPTPATH/.cache-config.sh
  printf "#Don't manually modify this file\n\n" > $SCRIPTPATH/.cache-config.sh

  printf "CACHED_GAME_APP_NAME=\"$CACHED_GAME_APP_NAME\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_GAME_APP_VERSION=\"$CACHED_GAME_APP_VERSION\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_GAME_APP_STAGE=\"$CACHED_GAME_APP_STAGE\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_FS_GAME=\"$CACHED_FS_GAME\"\n\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_DOWNLOAD_URL=\"$CACHED_DOWNLOAD_URL\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_SSH_REMOTE_HOST=\"$CACHED_SSH_REMOTE_HOST\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_SSH_REMOTE_SERVER_PATH=\"$CACHED_SSH_REMOTE_SERVER_PATH\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_TMUX_SESSION_NAME=\"$CACHED_TMUX_SESSION_NAME\"\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_LOGDAYS=\"$CACHED_LOGDAYS\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "#Build type options: Debug, Release, RelWithDebInfo, MinSizeRel"
  printf "CACHED_CMAKE_BUILD_TYPE=\"$CACHED_CMAKE_BUILD_TYPE\"\n\n" >> $SCRIPTPATH/.cache-config.sh

  printf "CACHED_BUILD_CLIENT=\"$CACHED_BUILD_CLIENT\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_SERVER=\"$CACHED_BUILD_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_AUTOUPDATE_SERVER=\"$CACHED_BUILD_AUTOUPDATE_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_MASTER_SERVER=\"$CACHED_BUILD_MASTER_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_AUTH_SERVER=\"$CACHED_BUILD_AUTH_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
  printf "CACHED_BUILD_COMMUNITY_SERVER=\"$BUILD_COMMUNITY_SERVER\"\n" >> $SCRIPTPATH/.cache-config.sh
}

# cache the config file if the config file hasn't been cached before
if [ ! -f $SCRIPTPATH/.cache-config.sh ]; then
  Cache_Config_File
  CACHE_CONFIG_FILE_CREATED=1
else
  CACHE_CONFIG_FILE_CREATED=0
fi

source $SCRIPTPATH/.cache-config.sh

List_Subcommands() {
  printf "Available subcommands:\n\n"

  echo "help"
  echo "init"
  echo "default_config"
  echo "run"
  echo "debug"
  echo "git"
  echo "configure_cmake"
  echo "makeclean"
  echo "build"
  echo "package_assets"
  echo "package_release"
  echo "install_default_paks"
  echo "sync"
  echo "rotate_logs"

  printf "\n"
  echo "For more info on the particular subcommands, use the help subcommand:"
  echo "./$SCRIPTNAME help <subcommand>"
}

Help_Subcommand() {
  case $1 in
    help)
      printf "Prints help for the subcommands\n\n"
      List_Subcommands
      return 0
      ;;

    init)
      echo "Initializes the $GAME_APP_NAME development environment."
      echo "Use the init subcommand right after cloning this development environment repo."
      echo "It will change the remote configurations of the submodules to original repos."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME init"
      return 0
      ;;

    default_config)
      echo "Sets the config.sh file to default values."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME default_config"
      return 0
      ;;

    run)
      echo "Runs the specific installed binary."
      echo "The binary automatically restarts after 5 seconds if it crashes."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME run <client|server|autoupdater|master|auth|community>"
      return 0
      ;;

    debug)
      echo "Runs the specific installed binary in gdb"

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME debug <client|server|autoupdater|master|auth|community>"
      return 0
      ;;

    git)
      echo "Executes git commands for the specified submodule"

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME git <engine|default_paks> <git arguments>"
      return 0
      ;;

    configure_cmake)
      echo "Configures cmake and generates the makefile(s) for the engine and/or game_logic"
      echo "Execute this subcommand after adjusting the config file or after adjuting the CMakeList.tx file(s) or to generate new makefile(s)."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME configure_cmake <engine|game_logic|both>"
      return 0
      ;;

    makeclean)
      echo "Executes make clean for the engine or game_logic source."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME makeclean <engine|game_logic|both>"
      return 0
      ;;

    build)
      echo "Builds and installs the engine and/or game=logic."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME build <engine|game_logic|both>"
      return 0
      ;;

    package_assets)
      echo "Packages a default or custom game assets or map assets pk3 file and install in the appropriate fs_game directory."
      echo "If an optional commit hash is specified to diff against, only the asset files that are different or new are included in the pk3."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME package_assets <custom> <game|map> <optoinal directory name> <optional package name>"
      echo "./$SCRIPTNAME package_assets <default> <map> <optoinal directory name> <optional package name>"
      echo "./$SCRIPTNAME package_assets <default> <game> <package name> <optional commit hash>"
      return 0
      ;;

    package_release)
      echo "Packages a client release into a zip file based on what is currently installed in the basepath"

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME package_release"
      return 0
      ;;

    install_default_paks)
      echo "Automatically generates and installs the default pk3 files from git into the basepath."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME install_default_paks"
      return 0
      ;;

    sync)
      echo "Syncs the local pk3 files with the remote host."

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME sync <web|remote_server>"
      return 0
      ;;

    rotate_logs)
      echo "Archives the current day's chat logs"

      printf "\n"
      echo "Usage:"
      echo "./$SCRIPTNAME rotate_logs"
      return 0
      ;;

    *)
      printf "Unknown subcommand \"$1\"\n\n"
      List_Subcommands
      return 0
      ;;
    esac

    return 1
}

Init_Subcommand() {
  echo "Initializing the $GAME_APP_NAME development environment."
  cd $SCRIPTPATH

  git submodule update --init --recursive

  cd $ENGINEPATH
  ENGINE_GIT_REMOTES=$(git remote)
  for i in $ENGINE_GIT_REMOTES
  do
    if [ $i = "upstream" ]; then
      git remote remove upstream
    fi
  done

  if [[ $(git config --get remote.origin.url) == git* ]];  then
    git remote add upstream git@github.com:TheDushan/OpenWolf-Engine.git
  else
    git remote add upstream https://github.com/TheDushan/OpenWolf-Engine.git
  fi

  cd $SCRIPTPATH

  mkdir -p "$BASEPATH"
  mkdir -p "$BASEPATH/bin"
  mkdir -p "$BINPATH"
  mkdir -p "$BASEPATH/$BASEGAME"
  mkdir -p "$BASEPATH/$FS_GAME"
  mkdir -p "$HOMEPATH"
  mkdir -p "$HOMEPATH/$BASEGAME"
  mkdir -p "$HOMEPATH/$FS_GAME"
  mkdir -p "$LOGPATH"
  mkdir -p "$SCRUBBED_LOGPATH"
  mkdir -p "$RELEASEPATH"

  cd $CURRENTPATH
}

Configure_cmake_game_logic() {
  cd $GAMELOGICPATH

  if [ -f $GAMELOGICPATH/Makefile ]; then
    make clean
  fi

  echo "Configuring cmake for the game logic of $GAME_APP_NAME."

  printf "\n"

  cmake -DGAME_APP_NAME="$GAME_APP_NAME" \
    -DGAME_APP_VERSION="$GAME_APP_VERSION" \
    -DGAME_APP_STAGE="$GAME_APP_STAGE" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" .
  cd $CURRENTPATH
}

Configure_cmake_engine() {
  cd $ENGINEPATH

  if [ -f $ENGINEPATH/Makefile ]; then
    make clean
  fi

  echo "Configuring cmake for the OpenWolf Engine."

  printf "\n"

  cmake -DGAME_APP_NAME="$GAME_APP_NAME" \
    -DGAME_APP_VERSION="$GAME_APP_VERSION" \
    -DGAME_APP_STAGE="$GAME_APP_STAGE" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
    -DBUILD_CLIENT="$BUILD_CLIENT" \
    -DBUILD_SERVER="$BUILD_SERVER" \
    -DBUILD_AUTOUPDATE_SERVER="$BUILD_AUTOUPDATE_SERVER" \
    -DBUILD_MASTER_SERVER="$BUILD_MASTER_SERVER" \
    -DBUILD_AUTH_SERVER="$BUILD_AUTH_SERVER" \
    -DBUILD_COMMUNITY_SERVER="$BUILD_COMMUNITY_SERVER" .
  cd $CURRENTPATH
}

Configure_Cmake_Subcommand() {
  case $1 in
    engine)
      Configure_cmake_engine
      ;;

    game_logic)
      Configure_cmake_game_logic
      ;;

    both)
      Configure_cmake_engine
      Configure_cmake_game_logic
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "engine"
      echo "game_logic"
      echo "both"
      printf "\n"
      Help_Subcommand configure_cmake
      return 1
      ;;
  esac
  return 0
}

Makeclean_Subcommand() {
  case $1 in
    engine)
      cd $ENGINEPATH
      if [ -f $ENGINEPATH/Makefile ]; then
        make clean
        cd $CURRENTPATH
      else
        echo "Makefile for the engine doesn't exist."
        echo "To generate a Makefile for the engine execute:"
        echo "./$SCRIPTNAME configure_cmake engine"
        cd $CURRENTPATH
        return 1
      fi
      ;;

    game_logic)
      cd $GAMELOGICPATH
      if [ -f $GAMELOGICPATH/Makefile ]; then
        make clean
        cd $CURRENTPATH
      else
        echo "Makefile for the game logic doesn't exist."
        echo "To generate a Makefile for the game logic execute:"
        echo "./$SCRIPTNAME configure_cmake game_logic"
        cd $CURRENTPATH
        return 1
      fi
      ;;

    both)
      cd $ENGINEPATH
      if [ -f $ENGINEPATH/Makefile ]; then
        make clean
        cd $CURRENTPATH
      else
        echo "Makefile for the engine doesn't exist."
        echo "To generate a Makefile for the engine execute:"
        echo "./$SCRIPTNAME configure_cmake engine"
        cd $CURRENTPATH
        return 1
      fi

      cd $GAMELOGICPATH
      if [ -f $GAMELOGICPATH/Makefile ]; then
        make clean
        cd $CURRENTPATH
      else
        echo "Makefile for the game logic doesn't exist."
        echo "To generate a Makefile for the game logic execute:"
        echo "./$SCRIPTNAME configure_cmake game_logic"
        cd $CURRENTPATH
        return 1
      fi
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "engine"
      echo "game_logic"
      echo "both"
      printf "\n"
      Help_Subcommand makeclean
      return 1
      ;;
  esac
  return 0
}

Git_Subcommand() {
  case $1 in
    engine)
      cd $ENGINEPATH
      git ${@:2}
      cd $CURRENTPATH
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "engine"
      printf "\n"
      Help_Subcommand git
      return 1
      ;;
  esac
  return 0
}

Build_game_logic() {
  local RECONFIGURE=0

  if [ $CACHE_CONFIG_FILE_CREATED = 1 ]; then
    RECONFIGURE=1
  fi

  if [ ! -f $GAMELOGICPATH/Makefile ]; then
    RECONFIGURE=1
  fi

  if [ ! "$GAME_APP_NAME" = "$CACHED_GAME_APP_NAME" ]; then
    CACHED_GAME_APP_NAME="$GAME_APP_NAME"
    RECONFIGURE=1
  fi

  if [ ! "$GAME_APP_VERSION" = "$CACHED_GAME_APP_VERSION" ]; then
    CACHED_GAME_APP_VERSION="$GAME_APP_VERSION"
    RECONFIGURE=1
  fi

  if [ ! "$GAME_APP_STAGE" = "$CACHED_GAME_APP_STAGE" ]; then
    CACHED_GAME_APP_STAGE="$GAME_APP_STAGE"
    RECONFIGURE=1
  fi

  if [ ! "$CMAKE_BUILD_TYPE" = "$CACHED_CMAKE_BUILD_TYPE" ]; then
    CACHED_CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE"
    RECONFIGURE=1
  fi

  if [ $RECONFIGURE = 1 ]; then
    Save_Cached_Variables
    Configure_cmake_game_logic
  fi

  cd $GAMELOGICPATH
  make $MAKE_FLAGS
  mkdir -p "$BASEPATH"
  mkdir -p "$BASEPATH/$FS_GAME"
  cp -a "$GAMELOGICPATH/main/." "$BASEPATH/$FS_GAME/"

  cd $BASEPATH/$FS_GAME
  for i in $GAMELOGICPATH/main/*; do
    j=${i#$GAMELOGICPATH/main/}
    if [ -f bin.pk3 ]; then
      zip -d  bin.pk3 $j
      zip -ur  bin.pk3 $j
    else
      zip -r  bin.pk3 $j
    fi
  done
  cd $CURRENTPATH
}

Build_engine() {
  local RECONFIGURE=0

  if [ $CACHE_CONFIG_FILE_CREATED = 1 ]; then
    RECONFIGURE=1
  fi

  if [ ! -f $ENGINEPATH/Makefile ]; then
    RECONFIGURE=1
  fi

  if [ ! "$GAME_APP_NAME" = "$CACHED_GAME_APP_NAME" ]; then
    CACHED_GAME_APP_NAME="$GAME_APP_NAME"
    RECONFIGURE=1
  fi

  if [ ! "$GAME_APP_VERSION" = "$CACHED_GAME_APP_VERSION" ]; then
    CACHED_GAME_APP_VERSION="$GAME_APP_VERSION"
    RECONFIGURE=1
  fi

  if [ ! "$GAME_APP_STAGE" = "$CACHED_GAME_APP_STAGE" ]; then
    CACHED_GAME_APP_STAGE="$GAME_APP_STAGE"
    RECONFIGURE=1
  fi

  if [ ! "$CMAKE_BUILD_TYPE" = "$CACHED_CMAKE_BUILD_TYPE" ]; then
    CACHED_CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE"
    RECONFIGURE=1
  fi

  if [ ! "$BUILD_CLIENT" = "$CACHED_BUILD_CLIENT" ]; then
    CACHED_BUILD_CLIENT="$BUILD_CLIENT"
    RECONFIGURE=1
  fi

  if [ ! "$BUILD_SERVER" = "$CACHED_BUILD_SERVER" ]; then
    CACHED_BUILD_SERVER="$BUILD_SERVER"
    RECONFIGURE=1
  fi

  if [ ! "$BUILD_AUTOUPDATE_SERVER" = "$CACHED_BUILD_AUTOUPDATE_SERVER" ]; then
    CACHED_BUILD_AUTOUPDATE_SERVER="$BUILD_AUTOUPDATE_SERVER"
    RECONFIGURE=1
  fi

  if [ ! "$BUILD_MASTER_SERVER" = "$CACHED_BUILD_MASTER_SERVER" ]; then
    CACHED_BUILD_MASTER_SERVER="$BUILD_MASTER_SERVER"
    RECONFIGURE=1
  fi

  if [ ! "$BUILD_AUTH_SERVER" = "$CACHED_BUILD_AUTH_SERVER" ]; then
    CACHED_BUILD_AUTH_SERVER="$BUILD_AUTH_SERVER"
    RECONFIGURE=1
  fi

  if [ ! "$BUILD_COMMUNITY_SERVER" = "$CACHED_BUILD_COMMUNITY_SERVER" ]; then
    CACHED_BUILD_COMMUNITY_SERVER="$BUILD_COMMUNITY_SERVER"
    RECONFIGURE=1
  fi

  if [ $RECONFIGURE ]; then
    Save_Cached_Variables
    Configure_cmake_game_logic
  fi

  cd $ENGINEPATH
  make $MAKE_FLAGS
  mkdir -p "$BASEPATH"
  mkdir -p "$BASEPATH/bin"
  mkdir -p "$BINPATH"
  cp -a "$ENGINEPATH/bin/$PLATFORM/." "$BINPATH"
  cd $CURRENTPATH
}

Build_Subcommand() {
  case $1 in
    engine)
      Build_engine
      ;;

    game_logic)
      Build_game_logic
      ;;

    both)
      Build_engine
      Build_game_logic
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "engine"
      echo "game_logic"
      echo "both"
      printf "\n"
      Help_Subcommand build
      return 1
      ;;
  esac
  return 0
}

Run_Subcommand() {
  case $1 in
    client)
      cd $BINPATH
      "$BINPATH/${GAME_APP_NAME_UPPER}.${BUILD_ARCH}" \
        +set com_ansiColor 1 \
        +set fs_game "$FS_GAME" \
        +set fs_basepath "$BASEPATH" \
        +set fs_homepath "$HOMEPATH"
      cd $CURRENTPATH
      ;;

    server)
      cd $BINPATH
      while true; do
        "$BINPATH/${GAME_APP_NAME_UPPER}ded.${BUILD_ARCH}" \
          +set com_ansiColor 1 \
          +set fs_game "$FS_GAME" \
          +set sv_wwwDownload 1 \
          +set sv_wwwBaseURL "$DOWNLOAD_URL" \
          +set fs_basepath "$BASEPATH" \
          +set fs_homepath "$HOMEPATH" \
          +set dedicated 2 \
          +set sv_allowDownload 1 \
          +exec server.cfg \
          +map eXcs

        exec 0<&-
        exec 0</dev/tty
        if read -r -s -n 1 -t 6 -p "Press any key in the next 5 seconds to abort restarting the game server..." key
        then
          echo $'\a\naborted'
          break
        else
          echo $'\nrestarting...'
        fi
      done
      cd $CURRENTPATH
      ;;

    autoupdater)
      cd $BINPATH
      while true; do
        "$BINPATH/autoupdateserver" \
          +set com_ansiColor 1 \
          +set fs_game "$FS_GAME" \
          +set sv_wwwDownload 1 \
          +set sv_wwwBaseURL "$DOWNLOAD_URL" \
          +set fs_basepath "$BASEPATH" \
          +set fs_homepath "$HOMEPATH" \
          +set dedicated 2 \
          +set sv_allowDownload 1

        exec 0<&-
        exec 0</dev/tty
        if read -r -s -n 1 -t 6 -p "Press any key in the next 5 seconds to abort restarting the autoupdater server..." key
        then
          echo $'\a\naborted'
          break
        else
          echo $'\nrestarting...'
        fi
      done
      cd $CURRENTPATH
      ;;

    master)
      cd $BINPATH
      while true; do
        "$BINPATH/OWMaster.${BUILD_ARCH}"

        exec 0<&-
        exec 0</dev/tty
        if read -r -s -n 1 -t 6 -p "Press any key in the next 5 seconds to abort restarting the master server..." key
        then
          echo $'\a\naborted'
          break
        else
          echo $'\nrestarting...'
        fi
      done
      cd $CURRENTPATH
      ;;

    auth)
      cd $BINPATH
      while true; do
        "$BINPATH/OWAuthServer.${BUILD_ARCH}"

        exec 0<&-
        exec 0</dev/tty
        if read -r -s -n 1 -t 6 -p "Press any key in the next 5 seconds to abort restarting the authentication server..." key
        then
          echo $'\a\naborted'
          break
        else
          echo $'\nrestarting...'
        fi
      done
      cd $CURRENTPATH
      ;;

    community)
      cd $BINPATH
      while true; do
        "$BINPATH/OWCommunityServer.${BUILD_ARCH}"

        exec 0<&-
        exec 0</dev/tty
        if read -r -s -n 1 -t 6 -p "Press any key in the next 5 seconds to abort restarting the community server..." key
        then
          echo $'\a\naborted'
          break
        else
          echo $'\nrestarting...'
        fi
      done
      cd $CURRENTPATH
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "client"
      echo "server"
      echo "autoupdater"
      echo "master"
      echo "auth"
      echo "community"
      printf "\n"
      Help_Subcommand run
      return 1
      ;;
  esac
  return 0
}

Debug_Subcommand() {
  case $1 in
    client)
      cd $BINPATH
      gdb -ex run --args "$BINPATH/${GAME_APP_NAME_UPPER}.${BUILD_ARCH}" \
        +set com_ansiColor 1 \
        +set fs_game "$FS_GAME" \
        +set fs_basepath "$BASEPATH" \
        +set fs_homepath "$HOMEPATH"
      cd $CURRENTPATH
      ;;

    server)
      cd $BINPATH
      gdb -ex run --args "$BINPATH/${GAME_APP_NAME_UPPER}ded.${BUILD_ARCH}" \
        +set com_ansiColor 1 \
        +set fs_game "$FS_GAME" \
        +set sv_wwwDownload 1 \
        +set sv_wwwBaseURL "$DOWNLOAD_URL" \
        +set fs_basepath "$BASEPATH" \
        +set fs_homepath "$HOMEPATH" \
        +set dedicated 2 \
        +set sv_allowDownload 1 \
        +exec server.cfg \
        +map eXcs
      cd $CURRENTPATH
      ;;

    autoupdater)
      cd $BINPATH
      gdb -ex run --args "$BINPATH/autoupdateserver" \
        +set com_ansiColor 1 \
        +set fs_game "$FS_GAME" \
        +set sv_wwwDownload 1 \
        +set sv_wwwBaseURL "$DOWNLOAD_URL" \
        +set fs_basepath "$BASEPATH" \
        +set fs_homepath "$HOMEPATH" \
        +set dedicated 2 \
        +set sv_allowDownload 1
      cd $CURRENTPATH
      ;;

    master)
      cd $BINPATH
      gdb -ex run --args "$BINPATH/OWMaster.${BUILD_ARCH}"
      cd $CURRENTPATH
      ;;

    auth)
      cd $BINPATH
      gdb -ex run --args "$BINPATH/OWAuthServer.${BUILD_ARCH}"
      cd $CURRENTPATH
      ;;

    community)
      cd $BINPATH
      gdb -ex run --args "$BINPATH/OWCommunityServer.${BUILD_ARCH}"
      cd $CURRENTPATH
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "client"
      echo "server"
      echo "autoupdater"
      echo "master"
      echo "auth"
      echo "community"
      printf "\n"
      Help_Subcommand debug
      return 1
      ;;
  esac
  return 0
}

Sync_Subcommand() {
  

  case $1 in
    web)
      mkdir -p $SCRIPTPATH/../www
      echo "syncing paks with the webhost..."

      printf "\n"
      rsync -rauvm --progress --include="*/" --include="*.pk3" --exclude="*" \
        -e 'ssh' $BASEPATH/ $SCRIPTPATH/../www/
      rsync -rauvm --progress --include="*/" --include="*.pk3" --exclude="*" \
        -e 'ssh' $HOMEPATH/ $SCRIPTPATH/../www/

      if [ -f $SCRIPTPATH/../sync-pk3.sh ]; then
        $SCRIPTPATH/../sync-pk3.sh
      fi
      ;;

    remote_server)
      echo "syncing paks with the remote server..."
      rsync -rauvm --progress --include="*/" --include="*.pk3" --exclude="*" \
        -e 'ssh' $BASEPATH/ "$SSH_REMOTE_HOST:$SSH_REMOTE_SERVER_PATH/$BASEPATH"
      rsync -rauvm --progress --include="*/" --include="*.pk3" --exclude="*" \
        -e 'ssh' $HOMEPATH/ "$SSH_REMOTE_HOST:$SSH_REMOTE_SERVER_PATH/$HOMEPATH"
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "web"
      echo "remote_server"
      printf "\n"
      Help_Subcommand sync
      return 1
      ;;
  esac
  return 0
}

Package_Assets_Subcommand() {
  if [ $# -lt 2 ]; then
    echo "Not enough arguments"
    Help_Subcommand package_assets
    return 1
  fi

  case $1 in
    default)
      case $2 in
        game)
          if [ $# -lt 3 ]; then
            echo "Not enough arguments"
            Help_Subcommand package_assets
            return 1
          fi

          if [ $# -lt 4 ]; then
            cd $SCRIPTPATH
            WORKING_DIRECTORY=""
            for i in "$(git status --porcelain source/game-assets/default/)"; do
              if [ ! -z $i ]; then
                WORKING_DIRECTORY="_WD"
              fi
            done

            if [ -z $WORKING_DIRECTORY ]; then
              for i in "$(git status --porcelain src/engine/GPURenderer/renderProgs/)"; do
                if [ ! -z $i ]; then
                  WORKING_DIRECTORY="_WD"
                fi
              done
            fi

            if [ -z $WORKING_DIRECTORY ]; then
              ENGINE_HASH_PREFIXED=($(git submodule status source/OpenWolf-Engine))
              if [[ ${ENGINE_HASH_PREFIXED[0]} = "+"* ]]; then
                WORKING_DIRECTORY="_WD"
              elif [[ ${ENGINE_HASH_PREFIXED[0]} = "-"* ]]; then
                WORKING_DIRECTORY="_WD"
              elif [[ ${ENGINE_HASH_PREFIXED[0]} = "U"* ]]; then
                WORKING_DIRECTORY="_WD"
              fi
            fi

            PAK_NAME="${3}_$(git rev-parse --short HEAD)$WORKING_DIRECTORY.pk3"
            cd $ENGINEPATH/src/engine/GPURenderer
            zip -r $PAK_NAME renderProgs
            mv $PAK_NAME $SOURCEPATH/game-assets/default/
            cd $SOURCEPATH/game-assets/default/
            zip -ur $PAK_NAME *
            mkdir -p $BASEPATH
            mkdir -p "$BASEPATH/main"
            mv $PAK_NAME "$BASEPATH/main"
            cd $CURRENTPATH
            return 0
          else
            cd $SCRIPTPATH
            if [ ! $(git cat-file -t $4) = "commit" ]; then
              echo "Commit hash '$4' doesn't exist"
              Help_Subcommand package_assets
              return 1
            fi

            CHANGED_FILES=$(git diff --name-only --diff-filter=d $4 source/game-assets/default/)

            ENGINE_OLD_HASH=$(git rev-parse ${4}:source/OpenWolf-Engine)
            cd $ENGINEPATH
            CHANGED_RENDERPROGS_FILES=$(git diff --name-only --diff-filter=d $ENGINE_OLD_HASH src/engine/GPURenderer/renderProgs)
            cd $SCRIPTPATH

            FILES_HAVE_CHANGED=""
            for i in $CHANGED_FILES; do
              if [ ! -z $i ]; then
                FILES_HAVE_CHANGED="true"
                break
              fi
            done

            RENDERPROGS_FILES_HAVE_CHANGED=""
            for i in $CHANGED_RENDERPROGS_FILES; do
              if [ ! -z $i ]; then
                RENDERPROGS_FILES_HAVE_CHANGED="true"
                break
              fi
            done

            if [ -z $FILES_HAVE_CHANGED ] && [ -z $RENDERPROGS_FILES_HAVE_CHANGED ]; then
              echo "Nothing new to package for the default game assets since the commit ${4}, aborting asset packaging."
              Help_Subcommand package_assets
              return 1
            fi

            WORKING_DIRECTORY=""
            for i in "$(git status --porcelain source/game-assets/default/)"; do
              if [ ! -z $i ]; then
                WORKING_DIRECTORY="_WD"
              fi
            done

            if [ -z $WORKING_DIRECTORY ]; then
              for i in "$(git status --porcelain src/engine/GPURenderer/renderProgs/)"; do
                if [ ! -z $i ]; then
                  WORKING_DIRECTORY="_WD"
                fi
              done
            fi

            if [ -z $WORKING_DIRECTORY ] && [ ! -z $RENDERPROGS_FILES_HAVE_CHANGED ]; then
              ENGINE_HASH_PREFIXED=($(git submodule status source/OpenWolf-Engine))
              if [[ ${ENGINE_HASH_PREFIXED[0]} = "+"* ]]; then
                WORKING_DIRECTORY="_WD"
              elif [[ ${ENGINE_HASH_PREFIXED[0]} = "-"* ]]; then
                WORKING_DIRECTORY="_WD"
              elif [[ ${ENGINE_HASH_PREFIXED[0]} = "U"* ]]; then
                WORKING_DIRECTORY="_WD"
              fi
            fi

            PAK_NAME="${3}_$(git rev-parse --short HEAD)${WORKING_DIRECTORY}_diff_$(git rev-parse --short ${4}).pk3"

            if [ ! -z $RENDERPROGS_FILES_HAVE_CHANGED ]; then
              cd $ENGINEPATH/src/engine/GPURenderer
              for i in $CHANGED_RENDERPROGS_FILES; do
                j=${i#src/engine/GPURenderer/}
                if [ -f $PAK_NAME ]; then
                  zip -ur $PAK_NAME $j
                else
                  zip -r $PAK_NAME $j
                fi
              done
              mv $PAK_NAME $SOURCEPATH/game-assets/default/
              cd $SCRIPTPATH
            fi

            cd $SOURCEPATH/game-assets/default/
            for i in $CHANGED_FILES; do
              j=${i#source/game-assets/default/}
              if [ -f $PAK_NAME ]; then
                zip -ur $PAK_NAME $j
              else
                zip -r $PAK_NAME $j
              fi
            done

            mkdir -p $BASEPATH
            mkdir -p "$BASEPATH/main"
            mv $PAK_NAME "$BASEPATH/main"
            cd $CURRENTPATH
            return 0
          fi
          ;;

        map)
          if [ $# -gt 2 ]; then
            if [ $# -gt 3 ]; then
              if [ -d "$SOURCEPATH/map-assets/default/$3" ]; then
                PAK_NAME="$4.pk3"
                cd "$SOURCEPATH/map-assets/default/$3"
                zip -r $PAK_NAME *
                mkdir -p $BASEPATH
                mkdir -p "$BASEPATH/main"
                mv $PAK_NAME "$BASEPATH/main"
                cd $CURRENTPATH
                return 0
              else
                echo "map assets directory '$3' doesn't exist"
                return 1
              fi
            else
              if [ -d "$SOURCEPATH/map-assets/default/$3" ]; then
                PAK_NAME="$3.pk3"
                cd "$SOURCEPATH/map-assets/default/$3"
                zip -r $PAK_NAME *
                mkdir -p $BASEPATH
                mkdir -p "$BASEPATH/main"
                mv $PAK_NAME "$BASEPATH/main"
                cd $CURRENTPATH
                return 0
              else
                echo "map assets directory '$3' doesn't exist"
                return 1
              fi
            fi
          else
            mkdir -p $BASEPATH
            mkdir -p "$BASEPATH/main"
            for i in $SOURCEPATH/map-assets/default/*/; do
              if [ ! -d $i ]; then
                continue
              fi

              PAK_NAME="$(basename $i).pk3"
              cd $i
              zip -r $PAK_NAME *
              mv $PAK_NAME "$BASEPATH/main"
            done
            cd $CURRENTPATH
            return 0
          fi
          ;;

        *)
          printf "$2 is an invalid argument.  Allowed arguments:\n"
          echo "game"
          echo "map"
          printf "\n"
          Help_Subcommand package_assets
          return 1
          ;;
      esac
      ;;

    custom)
      case $2 in
        game)
          if [ $# -gt 2 ]; then
            if [ $# -gt 3 ]; then
              if [ $3 = "default" ]; then
                echo "'default' isn't a valid directory"
                return 0
              fi

              if [ -d "$SOURCEPATH/game-assets/$3" ]; then
                PAK_NAME="$4.pk3"
                cd "$SOURCEPATH/game-assets/$3"
                zip -r $PAK_NAME *
                mkdir -p $HOMEPATH
                mkdir -p "$HOMEPATH/$FS_GAME"
                mv $PAK_NAME "$HOMEPATH/$FS_GAME"
                cd $CURRENTPATH
                return 0
              else
                echo "map assets directory '$3' doesn't exist"
                return 1
              fi
            else
              if [ -d "$SOURCEPATH/game-assets/$3" ]; then
                PAK_NAME="$3.pk3"
                cd "$SOURCEPATH/game-assets/$3"
                zip -r $PAK_NAME *
                mkdir -p $HOMEPATH
                mkdir -p "$HOMEPATH/$FS_GAME"
                mv $PAK_NAME "$HOMEPATH/$FS_GAME"
                cd $CURRENTPATH
                return 0
              else
                echo "map assets directory '$3' doesn't exist"
                return 1
              fi
            fi
          else
            mkdir -p $HOMEPATH
            mkdir -p "$HOMEPATH/$FS_GAME"
            for i in $SOURCEPATH/game-assets/*/; do
              if [ ! -d $i ]; then
                continue
              fi

              if [ $i = "$SOURCEPATH/game-assets/default/" ]; then
                continue
              fi

              PAK_NAME="$(basename $i).pk3"
              cd $i
              zip -r $PAK_NAME *
              mv $PAK_NAME "$HOMEPATH/$FS_GAME"
            done
            cd $CURRENTPATH
            return 0
          fi
          ;;

        map)
          if [ $# -gt 2 ]; then
            if [ $# -gt 3 ]; then
              if [ $3 = "default" ]; then
                echo "'default' isn't a valid directory"
                return 0
              fi

              if [ -d "$SOURCEPATH/map-assets/$3" ]; then
                PAK_NAME="$4.pk3"
                cd "$SOURCEPATH/map-assets/$3"
                zip -r $PAK_NAME *
                mkdir -p $HOMEPATH
                mkdir -p "$HOMEPATH/main"
                mv $PAK_NAME "$HOMEPATH/main"
                cd $CURRENTPATH
                return 0
              else
                echo "map assets directory '$3' doesn't exist"
                return 1
              fi
            else
              if [ -d "$SOURCEPATH/map-assets/$3" ]; then
                PAK_NAME="$3.pk3"
                cd "$SOURCEPATH/map-assets/$3"
                zip -r $PAK_NAME *
                mkdir -p $HOMEPATH
                mkdir -p "$HOMEPATH/main"
                mv $PAK_NAME "$HOMEPATH/main"
                cd $CURRENTPATH
                return 0
              else
                echo "map assets directory '$3' doesn't exist"
                return 1
              fi
            fi
          else
            mkdir -p $HOMEPATH
            mkdir -p "$HOMEPATH/main"
            for i in $SOURCEPATH/map-assets/*/; do
              if [ ! -d $i ]; then
                continue
              fi

              if [ $i = "$SOURCEPATH/map-assets/default/" ]; then
                continue
              fi

              PAK_NAME="$(basename $i).pk3"
              cd $i
              zip -r $PAK_NAME *
              mv $PAK_NAME "$HOMEPATH/main"
            done
            cd $CURRENTPATH
            return 0
          fi
          ;;

        *)
          printf "$2 is an invalid argument.  Allowed arguments:\n"
          echo "game"
          echo "map"
          printf "\n"
          Help_Subcommand package_assets
          return 1
          ;;
      esac
      ;;

    *)
      printf "$1 is an invalid argument.  Allowed arguments:\n"
      echo "default"
      echo "custom"
      printf "\n"
      Help_Subcommand package_assets
      return 1
      ;;
  esac
  return 0
}

Generate_Default_Assets_Pak() {
  if [ $# -lt 3 ]; then
    echo "Not enough arguments"
    echo "Generate_Default_Assets_Pak() Arguments: <install path> <resulting pak name> <commit hash> <optional base commit hash for diff pak>"
    echo "Generate_Default_Assets_Pak(): failed to install pak '${2}.pk3'"
    return 1
  fi

  if [ $# -gt 4 ]; then
    echo "Too many arguments"
    echo "Generate_Default_Assets_Pak() Arguments: <install path> <resulting pak name> <commit hash> <optional base commit hash for diff pak>"
    echo "Generate_Default_Assets_Pak(): failed to install pak '${2}.pk3'"
    return 1
  fi

  if [ ! -d $1 ]; then
    echo "Generate_Default_Assets_Pak(): Install path '${1}' doesn't exist"
    echo "Generate_Default_Assets_Pak(): failed to install pak '${2}.pk3'"
    return 1
  fi

  if [ $(git cat-file -t "${3}") != "commit" ]; then
    echo "Generate_Default_Assets_Pak(): Commit hash '${3}' doesn't exist"
    echo "Generate_Default_Assets_Pak(): failed to install pak '${2}.pk3'"
    return 1
  fi

  if [ $# -eq 4 ]; then
    if [ $(git cat-file -t "${4}") != "commit" ]; then
      echo "Generate_Default_Assets_Pak(): Optional base commit hash '${4}' for diff pak doesn't exist"
      echo "Generate_Default_Assets_Pak(): failed to install pak '${2}.pk3'"
      return 1
    fi

    cd $SCRIPTPATH
    CHANGED_FILES_PATHS=$(git diff --name-only --diff-filter=d ${4} ${3} source/game-assets/default/)

    if [ -z "${CHANGED_FILES_PATHS/\n/}" ]; then
      echo "Generate_Default_Assets_Pak(): Nothing changed for diff pak '${2}.pk3'"
      echo "Generate_Default_Assets_Pak(): failed to install pak '${2}.pk3'"
      cd $CURRENTPATH
      return 1
    fi

    echo "Generating and installing default pak '${2}.pk3'..."
    cd $SOURCEPATH/game-assets/default/
    CHANGED_FILES=""
    for i in $CHANGED_FILES_PATHS; do
      CHANGED_FILES+="./${i#source/game-assets/default/} "
    done

    TZ='America/New_York' git archive --format=zip --output=${1}/${2}.pk3 ${3} $CHANGED_FILES
    cd $CURRENTPATH

    return 0
  fi

  cd $SOURCEPATH/game-assets/default/
  echo "Generating and installing default pak '${2}.pk3'..."
  TZ='America/New_York' git archive --format=zip --output=${1}/${2}.pk3 ${3} ./
  cd $CURRENTPATH

  return 0
}

Generate_Default_Map_Pak() {
  if [ $# -lt 4 ]; then
    echo "Not enough arguments"
    echo "Generate_Default_Map_Pak() Arguments: <install path> <default map assets folder name> <resulting pak name> <commit hash>"
    echo "Generate_Default_Map_Pak(): failed to install pak '${3}.pk3'"
    return 1
  fi

  if [ $# -gt 4 ]; then
    echo "Too many arguments"
    echo "Generate_Default_Map_Pak() Arguments: <install path> <default map assets folder name> <resulting pak name> <commit hash>"
    echo "Generate_Default_Map_Pak(): failed to install pak '${3}.pk3'"
    return 1
  fi

  if [ ! -d $1 ]; then
    echo "Generate_Default_Map_Pak(): Install path '${1}' doesn't exist"
    echo "Generate_Default_Map_Pak(): failed to install pak '${3}.pk3'"
    return 1
  fi

  if [ ! -d $SOURCEPATH/map-assets/default/$2 ]; then
    echo "Generate_Default_Map_Pak(): Default map assets folder '${2}' doesn't exist"
    echo "Generate_Default_Map_Pak(): failed to install pak '${3}.pk3'"
    return 1
  fi

  if [ $(git cat-file -t "${4}") != "commit" ]; then
    echo "Generate_Default_Map_Pak(): Commit hash '${4}' doesn't exist"
    echo "Generate_Default_Map_Pak(): failed to install pak '${3}.pk3'"
    return 1
  fi

  cd $SOURCEPATH/map-assets/default/${2}
  echo "Generating and installing default pak '${3}.pk3'..."
  TZ='America/New_York' git archive --format=zip --output=${1}/${3}.pk3 ${4} ./
  cd $CURRENTPATH
  return 0
}

Install_Default_Paks() {
  if [ ! -d $1 ]; then
    echo "Install_Default_Paks(): Install path '${1}' doesn't exist"
    echo "Failed to install default paks"
    return 1
  fi

  echo "Installing default paks to ${1}..."
  Generate_Default_Assets_Pak $1 pak0 ecca0fee1f2496efc7cc0d710d07b9b988401c85
  Generate_Default_Map_Pak $1 eX-texture-pack1 eX-texture-pack1_a1 ecca0fee1f2496efc7cc0d710d07b9b988401c85
  Generate_Default_Map_Pak $1 eX-texture-pack2 eX-texture-pack2_a1 ecca0fee1f2496efc7cc0d710d07b9b988401c85
  Generate_Default_Map_Pak $1 map-eXcs map-eXcs_a1 ecca0fee1f2496efc7cc0d710d07b9b988401c85
  Generate_Default_Map_Pak $1 map-UTCSUD map-UTCSUD_a1 ecca0fee1f2496efc7cc0d710d07b9b988401c85
  echo "Default paks installed."
  return 0
}

Install_Default_Paks_Subcommand() {
  mkdir -p $BASEPATH
  mkdir -p $BASEPATH/main
  Install_Default_Paks "$BASEPATH/main"
  return 0
}

if [ $# -eq 0 ]; then
  List_Subcommands
  exit 0
fi

#Handle subcommands
case $1 in
  help)
    if [ $# -eq 1 ]; then
      List_Subcommands
      exit 0
    fi
    Help_Subcommand $2
    exit 0
    ;;

  init)
    Init_Subcommand
    exit 0
    ;;

  default_config)
    Set_Config_To_Defaults
    exit 0
    ;;

  run)
    Run_Subcommand $2
    exit 0
    ;;

  debug)
    Debug_Subcommand $2
    exit 0
    ;;

  git)
    Git_Subcommand $2 ${@:3}
    exit 0
    ;;

  configure_cmake)
    Configure_Cmake_Subcommand $2
    exit 0
    ;;

  makeclean)
    Makeclean_Subcommand $2
    exit 0
    ;;

  build)
    Build_Subcommand $2
    exit 0
    ;;

  package_assets)
    Package_Assets_Subcommand ${@:2}
    exit 0
    ;;

  package_release)
    ;;

  install_default_paks)
    Install_Default_Paks_Subcommand
    exit 0
    ;;

  sync)
    Sync_Subcommand $2
    ;;

  rotate_logs)
    ;;

  *)
    printf "Unknown subcommand \"$1\"\n\n"
    List_Subcommands
    exit 1
    ;;
  esac
