# 8-Bit Grauwerte
nc 127.0.0.1 8111 | mplayer - -demuxer rawvideo -rawvideo w=376:h=240:format=y8:fps=100 -vo x11

# RGB24 Bit
# nc 127.0.0.1 8111 | mplayer - -demuxer rawvideo -rawvideo w=376:h=240:format=rgb24:fps=100 -vo x11

# BGR24 Bit
# nc 127.0.0.1 8111 | mplayer - -demuxer rawvideo -rawvideo w=376:h=240:format=bgr24:fps=100 -vo x11

# YUV 422
# nc 127.0.0.1 8111 | mplayer - -demuxer rawvideo -rawvideo w=376:h=240:format=0x59565955:fps=100 -vo x11

##  YUV 422
# nc 127.0.0.1 8111 | mplayer - -demuxer rawvideo -rawvideo w=376:h=240:fps=100 -vo x11

