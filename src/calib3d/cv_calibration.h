#ifndef H_PCV_CAMERA_CALIBRATION
#define H_PCV_CAMERA_CALIBRATION

#include <cassert>
#include <string>
#include <opencv2/opencv.hpp>

namespace pcv
{
    class CameraCalibration
    {
    public:
        class Param
        {
        public:
            Param(int width, int height)
            {
                assert(width > 0 && height > 0 && "Chessboard dimensions must be positive");
                this->m_chessboard_rows = height;
                this->m_chessboard_cols = width;

                for (int i{0}; i < height; i++)
                {
                    for (int j{0}; j < width; j++)
                    {
                        this->objp.push_back(cv::Point3f(j, i, 0));
                    }
                }
            }
            ~Param() = default;

            cv::Size getCheckerBoardSize() const
            {
                return cv::Size(this->m_chessboard_cols, this->m_chessboard_rows);
            }
            // Defining the world coordinates for 3D points
            // 为三维点定义世界坐标系
            std::vector<cv::Point3f> objp;

            // Creating vector to store vectors of 3D points for each checkerboard image
            // 创建矢量以存储每个棋盘图像的三维点矢量
            std::vector<std::vector<cv::Point3f>> objPoints;

            // Creating vector to store vectors of 2D points for each checkerboard image
            // 创建矢量以存储每个棋盘图像的二维点矢量
            std::vector<std::vector<cv::Point2f>> imgPoints;

            // vector to store the pixel coordinates of detected checker board corners
            // 存储检测到的棋盘转角像素坐标的矢量
            std::vector<cv::Point2f> cornerPts;

            inline void setCameraMatrix(const cv::Mat& InMat)
            {
                this->CameraMatrix = InMat.clone();
            }
            inline void setDistCoeffs(const cv::Mat& InMat)
            {
                this->DistCoeffs = InMat.clone();
            }
            inline void setRotationVector(const cv::Mat& InMat)
            {
                this->RotationVector = InMat.clone();
            }
            inline void setTranslationVector(const cv::Mat& InMat)
            {
                this->TranslationVector = InMat.clone(); }

        private:
            // 定义棋盘格的尺寸
            int m_chessboard_rows;
            int m_chessboard_cols;

            cv::Mat CameraMatrix, DistCoeffs, RotationVector, TranslationVector;
        };

        CameraCalibration(const CameraCalibration::Param& Param);
        ~CameraCalibration();

        void getCheckerBoardImg(const std::string& FilesPath); // 第一步：从文件读取棋盘格图像
        void doCalibration(); // 第二步：开始标定

        void loadCameraParam(const std::string& FilesPath);
    private:
        int ImgCount = 0;
        std::string save_cc; // 参数保存路径
        std::vector<cv::Mat> ImgList;    // 棋盘格图片列表
    };

} // namespace pcv

#endif // !H_CAMERA_CALIBRATION
