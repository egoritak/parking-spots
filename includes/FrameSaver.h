#include <iostream>
#include <condition_variable>
#include <queue>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <mutex>

class FrameSaver {
private:
	std::queue<cv::Mat> framesQueue;
	std::mutex queueMutex;
	std::condition_variable condVar;
	int image_counter = 0;
	bool is_running_ = true;

public:
	void pushFrame(const cv::Mat& frame);
	void saveFrames();
	void notify();
	bool isRunning() const;
	void stop();
};
