#include "dnn/cv_face.h"
#include "core/cv_core.h"
/**
 * @brief 使用示例
 *     ./test_cv_face face_detection_yunet_2023mar.onnx face.jpg
 * 
 * 如果遇到模型加载失败的情况，可以尝试更换检测模型，当前使用：face_detection_yunet_2023mar.onnx
 */
int main(int argc, char const *argv[])
{
    pcv::face::FaceDetectorDNN::Param param;
    pcv::face::FaceDetectorDNN face_detector(std::string(argv[1]), param);
    cv::Mat img = cv::imread(std::string(argv[2]));

    pcv::face::FaceDetectorDNN::FaceGroup faces;
    face_detector.setInputSize(img.size());
    face_detector.detect(img, faces);
    cv::Mat out_img;
    face_detector.visualize(img, out_img, faces, true, 0.0, 1);
    cv::imshow("out_img", out_img);
    cv::waitKey(0);
    return 0;
}