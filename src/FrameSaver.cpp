#include "../includes/FrameSaver.h"

void FrameSaver::pushFrame(const cv::Mat& frame) {
	std::lock_guard<std::mutex> lock(queueMutex);
	framesQueue.push(frame.clone());
}

void FrameSaver::saveFrames() {
	std::unique_lock<std::mutex> lock(queueMutex);
	condVar.wait_for(lock, std::chrono::seconds(15),
					 [this]() { return !framesQueue.empty() || is_running_; });

	if (framesQueue.empty()) return;

	std::queue<cv::Mat> localQueue;
	std::swap(framesQueue, localQueue);
	lock.unlock();

	while (!localQueue.empty()) {
		cv::Mat image_to_save = localQueue.front();
		localQueue.pop();

		std::string filename =
			"../../frames/image_" + std::to_string(image_counter++) + ".jpg";
		cv::imwrite(filename, image_to_save);
	}
}

void FrameSaver::notify() {
	condVar.notify_all();
}

bool FrameSaver::isRunning() const {
	return is_running_;
}

void FrameSaver::stop() {
	is_running_ = false;
}
