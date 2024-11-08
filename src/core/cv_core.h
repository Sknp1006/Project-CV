#ifndef H_PCV_CORE
#define H_PCV_CORE

#include <opencv2/opencv.hpp>

namespace pcv
{
typedef struct _BOX_RECT
{
    int left;
    int right;
    int top;
    int bottom;
} BOX_RECT;


void resize(cv::Mat &InOutMat, double Ratio);

void resize(cv::Mat &InOutMat, const cv::Size& Size);

void letterbox(const cv::Mat &image, cv::Mat &padded_image, BOX_RECT &pads, 
                const cv::Size &target_size, 
                const cv::Scalar &pad_color = cv::Scalar(128, 128, 128));

void threshold(const cv::Mat &GrayInMat, cv::Mat &OutMat, double MinGray, double MaxGray);

void invertImage(const cv::Mat &InMat, cv::Mat &OutMat);

void scaleImage(const cv::Mat &GrayInMat, cv::Mat &OutMat, double MinGray, double MaxGray);

void zoomGray(cv::Mat &GrayInOutMat, int MaxGrayLevel, bool IsClosed = true);

void gammaImage(const cv::Mat &InMat, cv::Mat &OutMat, float Gamma);

void autoGammaImage(const cv::Mat &GrayInMat, cv::Mat &OutMat, float C);

void linearGrayLevelTrans(const cv::Mat &GrayInMat, cv::Mat &OutMat, int Th1, int Th2, int Goal1, int Goal2);

void linearLevelTrans(const cv::Mat &InMat, cv::Mat &OutMat, int Th1, int Th2, int Goal1, int Goal2);

void logImage(const cv::Mat &InMat, cv::Mat &OutMat, float C);

void equalizeColor(const cv::Mat &InMat, cv::Mat &OutMat, double ClipLimit = 2.0);

void bilateralFilter(const cv::Mat& InMat, cv::Mat& OutMat, int Iter, int D, int SColor, int SSpace);

void gaborFilter(const cv::Mat &GrayInMat, cv::Mat &OutMat, int KernelSize, 
                    double Sigma, double Theta, double Lambd, 
                    double Gamma, double Psi);

void LBP(const cv::Mat &GrayInMat, cv::Mat &OutMat);
}; // namespace pcv

#endif // H_PCV_CORE