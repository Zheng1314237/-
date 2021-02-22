// ch.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include "pch.h"
#include <iostream>
#include "Define.h"
#include "Opencv.h"


#define PIXEL_PER_GRID 48//һ������ռ50����

using namespace std;
using namespace cv;

bool opencvRun = false;

//�����µ�ȫ�����̣����Ӻͺ�����Ϣ�����棩
unsigned char chessBoardNew[10][9] = { {   B_CAR, B_HORSE, B_ELEPHANT, B_BISHOP,  B_KING, B_BISHOP, B_ELEPHANT, B_HORSE,   B_CAR },
									   { NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS, NOCHESS },
									   { NOCHESS, B_CANON,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, B_CANON, NOCHESS },
									   {  B_PAWN, NOCHESS,     B_PAWN,  NOCHESS,  B_PAWN,  NOCHESS,     B_PAWN, NOCHESS,  B_PAWN },
									   { NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS, NOCHESS },
	//------------------����-----------------------------------����------------------//
	{ NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS, NOCHESS },
	{  R_PAWN, NOCHESS,     R_PAWN,  NOCHESS,  R_PAWN,  NOCHESS,     R_PAWN, NOCHESS,  R_PAWN },
	{ NOCHESS, R_CANON,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, R_CANON, NOCHESS },
	{ NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS, NOCHESS },
	{   R_CAR, R_HORSE, R_ELEPHANT, R_BISHOP,  R_KING, R_BISHOP, R_ELEPHANT, R_HORSE,   R_CAR } };
unsigned char chessBoardLast[10][9];//�����ϴ�ȫ������

unsigned char chessBoardRTScan[10][9];//����opencvʵʱɨ������̣�Ҫ�ж��Ƿ���ȷ�����ʹ��

