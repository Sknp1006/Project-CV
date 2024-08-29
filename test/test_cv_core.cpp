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
}

TEST(CvCoreTest, ResizeBySize)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Size new_size(100, 100);
    cv::Mat resized_image = image.clone();
    resize(resized_image, new_size);

    EXPECT_EQ(resized_image.cols, new_size.width);
    EXPECT_EQ(resized_image.rows, new_size.height);
}

TEST(CvCoreTest, Letterbox)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat padded_image;
    BOX_RECT pads;
    float scale = 1.0;
    cv::Size target_size(200, 200);
    cv::Scalar pad_color(128, 128, 128);

    letterbox(image, padded_image, pads, scale, target_size, pad_color);

    EXPECT_EQ(padded_image.cols, target_size.width);
    EXPECT_EQ(padded_image.rows, target_size.height);
}

TEST(CvCoreTest, Threshold)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat thresholded_image;
    double min_gray = 100;
    double max_gray = 200;

    threshold(image, thresholded_image, min_gray, max_gray);

    double min_val, max_val;
    cv::minMaxLoc(thresholded_image, &min_val, &max_val);

    EXPECT_GE(min_val, min_gray);
    EXPECT_LE(max_val, max_gray);
}

TEST(CvCoreTest, InvertImage)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat inverted_image;
    invertImage(image, inverted_image);

    EXPECT_EQ(inverted_image.at<uchar>(0, 0), 255 - image.at<uchar>(0, 0));
}

TEST(CvCoreTest, ScaleImage)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat scaled_image;
    double min_gray = 50;
    double max_gray = 200;

    scaleImage(image, scaled_image, min_gray, max_gray);

    double min_val, max_val;
    cv::minMaxLoc(scaled_image, &min_val, &max_val);

    EXPECT_GE(min_val, min_gray);
    EXPECT_LE(max_val, max_gray);
}

TEST(CvCoreTest, ZoomGray)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    int max_gray_level = 128;
    cv::Mat zoomed_image = image.clone();
    zoomGray(zoomed_image, max_gray_level);

    double min_val, max_val;
    cv::minMaxLoc(zoomed_image, &min_val, &max_val);

    EXPECT_LE(max_val, max_gray_level);
}

TEST(CvCoreTest, GammaImage)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat gamma_image;
    float gamma = 2.0;

    gammaImage(image, gamma_image, gamma);

    // Check if the transformation is applied (not a precise check)
    EXPECT_NE(image.at<uchar>(0, 0), gamma_image.at<uchar>(0, 0));
}

TEST(CvCoreTest, AutoGammaImage)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat auto_gamma_image;
    float C = 1.0;

    autoGammaImage(image, auto_gamma_image, C);

    // Check if the transformation is applied (not a precise check)
    EXPECT_NE(image.at<uchar>(0, 0), auto_gamma_image.at<uchar>(0, 0));
}

TEST(CvCoreTest, LinearGrayLevelTrans)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat transformed_image;
    int Th1 = 50, Th2 = 200, Goal1 = 0, Goal2 = 255;

    linearGrayLevelTrans(image, transformed_image, Th1, Th2, Goal1, Goal2);

    double min_val, max_val;
    cv::minMaxLoc(transformed_image, &min_val, &max_val);

    EXPECT_GE(min_val, Goal1);
    EXPECT_LE(max_val, Goal2);
}

TEST(CvCoreTest, LinearLevelTrans)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat transformed_image;
    int Th1 = 50, Th2 = 200, Goal1 = 0, Goal2 = 255;

    linearLevelTrans(image, transformed_image, Th1, Th2, Goal1, Goal2);

    double min_val, max_val;
    cv::minMaxLoc(transformed_image, &min_val, &max_val);

    EXPECT_GE(min_val, Goal1);
    EXPECT_LE(max_val, Goal2);
}

TEST(CvCoreTest, LogImage)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat log_image;
    float C = 1.0;

    logImage(image, log_image, C);

    // Check if the transformation is applied (not a precise check)
    EXPECT_NE(image.at<uchar>(0, 0), log_image.at<uchar>(0, 0));
}

TEST(CvCoreTest, EqualizeColor)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat equalized_image;
    double clip_limit = 2.0;

    equalizeColor(image, equalized_image, clip_limit);

    // Check if the transformation is applied (not a precise check)
    EXPECT_NE(image.at<cv::Vec3b>(0, 0), equalized_image.at<cv::Vec3b>(0, 0));
}

TEST(CvCoreTest, BilateralFilter)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat filtered_image;
    int iter = 3, d = 9, s_color = 75, s_space = 75;

    bilateralFilter(image, filtered_image, iter, d, s_color, s_space);

    // Check if the transformation is applied (not a precise check)
    EXPECT_NE(image.at<cv::Vec3b>(0, 0), filtered_image.at<cv::Vec3b>(0, 0));
}

TEST(CvCoreTest, GaborFilter)
{
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(image.empty());

    cv::Mat gabor_image;
    int kernel_size = 21;
    double sigma = 5.0, theta = CV_PI / 4, lambd = 10.0, gamma = 0.5, psi = CV_PI / 2;

    gaborFilter(image, gabor_image, kernel_size, sigma, theta, lambd, gamma, psi);

    // Check if the transformation is applied (not a precise check)
    EXPECT_NE(image.at<uchar>(0, 0), gabor_image.at<uchar>(0, 0));
}

} // namespace pcv

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}