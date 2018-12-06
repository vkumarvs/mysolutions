NAME=live
PUBDIR=<mypath>
LICENSE="<your key>"

MP4SPLIT_OPTIONS="--archive_segment_length=60 --dvr_window_length=60 --archiving=0 --restart_on_encoder_reconnect"

mkdir -m 777 -p ${PUBDIR}
mp4split -v 4 --license-key=${LICENSE} -o ${PUBDIR}/${NAME}.isml ${MP4SPLIT_OPTIONS}
chmod ua+w ${PUBDIR}/${NAME}.isml
