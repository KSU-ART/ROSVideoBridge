#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8MultiArray.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
Mat* img;
void downCamCB(const std_msgs::UInt8MultiArray::ConstPtr& msg)
{
	/// Load source image and convert it to gray
	//for(std::vector<char>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	ROS_INFO("GOT: %d", msg->data.size());
	Mat dec = imdecode(msg->data, 1);
	imshow("Smile", dec);
	waitKey(33);
}

int main(int argc, char **argv){
	img = 0;
	ros::init(argc, argv, "listener");
	ros::NodeHandle n;
	//namedWindow("Smile");
	
	ros::Subscriber sub = n.subscribe("/usb_cam_0/image_jpeg", 1000, downCamCB);
	
	//~ while(ros::ok()){
		//~ if(img != 0 && img->size[0]>0 &&  img->size[1]>0)
			//~ imshow("Smile", *img);
		//~ waitKey(33);
		//~ ros::spinOnce();
	//~ }
	ros::spin();
	
	return 0;
}
