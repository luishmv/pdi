#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    Mat imageRGB, imageGray;
    VideoCapture cap;

    Mat histogram, histogramBefore;
    int nbins = 64;
    float range[] = {0, 256};
    const float *histrange = {range};
    int key;
    bool uniform = true;
    bool acummulate = false;
    float correlation;

    cap.open(0);

    if (!cap.isOpened())
    {
        cout << "cameras indisponiveis";
        return -1;
    }

    bool c = 0;

    while (1)
    {
        cap >> imageRGB;
        cvtColor(imageRGB, imageGray, COLOR_BGR2GRAY);

        calcHist(&imageGray, 1, 0, Mat(), histogram, 1,
                 &nbins, &histrange,
                 uniform, acummulate);

        if (c == 0)
        {
            histogramBefore = histogram.clone();
            c = 1;
        }

        correlation = compareHist(histogram, histogramBefore, HISTCMP_CORREL);

        if (correlation < 0.9)
        {
            putText(imageGray, "MOVIMENTO DETECTADO!", Point(1, 100),
                    FONT_HERSHEY_SIMPLEX, .7,
                    Scalar(0, 0, 255, 255), 2);
        }

        histogramBefore = histogram.clone();
        imshow("Imagem", imageGray);
        key = waitKey(30);
    }

    return 0;
}