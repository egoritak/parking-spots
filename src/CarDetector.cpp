#include "../includes/CarDetector.h"

CarDetector::CarDetector(const std::string& model, const std::string& config,
						 float confThreshold, float nmsThreshold)
	: net(cv::dnn::readNet(model, config)),
	  confThreshold(confThreshold),
	  nmsThreshold(nmsThreshold),
	  processingRunning(false) {
}

void CarDetector::pushFrame(const cv::Mat& frame) {
	std::lock_guard<std::mutex> lock(queueMutex);
	frameQueue.push(frame.clone());
}

void CarDetector::startProcessing() {
	processingRunning = true;
	processingThread = std::thread(&CarDetector::processFrames, this);
}

void CarDetector::stopProcessing() {
	processingRunning = false;
	if (processingThread.joinable()) {
		processingThread.join();
	}
}

void CarDetector::detectCars(cv::Mat& frame) {
	cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416),
										  cv::Scalar(0, 0, 0), true, false);

	net.setInput(blob);
	std::vector<cv::Mat> out;
	net.forward(out, getOutputsNames(net));

	postprocess(frame, out);
}

void CarDetector::processFrames() {
	while (processingRunning) {
		cv::Mat frame;
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			if (!frameQueue.empty()) {
				frame = frameQueue.front();
				frameQueue.pop();
			}
		}

		if (!frame.empty()) {
			detectCars(frame);
			pushProcessedFrame(frame);
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void CarDetector::pushProcessedFrame(const cv::Mat& frame) {
	std::lock_guard<std::mutex> lock(outputMutex);
	processedFramesQueue.push(frame.clone());
}

bool CarDetector::getProcessedFrame(cv::Mat& frame) {
	std::lock_guard<std::mutex> lock(outputMutex);
	if (!processedFramesQueue.empty()) {
		frame = processedFramesQueue.front();
		processedFramesQueue.pop();
		return true;
	}
	return false;
}

std::vector<std::string> CarDetector::getOutputsNames(const cv::dnn::Net& net) {
	std::vector<std::string> names;
	std::vector<int> outLayers = net.getUnconnectedOutLayers();
	std::vector<std::string> layersNames = net.getLayerNames();
	names.resize(outLayers.size());
	for (size_t i = 0; i < outLayers.size(); ++i) {
		names[i] = layersNames[outLayers[i] - 1];
	}
	return names;
}

void CarDetector::postprocess(cv::Mat& frame,
							  const std::vector<cv::Mat>& outs) {
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;

	for (size_t i = 0; i < outs.size(); ++i) {
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
			cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			cv::Point classIdPoint;
			double confidence;
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold) {
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
	}

	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	for (int idx : indices) {
		cv::Rect box = boxes[idx];
		int classId = classIds[idx];
		cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
	}
}

CarDetector::~CarDetector() {
	stopProcessing();
}
