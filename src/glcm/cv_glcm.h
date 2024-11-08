#ifndef H_PCV_GLCM
#define H_PCV_GLCM

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <cstdio>

namespace pcv
{
    namespace GLCM
    {
        enum class GLCM_TYPE
        {
            GLCM_0 = 0,
            GLCM_45 = 45,
            GLCM_90 = 90,
            GLCM_135 = 135
        };

        enum class GRAY_LEVEL
        {
            GL_4 = 4,
            GL_8 = 8,
            GL_16 = 16,
            GL_32 = 32,
            GL_64 = 64,
            GL_128 = 128,
            GL_256 = 256
        };

        struct GLCMDATA
        {
            GLCMDATA(GLCM_TYPE GlcmType, GRAY_LEVEL GrayLevel) : MaxProbability(0.0f), 
                                                                 AngularSecondMoment(0.0f), 
                                                                 Contrast(0.0f), Correlation(0.0f), 
                                                                 Entropy(0.0f), 
                                                                 Homogeneity(0.0f), 
                                                                 IDMoment(0.0f), 
                                                                 m_type(GlcmType), 
                                                                 m_grayLevel(GrayLevel) {}
            ~GLCMDATA() = default;

            GLCM_TYPE m_type;
            GRAY_LEVEL m_grayLevel;
            float MaxProbability;      // 最大概率
            float AngularSecondMoment; // 能量/二阶距/均匀性
            float Contrast;            // 对比度/反差
            float Correlation;         // 相关性/自相关性
            float Entropy;             // 熵
            float Homogeneity;         // 逆差距/同质性
            float IDMoment;            // 逆差距矩阵

            void print()
            {
                printf("MaxProbability: %.6f\n", MaxProbability);
                printf("AngularSecondMoment: %.6f\n", AngularSecondMoment);
                printf("Contrast: %.6f\n", Contrast);
                printf("Correlation: %.6f\n", Correlation);
                printf("Entropy: %.6f\n", Entropy);
                printf("Homogeneity: %.6f\n", Homogeneity);
                printf("IDMoment: %.6f\n", IDMoment);
            }
        };

        void calcGlcmMat(cv::Mat &GrayInMat, cv::Mat &GlcmMat, GLCM_TYPE GlcmType, GRAY_LEVEL GrayLevel = GRAY_LEVEL::GL_64);
        void calcGlcmData(const cv::Mat &GlcmMat, GLCMDATA &GlcmData);

        float calcContrast(const cv::Mat &GlcmMat);
        float calcEntropy(const cv::Mat &GlcmMat);
        float calcHomogeneity(const cv::Mat &GlcmMat);
        float calcCorrelation(const cv::Mat &GlcmMat);
        float calcEnergy(const cv::Mat &GlcmMat);
        float calcIDMoment(const cv::Mat &GlcmMat);
        float calcMaxProbability(const cv::Mat &GlcmMat);
    };

} // namespace pcv

#endif // H_PCV_GLCM