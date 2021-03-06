#include <vector>
#include <string>

#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <controller_manager/controller_manager.h>

#include <iostream>
#include <ros/ros.h>
#include "sensor_msgs/JointState.h"

class pa10 : public hardware_interface::RobotHW
{
public:
  ros::Subscriber sub;
  ros::NodeHandle n_; 
  
  //OnlyPositionFeedback = true;
public:
  pa10() 
 { 
   // connect and register the joint state interface
   hardware_interface::JointStateHandle state_handle_1("soldier_joint_1", &pos[0], &vel[0], &eff[0]);
	 jnt_state_interface.registerHandle(state_handle_1);

	 hardware_interface::JointStateHandle state_handle_2("soldier_joint_2", &pos[1], &vel[1], &eff[1]);
	 jnt_state_interface.registerHandle(state_handle_2);

	 hardware_interface::JointStateHandle state_handle_3("elbow_joint_1", &pos[2], &vel[2], &eff[2]);
	 jnt_state_interface.registerHandle(state_handle_3);

	 hardware_interface::JointStateHandle state_handle_4("elbow_joint_2", &pos[3], &vel[3], &eff[3]);
	 jnt_state_interface.registerHandle(state_handle_4);

	 hardware_interface::JointStateHandle state_handle_5("wrist_joint_1", &pos[4], &vel[4], &eff[4]);
	 jnt_state_interface.registerHandle(state_handle_5);

	 hardware_interface::JointStateHandle state_handle_6("wrist_joint_2", &pos[5], &vel[5], &eff[5]);
	 jnt_state_interface.registerHandle(state_handle_6);

	 hardware_interface::JointStateHandle state_handle_7("wrist_joint_3", &pos[6], &vel[6], &eff[6]);
   jnt_state_interface.registerHandle(state_handle_7);
   
   hardware_interface::JointStateHandle state_handle_8("finger_joint_1", &pos[7], &vel[7], &eff[7]);
	 jnt_state_interface.registerHandle(state_handle_8);

	 hardware_interface::JointStateHandle state_handle_9("finger_joint_2", &pos[8], &vel[8], &eff[8]);
   jnt_state_interface.registerHandle(state_handle_9);
   
   registerInterface(&jnt_state_interface);

   // connect and register the joint position interface
   hardware_interface::JointHandle vel_handle_1(jnt_state_interface.getHandle("soldier_joint_1"), &cmd[0]);
	 jnt_vel_interface.registerHandle(vel_handle_1);

	 hardware_interface::JointHandle vel_handle_2(jnt_state_interface.getHandle("soldier_joint_2"), &cmd[1]);
	 jnt_vel_interface.registerHandle(vel_handle_2);

	 hardware_interface::JointHandle vel_handle_3(jnt_state_interface.getHandle("elbow_joint_1"), &cmd[2]);
	 jnt_vel_interface.registerHandle(vel_handle_3);                                                       
                                                                                                                  
	 hardware_interface::JointHandle vel_handle_4(jnt_state_interface.getHandle("elbow_joint_2"), &cmd[3]);
	 jnt_vel_interface.registerHandle(vel_handle_4);                                                       
                                                                                                                  
	 hardware_interface::JointHandle vel_handle_5(jnt_state_interface.getHandle("wrist_joint_1"), &cmd[4]);
	 jnt_vel_interface.registerHandle(vel_handle_5);                                                       
                                                                                                                  
	 hardware_interface::JointHandle vel_handle_6(jnt_state_interface.getHandle("wrist_joint_2"), &cmd[5]);
	 jnt_vel_interface.registerHandle(vel_handle_6);                                                       
                                                                                                                  
	 hardware_interface::JointHandle vel_handle_7(jnt_state_interface.getHandle("wrist_joint_3"), &cmd[6]);
   jnt_vel_interface.registerHandle(vel_handle_7);
   
   hardware_interface::JointHandle vel_handle_8(jnt_state_interface.getHandle("finger_joint_1"), &cmd[7]);
	 jnt_vel_interface.registerHandle(vel_handle_8);                                                       
                                                                                                                  
	 hardware_interface::JointHandle vel_handle_9(jnt_state_interface.getHandle("finger_joint_2"), &cmd[8]);
   jnt_vel_interface.registerHandle(vel_handle_9);


   registerInterface(&jnt_vel_interface);
 
   n_ = ros::NodeHandle();
   //subscriber to joint states
   //sub = n_.subscribe("joint_states", 1, &pa10::updateJointState, this);


  }
  

  // Write Function
  void write()
  {
  	for (int i=0;i<9;++i) 
			{
				cmd_vel[i]=cmd[i]-cmd_prev[i]*10.0;
			}
    std::cout<<"##joints Commands: "<<cmd_vel[0]<<" "<<cmd_vel[1]<<" "<<cmd_vel[2]<<" "<<cmd_vel[3]<<" "<<cmd_vel[4]<<" "<<cmd_vel[5]<<" "<<cmd_vel[6]<<" "<<cmd_vel[7]<<" "<<cmd_vel[8]<<"\n";
    memcpy(cmd_prev,cmd,sizeof(double)*9);
  }
	
	void read()
	{
//		memcpy(pos,cmd,sizeof(double)*9);
		controller_type = "PositionJointInterface";
    VelocityInterface = false;
		if (controller_type=="VelocityJointInterface") 
		{
			memcpy(vel,cmd,sizeof(double)*9);
			for (int i=0;i<9;++i) 
			{
				pos[i]=cmd[i]*0.1+prev_pos[i];
				eff[i]=0.0;
			}
			memcpy(prev_pos,pos,sizeof(double)*9);
		}
		
		else if (controller_type =="EffortJointInterface")
		{
			for (int i=0;i<9;++i) 
			{
				pos[i]=0.0;
				vel[i]=0.0;
				eff[i]=0.0;
			}
		}
		else
		{
			memcpy(pos,cmd,sizeof(double)*9);
			for (int i=0;i<9;++i) 
			{
				vel[i]=0.0;
				eff[i]=0.0;
			}
		}
		
	}
  ros::Time get_time() { return ros::Time::now(); } ;
  ros::Duration get_period() { ros::Duration(0.1);};
private:
  hardware_interface::JointStateInterface jnt_state_interface;
  hardware_interface::PositionJointInterface jnt_vel_interface;
  double cmd[9],cmd_prev[9],cmd_vel[9];
  double pos[9];
	double prev_pos[9];
  double vel[9];
  double eff[9];
  std::string controller_type;
  //std::vector<std::string> names[7];
	double dt;
	bool OnlyPositionFeedback;
  //ros::NodeHandle n_;
  bool VelocityInterface;
};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "pa10_hardware_interface");
  ros::NodeHandle nh;
  ros::Rate loop_rate(100);
  pa10 robot;
  
  controller_manager::ControllerManager cm(&robot,nh);
  ros::AsyncSpinner spinner(1);
  spinner.start();
  while (ros::ok())
  {
     robot.read();
     cm.update(robot.get_time(), robot.get_period());
     robot.write();
     loop_rate.sleep();
  }
 spinner.stop();
}
