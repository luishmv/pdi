#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat image;
    int width, height;
    int nbolhas_com_buracos = 0, nbolhas_sem_buracos = 0;

    image = imread(argv[1], IMREAD_GRAYSCALE);

    if (!image.data)
    {
        cout << "Erro ao abrir a imagem" << endl;
        return 0;
    }
    imshow("Original", image);
    width = image.size().width;
    height = image.size().height;

    //remove da borda superior e inferior
    for (int i = 0; i < width; i++)
    {
        if (image.at<uint8_t>(0, i) == 255)
            floodFill(image, Point(i, 0), 0);
        if (image.at<uint8_t>(height - 1, i) == 255)
            floodFill(image, Point(i, height - 1), 0);
    }

    //remove das laterais
    for (int i = 0; i < height; i++)
    {
        //lateral esquerda
        if (image.at<uint8_t>(i, 0) == 255)
            floodFill(image, Point(0, i), 0);
        //lateral direita
        if (image.at<uint8_t>(i, width - 1) == 255)
            floodFill(image, Point(width - 1, i), 0);
    }
    imshow("Passo 1", image);
    imwrite("passo1.png", image);

    //troca o background, para facilitar a identificar os buracos das bolhas
    floodFill(image, cv::Point(0, 0), 127);
    imshow("Passo 2", image);
    imwrite("passo2.png", image);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            //identifica uma bolha com buraco
            if (image.at<uint8_t>(i, j) == 0 && image.at<uint8_t>(i, j - 1) == 255)
            {
                //soma um no numero de bolhas e "apaga" a bolha encontrada
                nbolhas_com_buracos++;
                floodFill(image, Point(j - 1, i), 127);
            }
        }

    imshow("Passo 3", image);
    imwrite("passo3.png", image);
    //conta bolhas sem buracos
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            //identifica uma bolha
            if (image.at<uint8_t>(i, j) == 255)
            {
                //soma um no numero de bolhas sem buracos e "apaga" a bolha encontrada
                nbolhas_sem_buracos++;
                floodFill(image, Point(j, i), 127);
            }
        }

    cout << "Bolhas sem buracos:" << nbolhas_sem_buracos << '\n';
    cout << "Bolhas com buracos:" << nbolhas_com_buracos << '\n';

    imshow("quest3/Resultado final", image);
    imwrite("final.png", image);
    waitKey();
    return 0;
}