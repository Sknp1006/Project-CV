#include <gtest/gtest.h>
#include "core/cv_region.h"


TEST(CvRegionTest, Region)
{
    cv::Mat image = cv::imread("test.jpg");
    ASSERT_FALSE(image.empty());

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray_image, gray_image, cv::Size(5, 5), 0, 0);

    cv::Mat thresholded_image;
    double min_gray = 0;
    double max_gray = 100;

    pcv::threshold(gray_image, thresholded_image, min_gray, max_gray);

    std::unordered_map<int, pcv::Region> regions_map;
    int num = pcv::connection(thresholded_image, regions_map);
    std::cout << "num: " << num << std::endl;

    pcv::Region max_area_region;
    pcv::getMaxAreaRegion(regions_map, max_area_region);

    cv::Mat regionMat;
    max_area_region.getRegion(regionMat);

    std::vector<cv::Point2f> Centroids;

    std::vector<std::vector<cv::Point>> outContours;
    max_area_region.getContours(outContours);
    pcv::calcCentroid(outContours, Centroids);
    std::cout << "Centroids: " << Centroids[0] << std::endl;

    cv::imwrite("Region.jpg", regionMat);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}