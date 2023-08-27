#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "../includes/FrameSaver.h"
#include "../includes/CameraReader.h"
#include "../includes/CarDetector.h"

int main() {
	std::string camera_path = "https://cdn08.vtomske.ru/hls/stream8.m3u8";
	CameraReader camera(camera_path);
	FrameSaver saver;

	if (!camera.isOpened()) {
		std::cerr << "Error: Unable to open the video stream." << std::endl;
		return -1;
	}

	CarDetector carDetector("../etc/yolo/yolov3.weights",
							"../etc/yolo/yolov3.cfg");

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

		carDetector.detectCars(frame);

		saver.pushFrame(frame);
		saver.notify();

		cv::imshow("IP Camera Stream", frame);

		if (cv::waitKey(1000 / 30) == 27) {
			break;
		}
	}

	saver.stop();
	camera.release();
	saverThread.join();
	cv::destroyAllWindows();

	return 0;
}
