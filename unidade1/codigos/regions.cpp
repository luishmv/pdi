#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char **argv)
{
  Mat image;
  Point P1, P2;

  image = imread(argv[1], IMREAD_GRAYSCALE);

  if (!image.data)
  {
    cout << "Erro ao abrir a imagem" << endl;
    return 0;
  }

  cout << "Dimensões da imagem: " << image.rows << "*" << image.cols << endl;

  cout << "Digite as coordenadas do primeiro ponto: " << endl;
  cout << "x:";
  cin >> P1.x;
  cout << "y:";
  cin >> P1.y;
  cout << "Digite as coordenadas do segundo ponto: " << endl;
  cout << "x entre " << P1.x  << " e " << image.rows << ":";
  cin >> P2.x;
  cout << "y entre " << P1.y << " e " << image.cols << ":";
  cin >> P2.y;

  for (int i = P1.x; i <= P2.x; i++)
  {
    for (int j = P1.y; j <= P2.y; j++)
    {
      image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
    }
  }

  imshow("Imagem com região negativada", image);
  imwrite("im_neg.png", image);
  waitKey();

  return 0;
}