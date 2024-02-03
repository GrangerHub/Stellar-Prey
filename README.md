# Stellar-Prey
GrangerHub's STELLAR PREY™ game is a first person strategy shooter that chronicles the conflict between human technology vs extraterrestrial nature, a conflict waged by construction, team coordination, and epic combat.  Help shape the course of your species together towards the top of the stellar food chain, or be the prey together on the road towards extinction.  Which path will you and your species choose to take?  It's a universal tale of survival vs death, and of order vs chaos, as old as life itself.

This is the main repo for STELLAR PREY™ game and can be used to build and package releases, used as a development environment, as well as for the setup/operation of the client and/or the dedicated/master/auto-update servers.  All related code, scripts, and assets can be found through submodules in this repo.

## Dependencies
### Linux (Ubuntu 20.04 LTS)

> sudo apt install cmake cmake-curses-gui gdb git g++ libbz2-dev libcurl4-openssl-dev libfreetype6-dev libgeoip-dev libglew-dev libjpeg-dev libmysqlclient-dev libncursesw5-dev libogg-dev libopenal-dev libsdl1.2-dev libsdl2-dev libtdb-dev libvorbis-dev mysql-client ncurses-dev opencl-dev rsync tmux zip

### Linux (Fedora 39)

> sudo dnf install gcc cmake-gui SDL2-devel gcc-c++ libjpeg-turbo openssl-devel libspng-devel openal-soft-devel libogg-devel libvorbis-devel openssl-devel ncurses-devel bzip2-devel GeoIP-devel freetype-devel libcurl-devel community-mysql-devel

### Sources

- [CMake](http://www.cmake.org/) >= 3.16
- [GeoIP](https://github.com/maxmind/geoip-api-c)
- [SDL2](https://www.libsdl.org/download-2.0.php) - or simply install your OS package

## Building

### Initialize

To setup this repo, execute the following commands from the root of this repo in order:

```bash
./sp-tools.sh default_config
```
which will create a local file `config.sh` with your settings.

### Set up of `config.sh`

Set the values in the `config.sh` accordingly.

This section in `config.sh` turns on compiling different parts:
```make
BUILD_CLIENT="ON"
BUILD_SERVER="ON"
BUILD_AUTOUPDATE_SERVER="ON"
BUILD_MASTER_SERVER="ON"
BUILD_AUTH_SERVER="ON"
```

### Download submodules and paks

Then continue with the following commands:

```bash
./sp-tools.sh init
./sp-tools.sh install_default_paks
./sp-tools.sh configure_cmake both
./sp-tools.sh build both
```

## Running

The built binaries can be run with the `run` subcommand:

```bash
./sp-tools.sh run client
```

To see other options, run:

```bash
./sp-tools.sh help run 
```

## Debugging

The built binaries can be debugged with the `debug` subcommand:

```bash
./sp-tools.sh debug client
```

To see other options, run:

```bash
./sp-tools.sh help debug 
```

## More help

For more details on how to manage and use this repo, check:

```
./sp-tools.sh help
```

## Credits

GrangerHub's STELLAR PREY™ game is originally based on [Darklegion Development](https://github.com/darklegion)'s game [Tremulous](https://github.com/darklegion/tremulous).  GrangerHub's STELLAR PREY™ game is built on [TheDushan](https://github.com/TheDushan)'s [OpenWolf Engine](https://github.com/TheDushan/OpenWolf-Engine).
