#ffmpeg -i <source mp4 file > -an -c copy <output mp4 withoit audio>
#ffmpeg -i <Source mp4 file with audio_video> -vn -acodec copy <mp4_only_audio>

mp4split -v 4 --license-key="<your key>" -o source_manifest.ism --hds.client_manifest_version=2 --hls.client_manifest_version=4 output-audio.mp4 --track_language=eng --track_type=audio source_1080_6000k_1.mp4 --track_language=eng --track_type=video source_1080_8000k.mp4_1.mp4 --track_language=eng --track_type=video source_270_600k.mp4_1.mp4 --track_language=eng --track_type=video source_360_1200k.mp4_1.mp4 --track_language=eng --track_type=video source_540_2500k.mp4_1.mp4 --track_language=eng --track_type=video source_720_4000k.mp4_1.mp4 --track_language=eng --track_type=video

