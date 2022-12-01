#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

// 右クリックの状態（押された状態:1, 離した状態:0）
bool is_click = 0;

// グローバル座標
int global_x = 0, global_y = 0;

// パソコンサイズの黒画面
cv::Mat img(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));

// グローバル変数に座標を代入
void get_xy(int event, int x, int y, int flags, void* userdata)
{
    global_x = x;
    global_y = y;

    // 右クリック押下
    if (event == cv::EVENT_RBUTTONDOWN)
    {
        is_click = 1;
    }

    // 左クリック離す
    if (event == cv::EVENT_RBUTTONUP)
    {
        is_click = 0;
    }
}

int main() 
{

    // 表示するウィンドウに名前を付ける
    cv::namedWindow("Level1", cv::WINDOW_NORMAL);
    // フルスクリーン表示
    cv::setWindowProperty("Level1", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::setMouseCallback("Level1", get_xy);


    // qが押されるまで表示を継続
    while (1) 
    {
        cv::imshow("Level1", img);

        // 右クリックされている状態に点を描写
        if (is_click == 1)
        {
            cv::circle(img, cv::Point(global_x, global_y), 3, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);
            std::cout << "(" << global_x << ", " << global_y << ")" << std::endl;
        }

        // qボタンが押されたときwhileループから抜ける
        int key = cv::waitKey(1);
        if (key == 113) 
        {
            break;
        }
    }

    // ウィンドウを閉じる
    cv::destroyAllWindows();

    return 0;
}
