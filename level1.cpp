#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

// 右クリックの状態（押された状態:true, 離した状態:false）
bool is_clicked = false;

// グローバル座標
int old_x = 0, old_y = 0, new_x = 0, new_y = 0;

// パソコンサイズの黒画面
cv::Mat img(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));

// 線を描写
void draw_line(int event, int x, int y, int flags, void* userdata)
{
    old_x = new_x;
    old_y = new_y;
    new_x = x;
    new_y = y;

    // 右クリック押す
    if (event == cv::EVENT_RBUTTONDOWN)
    {
        is_clicked = true;
    }

    // 右クリックされている状態に点を描写
    if (is_clicked == true)
    {
        cv::line(img, cv::Point(old_x, old_y), cv::Point(new_x, new_y), cv::Scalar(0, 255, 0), 10, cv::LINE_AA);
    }

    // 右クリック離す
    if (event == cv::EVENT_RBUTTONUP)
    {
        is_clicked = false;
    }
}

int main() 
{

    // 表示するウィンドウに名前を付ける
    cv::namedWindow("Level1", cv::WINDOW_NORMAL);
    // フルスクリーン表示
    cv::setWindowProperty("Level1", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::setMouseCallback("Level1", draw_line);

    // qが押されるまで表示を継続
    while (1)
    {
        cv::imshow("Level1", img);

        int key = cv::waitKey(1);

        // qキーが押されたときwhileループから抜ける(quit)
        if (key == 113)
        {
            break;
        }
        // rキーで全消し(reset)
        else if (key == 114)
        {
            cv::circle(img, cv::Point(new_x, new_y), 1920, cv::Scalar(0, 0, 0), -1, cv::LINE_AA);
        }
    }

    // ウィンドウを閉じる
    cv::destroyAllWindows();

    return 0;
}
