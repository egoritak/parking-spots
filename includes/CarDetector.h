#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <queue>
#include <mutex>
#include <thread>

class CarDetector {
private:
	cv::dnn::Net net;
	float confThreshold;
	float nmsThreshold;
	std::queue<cv::Mat> frameQueue;
	std::queue<cv::Mat> processedFramesQueue;
	std::mutex outputMutex;
	std::mutex queueMutex;
	bool processingRunning;
	std::thread processingThread;

	std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);
	void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);
	void detectCars(cv::Mat& frame);
	void processFrames();

public:
	CarDetector(const std::string& model, const std::string& config,
				float confThreshold = 0.5, float nmsThreshold = 0.4);
	~CarDetector();

	void pushFrame(const cv::Mat& frame);
	void startProcessing();
	void stopProcessing();
	void pushProcessedFrame(const cv::Mat& frame);
	bool getProcessedFrame(cv::Mat& frame);
};
