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


gst-launch-1.0 filesrc location=/home/meysam/Desktop/car1_s.mp4 ! \
 decodebin ! videoconvert ! autovideosink


gst-launch-1.0 v4l2src device=/dev/video1 ! 'video/x-raw,format=UYVY' ! nvvidconv ! 'video/x-raw(memory:NVMM),format=NV12' ! nvoverlaysink sync=false


gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=30/1' ! queue ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, framerate=25/1, format=NV12' ! nvoverlaysink

gst-launch-1.0 -v videotestsrc  ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=30/1'  ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, framerate=30/1, format=NV12' ! nvoverlaysink sync=false


gst-launch-1.0 v4l2src device='/dev/video0' ! 'video/x-raw, width=1920, height=1080, format=UYVY' ! xvimagesink -e
 
sudo gst-launch-1.0 -v v4l2src device=/dev/video0  ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, framerate=30/1, format=NV12' ! nvoverlaysink sync=false

gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert! videoscale ! 'video/x-raw, width=1920, height=1080' ! autovideosink -v 

gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert! videoscale ! 'video/x-raw, width=1920, height=1080' ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nvoverlaysink sync=false



gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=60/1'  ! videoconvert! videoscale ! 'video/x-raw, width=1920, height=1080' ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nvoverlaysink sync=false

gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=60/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nvoverlaysink sync=false



gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=60/1' \
! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=RGBA'! queue \
! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nvoverlaysink sync=false

gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), width=1920,height=1080, format=UYVY,framerate=60/1' \
! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nveglglessink winsys=wayland

nveglglessink


10.10.1.44 10.10.1.45 8.8.8.8 8.8.4.4


export PATH=/usr/local/cuda-10.2/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-10.2/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

export DISPLAY=:0

cd /usr/lib/aarch64-linux-gnu
sudo ln -sf libv4l2.so.0 libv4l2.so

./camera_v4l2_cuda -d /dev/video0 -s 1920x1080 -f UYVY -c



export DISPLAY=:0

sudo systemctl stop gdm
sudo loginctl terminate-seat seat0


sudo systemctl start gdm

gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=60/1' ! videoconvert ! xvimagesink sync=false

gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080' ! nvvidconv ! 'video/x-raw(memory:NVMM)'  ! nvvidconv ! 'video/x-raw,width=1920,height=1080' ! nvstabilize queue-size=5 ! 'video/x-raw,width=1920,height=1080' ! nveglglessink sync=false



gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=30/1' \
! nvvidconv ! 'video/x-raw(memory:NVMM), width=960, height=1080, format=NV12' ! nvoverlaysink sync=false overlay-x=0 overlay-y=0 overlay-w=960 overlay-h=1080  overlay=0 &&
gst-launch-1.0 v4l2src device=/dev/video1 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=30/1' \
! nvvidconv ! 'video/x-raw(memory:NVMM), width=960, height=1080, format=NV12' ! nvoverlaysink sync=false overlay-x=960 overlay-y=0 overlay-w=960 overlay-h=1080 overlay=1




sudo XDG_RUNTIME_DIR=/tmp/xdg gst-launch-1.0 filesrc \
location=<filename.mp4> ! qtdemux name=demux ! h264parse ! \
omxh264dec ! nveglglessink winsys=wayland


sudo systemctl stop gdm
sudo loginctl terminate-seat seat0
unset DISPLAY

mkdir /tmp/xdg

chmod 700 /tmp/xdg

sudo XDG_RUNTIME_DIR=/tmp/xdg weston --idle-time=0 &



sudo systemctl start gdm

gst-launch-1.0 nvarguscamerasrc ! 'video/x-raw(memory:NVMM), width=(int)1920, height=(int)1080,format=(string)NV12, framerate=(fraction)30/1' ! nvoverlaysink sync=false


nvegltransform


export DISPLAY=:0


gst-launch-1.0 v4l2src device=/dev/video1 !  'video/x-raw,width=1920,height=1080, format=UYVY,framerate=60/1'! videoconvert ! xvimagesink

57600000 
166400000 
268800000 
377600000 
480000000 
582400000 
691200000 
780800000

(v4l:10476): GLib-GObject-WARNING **: 06:47:21.849: g_object_set_is_valid_property: object class 'GstFakeSink' has no property named 'overlay'
NvxBaseWorkerFunction[2575] comp OMX.Nvidia.std.iv_renderer.overlay.yuv420 Error -2147479552 
Error received from element source0: Internal data stream error.
Debugging information: gstbasesrc.c(3055): gst_base_src_loop (): /GstPipeline:test-pipline/GstNvV4l2CameraSrc:source0:
streaming stopped, reason error (-5)
End-Of-Stream reached.


gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM),format=UYVY,width=1920,height=1080,framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nvoverlaysink sync=false 



$ gst-launch-1.0 nvarguscamerasrc ! "video/x-raw(memory:NVMM),format=NV12,width=640,height=480,framerate=30/1" 
! nvvidconv ! "video/x-raw(memory:NVMM),format=RGBA" ! queue 
! nvcompositor name=comp sink_0::width=640 sink_0::height=480 sink_0::xpos=0 sink_0::ypos=0 sink_1::width=640 sink_1::height=480 sink_1::xpos=640 sink_1::ypos=0 ! 
"video/x-raw(memory:NVMM),format=RGBA" ! nvvidconv ! video/x-raw,format=I420 ! xvimagesink 
v4l2src device=/dev/video1 ! video/x-raw,format=UYVY,width=640,height=480,framerate=30/1 
! nvvidconv ! "video/x-raw(memory:NVMM),format=RGBA" ! queue ! comp.


# steps to run VIC in max clock
# https://forums.developer.nvidia.com/t/nvvideoconvert-issue-nvvideoconvert-in-ds4-is-better-than-ds5/142115

echo on > /sys/devices/13e10000.host1x/15340000.vic/power/control
echo userspace > /sys/devices/13e10000.host1x/15340000.vic/devfreq/15340000.vic/governor
cat /sys/devices/13e10000.host1x/15340000.vic/devfreq/15340000.vic/available_frequencies
echo 780800000 > /sys/devices/13e10000.host1x/15340000.vic/devfreq/15340000.vic/max_freq
echo 780800000 > /sys/devices/13e10000.host1x/15340000.vic/devfreq/15340000.vic/userspace/set_freq


export DISPLAY=:0



fpsdisplaysink text-overlay=0 video-sink=autovideosink 


gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nv3dsink -e



sudo systemctl stop gdm
export DISPLAY=:0
sudo loginctl terminate-seat seat0
sudo jetson_clocks



gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nvoverlaysink sync=false

gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nv3dsink sync=false


gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nveglglessink sync=false 

gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nveglglessink sync=false winsys=wayland

sudo XDG_RUNTIME_DIR=/tmp/xdg gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nveglglessink sync=false winsys=wayland


gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! textoverlay text="Room A" valignment=top halignment=left font-desc="Sans, 72" ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nvoverlaysink sync=false


gst-launch-1.0 -v videotestsrc ! textoverlay text="Room A" valignment=top halignment=left font-desc="Sans, 72" ! autovideosink