#ifndef H_PCV_REGION
#define H_PCV_REGION

#include <opencv2/core.hpp>
#include <unordered_map>
#include "cv_core.h"

namespace pcv
{
    class Region
    {
    public:
        Region() = default;
        explicit Region(const cv::Mat &InMat, const cv::Point2f &Centroid);
        explicit Region(const cv::Mat &InMat) : Region(InMat, cv::Point2f(0.0f, 0.0f)) {}
        ~Region() = default;

        cv::Size getMatSize() const;          // 获取原始图像尺寸
        void getRegion(cv::Mat& RegionMat);   // 获取区域
        double getRegionArea();               // 获取区域面积
        cv::Point2f getCentroid();            // 获取区域质心
        cv::Rect getBoundingRect();           // 获取区域的外接矩形
        cv::RotatedRect getMinBoundingRect(); // 获取区域的最小外接矩形
        double getMinBoundingRectArea();      // 获取最小外接矩形面积
        void getContours(std::vector<std::vector<cv::Point>>& OutContours); // 获取区域轮廓
    private:
        int m_width;
        int m_height;

        std::vector<std::vector<cv::Point>> m_contours; // 区域轮廓
        cv::Point2f m_centroid;                         // 区域质心
        // cv::Mat m_region;                               // 掩膜区域CV_8UC1
        cv::Rect m_boundingRect;                        // 外接矩形
        cv::RotatedRect m_minBoundingRect;              // 最小外接矩形

        double m_regionArea;          // 区域面积
        double m_minBoundingRectArea; // 最小外接矩形面积
    };

    int connection(const cv::Mat &ThresMat, std::unordered_map<int, Region>& OutRegions);   // 分割连通域
    void getMaxAreaRegion(std::unordered_map<int, Region> &Regions, Region& OutRegion);      // 获取最大的连通域
    void filterRegionByArea(std::unordered_map<int, Region> &Regions, 
                            std::unordered_map<int, Region>& OutRegions,
                            float MinArea, 
                            float MaxArea = 1e10f);                                          // 根据面积过滤连通域
    void calcCentroid(const std::vector<std::vector<cv::Point>> &Contours, std::vector<cv::Point2f>& Centroid);    // 计算区域质心
}; // namespace pcv
#endif // H_PCV_REGION