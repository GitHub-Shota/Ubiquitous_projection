#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
Mat img(Size(1920, 1080), CV_8UC3, Scalar(0, 0, 0));

void mouse_callback(int event, int x, int y, int flags, void* userdata){
    if (event == EVENT_MOUSEMOVE) {
        cv::circle(img, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);
        cout << "(" << x << ", " << y << ")" << endl;
        imshow("example", img);
        waitKey();
    }
}

int main()
{
   
    imshow("example", img);
    setMouseCallback("example", mouse_callback);
    waitKey();

    return 0;
}
