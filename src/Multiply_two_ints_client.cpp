#include "ros/ros.h"
#include "arbtx_ros_training/MultiplyTwoInts.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "multiply_two_ints_client");
  if (argc != 3)
  {
    ROS_INFO("usage: multiply_two_ints_client X Y");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<arbtx_ros_training::MultiplyTwoInts>("multiply_two_ints");
  arbtx_ros_training::MultiplyTwoInts srv;
  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);
  if (client.call(srv))
  {
    ROS_INFO("tiplication: %ld", (long int)srv.response.mult);
  }
  else
  {
    ROS_ERROR("Failed to call service multiply_two_ints");
    return 1;
  }

  return 0;
}
