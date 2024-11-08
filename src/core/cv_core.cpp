#include "cv_core.h"
#include <spdlog/spdlog.h>

namespace pcv
{
    /// @brief 缩放图像
    /// @param InOutMat 输入输出图像
    /// @param Ratio 缩放比例
    void resize(cv::Mat &InOutMat, double Ratio)
    {
        assert(!InOutMat.empty() && "Input image is empty");
        assert(Ratio > 0 && "Ratio must be positive");
        cv::resize(InOutMat, InOutMat, cv::Size(), Ratio, Ratio);
    }
    /// @brief 等比例缩放到指定大小
    /// @param InOutMat 输入输出图像
    /// @param Size 目标大小
    void resize(cv::Mat &InOutMat, const cv::Size &Size)
    {
        assert(!InOutMat.empty() && "Input image is empty");
        assert(Size.width > 0 && Size.height > 0 && "Size must be positive");

        int height = InOutMat.rows;
        int width = InOutMat.cols;
        double aspect_ratio_input = static_cast<double>(width) / height;
        double aspect_ratio_target = static_cast<double>(Size.width) / Size.height;
        if (aspect_ratio_input >= aspect_ratio_target)
        {
            int new_height = static_cast<int>(height * static_cast<double>(Size.width) / width);
            cv::resize(InOutMat, InOutMat, cv::Size(Size.width, new_height));
        }
        else
        {
            int new_width = static_cast<int>(width * static_cast<double>(Size.height) / height);
            cv::resize(InOutMat, InOutMat, cv::Size(new_width, Size.height));
        }
    }
    /// @brief 正方形缩放
    /// @param image 输入图像
    /// @param padded_image 输出图像
    /// @param pads 填充大小
    /// @param scale 缩放比例
    /// @param target_size 目标大小
    /// @param pad_color 填充颜色
    void letterbox(const cv::Mat &image, cv::Mat &padded_image, BOX_RECT &pads,
                   const cv::Size &target_size,
                   const cv::Scalar &pad_color)
    {
        float scale_w = (float)target_size.width / image.cols;
        float scale_h = (float)target_size.height / image.rows;
        float min_scale = std::min(scale_w, scale_h);

        cv::Mat resized_image;
        cv::resize(image, resized_image, cv::Size(), min_scale, min_scale);

        // 计算填充大小
        int pad_width = target_size.width - resized_image.cols;
        int pad_height = target_size.height - resized_image.rows;

        pads.left = pad_width / 2;
        pads.right = pad_width - pads.left;
        pads.top = pad_height / 2;
        pads.bottom = pad_height - pads.top;

        // 在图像周围添加填充
        cv::copyMakeBorder(resized_image, padded_image, pads.top, pads.bottom, pads.left, pads.right, cv::BORDER_CONSTANT, pad_color);
    }
    /// @brief 二值化图像
    /// @param GrayInMat 输入灰度图像
    /// @param OutMat 输出图像
    /// @param MinGray 区间灰度下限
    /// @param MaxGray 区间灰度上限
    void threshold(const cv::Mat &GrayInMat, cv::Mat &OutMat, double MinGray, double MaxGray)
    {
        assert(!GrayInMat.empty() && "Input image is empty");
        assert(GrayInMat.type() == CV_8UC1 && "Input image must be a grayscale image");
        assert(MinGray >= 0 && MinGray <= 255 && "MinGray must be in the range [0, 255]");
        assert(MaxGray >= 0 && MaxGray <= 255 && "MaxGray must be in the range [0, 255]");

        cv::Mat thresMin, thresMax;
        cv::threshold(GrayInMat, thresMin, MinGray, 255, cv::THRESH_BINARY_INV);
        cv::threshold(GrayInMat, thresMax, MaxGray, 255, cv::THRESH_BINARY);
        invertImage(thresMin + thresMax, OutMat);
    }
    /// @brief 反转图像像素
    /// @param InMat 输入图像
    /// @param OutMat 输出图像
    void invertImage(const cv::Mat &InMat, cv::Mat &OutMat)
    {
        if (InMat.channels() == 3)
        {
            OutMat = cv::Scalar(255, 255, 255) - InMat;
        }
        else if (InMat.channels() == 1)
        {
            OutMat = cv::Scalar(255) - InMat;
        }
    }
    /// @brief 灰度缩放
    /// @param GrayInMat 输入灰度图像
    /// @param OutMat 输出图像
    /// @param MinGray 区间灰度下限
    /// @param MaxGray 区间灰度上限
    void scaleImage(const cv::Mat &GrayInMat, cv::Mat &OutMat, double MinGray, double MaxGray)
    {
        assert(!GrayInMat.empty() && "Input image is empty");
        assert(GrayInMat.type() == CV_8UC1 && "Input image must be a grayscale image");
        assert(MinGray >= 0 && MinGray <= 255 && "MinGray must be in the range [0, 255]");
        assert(MaxGray >= 0 && MaxGray <= 255 && "MaxGray must be in the range [0, 255]");

        // 根据公式计算
        double mult = 255.0 / (MaxGray - MinGray);
        double add = -mult * MinGray;

        // 创建查找表
        std::array<uchar, 256> lut;
        for (int i = 0; i < 256; ++i)
        {
            lut[i] = cv::saturate_cast<uchar>(i * mult + add);
        }

        // 应用查找表
        cv::Mat Lut(1, 256, CV_8UC1, lut.data());
        cv::LUT(GrayInMat, Lut, OutMat);
    }

