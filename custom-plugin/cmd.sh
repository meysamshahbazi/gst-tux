

# https://gstreamer.freedesktop.org/documentation/plugin-development/basics/boiler.html?gi-language=c

git clone https://gitlab.freedesktop.org/gstreamer/gst-template.git


cd gstreamer/subprojects/gst-plugins-bad/tools/

./gst-element-maker rtpsync basetransform

mkdir ../gst/rtpsync


cp *.h and *.c to ../gst/rtpsync

cd ../gst/rtpsync/

cp ../sdp/meson.build . # and edit with suitable names 
# also edit top level meson.build and add rtpsync

ninja -C build subprojects/gst-plugins-bad/gst/libgstmycustomelement.so

ninja -C build install


rm -rf ~/.cache/gstreamer-1.0/*
gst-inspect-1.0 rtpsync


