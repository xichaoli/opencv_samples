#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>

using namespace cv;
using std::cout;
using std::cerr;
using std::endl;

int main(int, char**)
{
    Mat frame;
    cout << "Opening camera..." << endl;

    // GStreamer string
    // use vaapi dec, need gpu support
    // std::string gst_pipeline = "v4l2src device=/dev/video0 ! image/jpeg, width=1920, height=1080, framerate=30/1 ! vaapijpegdec ! videoconvert ! appsink";
    // or soft jpeg dec
    std::string gst_pipeline = "v4l2src device=/dev/video0 ! image/jpeg, width=1920, height=1080, framerate=30/1 ! jpegdec ! videoconvert ! appsink";
    // or local file ?
    // std::string gst_pipeline = "/home/lixc/192.mp4";

    VideoCapture capture;

    // v4l2
    // capture.open(0, CAP_V4L2); // open the first camera

    // GStreamer
    // use vaapi dec
    //capture.open(gst_pipeline, CAP_GSTREAMER, {CAP_PROP_HW_ACCELERATION, VIDEO_ACCELERATION_VAAPI});
    // try all acceleration
    capture.open(gst_pipeline, CAP_GSTREAMER, {CAP_PROP_HW_ACCELERATION, VIDEO_ACCELERATION_ANY});

    // FFmpeg
    // capture.open("/home/lixc/192.mp4", CAP_FFMPEG);
    // incorrect usage!
    // capture.open(gst_pipeline, CAP_FFMPEG, {CAP_PROP_HW_ACCELERATION, VIDEO_ACCELERATION_ANY});

    if (!capture.isOpened())
    {
        cerr << "ERROR: Can't initialize camera capture" << endl;
        return 1;
    }

    // for v4l2
    // bool fourcc_set = capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // #ifndef NDEBUG
    // cout << "fourcc set: " << fourcc_set << endl;
    // #endif
    //
    // capture.set(CAP_PROP_FRAME_WIDTH, 1920);
    // capture.set(CAP_PROP_FRAME_HEIGHT, 1080);
    // capture.set(CAP_PROP_FPS, 30);
    // v4l2 end

    int frame_width = static_cast<int>(capture.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(capture.get(CAP_PROP_FRAME_HEIGHT));
    double fps = capture.get(CAP_PROP_FPS);

    cout << "Frame width: " << frame_width << endl;
    cout << "     height: " << frame_height << endl;
    cout << "Capturing FPS: " << fps << endl;

    cout << endl << "Press 'ESC' to quit, 'space' to toggle frame processing" << endl;
    cout << endl << "Start grabbing..." << endl;

    size_t nFrames = 0;
    bool enableProcessing = false;
    int64 t0 = cv::getTickCount();
    int64 processingTime = 0;
    double avg_fps = 0.0;
    char fps_text[32];
    char res_text[32];
    double avg_time_per_frame = 0.0;
    double avg_process_time = 0.0;

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        nFrames++;
        if (nFrames % 10 == 0)
        {
            constexpr int N = 10;
            int64 t1 = cv::getTickCount();
            avg_fps = static_cast<double>(getTickFrequency()) * N / (static_cast<double>(t1 - t0));
#ifndef NDEBUG
            avg_time_per_frame = static_cast<double>(t1 - t0) * 1000.0f / (N * getTickFrequency());
            avg_process_time = static_cast<double>(processingTime) * 1000.0f / (N * getTickFrequency());
            cout << "Frames captured: " << cv::format("%5lld", (long long int)nFrames)
                << "    Average FPS: " << cv::format("%9.1f", avg_fps)
                << "    Average time per frame: " << cv::format("%9.2f ms", avg_time_per_frame)
                << "    Average processing time: " << cv::format("%9.2f ms", avg_process_time)
                << std::endl;
#endif

            t0 = t1;
            processingTime = 0;
        }
        if (!enableProcessing)
        {
            sprintf(fps_text, "FPS: %.2f", avg_fps);
            sprintf(res_text, "Resolution:%d*%d", frame_width, frame_height);
            cv::putText(frame, fps_text, cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
            cv::putText(frame, res_text, cv::Point(10, 70),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 0), 2);
            imshow("Frame", frame);
        }
        else
        {
            int64 tp0 = cv::getTickCount();
            Mat processed;
            cv::Canny(frame, processed, 400, 1000, 5);
            processingTime += cv::getTickCount() - tp0;
            imshow("Frame", processed);
        }

        // for local file
        // int key = waitKey(30);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
            break;
        if (key == 32/*SPACE*/)
        {
            enableProcessing = !enableProcessing;
            cout << "Enable frame processing ('space' key): " << enableProcessing << endl;
        }
    }
    std::cout << "Number of captured frames: " << nFrames << endl;
    return nFrames > 0 ? 0 : 1;
}
