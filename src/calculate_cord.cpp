#include "ros/ros.h"
#include "arbtx_ros_training/Num.h"
#include "arbtx_ros_training/AddTwoInts.h"
#include "arbtx_ros_training/MultiplyTwoInts.h"
#include <iostream>
using namespace std;
class Cord
{
    public:
    static int Xcord;
    static int Ycord;
    static int Sum;
    static int Multi;
    static int Result;
};

int Cord :: Xcord ;
int Cord :: Ycord ;
int Cord :: Sum ;
int Cord :: Multi ;
int Cord :: Result ;

ros::ServiceClient *client1Ptr;
ros::ServiceClient *client2Ptr;
ros::ServiceClient *client3Ptr;

ros::Publisher *plublisherPtr;

void Y_CordCallback(const arbtx_ros_training::Num::ConstPtr& msg)
{	
  Cord :: Ycord = msg->num;
  arbtx_ros_training::Num resToPub;
  arbtx_ros_training::AddTwoInts srv1;
  arbtx_ros_training::MultiplyTwoInts srv2;
  arbtx_ros_training::AddTwoInts srv3;
  
  srv1.request.a = Cord :: Ycord;
  srv1.request.b = Cord :: Xcord;
  
  srv2.request.a = Cord :: Ycord;
  srv2.request.b = Cord :: Xcord;
  
  srv3.request.a = Cord :: Sum;
  
  srv3.request.b = Cord :: Multi;
  
  ros::ServiceClient client1 = (ros::ServiceClient)*client1Ptr;
  ros::ServiceClient client2 = (ros::ServiceClient)*client2Ptr;
  ros::ServiceClient client3 = (ros::ServiceClient)*client3Ptr;
  
  ros::Publisher result_pub = (ros::Publisher)*plublisherPtr;
  
  if ((client1.call(srv1))&&(client2.call(srv2)))
  { 
    ROS_INFO("sum1: %ld", (long int)srv1.response.sum);
    ROS_INFO("mult: %ld", (long int)srv2.response.Mult);
    Cord :: Sum = srv1.response.sum;
    Cord :: Multi = srv2.response.Mult;
    if(client3.call(srv3))
    {
    ROS_INFO("sum2: %ld", (long int)srv3.response.sum);
    Cord :: Result = srv3.response.sum;
    resToPub.num = srv3.response.sum;
    result_pub.publish(resToPub);
    }
  }
  else
  {
    ROS_ERROR("Can't to call service add_two_ints");
  }
}

void X_CordCallback(const arbtx_ros_training::Num::ConstPtr& msg)
{
  Cord :: Xcord = msg->num;
  arbtx_ros_training::Num resToPub;
  arbtx_ros_training::AddTwoInts srv1;
  arbtx_ros_training::MultiplyTwoInts srv2;
  arbtx_ros_training::AddTwoInts srv3;
  
  srv1.request.a = Cord :: Ycord;
  srv1.request.b = Cord :: Xcord;
  
  srv2.request.a = Cord :: Ycord;
  srv2.request.b = Cord :: Xcord;
  
  srv3.request.a = Cord :: Sum;
  srv3.request.b = Cord :: Multi;
  
  ros::ServiceClient client1 = (ros::ServiceClient)*client1Ptr;
  ros::ServiceClient client2 = (ros::ServiceClient)*client2Ptr;
  ros::ServiceClient client3 = (ros::ServiceClient)*client3Ptr;
  
  ros::Publisher result_pub = (ros::Publisher)*plublisherPtr;
  
  if ((client1.call(srv1))&&(client2.call(srv2)))
  { 
    ROS_INFO("sum1: %ld", (long int)srv1.response.sum);
    ROS_INFO("mult: %ld", (long int)srv2.response.Mult);
    Cord :: Sum = srv1.response.sum;
    Cord :: Multi = srv2.response.Mult;
    if(client3.call(srv3))
    {
    ROS_INFO("sum2: %ld", (long int)srv3.response.sum);
    Cord :: Result = srv3.response.sum;
    resToPub.num = srv3.response.sum;
    result_pub.publish(resToPub);
    }
  }
  else
  {
    ROS_ERROR("Failed to call service add_two_ints");
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "calculate_cord");
  
  arbtx_ros_training::Num msg;
  ros::NodeHandle n;
  
  ros::Subscriber sub1 = n.subscribe("Y_Cord", 1000, Y_CordCallback);
  ros::Subscriber sub2 = n.subscribe("X_Cord", 1000, X_CordCallback);
  
  
  ros::ServiceClient client1 = n.serviceClient<arbtx_ros_training::AddTwoInts>("add_two_ints");
  ros::ServiceClient client2 = n.serviceClient<arbtx_ros_training::MultiplyTwoInts>("multiply_two_ints");
  ros::ServiceClient client3 = n.serviceClient<arbtx_ros_training::AddTwoInts>("add_two_ints");
  
  client1Ptr = &client1;
  client2Ptr = &client2;
  client3Ptr = &client3;
  
  
  
  ros::Publisher result_pub = n.advertise<arbtx_ros_training::Num>("result", 1000);
  
  plublisherPtr = &result_pub;
  ros::Rate loop_rate(1);
  //while (ros::ok())
  //{	
  	ros::spin();
  //}
  
  /*
  arbtx_ros_training::AddTwoInts srv1;
  arbtx_ros_training::MultiplyTwoInts srv2;
  arbtx_ros_training::AddTwoInts srv3;
  
  srv1.request.a = Cord :: Ycord;
  srv1.request.b = Cord :: Xcord;
  
  srv2.request.a = Cord :: Ycord;
  srv2.request.b = Cord :: Xcord;
  
  /*srv3.request.a = Cord :: Sum;
  srv3.request.b = Cord :: Multi;
  ros::Rate loop_rate(1);
  
  while (ros::ok())
  {
  cout << "cord " << Cord :: Ycord<< '\n';
  if (client1.call(srv1))
  {
    ROS_INFO("sum: %ld", (long int)srv1.response.sum);
    msg.num = srv1.response.sum;
    //loop_rate.sleep();
    result_pub.publish(msg);
  }
  else
  {
    ROS_ERROR("Failed to call service add_two_ints");
  }
  msg.num = srv3.response.sum;
  result_pub.publish(msg);*/
  
  cout << "^ This Node has been stopped roughly!!\n"  ;
  return 0;
}
