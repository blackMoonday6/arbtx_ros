#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <sstream>
#include <camera_control_msgs/GetIntegerValue.h>
#include <camera_control_msgs/SetIntegerValue.h>
#include <visio_msgs/setEject.h>
#include <std_srvs/SetBool.h>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
bool Eject =false;
int duration=0;
/*void chatterCallback(const std_msgs::Bool::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", (msg->data) ? "EJECt":"don't eject");
  Eject=msg->data;
}*/

bool SetEjectCB(visio_msgs::setEject::Request  &req,//std_srvs::SetBool
         visio_msgs::setEject::Response &res)
{
  /*res.sum = req.a + req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.sum);*/
  //
  Eject=req.data;
  duration=req.duration;
  return true;
}

int main(int argc, char **argv)
{

  camera_control_msgs::GetIntegerValue PhCelluleSrv;
std_srvs::SetBool srv;
camera_control_msgs::SetIntegerValue modeSrv;
camera_control_msgs::SetIntegerValue sourceSrv;
camera_control_msgs::SetIntegerValue selectorSrv;
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "talker");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::ServiceClient PhCelluleClient = n.serviceClient<camera_control_msgs::GetIntegerValue>("/pylon_backCamera_node/get_line_status");
  ros::ServiceClient modeCam1Client = n.serviceClient<camera_control_msgs::SetIntegerValue>("/pylon_backCamera_node/set_line_mode");
  ros::ServiceClient sourceCam1Client = n.serviceClient<camera_control_msgs::SetIntegerValue>("/pylon_backCamera_node/set_line_source");//ros::ServiceClient selectorClient ;
  ros::ServiceClient selectorCam1Client = n.serviceClient<camera_control_msgs::SetIntegerValue>("/pylon_backCamera_node/set_line_selector");
	ros::ServiceClient EVerinClient = n.serviceClient<std_srvs::SetBool>("/pylon_backCamera_node/set_user_output_1");
	ros::ServiceClient SVerinClient = n.serviceClient<std_srvs::SetBool>("/pylon_backCamera_node/set_user_output_0");
  ros::ServiceServer service = n.advertiseService("/setEject", SetEjectCB);

  ros::Rate loop_rate(200);
  
  //ros::Subscriber sub = n.subscribe("/Ejection", 1000, chatterCallback);
  selectorSrv.request.value=2;//choose line for lights line 03 for rouge
    modeSrv.request.value=1;//select mode output
    sourceSrv.request.value=2;
    srv.request.data=true; 
    if(selectorCam1Client.call(selectorSrv))//affect line
      if(modeCam1Client.call(modeSrv))//affect selected mode (output)
        if( sourceCam1Client.call(sourceSrv))//affect source output1
          EVerinClient.call(srv);
  selectorSrv.request.value=1;//choose line for lights line 02 for rouge
	modeSrv.request.value=1;//select mode output
	sourceSrv.request.value=1;//select source user output 1
	srv.request.data=false; //affect state to data
	if(selectorCam1Client.call(selectorSrv))//affect line
		if(modeCam1Client.call(sourceSrv))//affect selected mode (output)
			if (sourceCam1Client.call(sourceSrv))//affect source 
				SVerinClient.call(srv);
  
  selectorSrv.request.value=0;//choose line for photo cellule
	modeSrv.request.value=0; //select mode input
	if(selectorCam1Client.call(selectorSrv))//affect line to be selected
		modeCam1Client.call(modeSrv);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    /*std_msgs::String msg;

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();*/
    bool state;
    if(PhCelluleClient.call(PhCelluleSrv))//get status of the line
		state=PhCelluleSrv.response.value;
    //select source user output 2
     //affect state to data
     //ROS_INFO("%s", state? "here product":"NO PRODUCT");
    if((state)&&(Eject))
    { ROS_INFO("I heard: [%s]", (state) ? "EJECT":"don't eject");
      
    
    ros::Duration(0.01*duration).sleep();//10 for cam 01 and 25 for the the others
      srv.request.data=true;
      SVerinClient.call(srv);
      srv.request.data=false;
      EVerinClient.call(srv);
    ros::Duration(0.15).sleep();

      srv.request.data=false;
      SVerinClient.call(srv);
      srv.request.data=true;
      EVerinClient.call(srv);
      Eject=false;
    }
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    //chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    //++count;
  }


  return 0;
}
