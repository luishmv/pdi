#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

#define Max_size_name 51

using namespace cv;
using namespace std;

int top_slider_max = 100;
int h_slider = 0, v_slider = 0, p_slider = 0;

int width, height; // dimensões da imagem

Mat image, blurry_image, blended;

char TrackbarName[Max_size_name]; // nome das labels
float l1 = 0, l2 = 0;             // linhas de mudança

void on_trackbar_blend(int, void *);
void on_trackbar_line(int, void *);
float tiltShift(const int pos);

int main(int argvc, char **argv)
{

  image = imread(argv[1]);

  if (!image.data)
  {
    cout << "Erro ao abrir a imagem" << endl;
    return 0;
  }

  width = image.rows;
  height = image.cols;

  float media[] = {0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0};
  Mat mask = Mat(5, 5, CV_32F, media);
  scaleAdd(mask, 1 / 9.0, Mat::zeros(5, 5, CV_32F), mask);
  filter2D(image, blurry_image, image.depth(), mask, Point(2, 2), 0);

  namedWindow("addweighted", 1);

  sprintf(TrackbarName, "Altura:");
  createTrackbar(TrackbarName, "addweighted", &h_slider, top_slider_max, on_trackbar_line);

  sprintf(TrackbarName, "Decaimento:");
  createTrackbar(TrackbarName, "addweighted", &v_slider, top_slider_max, on_trackbar_line);

  sprintf(TrackbarName, "Posicão:");
  createTrackbar(TrackbarName, "addweighted", &p_slider, top_slider_max, on_trackbar_line);

  on_trackbar_line(0, 0);

  waitKey(0);

  imwrite("./img_final.jpg", blended);
  return 0;
}

void on_trackbar_line(int, void *)
{
  // fazer atribuições a l1,l2 e d apartir dos dados coletados por '*_slider'
  float h_focus = ((h_slider / 100.0) * width) / 2.0;
  float p_real = ((p_slider / 100.0) * width);
  l1 = p_real - h_focus;
  l2 = p_real + h_focus;
  on_trackbar_blend(0, 0);
}

void on_trackbar_blend(int, void *)
{
  image.convertTo(image, CV_32F);
  blurry_image.convertTo(blurry_image, CV_32F);
  blurry_image.convertTo(blended, CV_32F);

  for (int i = 0; i < width; i++)
  {
    float pix = tiltShift(i);
    for (int j = 0; j < 3 * height; j++)
    {
      blended.at<float>(i, j) = image.at<float>(i, j) * pix + blurry_image.at<float>(i, j) * (1 - pix);
    }
  }

  blended.convertTo(blended, CV_8U);
  imshow("addweighted", blended);
}

float tiltShift(const int pos)
{
  return 0.5 * (tanh((pos - l1) / v_slider) - tanh((pos - l2) / v_slider));
}