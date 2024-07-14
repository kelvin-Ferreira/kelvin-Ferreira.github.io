#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

void swapQuadrants(cv::Mat& image) {
    cv::Mat tmp, A, B, C, D;

    image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

    int centerX = image.cols / 2;
    int centerY = image.rows / 2;

    A = image(cv::Rect(0, 0, centerX, centerY));
    B = image(cv::Rect(centerX, 0, centerX, centerY));
    C = image(cv::Rect(0, centerY, centerX, centerY));
    D = image(cv::Rect(centerX, centerY, centerX, centerY));

    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Uso: " << argv[0] << " <arquivo.yml>" << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat image, padded, complexImage;
    std::vector<cv::Mat> planos;

    cv::FileStorage fs(argv[1], cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cout << "Erro abrindo arquivo " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    fs["mat"] >> image; 
    fs.release();

    if (image.empty()) {
        std::cout << "Erro: imagem vazia no arquivo " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Imagem carregada com sucesso: " << image.size() << std::endl;

    int dft_M = cv::getOptimalDFTSize(image.rows);
    int dft_N = cv::getOptimalDFTSize(image.cols);
    cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                       dft_N - image.cols, cv::BORDER_CONSTANT,
                       cv::Scalar::all(0));

    planos.push_back(cv::Mat_<float>(padded));

    planos.push_back(cv::Mat::zeros(padded.size(), CV_32F));

    cv::merge(planos, complexImage);

    cv::dft(complexImage, complexImage);
    swapQuadrants(complexImage);

    cv::split(complexImage, planos);

    cv::Mat magn, fase;
    cv::cartToPolar(planos[0], planos[1], magn, fase, false);
    cv::normalize(fase, fase, 0, 1, cv::NORM_MINMAX);

    cv::magnitude(planos[0], planos[1], magn);

    magn += cv::Scalar::all(1);

    cv::log(magn, magn);
    cv::normalize(magn, magn, 0, 1, cv::NORM_MINMAX);

    cv::imshow("Espectro de magnitude", magn);
    cv::imshow("Espectro de fase", fase);

    cv::waitKey();
    return EXIT_SUCCESS;
}
