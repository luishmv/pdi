#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{

    Mat image, image_eq, hist, hist_eq;
    VideoCapture cap;
    int nbins = 64;
    float range[] = {0, 256};
    const float *histrange = {range};
    int key;

    bool uniform = true;
    bool acummulate = false;

    cap.open(0);

    if (!cap.isOpened())
    {
        cout << "cameras indisponiveis";
        return -1;
    }

    int histw = nbins, histh = nbins / 2;
    Mat histImg(histh, histw, CV_8UC1, Scalar(0, 0, 0));
    Mat histImg_eq(histh, histw, CV_8UC1, Scalar(0, 0, 0));

    while (true)
    {
        cap >> image;
        cvtColor(image, image, COLOR_BGR2GRAY);
        equalizeHist(image, image_eq);

        calcHist(&image, 1, 0, Mat(), hist, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        calcHist(&image_eq, 1, 0, Mat(), hist_eq, 1,
                 &nbins, &histrange,
                 uniform, acummulate);

        normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
        normalize(hist_eq, hist_eq, 0, histImg_eq.rows, NORM_MINMAX, -1,
                  Mat());

        histImg.setTo(Scalar(0));
        histImg_eq.setTo(Scalar(0));

        for (int i = 0; i < nbins; i++)
        {
            line(histImg,
                 Point(i, histh),
                 Point(i, histh - cvRound(hist.at<float>(i))),
                 Scalar(255, 255, 255), 1, 8, 0);
            line(histImg_eq,
                 Point(i, histh),
                 Point(i, histh - cvRound(hist_eq.at<float>(i))),
                 Scalar(255, 255, 255), 1, 8, 0);
        }

        histImg.copyTo(image(Rect(0, 0, nbins, histh)));
        histImg_eq.copyTo(image_eq(Rect(0, 0, nbins, histh)));

        imshow("Imagem Original", image);
        imshow("Imagem Equalizada", image_eq);
        imwrite("imagemOriginal.png", image);
        imwrite("imagemEqualizada.png", image_eq);
        key = waitKey(30);
    }
    return 0;
}