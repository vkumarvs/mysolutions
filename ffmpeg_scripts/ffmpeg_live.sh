PUBPOINT="http://localhost:8080/msd/live/live.isml"
EVENT_ID="$(date +%Y-%m-%d-%H_%M_%S)"
#allows the viewer to rewind 10 minutes back in time (--dvr_window_length is 10 minute)
PUBPOINT_OPTIONS="/Events($EVENT_ID)/Streams(video)?archive_length=3600&dvr_window_length=600&archiving=1"
#PUBPOINT_OPTIONS='/Streams(video)'
FFMPEG_OPTIONS="-movflags isml+frag_keyframe -f ismv -threads 0"
AUDIO_OPTIONS="-c:a libfdk_aac -ac 2 -b:a 64k"
VIDEO_OPTIONS="-c:v libx264 -x264opts pic-struct:no-scenecut -preset fast -profile:v baseline -g 48 -keyint_min 48 -sc_threshold 0"
MAP="-map 0:a:0 -map 0:v -b:v:0 477k -s:v:0 368x152 -map 0:v -b:v:1 331k -s:v:1 288x120 -map 0:v -b:v:2 230k -s:v:2 224x92"

CMD="-y -re
  -i $1
  $FFMPEG_OPTIONS
  $AUDIO_OPTIONS
  $VIDEO_OPTIONS
  $MAP
  ${PUBPOINT}${PUBPOINT_OPTIONS}"

/root/bin/ffmpeg $CMD
