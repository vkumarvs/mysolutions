#!/bin/bash

#Only sintel movie can be used for 40Mb bit rate
#4k link for sintel movie
#wget https://download.blender.org/durian/movies/sintel_4k.mov

#Elephant dream
#wget http://video.blendertestbuilds.de/download.blender.org/ED/ED_HD.avi

#For big bunny
#https://peach.blender.org/download/ -- download 1920*1080 version

#How to run
#./auto_ffmpeg_hls.sh <filepath> <Destination Directory for .ts>
#example: ./auto_ffmpeg_hls.sh /home/vkumar/VIEW/MOV/sintel_4k.mov /hls/sintel
#example: ./auto_ffmpeg_hls.sh /home/vkumar/VIEW/MOV/ED_HD.avi /hls/Elephant_Dream

#for more higher bitrate one can see below command too
#/root/bin/ffmpeg -i /home/vkumar/VIEW/MOV/sintel_4k.mov -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v main -level 5.0 -maxrate 40M -bufsize 120M -crf 18 -r 30 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 3840x2160 hls/big_buck_bunny/169ar/3840x2160.m3u8

#crf Select the quality for constant quality mode (from -1 to 63) (default -1)
#-g set the group of picture (GOP) size (from INT_MIN to INT_MAX) (default 12)
#-bufsize set ratecontrol buffer size (in bits) (from INT_MIN to INT_MAX) (default 0)
#-maxrate maximum bitrate (in bits/s). Used for VBV together with bufsize. (from 0 to INT_MAX) (default 0)
#-aspect 16:9 or 4:3

mkdir -p $2/169ar
mkdir -p $2/43ar
PATH169=$2/169ar
/root/bin/ffmpeg -i $1 -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v high -level 5.1 -maxrate 10M -bufsize 30M -crf 18 -r 30 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 3840x2160 $PATH169/3840x2160.m3u8

/root/bin/ffmpeg -i $1 -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v main -level 5.0 -maxrate 8M -bufsize 24M -crf 18 -r 24 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 1920x1080 $PATH169/1920x1080.m3u8

/root/bin/ffmpeg -i $1 -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v main -level 3.2 -maxrate 2M -bufsize 6M -crf 18 -r 24 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 1280x720 $PATH169/1280x720.m3u8

/root/bin/ffmpeg -i $1 -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v baseline -level 3.1 -maxrate 1M -bufsize 3M -crf 18 -r 24 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 640x360 $PATH169/640x360.m3u8

/root/bin/ffmpeg -i $1 -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 64k -ar 44100 -c:v libx264 -profile:v baseline -level 2.1 -maxrate 500K -bufsize 2M -crf 18 -r 10 -g 30  -f hls -hls_time 9 -hls_list_size 0 -s 480x270 $PATH169/480x270.m3u8

/root/bin/ffmpeg -i $1 -aspect 16:9 -c:a aac -strict experimental -ac 2 -b:a 64k -ar 44100 -c:v libx264 -profile:v baseline -level 1.3 -maxrate 192K -bufsize 1M -crf 18 -r 10 -g 30 -f hls -hls_time 9 -hls_list_size 0 -s 320x180 $PATH169/320x180.m3u8


#4:3 aspect ratio
PATH43=$2/43ar
/root/bin/ffmpeg -i $1 -aspect 4:3 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v high -level 5.1 -maxrate 10M -bufsize 30M -crf 18 -r 30 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 3840x2160 $PATH43/3840x2160.m3u8

/root/bin/ffmpeg -i $1 -aspect 4:3 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v main -level 5.0 -maxrate 8M -bufsize 24M -crf 18 -r 24 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 1920x1080 $PATH43/1920x1080.m3u8

/root/bin/ffmpeg -i $1 -aspect 4:3 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v main -level 3.2 -maxrate 2M -bufsize 6M -crf 18 -r 24 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 1280x720 $PATH43/1280x720.m3u8

/root/bin/ffmpeg -i $1 -aspect 4:3 -c:a aac -strict experimental -ac 2 -b:a 96k -ar 44100 -c:v libx264 -profile:v baseline -level 3.1 -maxrate 1M -bufsize 3M -crf 18 -r 24 -g 72 -f hls -hls_time 9 -hls_list_size 0 -s 640x360 $PATH43/640x360.m3u8

/root/bin/ffmpeg -i $1 -aspect 4:3 -c:a aac -strict experimental -ac 2 -b:a 64k -ar 44100 -c:v libx264 -profile:v baseline -level 2.1 -maxrate 500K -bufsize 2M -crf 18 -r 10 -g 30  -f hls -hls_time 9 -hls_list_size 0 -s 480x270 $PATH43/480x270.m3u8

/root/bin/ffmpeg -i $1 -aspect 4:3 -c:a aac -strict experimental -ac 2 -b:a 64k -ar 44100 -c:v libx264 -profile:v baseline -level 1.3 -maxrate 192K -bufsize 1M -crf 18 -r 10 -g 30 -f hls -hls_time 9 -hls_list_size 0 -s 320x180 $PATH43/320x180.m3u8
