#include "cv_features.h"

namespace pcv
{
    /// @brief 构造函数
    /// @param Param 参数
    SurfMatcher::SurfMatcher(SURFPARAM Param)
    {
        this->SurfHandle = cv::xfeatures2d::SURF::create(Param.hessianThreshold,
                                                         Param.nOctaves,
                                                         Param.nOctaveLayers,
                                                         Param.extended,
                                                         Param.upright);
        this->Matcher = cv::FlannBasedMatcher::create();
    }

    /// @brief 计算SURF特征点与特征描述符
    /// @param InMat 输入图像
    /// @param Mask 掩码
    /// @return
    SurfMatcher::SURFDATA SurfMatcher::calcSurfData(const cv::Mat &InMat, const cv::Mat &Mask)
    {
        SurfMatcher::SURFDATA data;
        if (Mask.empty() == true)
        {
            this->SurfHandle->detect(InMat, data.KeyPoints, Mask);
            this->SurfHandle->compute(InMat, data.KeyPoints, data.Description);
        }
        else
        {
            this->SurfHandle->detect(InMat, data.KeyPoints);
            this->SurfHandle->compute(InMat, data.KeyPoints, data.Description);
        }
        return data;
    }

    /// @brief 学习模板特征
    /// @param TemplateData
    void SurfMatcher::trainMatcher(const SURFDATA &TemplateData)
    {
        std::vector<cv::Mat> desc_collection(1, TemplateData.Description);
        this->Matcher->add(desc_collection);
        this->Matcher->train();
    }

    /// @brief 配准并输出变换矩阵
    /// @param ToMatch 待匹配数据
    /// @param Template 模板数据
    /// @param GoodMatches 匹配的数据
    /// @param PerspectiveMat 透视变换矩阵
    /// @param Threshold 阈值
    void SurfMatcher::match(const SURFDATA &ToMatch, const SURFDATA &Template, std::vector<cv::DMatch> &GoodMatches, cv::Mat &PerspectiveMat, float Threshold)
    {
        std::vector<std::vector<cv::DMatch>> Matches;
        this->Matcher->knnMatch(ToMatch.Description, Matches, 2);
        GoodMatches.clear();
        for (unsigned int i = 0; i < Matches.size(); ++i)
        {
            if (Matches[i][0].distance < Threshold * Matches[i][1].distance)
                GoodMatches.push_back(Matches[i][0]);
        }
        std::vector<cv::Point2f> ToMatchPoints, TemplatePoints;
        int GoodMatchesSize = 100;
        if ((GoodMatches.size() > 4) && (GoodMatches.size() < GoodMatchesSize))
        {
            GoodMatchesSize = GoodMatches.size();
        }
        else if (GoodMatches.size() <= 4)
        {
            PerspectiveMat = cv::Mat::ones(3, 3, CV_64F);
            return;
        }
        for (unsigned int i = 0; i < GoodMatchesSize; i++)
        {
            ToMatchPoints.push_back(ToMatch.KeyPoints[GoodMatches[i].queryIdx].pt);
            TemplatePoints.push_back(Template.KeyPoints[GoodMatches[i].trainIdx].pt);
        }
        PerspectiveMat = cv::findHomography(ToMatchPoints, TemplatePoints, cv::RANSAC);
    }

    /// @brief 透视变换
    /// @param ToMatchImage 待匹配图像
    /// @param AfterPerspective 输出图像
    /// @param PerspectiveMat 透视变换矩阵
    /// @param MatchSize 输出图像大小
    /// @return
    bool SurfMatcher::wrapPerspective(const cv::Mat &ToMatchImage, cv::Mat &AfterPerspective, const cv::Mat &PerspectiveMat, cv::Size MatchSize)
    {
        try
        {
            cv::warpPerspective(ToMatchImage, AfterPerspective, PerspectiveMat, MatchSize);
        }
        catch (cv::Exception &e)
        {
            return false;
        }
        return true;
    }

    /// @brief 显示匹配结果
    /// @param Template
    /// @param TemplateMat
    /// @param ToMatch
    /// @param ToMatchMat
    /// @param GoodMatches
    void SurfMatcher::showMatchResult(const SURFDATA &Template,
                                      const cv::Mat &TemplateMat,
                                      const SURFDATA &ToMatch,
                                      const cv::Mat &ToMatchMat,
                                      std::vector<cv::DMatch> &GoodMatches)
    {
        cv::Mat show;
        cv::drawMatches(ToMatchMat, ToMatch.KeyPoints, TemplateMat, Template.KeyPoints, GoodMatches, show);
        std::string WinName = "image[match]";
        try
        {
            int ret = cv::getWindowProperty(WinName.c_str(), 0);
            if (ret == -1)
            {
                cv::namedWindow(WinName.c_str(), cv::WINDOW_NORMAL);
                cv::imshow(WinName.c_str(), show);
            }
            else
            {
                cv::imshow(WinName.c_str(), show);
            }
        }
        catch (cv::Exception &e)
        {
            // 需要捕获这个异常，因为在第一次调用时，窗口还没有被创建
            cv::namedWindow(WinName.c_str(), cv::WINDOW_NORMAL);
            cv::imshow(WinName.c_str(), show);
        }
    }

}; // namespace pcv
