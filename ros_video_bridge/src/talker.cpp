#include "ros/ros.h"
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

void sleepReconcile(ros::Rate rate)
{
	int time = (int)((rate.expectedCycleTime() - rate.cycleTime()).toSec() * 1000);
	waitKey(time);
	ROS_DEBUG("Slept: %d milliseconds",time);
	rate.reset();
}

int main(int argc, char ** argv)
{
	VideoCapture cap(0);
	if (!cap.isOpened())  // if not success, exit program
    	{
        	ROS_ERROR("Cannot open the video cam");
        	return -1;
    	}

	std::vector<uchar> data;
	
	Mat image;

	std_msgs::UInt8MultiArray dat;

	dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
	dat.layout.dim[0].label = "img_data";
	dat.layout.dim[0].stride = 7;
	dat.layout.data_offset = 0;

	ros::init(argc, argv, "talker");

	ros::NodeHandle n;

	ros::Publisher chatter_pub = n.advertise<std_msgs::UInt8MultiArray>("/usb_cam_0/image_jpeg", 1000);

	ros::Rate loop_rate(30);

	while(ros::ok())
	{

		cap >> image;

		//char data[7];
		//data = {1, 2, 3, 4, 5, 6, 7};

		if (!image.empty())
		{

			imencode(".jpg", image, data);

			dat.data = data;

			chatter_pub.publish(dat);

			ros::spinOnce();

			data.clear();
			
			image.clear();

		}
		else
		{
			ROS_WARN("Camera image empty");
		}
		
		sleepReconcile(loop_rate);

	}

	return 0;
}
