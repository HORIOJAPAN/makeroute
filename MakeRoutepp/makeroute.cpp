#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <cstdlib>
#include <fstream>

#include "SharedMemory.h"

using namespace cv;
using namespace std;

// �摜
Mat pic;
Mat pic_col;
Mat pic_tmp;
Mat pic_zoom;

// �T�C�Y��_�Ȃ�
Size zoomSize(800,600);
Size reductionRate;
Point zoomPoint;
Point prePoint(-1,-1);

// ���L�������n
SharedMemory<int> shMem("CoordinateOfMap");
enum { ISCOODDECISION , COOD_X , COOD_Y , ISCONFIRMED , ISEXIT , ISINSERT};

// �R�[���o�b�N�Ŏ擾���ꂽ���W
int xy[2] = { 0, 0 };

// �摜�����T�C�Y���ĕ`��
void imshowResize(string& name, Mat& mat, Size size)
{
	Mat showmat;
	resize(mat, showmat, size);

	imshow(name, showmat);
}

// �o�H����ǂݍ��ݒ����ĉ摜�ɔ��f����
void reloadRoute(string path)
{
	cvtColor(pic, pic_tmp, CV_GRAY2BGR);
	Point prePoint(-1, -1);

	int	x_val, y_val;

	string str, x_str, y_str;
	string searchWord(",");
	string::size_type x_pos, y_pos;

	// cs�ŕۑ������o�H���̈ꎞ�t�@�C����ǂݍ���
	ifstream ifs(path + ".tmp");
	if (ifs.fail())
	{
		cerr << "False" << endl;
		shMem.setShMemData(false, ISINSERT);
		return;
	}
	//�w�b�_�������Ƃ΂�
	getline(ifs, str);

	while (getline(ifs, str))
	{
		//�擪����,�܂ł̕������int�^�Ŏ擾
		x_pos = str.find(searchWord);
		if (x_pos != string::npos){
			x_str = str.substr(0, x_pos);
			x_val = stoi(x_str);
		}

		//x�̒l�̌�납��,�܂ł̕������int�^�Ŏ擾
		y_pos = str.find(searchWord, x_pos + 1);
		if (y_pos != string::npos){
			y_str = str.substr(x_pos + 1, y_pos);
			y_val = stoi(y_str);
		}

		//�擾����[x,y]���摜�ɔ��f
		pic_tmp.at<Vec3b>(y_val, x_val ) = Vec3b(0, 255, 0);
		circle(pic_tmp, cv::Point(x_val, y_val), 10, cv::Scalar(0, 255, 0), 3);
		if (prePoint.x > 0 && prePoint.y > 0) line(pic_tmp, prePoint, Point(x_val, y_val), cv::Scalar(0, 0, 200), 3, 4);

		prePoint = Point(x_val, y_val);
	}
	pic_col = pic_tmp.clone();
	rectangle(pic_tmp, zoomPoint, Point(zoomPoint.x + zoomSize.width, zoomPoint.y + zoomSize.height), cv::Scalar(0, 0, 200), 5, 4);

	imshowResize(string("Origin"), pic_tmp, Size(pic_tmp.cols / reductionRate.width, pic_tmp.rows / reductionRate.height));

	// zoom�摜�ɓ_��`��
	pic_zoom = Mat(pic_col, Rect(zoomPoint, zoomSize));
	imshow("zoom", pic_zoom);

	::prePoint = prePoint;

	shMem.setShMemData(false, ISCONFIRMED);
	shMem.setShMemData(false, ISINSERT);
}

