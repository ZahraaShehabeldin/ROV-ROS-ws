#include <ros/ros.h>
#include <iostream>
#include "keyboard.h"
#include "std_msgs/String.h"
#include <sensor_msgs/PointField.h>
#include <string.h>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h> 

//#include <std_msgs/UInt16.h>
int x;
using namespace std;

// defining struct to access
keyboard::Key msg;

// defining msg to publish
std_msgs::String micro_msg;

// values to concatenate
int right_motor;
int left_motor;

int direction;
int speed=5;
int maped_Speed;

int press = 0;
int lastpress = 0;

// Concatinating Micro Str 
  std::stringstream micro_str;
   
/*void pub (int& direction, int& speed)
{
 
	micro_str.str("");
	micro_msg.data =  micro_str.str();
	micro_str << "D" << x << "S" << speed; 
                
	 	micro_msg.data = micro_str.str();

	        ROS_INFO("this is the key %s", micro_msg.data.c_str());
}*/

int main(int argc, char** argv)
{  

// assign Values 
   /* up = msg.KEY_UP;  
    down = msg.KEY_DOWN;
    rights = msg.KEY_s;
    lefts = msg.KEY_LEFT;

    pluss = msg.KEY_PLUS;
    minuss = msg.KEY_MINUS;*/

  ros::init(argc, argv, "keyboard");
  ros::NodeHandle n("~");

  ros::Publisher pub_down = n.advertise<keyboard::Key>("keydown", 10);
  ros::Publisher pub_up = n.advertise<keyboard::Key>("keyup", 10);


// Publisher to the msg
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("micro", 10);



  bool allow_repeat=false;
  int repeat_delay, repeat_interval;
  
  n.param<bool>( "allow_repeat", allow_repeat, false ); // disable by default
  n.param<int>( "repeat_delay", repeat_delay, SDL_DEFAULT_REPEAT_DELAY );
  n.param<int>( "repeat_interval", repeat_interval, SDL_DEFAULT_REPEAT_INTERVAL );
  
  if ( !allow_repeat ) repeat_delay=0; // disable 
  keyboard::Keyboard kbd( repeat_delay, repeat_interval );
  

  ros::Rate r(50);
 
  keyboard::Key k;
  bool pressed, new_event;

int D;

  while (ros::ok() && kbd.get_key(new_event, pressed, k.code, k.modifiers)) 
{
  //  lastpress = press;
  //  press = 0;
    x = k.code;

    if (new_event) 
{ 
      k.header.stamp = ros::Time::now();
      if (pressed) 
   {
       
       
      micro_str.str("");
      micro_msg.data =  micro_str.str();
	    pub_down.publish(k);
	
      if (x == 270)    // PLUS	
          {
             press =1;
          if (speed >= 10)
              speed = 10;			   
          else	
              speed ++;
          }

      if (x == 269)	 // MINUS
        { 
           press =1;
          if (speed <= 0)		    		    
              speed = 0; 			
          else 
              speed --;  

        }

  
 
      maped_Speed = (speed/10.0) * 400; // Mapping Motors Values 

		if (x == 273)
		  { 
         press =1;
		    x = 1 ; // Forward = 1
		    D = x;
		    right_motor = 1450 + maped_Speed;
		    left_motor = 1450 + maped_Speed ;
		  }

	    	else if (x == 274)
		  {
         press =1;
	      x = 2 ; // Backward = 2 
		    D = x;
		    right_motor = 1450 - maped_Speed;
		    left_motor = 1450 - maped_Speed;			
		  }

		else if (x == 275)
		 {  press =1; 
       x = 3 ; // RIGHT = 3 
		    D = x;
		    right_motor = 1450 - maped_Speed;
		    left_motor = 1450 + maped_Speed;
		 }

		else if (x == 276)
		 {  press =1; 
       x = 4 ; // LEFT = 4 
		    D = x;
		    right_motor = 1450 + maped_Speed;
		    left_motor = 1450 - maped_Speed;
		 }
                
       if ( x == 32)
		  {
         press =1;
	        right_motor = 1450;
		      left_motor = 1450;
   	  }
        

      } 
  
      else
       {
		    pub_up.publish(k);
		    x = 0;
        D = 0;
           lastpress = press;
           press = 0;

       }

      
}
 if ( press == 1 && lastpress == 0)  
            {

              micro_str <<"X" << right_motor << "Y" << left_motor; 
              micro_msg.data = micro_str.str();
              ROS_INFO("this is the key %s", micro_msg.data.c_str());	                
              chatter_pub.publish(micro_msg);
              micro_str.str("");


            } 
          if ( press == 0 && lastpress == 1)
            {
              micro_str <<"X" << 1450 << "Y" << 1450; 
              micro_msg.data = micro_str.str();
              ROS_INFO("this is the key %s", micro_msg.data.c_str());	                
              chatter_pub.publish(micro_msg);
              press = lastpress = 0;

            }


    ros::spinOnce();
    r.sleep();

  }
  
  ros::waitForShutdown();
}