    /// @brief 灰度映射
    /// @param GrayInOutMat 输入输出灰度图像
    /// @param MaxGrayLevel 最大灰度值
    /// @param IsClosed 是否封闭区间
    void zoomGray(cv::Mat &GrayInOutMat, int MaxGrayLevel, bool IsClosed)
    {
        assert(!GrayInOutMat.empty() && "Input image is empty");
        assert(GrayInOutMat.type() == CV_8UC1 && "Input image must be a grayscale image");
        assert(MaxGrayLevel >= 1 && MaxGrayLevel <= 256 && "MaxGrayLevel must be in the range [1, 256]");
        GrayInOutMat.convertTo(GrayInOutMat, CV_32F);
        if (IsClosed)
        {
            GrayInOutMat = ((MaxGrayLevel - 1.0f) / 255.0f * GrayInOutMat) + 1.0f;
        }
        else
        {
            GrayInOutMat = ((MaxGrayLevel - 1.0f) / 255.0f * GrayInOutMat);
        }
        GrayInOutMat.convertTo(GrayInOutMat, CV_8U);
    }

    /// @brief 伽马校正
    /// @param InMat 输入图像
    /// @param OutMat 输出图像
    /// @param Gamma 伽马值
    void gammaImage(const cv::Mat &InMat, cv::Mat &OutMat, float Gamma)
    {
        assert(!InMat.empty() && "Input image is empty");

        // 创建查找表
        std::array<uchar, 256> lut;
        for (int i = 0; i < 256; ++i)
        {
            lut[i] = cv::saturate_cast<uchar>(std::pow(static_cast<float>(i) / 255.0f, 1.0f / Gamma) * 255.0f);
        }

        // 应用查找表
        cv::Mat Lut(1, 256, CV_8UC1, lut.data());
        cv::LUT(InMat, Lut, OutMat);
    }

    /// @brief 自动伽马校正
    /// @param InMat 输入图像
    /// @param OutMat 输出图像
    /// @param C 目标平均灰度值[0-1]
    void autoGammaImage(const cv::Mat &GrayInMat, cv::Mat &OutMat, float C)
    {
        assert(!GrayInMat.empty() && "Input image is empty");
        assert(GrayInMat.type() == CV_8UC1 && "Input image must be a grayscale image");
        assert(C >= 0 && C <= 1 && "C must be in the range [0, 1]");

        auto meanGray = cv::mean(GrayInMat)[0];
        float gamma_val = static_cast<float>(log10(1 - C) / log10(1 - meanGray / 255.0f)); // 自动gamma参数
        gammaImage(GrayInMat, OutMat, gamma_val);
    }

