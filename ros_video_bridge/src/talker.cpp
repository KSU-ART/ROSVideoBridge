#include "ros/ros.h"
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>


using namespace cv;

void sleepReconcile(ros::Rate rate){
	int time = (int)((rate.expectedCycleTime() - rate.cycleTime()).toSec() * 1000);
	waitKey(time);
	ROS_DEBUG("Slept: %d milliseconds",time);
	rate.reset();
}

int main(int argc, char ** argv){
	VideoCapture cap(0);
	Mat image;
	ros::init(argc, argv, "talker");
	
	ros::NodeHandle n;
	
	ros::Publisher chatter_pub = n.advertise<std_msgs::UInt8MultiArray>("/usb_cam_0/image_jpeg", 1000);
	
	ros::Rate loop_rate(30);
	
	int count = 0;
	while(ros::ok()){
				
		cap >> image;
		//char data[7];
		//data = {1, 2, 3, 4, 5, 6, 7};
		
		std_msgs::UInt8MultiArray dat;
		
		dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
		dat.layout.dim[0].label = "img_data";
		//dat.layout.dim[0].size = 7;
		dat.layout.dim[0].stride = 7;
		dat.layout.data_offset = 0;
		//dat.data = data;
		dat.data.clear();
		
		std::vector<uchar> data;
		
		imencode(".jpg", image, data);
		
		dat.data = data;
		
		
				
		chatter_pub.publish(dat);
		
		ros::spinOnce();
		//imwrite("~/image.png",image);
		//Canny(image, image, 100, 250, 3);
		//cvtColor(image, image, COLOR_BGR2HSV);
		sleepReconcile(loop_rate);
		++count;
	}
	
	return 0;
}
