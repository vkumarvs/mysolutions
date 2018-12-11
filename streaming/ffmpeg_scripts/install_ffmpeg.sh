#!/bin/bash
#this script is only for centos 
 yum install autoconf automake gcc gcc-c++ git libtool make nasm pkgconfig unzip zlib-devel -y
ROOT_DIR=/opt/universalcache
cd $ROOT_DIR/packages/mydep/
tar xzvf ffmpeg_sources.tar

HOME=/root

#Yasm
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf yasm.tar
cd yasm
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin"
make
make install
make distclean

#libx264
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf x264.tar
cd x264
PATH=$PATH:$HOME/bin  ./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --enable-static
PATH=$PATH:$HOME/bin  make &&  make install
make distclean

#libfdk_aac
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf fdk-aac.tar
cd fdk-aac
 autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean

#libmp3lame
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf lame-3.99.5.tar.gz
cd lame-3.99.5
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --disable-shared --enable-nasm
make
make install
make distclean

#libopus
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf opus.tar
cd opus
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean

#libogg
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf libogg-1.3.2.tar.gz
cd libogg-1.3.2
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean


#libvorbis
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf libvorbis-1.3.4.tar.gz
cd libvorbis-1.3.4
./configure --prefix="$HOME/ffmpeg_build" --with-ogg="$HOME/ffmpeg_build" --disable-shared
make
make install
make distclean

#libvpx
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf libvpx.tar
cd libvpx
PATH=$PATH:$HOME/bin ./configure --prefix="$HOME/ffmpeg_build" --disable-examples
PATH=$PATH:$HOME/bin  make &&  make install
make clean

#ffmpeg
cd $ROOT_DIR/packages/mydep/ffmpeg_sources
tar xzvf ffmpeg.tar
cd ffmpeg
PATH=$PATH:$HOME/bin PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig"  ./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="$HOME/bin" --enable-gpl --enable-nonfree --enable-libfdk_aac --enable-libmp3lame --enable-libopus --enable-libvorbis --enable-libvpx --enable-libx264 
PATH=$PATH:$HOME/bin  make &&  make install
make distclean
hash -r

#removed untared files now
rm -rf $ROOT_DIR/packages/mydep/ffmpeg_sources