    /// @brief 线性灰度变换
    /// @param GrayInMat 输入灰度图像
    /// @param OutMat 输出图像
    /// @param Th1
    /// @param Th2
    /// @param Goal1
    /// @param Goal2
    void linearGrayLevelTrans(const cv::Mat &GrayInMat, cv::Mat &OutMat, int Th1, int Th2, int Goal1, int Goal2)
    {
        assert(!GrayInMat.empty() && "Input image is empty");
        assert(GrayInMat.type() == CV_8UC1 && "Input image must be a grayscale image");
        assert(Th1 >= 0 && Th1 <= 255 && "Th1 must be in the range [0, 255]");
        assert(Th2 >= 0 && Th2 <= 255 && "Th2 must be in the range [0, 255]");
        assert(Goal1 >= 0 && Goal1 <= 255 && "Goal1 must be in the range [0, 255]");
        assert(Goal2 >= 0 && Goal2 <= 255 && "Goal2 must be in the range [0, 255]");

        cv::Mat temp;
        GrayInMat.convertTo(temp, CV_32F);

        for (int i = 0; i < temp.rows; i++)
        {
            for (int j = 0; j < temp.cols; j++)
            {
                if (temp.at<float>(i, j) <= Th1)
                    temp.at<float>(i, j) = temp.at<float>(i, j) * Goal1 / Th1;
                else if (temp.at<float>(i, j) > Th1 && temp.at<float>(i, j) <= Th2)
                    temp.at<float>(i, j) = (temp.at<float>(i, j) - Th1) * (Goal2 - Goal1) / (Th2 - Th1) + Goal1;
                else
                    temp.at<float>(i, j) = (temp.at<float>(i, j) - Th2) * (255 - Goal2) / (255 - Th2) + Goal2;
            }
        }
        temp.convertTo(OutMat, CV_8U);
    }

    /// @brief 线性变换
    /// @param InMat 输入图像
    /// @param OutMat 输出图像
    /// @param Th1
    /// @param Th2
    /// @param Goal1
    /// @param Goal2
    void linearLevelTrans(const cv::Mat &InMat, cv::Mat &OutMat, int Th1, int Th2, int Goal1, int Goal2)
    {
        assert(!InMat.empty() && "Input image is empty");
        assert(Th1 >= 0 && Th1 <= 255 && "Th1 must be in the range [0, 255]");
        assert(Th2 >= 0 && Th2 <= 255 && "Th2 must be in the range [0, 255]");
        assert(Goal1 >= 0 && Goal1 <= 255 && "Goal1 must be in the range [0, 255]");
        assert(Goal2 >= 0 && Goal2 <= 255 && "Goal2 must be in the range [0, 255]");

        cv::Mat temp;
        InMat.convertTo(temp, CV_32F);

        for (int i = 0; i < temp.rows; i++)
        {
            for (int j = 0; j < temp.cols; j++)
            {
                for (int c = 0; c < 3; c++)
                {
                    float &pixel = temp.at<cv::Vec3f>(i, j)[c];
                    if (pixel <= Th1)
                        pixel = pixel * Goal1 / Th1;
                    else if (pixel > Th1 && pixel <= Th2)
                        pixel = (pixel - Th1) * (Goal2 - Goal1) / (Th2 - Th1) + Goal1;
                    else
                        pixel = (pixel - Th2) * (255 - Goal2) / (255 - Th2) + Goal2;
                }
            }
        }

        temp.convertTo(OutMat, CV_8U);
    }

    /// @brief 对数变换
    /// @param InMat 输入图像
    /// @param OutMat 输出图像
    /// @param C 对数变换参数
    void logImage(const cv::Mat &InMat, cv::Mat &OutMat, float C)
    {
        assert(!InMat.empty() && "Input image is empty");
        std::array<uchar, 256> lut;
        for (int i = 0; i < 256; i++)
        {
            lut[i] = cv::saturate_cast<uchar>(C * log(1 + i));
        }
        cv::Mat Lut(1, 256, CV_8UC1, lut.data());
        cv::LUT(InMat, Lut, OutMat);
    }

    /// @brief 直方图均衡
    /// @param InMat 输入图像bgr
    /// @param OutMat 输出图像
    void equalizeColor(const cv::Mat &InMat, cv::Mat &OutMat, double ClipLimit)
    {
        assert(!InMat.empty() && "Input image is empty");
        assert(InMat.channels() == 3 && "Input image must be a color image");

        // 将图像从 BGR 转换为 Lab 色彩空间
        cv::Mat Lab;
        cv::cvtColor(InMat, Lab, cv::COLOR_BGR2Lab);

        // 分离 Lab 图像的通道
        std::vector<cv::Mat> channels(3);
        cv::split(Lab, channels);

        // 创建 CLAHE 对象并设置剪切限制
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(ClipLimit);

        // 对 L 通道应用 CLAHE
        clahe->apply(channels[0], channels[0]);

        // 合并通道并转换回 BGR 色彩空间
        cv::merge(channels, Lab);
        cv::cvtColor(Lab, OutMat, cv::COLOR_Lab2BGR);
    }

