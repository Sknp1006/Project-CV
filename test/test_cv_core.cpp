#include <gtest/gtest.h>
#include "cv_core.h"

namespace pcv
{

TEST(CvCoreTest, ResizeByRatio)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    double ratio = 0.5;
    cv::Mat resized_image = image.clone();
    resize(resized_image, ratio);

    EXPECT_EQ(resized_image.cols, static_cast<int>(image.cols * ratio));
    EXPECT_EQ(resized_image.rows, static_cast<int>(image.rows * ratio));

    cv::imwrite("ResizeByRatio.jpg", resized_image);
}

TEST(CvCoreTest, ResizeBySize)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Size new_size(640, 640);
    cv::Mat resized_image = image.clone();
    resize(resized_image, new_size);

    cv::imwrite("ResizeBySize.jpg", resized_image);
}

TEST(CvCoreTest, Letterbox)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat padded_image;
    BOX_RECT pads;
    cv::Size target_size(640, 640);

    letterbox(image, padded_image, pads, target_size);

    cv::imwrite("Letterbox.jpg", padded_image);
}

TEST(CvCoreTest, Threshold)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    cv::Mat thresholded_image;
    double min_gray = 0;
    double max_gray = 100;

    threshold(gray_image, thresholded_image, min_gray, max_gray);

    cv::imwrite("Threshold.jpg", thresholded_image);
}

TEST(CvCoreTest, InvertImage)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat inverted_image;
    invertImage(image, inverted_image);

    EXPECT_EQ(inverted_image.at<uchar>(0, 0), 255 - image.at<uchar>(0, 0));

    cv::imwrite("InvertImage.jpg", inverted_image);
}

TEST(CvCoreTest, ScaleImage)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat scaled_image;
    double min_gray = 50;
    double max_gray = 100;

    scaleImage(image, scaled_image, min_gray, max_gray);

    cv::imwrite("ScaleImage.jpg", scaled_image);
}

TEST(CvCoreTest, ZoomGray)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    int max_gray_level = 16;
    cv::Mat zoomed_image = image.clone();
    zoomGray(zoomed_image, max_gray_level);

    double min_val, max_val;
    cv::minMaxLoc(zoomed_image, &min_val, &max_val);

    EXPECT_LE(max_val, max_gray_level);

    cv::imwrite("ZoomGray.jpg", zoomed_image);
}

TEST(CvCoreTest, GammaImage)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat gamma_image;
    float gamma = 2.0;

    gammaImage(image, gamma_image, gamma);

    cv::imwrite("GammaImage.jpg", gamma_image);
}

TEST(CvCoreTest, AutoGammaImage)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat auto_gamma_image;
    float C = 0.4;

    autoGammaImage(image, auto_gamma_image, C);

    cv::imwrite("AutoGammaImage.jpg", auto_gamma_image);
}

TEST(CvCoreTest, LinearGrayLevelTrans)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat transformed_image;
    int Th1 = 50, Th2 = 200, Goal1 = 0, Goal2 = 255;

    linearGrayLevelTrans(image, transformed_image, Th1, Th2, Goal1, Goal2);

    cv::imwrite("LinearGrayLevelTrans.jpg", transformed_image);
}

TEST(CvCoreTest, LinearLevelTrans)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat transformed_image;
    int Th1 = 50, Th2 = 200, Goal1 = 0, Goal2 = 255;

    linearLevelTrans(image, transformed_image, Th1, Th2, Goal1, Goal2);

    cv::imwrite("LinearLevelTrans.jpg", transformed_image);
}

TEST(CvCoreTest, LogImage)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat log_image;
    float C = 20;

    logImage(image, log_image, C);

    cv::imwrite("LogImage.jpg", log_image);
}

TEST(CvCoreTest, EqualizeColor)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat equalized_image;
    double clip_limit = 2.0;

    equalizeColor(image, equalized_image, clip_limit);

    cv::imwrite("EqualizeColor.jpg", equalized_image);
}

TEST(CvCoreTest, BilateralFilter)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat filtered_image;
    int iter = 3, d = 9, s_color = 75, s_space = 75;

    bilateralFilter(image, filtered_image, iter, d, s_color, s_space);

    cv::imwrite("BilateralFilter.jpg", filtered_image);
}

TEST(CvCoreTest, GaborFilter)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat gabor_image;
    int kernel_size = 21;
    double sigma = 5.0, theta = CV_PI / 4, lambd = 10.0, gamma = 0.5, psi = CV_PI / 2;

    gaborFilter(image, gabor_image, kernel_size, sigma, theta, lambd, gamma, psi);

    cv::imwrite("GaborFilter.jpg", gabor_image);
}

} // namespace pcv

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}