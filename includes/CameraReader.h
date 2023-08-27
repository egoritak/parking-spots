#include <opencv2/opencv.hpp>

class CameraReader {
private:
	cv::VideoCapture cap;

public:
	CameraReader(const std::string& cameraIndex);
	CameraReader(int cameraIndex);
	bool isOpened() const;
	bool readFrame(cv::Mat& frame);
	void release();
};
