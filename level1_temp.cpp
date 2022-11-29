#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Windows.h>


// パソコンサイズの黒画面
cv::Mat img(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));

// 点を描写
void draw_circle(int event, int x, int y, int flags, void* userdata) 
{
    if (event == cv::EVENT_MOUSEMOVE) 
    {
        cv::circle(img, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
}

int main()
{
    cv::setMouseCallback("Level1", draw_circle);
    // 表示するウィンドウに名前を付ける
    cv:: namedWindow("Level1", cv::WINDOW_AUTOSIZE);

    // qが押されるまで表示を継続
    while (true)
    {
        cv::imshow("Level1", img);
        if (cv::waitKey(1) && GetAsyncKeyState('q'))
        {
            break;
        }
    }

    // ウィンドウを閉じる
    cv::destroyAllWindows();

    return 0;
}
