
#ifndef H_PCV_FACE
#define H_PCV_FACE

#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>

namespace pcv
{
    namespace face
    {
        class FaceDetectorDNN
        {
        public:
            class Param
            {
            public:
                Param() : backendId(0), targetId(0), scoreThreshold(0.9f), nmsThreshold(0.3f), topK(5000), save(false), vis(true) {}
                Param(int BackendId, int TargetId, float ScoreThreshold,
                      float NmsThreshold, int TopK, bool Save, bool Vis)
                    : backendId(BackendId), targetId(TargetId), scoreThreshold(ScoreThreshold),
                      nmsThreshold(NmsThreshold), topK(TopK), save(Save), vis(Vis) {}
                ~Param() = default;
                inline int getBackendId() const { return this->backendId; }
                inline int getTragetId() const { return this->targetId; }
                inline float getScoreThreshold() const { return this->scoreThreshold; }
                inline float getNmsThreshold() const { return this->nmsThreshold; }
                inline int getTopK() const { return this->topK; }
                inline bool getSave() const { return this->save; }
                inline bool getVis() const { return this->vis; }

            private:
                int backendId;
                int targetId;
                float scoreThreshold;
                float nmsThreshold;
                int topK;
                bool save;
                bool vis;
            };

            class AFace
            {
            public:
                AFace() : faceRegion(cv::Rect(0, 0, 0, 0)),
                          leftEye(cv::Point2f(0, 0)),
                          rightEye(cv::Point2f(0, 0)),
                          nose(cv::Point2f(0, 0)),
                          leftMouth(cv::Point2f(0, 0)),
                          rightMouth(cv::Point2f(0, 0)),
                          faceScore(0.0) {}
                AFace(const cv::Mat &Face, const cv::Size &Size) : AFace()
                {
                    float x = (Face.at<float>(0, 0));
                    float y = (Face.at<float>(0, 1));
                    int width = static_cast<int>(Face.at<float>(0, 2));
                    int height = static_cast<int>(Face.at<float>(0, 3));
                    // 边界检查
                    if (x < 0)
                    {
                        width += static_cast<int>(x);
                        x = 0;
                    }
                    if (y < 0)
                    {
                        height += static_cast<int>(y);
                        y = 0;
                    }
                    if (x + width > Size.width - 1)
                    {
                        width = Size.width - 1 - static_cast<int>(x);
                    }
                    if (y + height > Size.height - 1)
                    {
                        height = Size.height - 1 - static_cast<int>(y);
                    }
                    this->faceRegion = cv::Rect2i(static_cast<int>(x), static_cast<int>(y), width, height); // 保存最大的矩形框
                    this->leftEye = cv::Point2f((Face.at<float>(0, 4)), (Face.at<float>(0, 5)));
                    this->rightEye = cv::Point2f((Face.at<float>(0, 6)), (Face.at<float>(0, 7)));
                    this->nose = cv::Point2f((Face.at<float>(0, 8)), (Face.at<float>(0, 9)));
                    this->leftMouth = cv::Point2f((Face.at<float>(0, 10)), (Face.at<float>(0, 11)));
                    this->rightMouth = cv::Point2f((Face.at<float>(0, 12)), (Face.at<float>(0, 13)));
                    this->faceScore = Face.at<float>(0, 14);
                }
                ~AFace() = default;
                inline cv::Rect getFaceRegion() const { return this->faceRegion; }
                inline cv::Point2f getLeftEye() const { return this->leftEye; }
                inline cv::Point2f getRightEye() const { return this->rightEye; }
                inline cv::Point2f getNose() const { return this->nose; }
                inline cv::Point2f getLeftMouth() const { return this->leftMouth; }
                inline cv::Point2f getRightMouth() const { return this->rightMouth; }
                inline float getFaceScore() const { return this->faceScore; }

            private:
                cv::Rect faceRegion;    // 人脸矩形框
                cv::Point2f leftEye;    // 图像左眼
                cv::Point2f rightEye;   // 图像右眼
                cv::Point2f nose;       // 鼻子
                cv::Point2f leftMouth;  // 图像左嘴角
                cv::Point2f rightMouth; // 图像右嘴角
                float faceScore;        // 人脸分数
            };

            class FaceGroup : public std::vector<AFace>
            {
            public:
                FaceGroup() = default;
                FaceGroup(const cv::Mat &Faces, const cv::Size &Size)
                {
                    if (Faces.rows > 0)
                    {
                        for (size_t i = 0; i < Faces.rows; i++)
                        {
                            this->push_back(AFace(Faces.row(i), Size));
                        }
                    }
                }
                ~FaceGroup() = default;
            };

            FaceDetectorDNN(const std::string &ModelPath, FaceDetectorDNN::Param Param = FaceDetectorDNN::Param());
            ~FaceDetectorDNN();

            void setInputSize(const cv::Size &Size);
            void detect(const cv::Mat &InMat, FaceDetectorDNN::FaceGroup &Faces);
            void visualize(const cv::Mat &InMat, cv::Mat &OutMat,
                           const FaceDetectorDNN::FaceGroup &Faces,
                           bool PrintFlag = false,
                           double FPS = -1,
                           int Thickness = 2);

        private:
            cv::Ptr<cv::FaceDetectorYN> detector;
        };
    } // namespace face
} // namespace pcv

#endif // H_PCV_FACE