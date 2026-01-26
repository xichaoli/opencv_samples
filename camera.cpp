#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int, char **) {
    Mat frame;
    cout << "Opening camera..." << endl;
    VideoCapture capture;
#ifdef ENABLE_HARD
    const string gst_pipeline =
            "v4l2src device=/dev/video0 do-timestamp=true ! image/jpeg,width=1920,height=1080,framerate=30/1 ! vaapijpegdec ! video/x-raw,format=NV12 ! queue leaky=downstream ! appsink sync=false max-buffers=5 drop=true";
#else
    const string gst_pipeline =
            "v4l2src device=/dev/video0 do-timestamp=true ! image/jpeg,width=1920,height=1080,framerate=30/1 ! jpegdec ! videoconvert n-threads=8 ! video/x-raw,format=BGR ! queue leaky=downstream ! appsink sync=false max-buffers=5 drop=true";
#endif

    capture.open(gst_pipeline, CAP_GSTREAMER);
    if (!capture.isOpened()) {
        cerr << "ERROR: Can't initialize camera capture" << endl;
        return 1;
    }
    const int width = static_cast<int>(capture.get(CAP_PROP_FRAME_WIDTH));
    const int height = static_cast<int>(capture.get(CAP_PROP_FRAME_HEIGHT));
    cout << "Frame width: " << width << endl;
    cout << "     height: " << height << endl;
    cout << "Capturing FPS: " << capture.get(CAP_PROP_FPS) << endl;
    cout << endl << "Press 'ESC' to quit, 'space' to toggle frame processing" << endl;
    cout << endl << "Start grabbing..." << endl;

    size_t nFrames = 0;
    int64 t0 = getTickCount();
    int64 processingTime = 0;
    const string window_name("camera_test_window");
#ifdef ENABLE_HARD
    namedWindow(window_name, WINDOW_OPENGL | WINDOW_AUTOSIZE);
    setOpenGlContext(window_name);
#endif

    double fps = 0.0;
    char fps_text[32];
    char res_text[32];
    for (;;) {
#ifdef ENABLE_HARD
        Mat frame_nv12;
        capture >> frame_nv12;
        cvtColor(frame_nv12, frame, COLOR_YUV2BGR_NV12);
#else
        capture >> frame;
#endif

        if (frame.empty()) {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        nFrames++;
        if (nFrames % 10 == 0) {
            constexpr int N = 10;
            const int64 t1 = getTickCount();
            fps = getTickFrequency() * N / static_cast<double>(t1 - t0);
            cout << "Frames captured: " << format("%5zu", nFrames)
                    << "    Average FPS: " << format(
                        "%9.1f", getTickFrequency() * N / static_cast<double>(t1 - t0))
                    << "    Average time per frame: " << format(
                        "%9.2f ms", static_cast<double>(t1 - t0) * 1000.0f / (N * getTickFrequency()))
                    << "    Average processing time: " << format(
                        "%9.2f ms", static_cast<double>(processingTime) * 1000.0f / (N * getTickFrequency()))
                    << std::endl;
            t0 = t1;
            processingTime = 0;
        }
        sprintf(fps_text, "FPS: %.2f", fps);
        sprintf(res_text, "Resolution:%d*%d", width, height);
        putText(frame, fps_text, Point(10, 30),
                FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 255, 0), 2);
        putText(frame, res_text, Point(10, 70),
                FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 0), 2);

        imshow(window_name, frame);

        const int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
    }
    std::cout << "Number of captured frames: " << nFrames << endl;
    return nFrames > 0 ? 0 : 1;
}
