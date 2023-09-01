#pragma once

#include <opencv2/opencv.hpp>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

class IDetector {
public:
    IDetector(const std::string& model, const std::string& config,
              float confThreshold, float nmsThreshold);
    virtual ~IDetector();

    void pushFrame(const cv::Mat& frame);
    void startProcessing();
    void stopProcessing();
    bool getProcessedFrame(cv::Mat& frame);
    std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);

protected:
    virtual void detectObjects(cv::Mat& frame);
    virtual void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);
    virtual void processFrames();

    std::queue<cv::Mat> frameQueue;
    std::queue<cv::Mat> processedFramesQueue;
    std::mutex outputMutex;
    std::mutex queueMutex;
    bool processingRunning = false;
    std::thread processingThread;
    std::condition_variable frameAvailableCondition;

    cv::dnn::Net net;
    float confThreshold;
    float nmsThreshold;

    int targetClassId;
};