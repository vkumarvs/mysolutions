HOME=/root
yum erase ffmpeg faac libfaac x264 libx264 libvpx
rm -rf $HOME/ffmpeg_build $HOME/ffmpeg_sources $HOME/bin/{ffmpeg,ffprobe,ffserver,lame,vsyasm,x264,yasm,ytasm}
