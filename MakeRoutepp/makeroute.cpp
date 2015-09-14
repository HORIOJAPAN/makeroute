#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <cstdlib>

#include "SharedMemory.h"

using namespace cv;
using namespace std;

Mat pic;
Mat pic_col;

SharedMemory<int> shMem("CoordinateOfMap");

int xy[2] = { 0, 0 };

void onMouse(int event, int x, int y, int flag, void* )
{

	// マウスイベントを取得
	switch (event) {
	case cv::EVENT_MOUSEMOVE:
		break;
	case cv::EVENT_LBUTTONDOWN:
		xy[0] = x;
		xy[1] = y;

		shMem.setShMemData(1);
		shMem.setShMemData(x, 1);
		shMem.setShMemData(y, 2);

		pic_col.data[y * pic_col.step + x * pic_col.elemSize()] = 0;
		pic_col.data[y * pic_col.step + x * pic_col.elemSize() + 1 ] = 0;
		pic_col.data[y * pic_col.step + x * pic_col.elemSize() + 2 ] = 255;

		break;
	case cv::EVENT_RBUTTONDOWN:
		break;
	case cv::EVENT_MBUTTONDOWN:
		break;
	case cv::EVENT_LBUTTONUP:
		break;
	case cv::EVENT_RBUTTONUP:
		break;
	case cv::EVENT_MBUTTONUP:
		break;
	case cv::EVENT_LBUTTONDBLCLK:
		break;
	case cv::EVENT_RBUTTONDBLCLK:
		break;
	case cv::EVENT_MBUTTONDBLCLK:
		break;
	}

}


void main(int argc, char* argv[])
{
	string str;

	//pic = imread("C:\\Users\\user\\Documents\\Visual Studio 2013\\Projects\\HJ_MakeRoute\\Debug\\0_0_3.jpg");

	cout << argc << endl;
	for (int i = 0; i < argc; i++)	cout << argv[i] << endl;
	for (int i = 1; i < argc; i++)
	{
		str += argv[i];
		str += " ";
	}
	cout << str << endl;
	pic = imread(str , 0);

	pic_col = Mat(pic.rows, pic.cols, CV_8UC3);
	cvtColor(pic, pic_col, CV_GRAY2BGR);

	namedWindow("show", 1 );

	cv::setMouseCallback("show", onMouse, 0);

	imshow("show", pic_col);

	int key;
	while (true){
		key = waitKey(10);
		if (key == 's'){//保存
			cout << "s" << endl;
			cout << "保存名=>";
			getline(cin, str);
			imwrite(str, pic_col);
			cout << "Saved" << endl;
		}
		else if (key == 'q')
		{
			break;
		}
		if (shMem.getShMemData(3) != 0)
		{
			for (int height = 0; height < pic_col.rows; height++){
				for (int width = 0; width < pic_col.cols; width++){
					Vec3b bgr = pic_col.at<Vec3b>(height, width);
					if (bgr[0] == 0 && bgr[1] == 0 && bgr[2] == 255)
						pic_col.at<Vec3b>(height, width) = Vec3b(0,0,0);
				}
			}
			pic_col.at<Vec3b>(xy[1], xy[0]) = Vec3b(0, 255, 0);
			circle(pic_col , cv::Point(xy[0], xy[1]), 8, cv::Scalar(0, 255, 0), 2);
			shMem.setShMemData(0, 3);
		}
		imshow("show", pic_col);
	}


}