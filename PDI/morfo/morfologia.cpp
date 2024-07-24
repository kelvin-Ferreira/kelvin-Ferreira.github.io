

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Mat image, morfoImg, dilatacao;
  cv::Mat str;

  if (argc != 2) {
    std::cout << "morfologia entrada saida\n";
  }

  image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  cv::bitwise_not(image, image);

//  image = cv::imread(argv[1], -1);
  
  if(image.empty()) {
    std::cout << "Erro ao carregar a imagem: " << argv[1] << std::endl;
    return -1;
  }

  // elemento estruturante
  str = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 11));
  
  // dilatacao
  cv::dilate(image, dilatacao, str);
  cv::erode(dilatacao, morfoImg, str);
  cv::bitwise_not(image, image);
  cv::bitwise_not(morfoImg, morfoImg);

 cv::Mat matArray[] = {image, morfoImg};
  cv::hconcat(matArray, 2, image);
  cv::imshow("morfologia", image);

  cv::waitKey();
  return 0;
}