    /// @brief 多重双边滤波
    /// @param InMat 输入图像
    /// @param OutMat 输出图像
    /// @param Iter 迭代次数
    /// @param D Diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
    /// @param SColor Filter sigma in the color space. A larger value of the parameter means that farther colors within the pixel neighborhood (see sigmaSpace) will be mixed together, resulting in larger areas of semi-equal color.
    /// @param SSpace Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other as long as their colors are close enough (see sigmaColor ). When d>0, it specifies the neighborhood size regardless of sigmaSpace. Otherwise, d is proportional to sigmaSpace.
    void bilateralFilter(const cv::Mat &InMat, cv::Mat &OutMat, int Iter, int D, int SColor, int SSpace)
    {
        assert(!InMat.empty() && "Input image is empty");

        cv::Mat bilateral, bilateral_temp;
        cv::bilateralFilter(InMat, bilateral, D, SColor, SSpace);
        bilateral_temp = bilateral.clone();
        bilateral.release();
        for (int i = 0; i < Iter - 1; i++)
        {
            cv::bilateralFilter(bilateral_temp, bilateral, D, SColor, SSpace);
            bilateral_temp = bilateral.clone();
            bilateral.release();
        }
        OutMat = bilateral_temp.clone();
    }

    /// @brief Gabor 滤波
    /// @param GrayInMat 输入灰度图像
    /// @param OutMat 输出图像
    /// @param KernelSize Gabor 核大小
    /// @param Sigma Gabor 核的标准差
    /// @param Theta Gabor 核的角度
    /// @param Lambd Gabor 核的波长
    /// @param Gamma Gabor 核的空间纵横比
    /// @param Psi Gabor 核的相位偏移
    void gaborFilter(const cv::Mat &GrayInMat, cv::Mat &OutMat, int KernelSize,
                     double Sigma, double Theta, double Lambd,
                     double Gamma, double Psi)
    {
        assert(!GrayInMat.empty() && "Input image is empty");
        assert(GrayInMat.type() == CV_8UC1 && "Input image must be a grayscale image");

        cv::Mat temp;
        GrayInMat.convertTo(temp, CV_32F);
        cv::Mat kernel = cv::getGaborKernel(cv::Size(KernelSize, KernelSize), Sigma, Theta, Lambd, Gamma, Psi, CV_32F);
        filter2D(temp, OutMat, CV_32F, kernel);                 // 在频域滤波，有负数
        cv::normalize(OutMat, OutMat, 1.0, 0, cv::NORM_MINMAX); // 缩放一下
        cv::convertScaleAbs(OutMat, OutMat, 255);               // 映射到【0~255】8U
    }

    /// @brief LBP 纹理特征提取
    /// @param GrayInMat 输入灰度图像
    /// @param OutMat 输出图像
    void LBP(const cv::Mat &GrayInMat, cv::Mat &OutMat)
    {
        assert(!GrayInMat.empty() && "Input image is empty");
        assert(GrayInMat.type() == CV_8UC1 && "Input image must be a grayscale image");

        cv::Mat temp = cv::Mat::zeros(GrayInMat.size(), GrayInMat.type());
        cv::Mat InMat;
        cv::copyMakeBorder(GrayInMat, InMat, 0, 2, 0, 2, cv::BORDER_CONSTANT, cv::Scalar::all(0));
        for (int i = 1; i < InMat.rows - 1; i++)
        {
            for (int j = 1; j < InMat.cols - 1; j++)
            {
                uchar center = InMat.at<uchar>(i, j);
                uchar code = 0;
                code |= (InMat.at<uchar>(i - 1, j - 1) >= center) << 7;
                code |= (InMat.at<uchar>(i - 1, j) >= center) << 6;
                code |= (InMat.at<uchar>(i - 1, j + 1) >= center) << 5;
                code |= (InMat.at<uchar>(i, j + 1) >= center) << 4;
                code |= (InMat.at<uchar>(i + 1, j + 1) >= center) << 3;
                code |= (InMat.at<uchar>(i + 1, j) >= center) << 2;
                code |= (InMat.at<uchar>(i + 1, j - 1) >= center) << 1;
                code |= (InMat.at<uchar>(i, j - 1) >= center) << 0;
                temp.at<uchar>(i - 1, j - 1) = code;
            }
        }
        OutMat = temp.clone();
    }

} // namespace pcv