//�����µ�������̣�ֻ�����û�������Ϣ��
unsigned char userChessBoardNew[10][9] = { { NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
											{ NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
											{ NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
											{ NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
											{ NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
	//------------------����-----------------------------------����------------------//
	{ NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
	{  R_PAWN, NOCHESS,  R_PAWN, NOCHESS,  R_PAWN, NOCHESS,  R_PAWN, NOCHESS,  R_PAWN },
	{ NOCHESS, R_CANON, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, R_CANON, NOCHESS },
	{ NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS, NOCHESS },
	{   R_CAR, R_HORSE, R_ELEPHANT, R_BISHOP, R_KING, R_BISHOP, R_ELEPHANT, R_HORSE, R_CAR } };
unsigned char userChessBoardLast[10][9];////�����ϴ��������

int redChessNum = 0;//�����ʣ���ɫ��������
int blackChessNum = 0;//�����ʣ���ɫ��������
char userMovePath[4];//�����������·�ߣ��ڵ���������

void findTrianglePlus(Mat& img, Mat& result) {
	Mat temp;
	img.copyTo(temp);
	Point2f srcPoints[4];//�ĸ��ǵ��λ��
	Point2f dstPoints[4];//�任�������λ��
	//�����λ�� ���� ���� ���� ����
	dstPoints[0] = Point2f(0, 0);
	dstPoints[1] = Point2f(520, 0);
	dstPoints[2] = Point2f(520, 470);
	dstPoints[3] = Point2f(0, 470);
	srcPoints[0] = Point2f(344, 255);
	srcPoints[1] = Point2f(957, 255);
	srcPoints[2] = Point2f(906, 716);
	srcPoints[3] = Point2f(397, 719);
	Mat transMat = getPerspectiveTransform(srcPoints, dstPoints);
	warpPerspective(img, temp, transMat, Size(520, 470));
	result = temp;
}
void findTriangle(Mat& img, Mat& result) {
	Mat temp;
	img.copyTo(temp);
	Mat gaussImage;
	//��˹�˲�
	GaussianBlur(img, gaussImage, Size(5, 5), 0);

	Mat grayImage;
	cvtColor(gaussImage, grayImage, CV_BGR2GRAY);
	//adaptiveThreshold(grayImage, grayImage, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, 6);
	imwrite("1.jpg", grayImage);
	Mat cannyImage;
	Canny(grayImage, cannyImage, 60, 180, 3);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	dilate(cannyImage, cannyImage, element);
	erode(cannyImage, cannyImage, element);

	vector< vector< Point> > contours;
	findContours(cannyImage,
		contours, // a vector of contours
		CV_RETR_EXTERNAL, // ��ȡ�ⲿ����
		CV_CHAIN_APPROX_NONE); // all pixels of each contours
//	result = grayImg;

	int max = 0;
	int index = 0;
	//���Ǹ���������������������ɸѡ
	/*
	vector<Point> maxArea;
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> p;
		p = contours[i];
		printf("%d\n", p.size());
		if (p.size() > max)
		{
			max = p.size();

			maxArea = p;
			index = i;
		}
	}
	*/
	//�����������������ָ�
	vector<Point> maxArea;
	vector<Point> maxAreaTemp;
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> p;
		p = contours[i];
		if (p.size() > 1500 && p.size() < 3000)
		{
			max = p.size();
			maxArea = p;
			index = i;
		}
	}


	/*�����������
	for (size_t i = 0; i < maxArea.size(); i++)
	{
		circle(temp, maxArea[i], 1, Scalar(240, 255, 25));
	}
	imshow("temp", temp);
	*/
	//��������͹��
	vector<Point> hull;
	vector<Point> approx;
	convexHull(maxArea, hull);
	double epsilon = 0.02 * arcLength(maxArea, true);
	approxPolyDP(hull, approx, epsilon, true);
	for (int i = 0; i < approx.size(); i++) {
		circle(temp, approx[i], 2, Scalar(255, 255, 0), 5);
	}
	imwrite("tmp.jpg", temp);

	//����ӳ��
	Point2f srcPoints[4];//�ĸ��ǵ��λ��
	Point2f dstPoints[4];//�任�������λ��
	//�����λ�� ���� ���� ���� ����
	dstPoints[0] = Point2f(0, 0);
	dstPoints[1] = Point2f(520, 0);
	dstPoints[2] = Point2f(520, 470);
	dstPoints[3] = Point2f(0, 470);
	bool sorted = false;
	int n = 4;
	while (!sorted) {
		sorted = true;
		for (int i = 1; i < n; i++) {
			if (approx[i - 1].x > approx[i].x) {
				swap(approx[i - 1], approx[i]);
				sorted = false;
			}
		}
		n--;
	}
	if (approx[0].y < approx[1].y) {
		srcPoints[0] = approx[0];
		srcPoints[3] = approx[1];
	}
	else {
		srcPoints[0] = approx[1];
		srcPoints[3] = approx[0];
	}

	if (approx[2].y < approx[3].y) {
		srcPoints[1] = approx[2];
		srcPoints[2] = approx[3];
	}
	else {
		srcPoints[1] = approx[3];
		srcPoints[2] = approx[2];
	}

	for (int i = 0; i < 4; ++i) {
		printf("%f,%f\n", srcPoints[i].x, srcPoints[i].y);
	}

	//srcPoints[0] = Point2f(356,206);
	//srcPoints[1] = Point2f(980, 184);
	//srcPoints[2] = Point2f(932, 665);
	//srcPoints[3] = Point2f(423, 670);
	Mat transMat = getPerspectiveTransform(srcPoints, dstPoints);
	warpPerspective(img, temp, transMat, Size(520, 470));
	result = temp;
}


void opencv_test() {
	VideoCapture capture(1);

	Mat showImg;//������ʾ��ͼ��
	Mat sourceImg;//ԭʼͼ��
	Mat rotateImg;//��ת������������ͼ��
	Mat medianFilterImg;//��ֵ�˲�ͼ��
	Mat grayImg;//�Ҷ�ͼ
	Mat erodeImg;//��ʴͼ��
	Mat element;//��ʴ���ͽṹԪ
	Mat erzhihua(40, 40, CV_8UC3);//62*62�ֱ��ʣ�����ָ�������ͼ�����ڷ���ÿ������

	vector<Vec3f> pcircles;//�������Բ��⵽�Ľ��
	int redChessCount = 0;//ͳ�ƺ�ɫ��������
	int blackChessCount = 0;//ͳ�ƺ�ɫ��������
	int pixelNum = 0;//���������ۼƱ���
	int pixelCount = 0;//����ֵ�ۼƱ���

	char st[10];//���ڴ��֡�ʵ��ַ���
	double t;//��ʱ����������֡��

	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	capture.set(CV_CAP_PROP_FPS, 30);//֡��
	//��ӡ����ͷ����
	printf("width = %.2f\n", capture.get(CV_CAP_PROP_FRAME_WIDTH));
	printf("height = %.2f\n", capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("fbs = %.2f\n", capture.get(CV_CAP_PROP_FPS));
	printf("brightness = %.2f\n", capture.get(CV_CAP_PROP_BRIGHTNESS));
	printf("contrast = %.2f\n", capture.get(CV_CAP_PROP_CONTRAST));
	printf("saturation = %.2f\n", capture.get(CV_CAP_PROP_SATURATION));
	printf("hue = %.2f\n", capture.get(CV_CAP_PROP_HUE));
	printf("exposure = %.2f\n", capture.get(CV_CAP_PROP_EXPOSURE));

	while (true)
	{
		t = (double)cv::getTickCount();//��¼��ʼʱ��

		capture >> sourceImg;//��һ֡ԭʼͼ��

		//��ԭʼͼ����з��α任

		findTrianglePlus(sourceImg, sourceImg);

		Rect ROI(0, 0, 470, 520);//����Ȥ����ȥ������ͷ��Ұ�в���Ҫ���ķ�Χ
		Rect board0(40, 40, 390, 436);//��������Ȧ���ο�
		transpose(sourceImg, sourceImg);//����

		flip(sourceImg, sourceImg, 0);//��ת

		sourceImg(ROI).copyTo(rotateImg);//����Ѱ������ROI����ȥ��������Ұ

		cvtColor(rotateImg, grayImg, CV_BGR2GRAY);//RGBת�Ҷ�ͼ

		//threshold(grayImg, grayImg, 128, 255, CV_THRESH_BINARY);//��ֵ��
		cv::imwrite("gray.jpg", rotateImg);
		medianBlur(grayImg, medianFilterImg, 3);//��ֵ�˲�����3
		element = getStructuringElement(MORPH_RECT, Size(2, 2), Point(1, 1));
		//dilate(grayImg, frame, element);//����
		//erode(medianFilterImg, erodeImg, element);//��ʴ

		memset(chessBoardRTScan, NOCHESS, sizeof(chessBoardRTScan));//��ʵʱɨ��������գ�׼�������µ���������

		//���룬��������������ͣ�-HOUGH_GRADIENT��dp(dp=1ʱ��ʾ����ռ�������ͼ��ռ�Ĵ�Сһ�£�dp=2ʱ����ռ�������ͼ��ռ��һ�룬�Դ�����)����̾���-���Էֱ�������Բ�� ����Ϊ��ͬ��Բ ,��Ե���ʱʹ��Canny���ӵĸ���ֵ�����ĵ��ۼ�����ֵ����ѡԲ�ģ�����ռ����ۼӺʹ��ڸ���ֵ�ĵ�Ͷ�Ӧ��Բ�ģ�����⵽Բ����С�뾶����⵽Բ�ĵ����뾶
		//
		HoughCircles(medianFilterImg, pcircles, CV_HOUGH_GRADIENT, 1, 20, 25, 20, 15, 25);
		bool circleFlag = (pcircles.size() > 0);
		if (circleFlag) {
			for (int c = 0; c < pcircles.size(); c++)//��������ʶ�𵽵�����
			{
				Point center((int)(pcircles[c][0] + 0.5), (int)(pcircles[c][1] + 0.5));//��ȡԲ�����꣬��������
				int radius = (int)(pcircles[c][2] + 0.5);//��ȡ�뾶����������
				Rect rec(center.x - 20, center.y - 20, 40, 40);//����һ����ס���ӵķ���30*30��С

				//cv::circle(rotateImg, center, radius, Scalar(0, 255, 0), 2);

				if (!(ROI.contains(rec.tl()) && ROI.contains(rec.br()))) continue;//������ӹ��ڿ�����Ե��rec��ȡ�ľ���ͼ�񽫻ᳬ����Χ�������������

				//////��ͼ��ת��ΪHSV�ռ����
				//Mat src_hsv;
				//Mat V;
				//cvtColor(rotateImg  (rec), src_hsv, CV_BGR2HSV);
				//vector<Mat> hsvSplit;
				//split(src_hsv, hsvSplit);
				//equalizeHist(hsvSplit[2], hsvSplit[2]);
				//merge(hsvSplit, src_hsv);
				//inRange(src_hsv, Scalar(0, 46, 50), Scalar(180, 255,190), V); //Threshold the image
				//V.copyTo(erzhihua);//���Ҷ�ͼ�е������ӵľֲ�ͼ���Ƶ�erzhihua���з���

				grayImg(rec).copyTo(erzhihua);//���Ҷ�ͼ�е������ӵľֲ�ͼ���Ƶ�erzhihua���з���

				pixelNum = pixelCount = 0;//����ֵ�ۼƱ���

				for (int i = 0; i < 40; i++)//��������Բ��⵽��Բ����Χ30*30��������������
				{
					for (int j = 0; j < 40; j++)
					{
						if (i + j < 12 || i + j>68 || i > j + 28 || j > i + 28) continue;//ȥ��9��������,�ƽ�Բ��

						if (erzhihua.at<uchar>(i, j) > 100)//100Ϊ�趨�ľ�̬��ֵ����ֵ�����޸�Ϊ��̬��ֵ��********************************************
							erzhihua.at<uchar>(i, j) = 255;//��ɫ
						else
						{
							pixelNum++;
							pixelCount += rotateImg.at<Vec3b>(rec.y + i, rec.x + j)[2];
							erzhihua.at<uchar>(i, j) = 0;//��ɫ
						}
					}
				}

				//	cv::imshow("�������ֵ����", erzhihua);

				if (pixelNum == 0) continue;//û�м�⵽����ͼ����һ�����и������ڵ���������һ������
				if (pixelCount / pixelNum > 100)//80Ϊ�趨�ľ�̬��ֵ����ֵ�����޸�Ϊ��̬��ֵ��********************************************************
					chessBoardRTScan[(center.y - board0.y + (PIXEL_PER_GRID >> 1)) / PIXEL_PER_GRID] \
					[(center.x - board0.x + (PIXEL_PER_GRID >> 1)) / PIXEL_PER_GRID] = R_CHESS;//��ɫ��ʱһ����Ϊ�Ǻ�˧
				else
					chessBoardRTScan[(center.y - board0.y + (PIXEL_PER_GRID >> 1)) / PIXEL_PER_GRID] \
					[(center.x - board0.x + (PIXEL_PER_GRID >> 1)) / PIXEL_PER_GRID] = B_CHESS;//��ɫ��ʱһ����Ϊ�Ǻڽ�

			cv::circle(rotateImg, center, 4, IsRed(chessBoardRTScan[(center.y - board0.y + (PIXEL_PER_GRID >> 1)) / PIXEL_PER_GRID][(center.x - board0.x + (PIXEL_PER_GRID >> 1)) / PIXEL_PER_GRID]) ? \
					Scalar(0, 255, 0) : Scalar(255, 255, 255), -1, LINE_8, 0);//�ڲ�ͬ�����ϱ�ǲ�ͬ��ɫ�ĵ㣬��ɫ��׵㣬��ɫ���̵�
			}
			cv::imwrite("rotate.jpg", rotateImg);
		}
		if (opencvRun && circleFlag)
		{
			redChessCount = 0;//��������������㣬׼������
			for (int i = 0; i < 10; i++)//ͳ������ͷɨ��ĺ�ɫ����ң���������
			{
				for (int j = 0; j < 9; j++)
					if (IsRed(chessBoardRTScan[i][j])) redChessCount++;
			}

			if (redChessCount == redChessNum)//���������������
			{
				memset(userChessBoardNew, NOCHESS, sizeof(userChessBoardNew));
				memset(userChessBoardLast, NOCHESS, sizeof(userChessBoardLast));
				//ע��˴�chessBoardRTScanΪʵʱ���̣������������ȫ�����̣�chessBoardNewΪ�ϴ����̣������������ȴ��������ʱ��ȫ������
				//userChessBoardNewΪʵʱ������̣������������������̣�userChessBoardLastΪ���������ȴ��������ʱ���������
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 9; j++)
					{
						if (IsRed(chessBoardRTScan[i][j]))
							userChessBoardNew[i][j] = chessBoardRTScan[i][j];//������ͷɨ������ֻ��������Ӹ��Ƶ����������
						if (IsRed(chessBoardNew[i][j]))
							userChessBoardLast[i][j] = chessBoardNew[i][j];//������������������̸��Ƶ�����ϴ�����
					}
				}

				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 9; j++)
					{
						if (IsRed(userChessBoardNew[i][j]) && (userChessBoardLast[i][j] == NOCHESS))//���������յ�
						{
							userMovePath[2] = j;
							userMovePath[3] = i;
							if (IsBlack(chessBoardNew[i][j]))
							{
								blackChessNum--;//������ӣ�������������
								printf("�����յ�(%d,%d)\n", j, i);
							}
							else
								printf("�����յ�(%d,%d)\n", j, i);
						}
						else if (IsRed(userChessBoardLast[i][j]) && (userChessBoardNew[i][j] == NOCHESS))//�����������
						{
							userMovePath[0] = j;
							userMovePath[1] = i;
							printf("�������(%d,%d)\n", j, i);
						}
					}
				}

				opencvRun = false;//��־λ�ر�
			}
		}

		t = ((double)getTickCount() - t) / getTickFrequency();
		sprintf_s(st, "FPS:%.2f", 1.0f / t);
		cv::putText(rotateImg, st, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
		//֡����ʾ����������������������������������������������������������������������������������

		cv::rectangle(rotateImg, board0, Scalar(255, 255, 0), 1, LINE_8, 0);//������������

		cv::rectangle(rotateImg, ROI, Scalar(255, 0, 255), 1, LINE_8, 0);//������������

		//resize(rotateImg, showImg, Size(370, 440), 0, 0, CV_INTER_LINEAR);
		rotateImg.copyTo(showImg);
		int width = rotateImg.cols;
		int height = rotateImg.rows;



		cv::resize(rotateImg, rotateImg, Size(width, height), 0, 0, CV_INTER_LINEAR);
		cv::imshow("OpenCV", showImg);
		if (waitKey(10) == 27) break;
	}
}


int calDistance(Point a, Point b) {
	return  sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

