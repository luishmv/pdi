#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image, imgModificada;
  int i, j;
  
  image = imread("cat.jpg", IMREAD_GRAYSCALE);
  
  if(!image.data) 
    cout << "nao abriu cat.jpg" << endl;
    
  int width = image.size().width;
  int height = image.size().height;

  namedWindow("imagem original", WINDOW_AUTOSIZE);
  namedWindow("imagem modificada", WINDOW_AUTOSIZE);
  
  image.copyTo(imgModificada);
  
  //Primeiro Quadrante - (y, x): (0, 0) -> (width/2, height/2)
  //Segundo Quadrante - (y, x): (width/2, 0) -> (width, height/2)
  //Terceiro Quadrante - (y, x): (0, height/2) -> (width/2, height)
  //Quarto Quadrante - (y, x): (width/2, height/2) -> (width, height)
  
  //for utilizado para trocar o primeiro e quarto quadrante
  for(i = 0; i < width/2; i++){
    for(j = 0; j < height/2; j++){
	imgModificada.at<uchar>(i, j) = image.at<uchar>(i + width/2, j + height/2);
	imgModificada.at<uchar>(i + width/2, j + height/2) = image.at<uchar>(i,j);
    }
  }

  //for utilizado para trocar o segundo e terceiro quadrante
  for(i = width/2; i < width; i++){
      for(j = 0; j < height/2; j++){
        imgModificada.at<uchar>(i, j) = image.at<uchar>(i - width/2, j + height/2);
        imgModificada.at<uchar>(i - width/2, j + height/2) = image.at<uchar>(i, j);
    }
  }
  
  imshow("imagem original ", image);
  imshow("imagem modificada", imgModificada); 
  imwrite("imgModificadaQ2.jpg", imgModificada);
   
  waitKey();
  return 0;
}
