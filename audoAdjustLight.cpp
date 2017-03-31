#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
using namespace cv;

void MyAutoAdjustBright(Mat& img)
{
    if (img.empty())
    {
        cerr << "Oh no! The input image is empty!" << endl;
        return;
    }
    if (img.channels() != 3)
    {
        cerr << "Oh no! The input image only allows 3-channels BGR image!" << endl;
        return;
    }
    // abstarct Y
    cv::cvtColor(img, img, COLOR_BGR2YCrCb);
    vector<Mat> yCrCb(img.channels());
    cv::split(img, yCrCb);
    Mat Y = yCrCb[0];

    // construct lut
    int dim(256);
    Mat lut(1, &dim, CV_8U);
    double afa = 0.0, yhdrv = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (i < 128) {
            afa = 1.0*i / (255 - i);
        }
        else {
            afa = 1.0*(255 - i) / i;
        }
        yhdrv = (1 - afa)*i + afa*(255 - i);
        lut.at<uchar>(i) = static_cast<uchar>(yhdrv);
    }

    Mat Yhdr;
    cv::LUT(Y, lut, Yhdr);

    // Setting Y channel to Yhdr.
    yCrCb[0] = Yhdr;
    // Merge vector<Mat> yCrCb to img(YCrYb format)
    cv::merge(yCrCb, img);
    // convert YCrCb to BGR format
    cv::cvtColor(img, img, COLOR_YCrCb2BGR);
}
