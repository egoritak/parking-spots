#include <iostream>
#include <opencv2/opencv.hpp>
#include "../includes/FrameSaver.h"
#include "../includes/CameraReader.h"
#include "../includes/CarDetector.h"

int main() {
	std::string camera_path = "https://cdn08.vtomske.ru/hls/stream8.m3u8";
	CameraReader camera(camera_path);

	if (!camera.isOpened()) {
		std::cerr << "Error: Unable to open the video stream." << std::endl;
		return -1;
	}

	CarDetector carDetector("../etc/yolo/yolov3.weights",
							"../etc/yolo/yolov3.cfg");
	FrameSaver saver;

	// Запускаем асинхронные потоки
	carDetector.startProcessing();
	saver.start();

	cv::namedWindow("IP Camera Stream", cv::WINDOW_AUTOSIZE);
	cv::Mat frame;

	while (true) {
		if (!camera.readFrame(frame)) {
			std::cerr << "Error: Unable to read the frame." << std::endl;
			break;
		}

		carDetector.pushFrame(frame);

		cv::Mat processedFrame;
		if (carDetector.getProcessedFrame(processedFrame)) {
			saver.pushFrame(processedFrame);
			cv::imshow("IP Camera Stream", processedFrame);
		}

		if (cv::waitKey(1000 / 30) == 27) {
			break;
		}
	}

	// Завершаем асинхронные потоки
	carDetector.stopProcessing();
	saver.stop();

	camera.release();
	cv::destroyAllWindows();

	return 0;
}
