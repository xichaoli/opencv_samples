#include <QApplication>
#include <QTimer>
#include <opencv2/opencv.hpp>

using std::string;
using std::cout;
using std::endl;
using namespace cv;

int main(int argc, char* argv[])
{
    // 初始化 Qt 应用程序
    QApplication app(argc, argv);

    // 使用 OpenCV 读取图像
    cv::Mat frame;
    cv::Mat frame_nv12;
    cv::VideoCapture capture;
    string gst_pipeline = "v4l2src device=/dev/video0 do-timestamp=true ! image/jpeg,width=1920,height=1080,framerate=30/1 ! jpegdec ! videoconvert n-threads=8 ! video/x-raw,format=BGR ! queue leaky=downstream ! appsink sync=false";
    // hw dec
    //string gst_pipeline = "v4l2src device=/dev/video0 do-timestamp=true ! image/jpeg,width=1920,height=1080,framerate=30/1 ! vaapijpegdec ! video/x-raw,format=NV12 ! queue leaky=downstream ! appsink sync=false";
    // 打开摄像头
    capture.open(gst_pipeline, cv::CAP_GSTREAMER);

    if (!capture.isOpened())
    {
        std::cerr << "ERROR: Can't initialize camera capture" << std::endl;
        return 1;
    }

    // 获取视频帧率
    double fps = capture.get(cv::CAP_PROP_FPS);
    if (fps <= 0) {
        fps = 30; // 如果获取失败，默认设置为 30 FPS
    }
    int interval = static_cast<int>(1000 / fps); // 计算帧间隔（毫秒）

    std::cout << "Press 'ESC' to quit." << std::endl;

    size_t nFrames = 0;
    bool enableProcessing = false;
    int64 t0 = cv::getTickCount();
    int64 processingTime = 0;
    char fps_text[32];
    char res_text[32];
    int width=capture.get(cv::CAP_PROP_FRAME_WIDTH);
    int height=capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    // 创建窗口，使用 Qt 后端
    const string window_name("camera_test_window");
    cv::namedWindow(window_name, WINDOW_OPENGL | WINDOW_AUTOSIZE);
    //cv::setOpenGlContext(window_name);

    while (true)
    {
        capture >> frame;
        // for hw dec
        //capture >> frame_nv12;
        //cv::cvtColor(frame_nv12, frame, cv::COLOR_YUV2BGR_NV12);
        if (frame.empty())
        {
            std::cerr << "ERROR: Can't grab camera frame." << std::endl;
            break;
        }

        nFrames++;
        if (nFrames % 10 == 0)
        {
            const int N = 10;
            int64 t1 = cv::getTickCount();
            fps = (double)cv::getTickFrequency() * N / (t1 - t0);
            cout << "Frames captured: " << cv::format("%5lld", (long long int)nFrames)
                 << "    Average FPS: " << cv::format("%9.1f", (double)getTickFrequency() * N / (t1 - t0))
                 << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (N * getTickFrequency()))
                 << "    Average processing time: " << cv::format("%9.2f ms", (double)(processingTime) * 1000.0f / (N * getTickFrequency()))
                 << std::endl;
            t0 = t1;
            processingTime = 0;
        }
        if (!enableProcessing)
        {
           sprintf(fps_text, "FPS: %.2f", fps);
           sprintf(res_text,"Resolution:%d*%d",width,height);
           cv::putText(frame, fps_text, cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
cv::putText(frame, res_text, cv::Point(10, 70),
                    cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 0), 2);
           imshow(window_name, frame);
        }
        else
        {
            int64 tp0 = cv::getTickCount();
            Mat processed;
            cv::Canny(frame, processed, 400, 1000, 5);
            processingTime += cv::getTickCount() - tp0;
            imshow(window_name, processed);
        }

        // 等待按键事件，根据帧率设置等待时间
        //int key = waitKey(1);
        int key = cv::waitKey(interval);
        if (key == 27/*ESC*/)
            break;
        if (key == 32/*SPACE*/)
        {
            enableProcessing = !enableProcessing;
            cout << "Enable frame processing ('space' key): " << enableProcessing << endl;
        }

    }

    // 释放资源
    capture.release();
    cv::destroyAllWindows();

    return 0;
}
