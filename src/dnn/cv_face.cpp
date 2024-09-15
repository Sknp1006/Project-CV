#include "cv_face.h"
#include "core/cv_core.h"

using namespace pcv::face;
//--------------------------------------------------------------------------------------------------------------------------------------
//												FaceDetector_DNN构造函数
// 参数:
//          ModelPath:          Path to the model.
//          BackendId:          Backend to run on. 0: default, 1: Halide, 2: Intel's Inference Engine, 3: OpenCV, 4: VKCOM, 5: CUDA
//          TargetId:           Target to run on. 0: CPU, 1: OpenCL, 2: OpenCL FP16, 3: Myriad, 4: Vulkan, 5: FPGA, 6: CUDA, 7: CUDA FP16, 8: HDDL
//          ScoreThreshold:     Filter out faces of score < score_threshold.
//          NmsThreshold:       Suppress bounding boxes of iou >= nms_threshold.
//          TopK:               Keep top_k bounding boxes before NMS.
//          Save:               Set true to save results. This flag is invalid when using camera.
//          Vis:                Set true to open a window for result visualization. This flag is invalid when using camera.
//--------------------------------------------------------------------------------------------------------------------------------------

/// @brief 构造函数
/// @param ModelPath
/// @param Param
FaceDetectorDNN::FaceDetectorDNN(const std::string &ModelPath, FaceDetectorDNN::Param Param)
{
    int BackendId = Param.getBackendId();
    int TargetId = Param.getTragetId();
    float ScoreThreshold = Param.getScoreThreshold();
    float NmsThreshold = Param.getNmsThreshold();
    int TopK = Param.getTopK();
    // bool Save = Param.getSave();
    // bool Vis = Param.getVis();
    this->detector = cv::FaceDetectorYN::create(ModelPath, "", cv::Size(320, 320), ScoreThreshold, NmsThreshold, TopK, BackendId, TargetId);
}
/// @brief 析构函数
FaceDetectorDNN::~FaceDetectorDNN()
{
    this->detector.release();
}
/// @brief 设置输入尺寸
/// @param Size
void FaceDetectorDNN::setInputSize(const cv::Size &Size)
{
    this->detector->setInputSize(Size);
}
/// @brief 获取输入尺寸
/// @param InMat
/// @return
void FaceDetectorDNN::detect(const cv::Mat &InMat, FaceDetectorDNN::FaceGroup &Faces)
{
    cv::Mat faces;                        // 【15 x n】
    this->detector->detect(InMat, faces); // 人脸检测

    Faces = FaceDetectorDNN::FaceGroup(faces, InMat.size());
}
/// @brief 可视化
/// @param InMat
/// @param OutMat
/// @param Aface
/// @param PrintFlag
/// @param FPS
/// @param Thickness
/// @return
void FaceDetectorDNN::visualize(const cv::Mat &InMat, cv::Mat &OutMat, const FaceDetectorDNN::FaceGroup &Faces, bool PrintFlag, double FPS, int Thickness)
{
    OutMat = InMat.clone();
    if (FPS > 0)
    {
        // 在视频流中显示FPS
        cv::putText(OutMat, cv::format("FPS: %.2f", FPS), cv::Point2i(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
    }

    for (auto &aface : Faces)
    {
        if (PrintFlag)
        {
            std::cout << "area: " << aface.getFaceRegion().area()
                      << ", top-left coordinates: (" << aface.getFaceRegion().x << ", " << aface.getFaceRegion().y << "), "
                      << "box width: " << aface.getFaceRegion().width << ", box height: " << aface.getFaceRegion().height << ", "
                      << "score: " << aface.getFaceScore() << "\n";
        }
        cv::rectangle(OutMat, aface.getFaceRegion(), cv::Scalar(0, 255, 0), Thickness);   // 人脸框（绿色）
        cv::circle(OutMat, aface.getLeftEye(), 2, cv::Scalar(255, 0, 0), Thickness);      // 图像左眼，实际右眼（蓝色）
        cv::circle(OutMat, aface.getRightEye(), 2, cv::Scalar(0, 0, 255), Thickness);     // 图像右眼，实际左眼（红色）
        cv::circle(OutMat, aface.getNose(), 2, cv::Scalar(0, 255, 0), Thickness);         // 图像鼻子（绿色）
        cv::circle(OutMat, aface.getLeftMouth(), 2, cv::Scalar(255, 0, 255), Thickness);  // 图像左嘴角，实际右嘴角（粉色）
        cv::circle(OutMat, aface.getRightMouth(), 2, cv::Scalar(0, 255, 255), Thickness); // 图像右嘴角，实际左嘴角（黄色）
        cv::Rect faceRegion = aface.getFaceRegion();
        cv::circle(OutMat, aface.getLeftEye(), static_cast<int>(faceRegion.width * 0.15), cv::Scalar(255, 0, 0), 1);  // 左眼
        cv::circle(OutMat, aface.getRightEye(), static_cast<int>(faceRegion.width * 0.15), cv::Scalar(0, 0, 255), 1); // 右眼
        cv::circle(OutMat, aface.getNose(), static_cast<int>(faceRegion.width * 0.15), cv::Scalar(0, 255, 0), 1);     // 鼻子

        cv::rectangle(OutMat, cv::Rect(static_cast<int>(aface.getLeftMouth().x), static_cast<int>(aface.getLeftMouth().y), static_cast<int>(aface.getRightMouth().x - aface.getLeftMouth().x), static_cast<int>(aface.getFaceRegion().height * 0.14)), cv::Scalar::all(255), 1); // 嘴巴
        // cv::circle(output, aface.getLeftMouth(), faceRegion.width * 0.2, cv::Scalar(255, 0, 255), 1);   //嘴巴
        // cv::circle(output, aface.getRightMouth(), faceRegion.width * 0.2, cv::Scalar(0, 255, 255), 1);  //嘴巴

        cv::putText(OutMat, cv::format("%.4f", aface.getFaceScore()), cv::Point2i(aface.getFaceRegion().x, aface.getFaceRegion().y + 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
    }
}