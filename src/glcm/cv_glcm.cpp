#include "cv_glcm.h"
#include "core/cv_core.h"
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

namespace pcv::GLCM
{
    void calcGlcmMat(const cv::Mat &GrayInMat, cv::Mat &GlcmMat, GLCM_TYPE GlcmType, GRAY_LEVEL GrayLevel)
    {
        // Step 1: Verify input type
        assert(!GrayInMat.empty() && "Input gray image is empty");

        // Step 2: 内部拷贝，避免修改调用者的输入图像
        cv::Mat grayClone = GrayInMat.clone();

        // Step 3: Resize gray levels
        double minVal, maxVal;
        cv::minMaxLoc(grayClone, &minVal, &maxVal);
        if (maxVal >= (int)GrayLevel)
        {
            zoomGray(grayClone, (int)GrayLevel, false); // Scale to [0, GrayLevel) range
        }

        // Step 4: Initialize GLCM matrix
        GlcmMat = cv::Mat::zeros((int)GrayLevel, (int)GrayLevel, CV_32F);

        // Step 5: Calculate GLCM based on the direction
        int dx = 0, dy = 0;
        switch (GlcmType)
        {
        case GLCM_TYPE::GLCM_0:
            dx = 1;
            dy = 0;
            break;
        case GLCM_TYPE::GLCM_45:
            dx = 1;
            dy = -1;
            break;
        case GLCM_TYPE::GLCM_90:
            dx = 0;
            dy = 1;
            break;
        case GLCM_TYPE::GLCM_135:
            dx = 1;
            dy = 1;
            break;
        default:
            break;
        }

        // Step 6: Traverse the image and update GLCM
        for (int i = 0; i < grayClone.rows; ++i)
        {
            const uchar* row = grayClone.ptr<uchar>(i);
            for (int j = 0; j < grayClone.cols; ++j)
            {
                if (i + dy >= 0 && i + dy < grayClone.rows && j + dx >= 0 && j + dx < grayClone.cols)
                {
                    int r = row[j];
                    int c = grayClone.at<uchar>(i + dy, j + dx);
                    GlcmMat.at<float>(r, c)++;
                }
            }
        }

        // Step 7: Normalize GLCM by dividing by total count
        double totalSum = cv::sum(GlcmMat)[0];
        if (totalSum > 0.0)
        {
            GlcmMat /= static_cast<float>(totalSum);
        }
    }
    /// @brief Calculate GLCM data
    /// @param GlcmMat
    /// @param GlcmData
    void calcGlcmData(const cv::Mat &GlcmMat, GLCMDATA &GlcmData)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        GlcmData.Contrast = calcContrast(GlcmMat);
        GlcmData.Correlation = calcCorrelation(GlcmMat);
        GlcmData.Entropy = calcEntropy(GlcmMat);
        GlcmData.Homogeneity = calcHomogeneity(GlcmMat);
        GlcmData.IDMoment = calcIDMoment(GlcmMat);
        GlcmData.MaxProbability = calcMaxProbability(GlcmMat);
        GlcmData.AngularSecondMoment = calcEnergy(GlcmMat);
    }
    /// @brief Calculate the contrast of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcContrast(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // Contrast
        // $$
        // \text{Contrast} = \sum_{i,j} (i - j)^2 \, p(i, j)
        // $$
        float contrast = 0.0f;
        for (int i = 0; i < height; i++)
        {
            const float* row = GlcmMat.ptr<float>(i);
            for (int j = 0; j < width; j++)
            {
                contrast += (i - j) * (i - j) * row[j];
            }
        }
        return contrast;
    }
    /// @brief Calculate the entropy of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcEntropy(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // Entropy
        // $$
        // \text{Entropy} = - \sum_{i,j} p(i, j) \, \log(p(i, j) + \epsilon)
        // $$
        float entropy = 0.0f;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                entropy += -GlcmMat.at<float>(i, j) * std::log(GlcmMat.at<float>(i, j) + 1e-6);
            }
        }
        return entropy;
    }
    /// @brief Calculate the homogeneity of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcHomogeneity(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // Homogeneity
        // $$
        // \text{Homogeneity} = \sum_{i,j} \frac{p(i, j)}{1 + |i - j|}
        // $$
        float homogeneity = 0.0f;
        for (int i = 0; i < height; i++)
        {
            const float* row = GlcmMat.ptr<float>(i);
            for (int j = 0; j < width; j++)
            {
                homogeneity += row[j] / (1 + std::abs(i - j));
            }
        }
        return homogeneity;
    }
    /// @brief Calculate the correlation of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcCorrelation(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // Initialize variables for correlation calculation
        float correlation = 0.0f;
        float mu_i = 0.0f, mu_j = 0.0f, sigma_i = 0.0f, sigma_j = 0.0f;

        // Calculate means (mu_i and mu_j)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                mu_i += i * GlcmMat.at<float>(i, j);
                mu_j += j * GlcmMat.at<float>(i, j);
            }
        }

        // Calculate variances (sigma_i^2 and sigma_j^2)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                sigma_i += (i - mu_i) * (i - mu_i) * GlcmMat.at<float>(i, j);
                sigma_j += (j - mu_j) * (j - mu_j) * GlcmMat.at<float>(i, j);
            }
        }

        // Ensure standard deviations are non-zero to avoid division by zero
        sigma_i = std::sqrt(sigma_i);
        sigma_j = std::sqrt(sigma_j);
        if (sigma_i == 0.0f || sigma_j == 0.0f)
            return 0.0f; // correlation is undefined if any variance is zero

        // Calculate correlation
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                correlation += ((i - mu_i) * (j - mu_j) * GlcmMat.at<float>(i, j)) / (sigma_i * sigma_j);
            }
        }

        return correlation;
    }
    /// @brief Calculate the energy of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcEnergy(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // Energy
        // $$
        // \text{Energy} = \sum_{i,j} p(i, j)^2
        // $$
        float energy = 0.0f;
        for (int i = 0; i < height; i++)
        {
            const float* row = GlcmMat.ptr<float>(i);
            for (int j = 0; j < width; j++)
            {
                energy += row[j] * row[j];
            }
        }
        return energy;
    }
    /// @brief Calculate the inverse difference moment of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcIDMoment(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // IDMoment
        // $$
        // \text{Inverse Difference Moment} = \sum_{i,j} \frac{p(i, j)}{1 + (i - j)^2}
        // $$
        float idmoment = 0.0f;
        for (int i = 0; i < height; i++)
        {
            const float* row = GlcmMat.ptr<float>(i);
            for (int j = 0; j < width; j++)
            {
                idmoment += row[j] / (1 + (i - j) * (i - j));
            }
        }
        return idmoment;
    }
    /// @brief Calculate the maximum probability of the GLCM matrix
    /// @param GlcmMat
    /// @return
    float calcMaxProbability(const cv::Mat &GlcmMat)
    {
        assert(!GlcmMat.empty() && "Input GLCM matrix is empty");
        assert(GlcmMat.type() == CV_32FC1 && "Input GLCM matrix must be a float matrix");

        int height = GlcmMat.rows;
        int width = GlcmMat.cols;

        // MaxProbability
        // $$
        // \text{Maximum Probability} = \max(p(i, j))
        // $$
        float maxProbability = 0.0f;
        for (int i = 0; i < height; i++)
        {
            const float* row = GlcmMat.ptr<float>(i);
            for (int j = 0; j < width; j++)
            {
                maxProbability = std::max(maxProbability, row[j]);
            }
        }
        return maxProbability;
    }
};
