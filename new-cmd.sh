
gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=UYVY, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM),width=1920, height=1080, format=NV12' ! nvdrmvideosink  plane-id=1 -e



gst-launch-1.0 nvv4l2camerasrc device=/dev/video0 ! 'video/x-raw(memory:NVMM), format=YUYV, width=1920, height=1080, interlace-mode=progressive, framerate=30/1' ! nvvidconv ! 'video/x-raw(memory:NVMM),width=1920, height=1080, format=NV12' ! nvdrmvideosink  plane-id=1 -e

# oghab
gst-launch-1.0 nvv4l2camerasrc device=/dev/video3 ! 'video/x-raw(memory:NVMM), width=1920, height=1080' ! nvvidconv ! 'video/x-raw(memory:NVMM),width=1920, height=1080, format=NV12' ! nvdrmvideosink conn-id=1 plane-id=0 -e

gst-launch-1.0 nvv4l2camerasrc device=/dev/video3 ! 'video/x-raw(memory:NVMM), width=1920, height=1080' ! nvvidconv ! 'video/x-raw(memory:NVMM),width=720, height=576, format=NV12' ! nvdrmvideosink conn-id=1 plane-id=0 -e
