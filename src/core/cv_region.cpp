#include "cv_region.h"
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
/// @brief Region类构造函数
/// @param InMat 输入区域(CV_8UC1)
/// @param Centroid 输入区域质心
pcv::Region::Region(const cv::Mat &InMat, const cv::Point2f &Centroid)
{
    if (InMat.type() != CV_8UC1)
    {
        // TODO:只能包含0和255值
        CV_Error(cv::Error::StsBadArg, "输入的Region或Centroid不合法。");
    }
    this->m_width = InMat.cols;
    this->m_height = InMat.rows;
    // this->m_region = InMat.clone();// ROI区域大小
    std::vector<cv::Vec4i> hierarchy; // 轮廓层级
    // cv::findContours(this->m_region, this->contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point()); // 计算区域轮廓
    cv::findContours(InMat, this->m_contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point()); // 计算区域轮廓

    if ((Centroid.x == 0.0f) && (Centroid.y == 0.0f))
    {
        std::vector<cv::Point2f> Centroids;
        pcv::calcCentroid(this->m_contours, Centroids); // 计算质心
        this->m_centroid = Centroids[0];
    }
    else
    {
        this->m_centroid = Centroid; // 初始化：质心（从外接输入）
    }
    this->m_boundingRect = cv::Rect();           // 初始化：外接矩形
    this->m_minBoundingRect = cv::RotatedRect(); // 初始化：最小外接矩形

    this->m_regionArea = 0.0;          // 初始化：区域面积
    this->m_minBoundingRectArea = 0.0; // 初始化：最小外接矩形面积
}
/// @brief 获取区域大小
/// @return 区域大小
cv::Size pcv::Region::getMatSize() const { return cv::Size(this->m_width, this->m_height); }
/// @brief 获取区域图像
/// @return 区域图像
void pcv::Region::getRegion(cv::Mat& RegionMat)
{
    RegionMat = cv::Mat::zeros(this->m_height, this->m_width, CV_8UC1);
    cv::drawContours(RegionMat, this->m_contours, -1, cv::Scalar::all(255), -1);
}
/// @brief 获取区域面积
/// @return 区域面积
double pcv::Region::getRegionArea()
{
    if (this->m_regionArea == 0)
    {
        this->m_regionArea = cv::contourArea(this->m_contours[0]);
    }
    return this->m_regionArea;
}
/// @brief 获取区域质心
/// @return 区域质心
cv::Point2f pcv::Region::getCentroid()
{
    return this->m_centroid;
}
/// @brief 获取区域的外接矩形
/// @return 区域外接矩形
cv::Rect pcv::Region::getBoundingRect()
{
    if (this->m_boundingRect.empty())
    {
        this->m_boundingRect = cv::boundingRect(this->m_contours[0]);
    }
    return this->m_boundingRect;
}
/// @brief 获取区域的最小外接矩形
/// @return 区域最小外接矩形
cv::RotatedRect pcv::Region::getMinBoundingRect()
{
    if (this->m_minBoundingRect.size.width == 0 && this->m_minBoundingRect.size.height == 0)
    {
        this->m_minBoundingRect = cv::minAreaRect(this->m_contours[0]); // 连通域的最小外接矩形
    }
    return this->m_minBoundingRect;
}
/// @brief 获取区域的最小外接矩形面积
/// @return 区域最小外接矩形的面积
double pcv::Region::getMinBoundingRectArea()
{
    if (this->m_minBoundingRectArea == 0)
    {
        cv::Mat boxPts;
        cv::boxPoints(this->getMinBoundingRect(), boxPts);     // 获得外接矩形的四个顶点
        this->m_minBoundingRectArea = cv::contourArea(boxPts); // 最小外接矩形的面积
    }
    return this->m_minBoundingRectArea;
}
/// @brief 获取区域轮廓
/// @param OutContours 
void pcv::Region::getContours(std::vector<std::vector<cv::Point>>& OutContours)
{
    OutContours.clear();
    OutContours = this->m_contours;
}
/// @brief 连通域分割
/// @param ThresMat 输入二值化图像
/// @param OutRegions 输出连通域字典
int pcv::connection(const cv::Mat &ThresMat, std::unordered_map<int, Region>& OutRegions)
{
    OutRegions.clear();
    if (ThresMat.type() != CV_8UC1)
    {
        CV_Error(cv::Error::StsBadArg, "输入的ThresMat不是二值化图像。");
    }

    cv::Mat Labels, Stats, Centroids;
    int RegionNum = cv::connectedComponentsWithStats(ThresMat, Labels, Stats, Centroids);

    if (RegionNum > 1)
    {
        Eigen::MatrixXi LabelsMat;
        cv::cv2eigen(Labels, LabelsMat);

        for (int i = 1; i < RegionNum; i++)
        {
            Eigen::MatrixXi mask = (LabelsMat.array() == i).cast<int>() * 255;
            cv::Mat connectedRegion;
            cv::eigen2cv(mask, connectedRegion);
            connectedRegion.convertTo(connectedRegion, CV_8U);

            cv::Point2f centroid(static_cast<float>(Centroids.at<double>(i, 0)), static_cast<float>(Centroids.at<double>(i, 1)));
            OutRegions.emplace(i, pcv::Region(connectedRegion, centroid));
        }
    }
    return RegionNum;
}
/// @brief 获取最大的连通域
/// @param Regions 连通域字典
/// @param OutRegion 输出最大的连通域
void pcv::getMaxAreaRegion(std::unordered_map<int, pcv::Region> &Regions, pcv::Region& OutRegion)
{
    int MaxIndex = 0;
    double MaxArea = 0.0;
    for (auto R = Regions.begin(); R != Regions.end(); ++R)
    {
        int Index = R->first;
        pcv::Region &region = R->second;

        double area = region.getRegionArea();
        if (area > MaxArea)
        {
            MaxIndex = Index;
            MaxArea = area;
        }
    }
    OutRegion = std::move(Regions[MaxIndex]);
}
/// @brief 按照面积筛选区域
/// @param Regions 连通域字典
/// @param OutRegions 输出连通域字典
/// @param MinArea 最小面积
/// @param MaxArea 最大面积
void pcv::filterRegionByArea(std::unordered_map<int, pcv::Region> &Regions, 
                        std::unordered_map<int, pcv::Region>& OutRegions,
                        float MinArea, 
                        float MaxArea)
{
    OutRegions.clear(); // 清空输出区域
    for (auto R = Regions.begin(); R != Regions.end(); )
    {
        int Index = R->first;
        pcv::Region &region = R->second;

        double area = region.getRegionArea();
        if ((area >= MinArea) && (area <= MaxArea))
        {
            OutRegions.emplace(Index, std::move(region));
            R = Regions.erase(R); // 移动后从原始区域中删除
        }
        else
        {
            ++R;
        }
    }
}
/// @brief 计算区域质心
/// @param Contours 区域轮廓
/// @param Centroid 质心集合
void pcv::calcCentroid(const std::vector<std::vector<cv::Point>> &Contours, std::vector<cv::Point2f>& Centroid)
{
    // 5、计算每个轮廓所有矩
    std::vector<cv::Moments> mu(Contours.size()); // 创建一个vector,元素个数为contours.size()
    for (int i = 0; i < Contours.size(); i++)
    {
        mu[i] = moments(Contours[i], false); // 获得轮廓的所有最高达三阶所有矩
    }
    // 6、计算轮廓的质心
    std::vector<cv::Point2f> mc(Contours.size());
    for (int i = 0; i < Contours.size(); i++)
    {
        mc[i] = cv::Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00)); // 质心的 X,Y 坐标：(m10/m00, m01/m00)
    }
    Centroid = std::move(mc);
}