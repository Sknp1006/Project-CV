#include <gtest/gtest.h>
#include "glcm/cv_glcm.h"
#include <iostream>

TEST(GLCMTest, GLCM)
{
    cv::Mat gray_image = (cv::Mat_<uchar>(7, 7) << 2, 6, 10, 14, 2, 6, 10,
                          6, 10, 14, 2, 6, 10, 14,
                          10, 14, 2, 6, 10, 14, 2,
                          14, 2, 6, 10, 14, 2, 6,
                          2, 6, 10, 14, 2, 6, 10,
                          6, 10, 14, 2, 6, 10, 14,
                          10, 14, 2, 6, 10, 14, 2);

    {
        cv::Mat glcm_mat;
        pcv::GLCM::calcGlcmMat(gray_image, glcm_mat, pcv::GLCM::GLCM_TYPE::GLCM_0, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::GLCMDATA glcm_data(pcv::GLCM::GLCM_TYPE::GLCM_0, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::calcGlcmData(glcm_mat, glcm_data);
        glcm_data.print();
    }

    {
        cv::Mat glcm_mat;
        pcv::GLCM::calcGlcmMat(gray_image, glcm_mat, pcv::GLCM::GLCM_TYPE::GLCM_45, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::GLCMDATA glcm_data(pcv::GLCM::GLCM_TYPE::GLCM_45, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::calcGlcmData(glcm_mat, glcm_data);
        glcm_data.print();
    }

    {
        cv::Mat glcm_mat;
        pcv::GLCM::calcGlcmMat(gray_image, glcm_mat, pcv::GLCM::GLCM_TYPE::GLCM_90, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::GLCMDATA glcm_data(pcv::GLCM::GLCM_TYPE::GLCM_90, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::calcGlcmData(glcm_mat, glcm_data);
        glcm_data.print();
    }

    {
        cv::Mat glcm_mat;
        pcv::GLCM::calcGlcmMat(gray_image, glcm_mat, pcv::GLCM::GLCM_TYPE::GLCM_135, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::GLCMDATA glcm_data(pcv::GLCM::GLCM_TYPE::GLCM_135, pcv::GLCM::GRAY_LEVEL::GL_16);
        pcv::GLCM::calcGlcmData(glcm_mat, glcm_data);
        glcm_data.print();
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}