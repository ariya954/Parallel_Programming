#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using std::cout;
using std::endl;

int main()
{
    cv::Mat img = cv::imread("lena.png");
    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
    cv::imshow("image", img);
    cv::waitKey(0);
    return 0;
}