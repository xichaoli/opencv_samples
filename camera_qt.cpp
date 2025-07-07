#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
    // 初始化 Qt 应用程序
    QApplication app(argc, argv);

    // 使用 OpenCV 读取图像
    cv::Mat frame;
    cv::VideoCapture capture;
    // 打开摄像头
    capture.open(0, cv::CAP_V4L2);

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

    // 创建窗口，使用 Qt 后端
    cv::namedWindow("Camera Feed", cv::WINDOW_NORMAL);

    while (true)
    {
        capture >> frame;
        if (frame.empty())
        {
            std::cerr << "ERROR: Can't grab camera frame." << std::endl;
            break;
        }

        // 显示当前帧
        cv::imshow("Camera Feed", frame);

        // 等待按键事件，根据帧率设置等待时间
        int key = cv::waitKey(interval);
        if (key == 27) // ESC 键
        {
            break;
        }
    }

    // 释放资源
    capture.release();
    cv::destroyAllWindows();

    return 0;
}