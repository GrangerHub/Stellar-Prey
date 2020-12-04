# Stellar-Prey
GrangerHub's STELLAR PREY™ game is a first person strategy shooter that chronicles the conflict between human technology vs extraterrestrial nature, a conflict waged by construction, team coordination, and epic combat.  Help shape the course of your species together towards the top of the stellar food chain, or be the prey together on the road towards extinction.  Which path will you and your species choose to take?  It's a universal tale of survival vs death, and of order vs chaos, as old as life itself.

This is the main repo for STELLAR PREY™ game and can be used to build and package releases, used as a development environment, as well as for the setup/operation of the client and/or the dedicated/master/auto-update servers.  All related code, scripts, and assets can be found through submodules in this repo.

## Dependencies
### Linux (Ubuntu 20.04 LTS)
* cmake
* cmake-curses-gui
* gdb
* git
* g++
* libbz2-dev
* libcurl4-openssl-dev
* libfreetype6-dev
* libgeoip-dev
* libglew-dev
* libjpeg-dev
* libmysqlclient-dev
* libncursesw5-dev
* libogg-dev
* libopenal-dev
* libsdl1.2-dev
* libsdl2-dev
* libtdb-dev
* libvorbis-dev
* mysql-client
* ncurses-dev
* opencl-dev
* rsync
* tmux
* zip

## Setup
To setup this repo, execute the following commands from the root of this repo in order:

```
./sp-tools.sh default_config
```
Set the values in the `config.sh` accordingly.  Then continue with the following commands:

```
./sp-tools.sh init
./sp-tools.sh install_default_paks
./sp-tools.sh configure_cmake both
./sp-tools.sh build both
```

For more details on how to manage and use this repo, check:

```
./sp-tools.sh help
```

GrangerHub's STELLAR PREY™ game is originally based on darklegion development's Tremulous game.  GrangerHub's STELLAR PREY™ game is built on TheDushan's OpenWolf Engine.
