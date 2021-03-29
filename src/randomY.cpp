#include "ros/ros.h"
#include "arbtx_ros_training/Num.h"


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "randomY");

  ros::NodeHandle n;
  
  ros::Publisher randomY_pub = n.advertise<arbtx_ros_training::Num>("Y_Cord", 1000);

  ros::Rate loop_rate(1);

  while (ros::ok())
  {
    arbtx_ros_training::Num msg;
    
    msg.num = rand() % 100;
    
    randomY_pub.publish(msg);
    
    ros::spinOnce();
    
    loop_rate.sleep();
    
  }
  return 0;
}
