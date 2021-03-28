FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && \
	apt-get install -y \
		cmake \
		cmake-curses-gui \
		gdb \
		git \
		g++ \
		libbz2-dev \
		libcurl4-openssl-dev \
		libfreetype6-dev \
		libgeoip-dev \
		libglew-dev \
		libjpeg-dev \
		libmysqlclient-dev \
		libncursesw5-dev \
		libogg-dev \
		libopenal-dev \
		libsdl1.2-dev \
		libsdl2-dev \
		libtdb-dev \
		libvorbis-dev \
		mysql-client \
		ncurses-dev \
		opencl-dev \
		rsync \
		tmux \
		zip && \
	apt-get clean