// ���N���b�N�Ōo�H���W��I��
void coodClick(int event, int x, int y, int flag, void* )
{

	// �}�E�X�C�x���g���擾
	switch (event) {
	case cv::EVENT_MOUSEMOVE:
		break;
	case cv::EVENT_LBUTTONDOWN:
		// �N���b�N���ꂽ���W���O���[�o���ϐ��ɕۑ�
		xy[0] = x;
		xy[1] = y;

		// ���W�����L�������ɕۑ�
		shMem.setShMemData(true, ISCOODDECISION);
		shMem.setShMemData(x + zoomPoint.x, COOD_X);
		shMem.setShMemData(y + zoomPoint.y, COOD_Y);

		// ���W�ɓ_��ł�(�m��ł͂Ȃ��̂ňꎞ�摜�ɑ}�����ĕ\��)
		pic_tmp = pic_zoom.clone();
		pic_tmp.at<Vec3b>(xy[1], xy[0]) = Vec3b(150, 50, 200);
		circle(pic_tmp, Point(xy[0], xy[1]), 4, Scalar(150, 50, 200), 2);
		imshow("zoom", pic_tmp);

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

// ���N���b�N��zoom�̈ʒu��ύX
void zoomCoodClick(int event, int x, int y, int flag, void*)
{

	// �}�E�X�C�x���g���擾
	switch (event) {
	case cv::EVENT_LBUTTONDOWN:
		// zoom�摜�̍���̍��W
		zoomPoint = Point(x * reductionRate.width - zoomSize.width / 2, y * reductionRate.height - zoomSize.height / 2);

		// zoom�摜�̍��W�����摜�𒴂��Ȃ��悤�Ƀ`�F�b�N
		zoomPoint.x < 0 ? zoomPoint.x = 0 : 0;
		zoomPoint.x + zoomSize.width > pic.cols ? zoomPoint.x = pic.cols - zoomSize.width : 0;
		zoomPoint.y < 0 ? zoomPoint.y = 0 : 0;
		zoomPoint.y + zoomSize.height > pic.rows ? zoomPoint.y = pic.rows - zoomSize.height : 0;


		// zoom�����ʒu��Ԃň͂�
		pic_tmp = pic_col.clone();
		rectangle(pic_tmp, zoomPoint, Point(zoomPoint.x + zoomSize.width, zoomPoint.y + zoomSize.height), cv::Scalar(0, 0, 200), 5, 4);
		imshowResize(string("Origin"), pic_tmp, Size(pic_tmp.cols / reductionRate.width, pic_tmp.rows / reductionRate.height));

		// zoom�摜���쐬���ĕ\��
		pic_zoom = Mat(pic_col, Rect(zoomPoint, zoomSize));
		imshow("zoom", pic_zoom);

		break;
	}

}

void main(int argc, char* argv[])
{
	// �R�}���h���C����������p�X���擾���ĉ摜��ǂݍ���
	pic = imread(argv[1] , 0);

	// ����
	pic_col = Mat(pic.rows, pic.cols, CV_8UC3);
	cvtColor(pic, pic_col, CV_GRAY2BGR);
	pic_tmp = pic_col.clone();
	pic_zoom = Mat(pic_col, Rect((pic_col.cols - zoomSize.width) / 2, (pic_col.rows - zoomSize.height) / 2,  zoomSize.width, zoomSize.height));

	// �S�̉摜�̏k�����D�c����Œ�̂���zoomSize�̑傫���ق��݂̂Ō��߂�
	int largerSize = zoomSize.width > zoomSize.height ? zoomSize.width : zoomSize.height;
	reductionRate = Size(pic.cols / largerSize, pic.rows / largerSize);

	// �S�̉摜���k�����ĕ\��
	namedWindow("Origin", 1);
	cv::setMouseCallback("Origin", zoomCoodClick, 0);
	imshowResize(string("Origin"), pic_col,Size(pic.cols/reductionRate.width,pic.rows/reductionRate.height));

	// �Y�[���摜��\��
	namedWindow("zoom", 1 );
	cv::setMouseCallback("zoom", coodClick, 0);
	imshow("zoom", pic_zoom);

	int key;
	while (true){
		key = waitKey(5);
		if (key == 's'){//�ۑ�
			string str;
			cout << "s" << endl;
			cout << "�ۑ���=>";
			getline(cin, str);
			imwrite(str, pic_col);
			cout << "Saved" << endl;
		}
		else if (key == 'q' || shMem.getShMemData(ISEXIT))
		{
			string imgname = argv[1];
			imgname += ".jpg";
			imwrite(imgname, pic_col);
			cout << "Saved" << endl;
			shMem.setShMemData(false ,ISEXIT);
			break;
		}
		//�_���m�肵���Ƃ�
		if (shMem.getShMemData( ISCONFIRMED ) != 0 )
		{
			// �m��摜�ɂ��_��`��
			pic_col.at<Vec3b>(xy[1] + zoomPoint.y, xy[0] + zoomPoint.x) = Vec3b(0, 255, 0);
			circle(pic_col, cv::Point(xy[0] + zoomPoint.x, xy[1] + zoomPoint.y), 10, cv::Scalar(0, 255, 0), 3);
			if (prePoint.x > 0 && prePoint.y > 0) line(pic_col, prePoint, Point(xy[0] + zoomPoint.x, xy[1] + zoomPoint.y), cv::Scalar(0, 0, 200), 3, 4);
			pic_tmp = pic_col.clone();
			rectangle(pic_tmp, zoomPoint, Point(zoomPoint.x + zoomSize.width, zoomPoint.y + zoomSize.height), cv::Scalar(0, 0, 200), 5, 4);

			imshowResize(string("Origin"), pic_tmp, Size(pic_tmp.cols / reductionRate.width, pic_tmp.rows / reductionRate.height));

			prePoint = Point(xy[0] + zoomPoint.x, xy[1] + zoomPoint.y);

			// zoom�摜�ɓ_��`��
			pic_zoom = Mat(pic_col, Rect(zoomPoint, zoomSize));
			imshow("zoom", pic_zoom);

			shMem.setShMemData( false , ISCONFIRMED );
		}

		// cs���Ōo�H��}�������甽�f�̂��߂Ɍo�H����ǂ݂��ݒ���
		if (shMem.getShMemData(ISINSERT))
		{
			reloadRoute(argv[1]);
		}

	}
}