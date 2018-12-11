#PUBPOINT=http://www.example.com/channel1/channel1.isml
#PUBPOINT=/home/vipin/VIEW/PERSONAL_CODE/CODING/POCO_LEARNING/SIMPLE_HTTP_SERVER/live/live.isml
PUBPOINT="http://10.0.2.15:8080/live/live.isml"
EVENT_ID="$(date +%Y-%m-%d-%H_%M_%S)"
#PUBPOINT_OPTIONS="/Events($EVENT_ID)/Streams(video)"
PUBPOINT_OPTIONS='/Streams(video)'
FFMPEG_OPTIONS="-movflags isml+frag_keyframe -f ismv -threads 0"
#AUDIO_OPTIONS="-c:a libfdk_aac -ac 2 -b:a 32k"
#AUDIO_OPTIONS="-c:a libfdk_aac -ac 2 -b:a 64k"
VIDEO_OPTIONS="-c:v libx264 -x264opts pic-struct:no-scenecut -preset fast -profile:v baseline -g 48 -keyint_min 48 -sc_threshold 0"
#MAP="-map 0:a:0 -map 0:v -b:v:0 477k -s:v:0 368x152 -map 0:v -b:v:1 331k -s:v:1 288x120 -map 0:v -b:v:2 230k -s:v:2 224x92 -map 0:v -b:v:0 2877k -s:v:0 1280x720"
MAP="-map 0:v -b:v:0 477k -s:v:0 368x152 -map 0:v -b:v:1 331k -s:v:1 288x120 -map 0:v -b:v:2 230k -s:v:2 224x92"
#MAP="-map 0:v -b:v:0 477k -s:v:0 368x152 -map 0:v -b:v:1 331k -s:v:1 288x120 -map 0:v -b:v:2 230k -s:v:2 224x92 "

#CMD="-f video4linux2 -i /dev/video0 $FFMPEG_OPTIONS $AUDIO_OPTIONS $VIDEO_OPTIONS $MAP ${PUBPOINT}${PUBPOINT_OPTIONS}"
CMD="-f video4linux2 -i /dev/video0 -movflags isml+frag_keyframe -f ismv $MAP ${PUBPOINT}${PUBPOINT_OPTIONS}"

/root/bin/ffmpeg $CMD
