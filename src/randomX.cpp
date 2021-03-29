#include "ros/ros.h"
#include "arbtx_ros_training/Num.h"


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "randomX");

  ros::NodeHandle n;
  
  ros::Publisher randomX_pub = n.advertise<arbtx_ros_training::Num>("X_Cord", 1000);

  ros::Rate loop_rate(1);
  
  while (ros::ok())
  {
    arbtx_ros_training::Num msg;
    
    msg.num = rand() % 100;
    
    randomX_pub.publish(msg);
    
    ros::spinOnce();
    
    loop_rate.sleep();
    
  }
  return 0;
}
