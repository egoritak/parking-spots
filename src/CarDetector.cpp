#include "../includes/CarDetector.h"

CarDetector::CarDetector(const std::string& model, const std::string& config,
                         float confThreshold, float nmsThreshold)
        : IDetector::IDetector(model, config, confThreshold, nmsThreshold) {
    targetClassId = 2;
}