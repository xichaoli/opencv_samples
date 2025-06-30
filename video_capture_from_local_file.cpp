#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int, char**)
{
    Mat frame;

    auto t = static_cast<double>(getTickCount());
    VideoCapture capture("/home/lixc/192.mp4");

    for (;;)
    {
        constexpr int FPS = 30; // 定义FPS常量
        capture >> frame;
        if (frame.empty())
            break;
        imshow("Frame", frame);
        int key = 0;
        while (((static_cast<double>(getTickCount()) - t) / getTickFrequency()) < 1.0 / FPS)
            key = waitKey(1);
        t = static_cast<double>(getTickCount());
        if (key == 'q')
            break;
    }
}
