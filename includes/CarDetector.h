#pragma once

#include "IDetector.h"
#include <opencv2/dnn/dnn.hpp>

class CarDetector : public IDetector {
public:
    CarDetector(const std::string& model, const std::string& config,
                float confThreshold = 0.5, float nmsThreshold = 0.4);
};