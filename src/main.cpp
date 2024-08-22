#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main()
{
    cv::VideoCapture cam(0);

    if (!cam.isOpened())
    {
        std::cout << "Couldn't open the camera.\n";
        return 1;
    }

    cv::Mat frame;
    cam >> frame;

    if (frame.empty())
    {
        std::cout << "Frame was empty.\n";
        return 1;
    }

    const char* window = "Show Me";
    namedWindow(window, cv::WINDOW_AUTOSIZE);
    imshow(window, frame);
    cv::waitKey(0);
}
