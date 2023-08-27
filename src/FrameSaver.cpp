#include "../includes/FrameSaver.h"

FrameSaver::FrameSaver() : is_running_(false) {
}

void FrameSaver::pushFrame(const cv::Mat& frame) {
	std::lock_guard<std::mutex> lock(queueMutex);
	framesQueue.push(frame.clone());
	condVar.notify_all();  // Уведомляем поток о новом кадре
}

void FrameSaver::saveFrames() {
	while (isRunning()) {
		std::unique_lock<std::mutex> lock(queueMutex);
		condVar.wait(lock,
					 [this]() { return !framesQueue.empty() || !is_running_; });

		if (framesQueue.empty()) {
			lock.unlock();
			continue;
		}

		std::queue<cv::Mat> localQueue;
		std::swap(framesQueue, localQueue);
		lock.unlock();

		while (!localQueue.empty()) {
			cv::Mat image_to_save = localQueue.front();
			localQueue.pop();

			std::string filename = "../etc/frames/image_" +
								   std::to_string(image_counter++) + ".jpg";
			cv::imwrite(filename, image_to_save);
		}
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
	notify();  // Уведомляем поток о том, что пора завершаться
	if (savingThread.joinable()) {
		savingThread.join();
	}
}

void FrameSaver::start() {
	is_running_ = true;
	savingThread =
		std::thread(&FrameSaver::saveFrames, this);	 // Запускаем поток
}

FrameSaver::~FrameSaver() {
	stop();
}
