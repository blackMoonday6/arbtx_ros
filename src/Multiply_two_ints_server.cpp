#include "ros/ros.h"
#include "arbtx_ros_training/MultiplyTwoInts.h"

bool multiplying_callback(arbtx_ros_training::MultiplyTwoInts::Request  &req,
         arbtx_ros_training::MultiplyTwoInts::Response &res)
{
  res.mult = req.a * req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.mult);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "multiply_two_ints_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("multiply_two_ints", multiplying_callback);
  ROS_INFO("Ready to multiply two ints.");
  ros::spin();

  return 0;
}
