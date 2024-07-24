#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
  double huMoments1[7], huMoments2[7], dist=0, bestdist=10000000;
  
  cv::Mat image1 = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  cv::Mat image2 = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
  cv::Mat resultado = cv::imread(argv[2], cv::IMREAD_COLOR);
  int i=0, j=0, k=0, l=0, c=0;
  int w1 = image1.cols, h1 = image1.rows, w2 = image2.cols, h2 = image2.rows;

  cv::Moments moment1 = cv::moments(image1, false);
  
  cv::HuMoments(moment1, huMoments1);
  for ( i = 0; i < 7; i++) {
        huMoments1[i] = -1 * std::copysign(1.0, huMoments1[i]) * log10(abs(huMoments1[i]));
    }
  
  for(i=0; i <= w2-w1; i++){
    for(j=0; j <= h2-h1; j++){
        cv::Rect roi(i, j, w1, h1);
        cv::Mat regiao = image2(roi);
        cv::Moments moment2 = cv::moments(regiao, false);
        cv::HuMoments(moment2, huMoments2);
        for (k = 0; k < 7; k++) {
        huMoments2[k] = -1 * std::copysign(1.0, huMoments2[k]) * log10(abs(huMoments2[k]));
        }
        dist = 0;
        for(k=0; k<7; k++){
            dist += abs(huMoments1[k]-huMoments2[k]);
        }
        if(dist < bestdist){
            bestdist = dist;
            c=i;
            l=j;
        }
    }
  }

  cv::rectangle(resultado, cv::Point(c, l), cv::Point(c + w1, l + h1), cv::Scalar(0, 0, 255), 2);

  cv::imshow("Imagem", image1);
  cv::imshow("Imagem Resultado", resultado);
  cv::imwrite("ImagemResultado.jpg", resultado);
  cv::waitKey(0);
  
  return 0;
}

