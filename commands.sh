gst-launch-1.0   \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! \
    decodebin ! videoconvert ! \
    videoscale ! video/x-raw,width=640,height=360 ! \
    compositor name=mix sink_0::alpha=1 sink_1::alpha=1 !   \
    videoconvert ! autovideosink \
    filesrc location=/home/meysam/Desktop/test.mp4 ! \
    decodebin ! videoconvert ! \
    videoscale ! video/x-raw,width=320,height=180! \
    mix.



gst-launch-1.0   \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! \
    decodebin ! videoconvert ! \
    videoscale ! video/x-raw,width=640,height=360 ! \
    compositor name=mix sink_0::alpha=1 sink_1::alpha=1 !   \
    videoconvert ! autovideosink \
    filesrc location=/home/meysam/Desktop/test.mp4 ! \
    decodebin ! videoconvert ! \
    videoscale ! video/x-raw,width=320,height=180! \
    videobox border-alpha=0 top=-10 bottom=-10 right=-10 left=-10 ! \
    mix.


# this works
gst-launch-1.0   \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! \
    decodebin ! videoconvert ! \
    videoscale ! video/x-raw,width=320,height=360 ! \
    compositor name=mix \
    sink_0::alpha=1 sink_0::xpos=0  sink_0::ypos=0 sink_0::width=320  sink_0::height=360 \
    sink_1::alpha=1 sink_0::xpos=640  sink_0::ypos=0 sink_0::width=320  sink_0::height=360 !   \
    videoconvert ! autovideosink \
    filesrc location=/home/meysam/Desktop/test.mp4 ! \
    decodebin ! videoconvert ! \
    videoscale ! video/x-raw,width=320,height=360! \
    mix.



gst-launch-1.0   \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! \
    decodebin ! videoconvert ! \
    compositor name=mix \
    sink_0::alpha=1 sink_0::xpos=0  sink_0::ypos=0 sink_0::width=640  sink_0::height=360 \
    sink_1::alpha=1 sink_0::xpos=0  sink_0::ypos=360 sink_0::width=640  sink_0::height=360 !   \
    videoconvert ! autovideosink \
    filesrc location=/home/meysam/Desktop/test.mp4 ! \
    decodebin ! videoconvert ! \
    mix.

gst-launch-1.0
gst-launch-1.0 -e videotestsrc pattern="snow" ! video/x-raw-yuv, framerate=10/1, width=200, height=150 ! videomixer name=mix ! xvimagesink videotestsrc ! video/x-raw-yuv, framerate=10/1, width=640, height=360 ! mix.


 gst-launch-1.0 -v -e videomixer name=mix  ! xvimagesink \
    videotestsrc pattern=1 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=0 left=0 ! mix. \
    videotestsrc pattern=15 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=0 left=-320 ! mix. \
    videotestsrc pattern=13 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=-180 left=0 ! mix. \
    videotestsrc pattern=0 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=-180 left=-320 ! mix. \
    videotestsrc pattern=3 ! video/x-raw-yuv,framerate=5/1,width=640,height=360 ! mix.



gst-launch-1.0 -v -e videomixer name=mix  ! xvimagesink \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=0 left=0 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=0 left=-320 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=-180 left=0 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw-yuv,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=-180 left=-320 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw-yuv,framerate=5/1,width=640,height=360 ! mix.


    gst-launch-1.0 -v -e videomixer name=mix  ! xvimagesink \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=0 left=0 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=0 left=-320 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=-180 left=0 ! mix. \
    filesrc location=/home/meysam/Desktop/car1_s.mp4 ! video/x-raw,framerate=5/1,width=320,height=180 ! videobox border-alpha=0 top=-180 left=-320 ! mix. \
     mix.