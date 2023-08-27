#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "../includes/FrameSaver.h"
#include "../includes/CameraReader.h"

int main() {
	std::string camera_path = "https://cdn08.vtomske.ru/hls/stream8.m3u8";
	CameraReader camera(camera_path);
	FrameSaver saver;

	if (!camera.isOpened()) {
		std::cerr << "Error: Unable to open the video stream." << std::endl;
		return -1;
	}

	cv::namedWindow("IP Camera Stream", cv::WINDOW_AUTOSIZE);
	cv::Mat frame;

	std::thread saverThread([&saver]() {
		while (saver.isRunning()) {
			saver.saveFrames();
		}
	});

	while (true) {
		if (!camera.readFrame(frame)) {
			std::cerr << "Error: Unable to read the frame." << std::endl;
			break;
		}

		saver.pushFrame(frame);
		saver.notify();

		cv::imshow("IP Camera Stream", frame);

		if (cv::waitKey(1000 / 30) == 27) {
			break;
		}
	}

	saver.stop();  // Уведомляем saver о том, что пора завершаться
	camera.release();
	saverThread.join();
	cv::destroyAllWindows();

	return 0;
}
