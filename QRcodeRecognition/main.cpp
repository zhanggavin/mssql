// OPENCVTEST01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <iterator>

#include <zbar.h>  
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

std::vector<std::string> qrChars;
//Mark Bound
std::vector<std::vector<cv::Point>> rect;
std::vector<cv::Rect2d> bbox;

int main(int argc, char*argv[])
{

	//定义扫描器
	zbar::ImageScanner scanner;
	scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
	//scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
	clock_t start = clock(); // 记录开始时间
	//加载图片
	cv::Mat image = cv::imread("test04.jpg");//图片路径或相对路径
	if (!image.data)
	{
		std::cout<<"输入合适图片："<< std::endl;
		system("pause");
		return 0;
	}
	cv::Mat imageCopy = image.clone();
	//图片转换
	cv::Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;

	zbar::Image imageZbar(width, height, "Y800", raw, width * height);
	int n = scanner.scan(imageZbar); //扫描条码    
	zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();

	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		std::cout << "失败，请检查图片！" << std::endl;
	}
	if (n > 0)
	{
		for (; symbol != imageZbar.symbol_end(); ++symbol)
		{
			std::cout << "类型：" << symbol->get_type_name() << std::endl;
			std::cout << "内容：" << symbol->get_data() << std::endl << std::endl;

			int pos = 0;
			auto data = symbol->get_data();
			std::vector<std::string>::iterator itemPosIte = std::find(qrChars.begin(), qrChars.end(), data);
			if (itemPosIte != qrChars.end())
			{
				pos = std::distance(qrChars.begin(), itemPosIte);
			}
			else
			{
				qrChars.push_back(data);
				bbox.push_back(cv::Rect2d(0, 0, 0, 0));
				rect.push_back(std::vector<cv::Point>());
				pos = std::distance(qrChars.begin(), qrChars.end()) - 1;
			}
			int minX, maxX, minY, maxY;
			minX = symbol->get_location_x(0);
			maxX = symbol->get_location_x(0);
			minY = symbol->get_location_y(0);
			maxY = symbol->get_location_y(0);
			int t = symbol->get_location_size();
			for (int i = 1; i < symbol->get_location_size(); i++)
			{
				if (symbol->get_location_x(i) < minX) minX = symbol->get_location_x(i);
				if (symbol->get_location_x(i) > maxX) maxX = symbol->get_location_x(i);
				if (symbol->get_location_y(i) < minY) minY = symbol->get_location_y(i);
				if (symbol->get_location_y(i) > maxY) maxY = symbol->get_location_y(i);
			}
			
			bbox[pos] = cv::Rect2d(cv::Point(minX, minY),cv::Point(maxX, maxY));
			cv::rectangle(imageCopy, bbox[pos], cv::Scalar(255, 0, 0), 2);
		}
	}
 
	clock_t finish = clock();  // 记录程序结束时间
	double time_length = (double)(finish - start) / CLOCKS_PER_SEC; //计算出扫描的时间差  
	std::cout << "扫描耗时 " << time_length << " seconds." << std::endl;

	//显示二维码
	cv::imshow("原码图", image);
	cv::imshow("码图定位", imageCopy);

	cv::waitKey();
	imageZbar.set_data(NULL, 0);//清除缓存

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
