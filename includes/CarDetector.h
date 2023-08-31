#pragma once

#include "IDetector.h"
#include <opencv2/dnn/dnn.hpp>

class CarDetector : public IDetector {
public:
    CarDetector(const std::string& model, const std::string& config,
                float confThreshold = 0.5, float nmsThreshold = 0.4);

protected:
    void detectObjects(cv::Mat& frame) override;
    void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs) override;
    void processFrames() override;

private:
    cv::dnn::Net net;
    float confThreshold;
    float nmsThreshold;

    static std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);
};