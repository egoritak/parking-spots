#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>

class CarDetector {
private:
	cv::dnn::Net net;
	float confThreshold;
	float nmsThreshold;

public:
	CarDetector(const std::string& model, const std::string& config,
					float confThreshold = 0.5, float nmsThreshold = 0.4);

	void detectCars(cv::Mat& frame);

private:
	std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);

	void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);
};
