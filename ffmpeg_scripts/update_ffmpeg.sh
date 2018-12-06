rm -rf ~/ffmpeg_build ~/bin/{ffmpeg,ffprobe,ffserver,lame,vsyasm,x264,yasm,ytasm}

#Update x264
cd ~/ffmpeg_sources/x264
make distclean
git pull

#Update libfdk_aac
cd ~/ffmpeg_sources/libfdk_aac
make distclean
git pull

#Update libvpx
cd ~/ffmpeg_sources/libvpx
make clean
git pull

#Update FFmpeg
cd ~/ffmpeg_sources/ffmpeg
make distclean
git pull
