#include "../includes/IDetector.h"

IDetector::~IDetector() {
    stopProcessing();
}

void IDetector::pushFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(queueMutex);
    frameQueue.push(frame.clone());
}

void IDetector::startProcessing() {
    processingRunning = true;
    processingThread = std::thread(&IDetector::processFrames, this);
}

void IDetector::stopProcessing() {
    processingRunning = false;
    if (processingThread.joinable()) {
        processingThread.join();
    }
}

bool IDetector::getProcessedFrame(cv::Mat& frame) {
    std::unique_lock<std::mutex> lock(outputMutex);
    frameAvailableCondition.wait(
            lock, [this]() { return !processedFramesQueue.empty(); });
    if (!processedFramesQueue.empty()) {
        frame = processedFramesQueue.front();
        processedFramesQueue.pop();
        return true;
    }
    return false;
}
