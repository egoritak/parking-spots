#pragma once

#include <opencv2/opencv.hpp>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

class IDetector {
public:
    virtual ~IDetector();

    void pushFrame(const cv::Mat& frame);
    void startProcessing();
    void stopProcessing();
    bool getProcessedFrame(cv::Mat& frame);

protected:
    virtual void detectObjects(cv::Mat& frame) = 0;
    virtual void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs) = 0;
    virtual void processFrames() = 0;

    std::queue<cv::Mat> frameQueue;
    std::queue<cv::Mat> processedFramesQueue;
    std::mutex outputMutex;
    std::mutex queueMutex;
    bool processingRunning = false;
    std::thread processingThread;
    std::condition_variable frameAvailableCondition;
};