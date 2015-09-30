#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <cstdlib>

#include "SharedMemory.h"

using namespace cv;
using namespace std;

Mat pic;
Mat pic_col;
Mat pic_tmp;

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
		shMem.setShMemData(x * 5, 1);
		shMem.setShMemData(y * 5, 2);

		pic_tmp = pic_col.clone();
		pic_tmp.at<Vec3b>(xy[1], xy[0]) = Vec3b(150, 50, 200);
		circle(pic_tmp, cv::Point(xy[0], xy[1]), 4, cv::Scalar(150, 50, 200), 2);
		imshow("show", pic_tmp);

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

void imshow( string& name , Mat& mat , Size size )
{
	Mat showmat;
	resize(mat, showmat, size);
	imshow(name, showmat);
}


void main(int argc, char* argv[])
{
	pic = imread(argv[1] , 0);

	resize(pic, pic, Size(1000, 1000));

	pic_col = Mat(pic.rows, pic.cols, CV_8UC3);
	cvtColor(pic, pic_col, CV_GRAY2BGR);
	pic_tmp = pic_col.clone();

	namedWindow("show", 1 );

	cv::setMouseCallback("show", onMouse, 0);

	imshow("show", pic_col);

	int key;
	while (true){
		key = waitKey(5);
		if (key == 's'){//保存
			string str;
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
		//点が確定したとき
		if (shMem.getShMemData(3) != 0 )
		{
			pic_col.at<Vec3b>(xy[1], xy[0]) = Vec3b(0, 200, 0);
			circle(pic_col, cv::Point(xy[0], xy[1]), 6, cv::Scalar(0, 255, 0), 2);
			imshow("show", pic_col);

			shMem.setShMemData(0, 3);
		}
		//imshow("show", pic_col);
	}


}