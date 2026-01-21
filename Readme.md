# soft 模式
cmake -B build

# hard 模式
cmake -DENABLE_HARD=ON -B build

# soft 和 hard 模式是互斥的!!!

# 构建
cmake --build build

# 运行
./build/camera

# hard 模式需要以下依赖
1. 安装 gstreamer-vaapi 库
2. 配置 opencv 时使能OpenGL
3. 设置以下环境变量
export LIBVA_DRIVER_NAME=jmgpu
export GST_VAAPI_ALL_DRIVERS=1
4. 以上条件都满足后会有类似下面的输出(图形界面终端下)
$ gst-inspect-1.0 vaapi
Plugin Details:
  Name                     vaapi
  Description              VA-API based elements
  Filename                 /usr/lib/sw_64-linux-gnu/gstreamer-1.0/libgstvaapi.so
  Version                  1.14.4
  License                  LGPL
  Source module            gstreamer-vaapi
  Binary package           gstreamer-vaapi
  Origin URL               http://bugzilla.gnome.org/enter_bug.cgi?product=GStreamer

  vaapijpegdec: VA-API JPEG decoder
  vaapih264dec: VA-API H264 decoder
  vaapivp9dec: VA-API VP9 decoder
  vaapih265dec: VA-API H265 decoder
  vaapipostproc: VA-API video postprocessing
  vaapidecodebin: VA-API Decode Bin
  vaapisink: VA-API sink

  7 features:
  +-- 7 elements

