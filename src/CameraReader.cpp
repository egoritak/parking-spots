#include "../includes/CameraReader.h"

CameraReader::CameraReader(const std::string& cameraIndex) : cap(cameraIndex) {
}

CameraReader::CameraReader(int cameraIndex) : cap(cameraIndex) {
}

bool CameraReader::isOpened() const {
	return cap.isOpened();
}

bool CameraReader::readFrame(cv::Mat& frame) {
	return cap.read(frame);
}

void CameraReader::release() {
	cap.release();
}
