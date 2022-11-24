
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_MOUSEMOVE) {
        cout << "(" << x << ", " << y << ")" << endl;
    }
}

int main()
{
    Mat img(Size(1920,1080),CV_8UC3,Scalar(0,0,0) );
    imshow("example", img);
    setMouseCallback("example", mouse_callback);
    waitKey();

    return 0;
}
