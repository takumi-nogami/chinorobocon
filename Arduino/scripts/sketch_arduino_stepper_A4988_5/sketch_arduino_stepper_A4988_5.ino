#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <AccelStepper.h>

#define EnablePin_R 12
#define StepPin_R 8
#define DirPin_R 7

#define EnablePin_L 6
#define StepPin_L 5
#define DirPin_L 4

ros::NodeHandle node;
geometry_msgs::Point order_msg;
ros::Publisher order("/mtoa/cmd_order", &order_msg);
std_msgs::Int32 hand_alpha_msg;
ros::Publisher hand_alpha("/mtoh/cmd_hand_alpha", &hand_alpha_msg);

int fg;

AccelStepper stepperL(1, StepPin_L, DirPin_L);
AccelStepper stepperR(1, StepPin_R, DirPin_R);

void instruct_Callback(const geometry_msgs::Point &instruct_msg){
  //hand_start_msg.data = 0;              //releaseball
  //hand_start.publish(&hand_start_msg);
  //delay(6000);
  
  if(instruct_msg.x == 0){
     stepper_run_fb(1950);
     stepper_run_turn(-90); 
     stepper_run_fb(610);
  }else if(instruct_msg.x == 1){
     fg = 1;
  }else if(instruct_msg.x == 2){
     if(instruct_msg.y == 1){ //ball = bleu
       stepper_run_fb(350);
       stepper_run_turn(-20);
       hand_alpha_msg.data = 1;//releaseball
       hand_alpha.publish(&hand_alpha_msg);
       delay(6000);
       stepper_run_turn(110);
       stepper_run_fb(1120);
       stepper_run_turn(90);
       stepper_run_fb(350);
       order_msg.x = 3;                         //next flag
       order_msg.y = 0;           //coller
       order.publish(&order_msg);
     }else if(instruct_msg.y == 2){ //ball = yello
       stepper_run_fb(350);
       stepper_run_turn(90);
       stepper_run_fb(500);
       stepper_run_turn(-90);
       hand_alpha_msg.data = 1;//releaseball
       hand_alpha.publish(&hand_alpha_msg);
       stepper_run_turn(90);
       stepper_run_fb(620);
       stepper_run_turn(90);
       stepper_run_fb(350);
       order_msg.x = 3;            //next flag
       order_msg.y = 0;           //coller
       order.publish(&order_msg);
     }else if(instruct_msg.y == 3){ //ball = red
       stepper_run_fb(350);
       stepper_run_turn(90);
       stepper_run_fb(1120);
       stepper_run_turn(-90);
       hand_alpha_msg.data = 1;//releaseball
       hand_alpha.publish(&hand_alpha_msg);
       delay(6000);
       stepper_run_turn(180);
       stepper_run_fb(350);
       order_msg.x = 3;            //next flag
       order_msg.y = 0;           //coller
       order.publish(&order_msg);
     }
  }else if(instruct_msg.x == 3){
     fg = 3;
  }else if(instruct_msg.x == 4){
     if(instruct_msg.y == 1){ //ball = blue
       stepper_run_fb(650);
       stepper_run_turn(-90);
       stepper_run_fb(1550);
       stepper_run_turn(110);
       hand_alpha_msg.data = 1;//releaseball
       hand_alpha.publish(&hand_alpha_msg);
       delay(6000);
       stepper_run_turn(-200);
       stepper_run_fb(650);
       order_msg.x = 1;            //next flag
       order_msg.y = 0;           //coller
       order.publish(&order_msg);
     }else if(instruct_msg.y == 2){ //ball = yello
       stepper_run_fb(650);
       stepper_run_turn(-90);
       stepper_run_fb(800);
       stepper_run_turn(90);
       hand_alpha_msg.data = 1;//releaseball
       hand_alpha.publish(&hand_alpha_msg);
       delay(6000);
       stepper_run_turn(-90);
       stepper_run_fb(780);
       stepper_run_turn(-90);
       stepper_run_fb(650);
       order_msg.x = 1;            //next flag
       order_msg.y = 0;           //coller
       order.publish(&order_msg);
     }else if(instruct_msg.y == 3){ //ball = red
       stepper_run_fb(650);
       stepper_run_turn(-30);
       hand_alpha_msg.data = 1;//releaseball
       hand_alpha.publish(&hand_alpha_msg);
       delay(6000);
       stepper_run_turn(-60);
       stepper_run_fb(1550);
       stepper_run_turn(-90);
       stepper_run_fb(650);
       order_msg.x = 1;            //next flag
       order_msg.y = 0;           //coller
       order.publish(&order_msg);
     }
  }
} 

void motor_info_Callback(const geometry_msgs::Point &motor_info_msg){
     if(fg != 0){
       if(fg==1){
         stepper_run_fb(motor_info_msg.x);   //go to ball position
         stepper_run_turn(-90);
         stepper_run_fb(motor_info_msg.y);
         hand_alpha_msg.data = 0;         //catchball
         for(int i=0;i<1;i++){
          hand_alpha.publish(&hand_alpha_msg);
         }
         delay(6000);
         stepper_run_fb( -( motor_info_msg.y - 260) );   //go to staying position
         stepper_run_turn(90);
         stepper_run_fb(-motor_info_msg.x);
         stepper_run_turn(180);
         order_msg.x = 2;                         //next flag
         order_msg.y = motor_info_msg.z;           //coller
         order.publish(&order_msg);
         fg = 0;
       }else if(fg==3){
         stepper_run_fb(motor_info_msg.x);   //go to ball position
         stepper_run_turn(90);
         stepper_run_fb(motor_info_msg.y);
         hand_alpha_msg.data = 0;
         for(int i=0;i<1;i++){
          hand_alpha.publish(&hand_alpha_msg);
         }
         delay(6000);
         stepper_run_fb( -(motor_info_msg.y - 190) );   //go to staying position
         stepper_run_turn(-90);
         stepper_run_fb(-motor_info_msg.x);
         stepper_run_turn(-180);
         order_msg.x = 4;                         //next flag
         order_msg.y = motor_info_msg.z;           //coller
         order.publish(&order_msg);
         fg = 0;
       }
     }
} 

void wait_Callback(const geometry_msgs::Point &order_msg){
       delay(5000);
       order.publish(&order_msg);
}

void stepper_zero(){
  stepperL.setCurrentPosition(0);
  stepperR.setCurrentPosition(0);
}
void stepper_set_fb(int x, int y, int z){
  stepperR.setMaxSpeed(x);
  stepperR.setAcceleration(y);
  stepperR.moveTo(z);
  stepperL.setMaxSpeed(x);
  stepperL.setAcceleration(y);
  stepperL.moveTo(-z);
}
void stepper_set_turn(int x, int y, int z){
  stepperR.setMaxSpeed(x);
  stepperR.setAcceleration(y);
  stepperR.moveTo(z);
  stepperL.setMaxSpeed(x);
  stepperL.setAcceleration(y);
  stepperL.moveTo(z);
}
void stepper_run_fb(float x){
  int step_x = x / 0.275;
  stepper_set_fb(1846, 3692, step_x);
  while( (stepperL.distanceToGo() != 0) && (stepperR.distanceToGo() != 0) ){
      stepperL.run();
      stepperR.run();
    }  
  stepper_zero();
  delay(1000);
}
void stepper_run_turn(float z){
  int step_angular_z = 137 * z * 3.14 / 360 / 0.275;
  stepper_set_turn(1846, 3692, step_angular_z);
  while( (stepperL.distanceToGo() != 0) && (stepperR.distanceToGo() != 0) ){
      stepperL.run();
      stepperR.run();
    }  
  stepper_zero();
  delay(1000);
}

ros::Subscriber<geometry_msgs::Point>instruct("/atom/cmd_instruct", &instruct_Callback);
ros::Subscriber<geometry_msgs::Point>motor_info("/ctom1/cmd_motor_info", &motor_info_Callback);
ros::Subscriber<geometry_msgs::Point>Wait("/atom/cmd_wait", &wait_Callback);

void setup() { 
  pinMode(EnablePin_L,OUTPUT); // Enable
  pinMode(EnablePin_R,OUTPUT); // Enable
  
  pinMode(11,OUTPUT); 
  pinMode(10,OUTPUT);  //HowStep
  pinMode(9,OUTPUT);  
  
  digitalWrite(EnablePin_L,LOW); // Set Enable low　→　Low状態でEnable
  digitalWrite(EnablePin_R,LOW); // Set Enable low　→　Low状態でEnable
  
  digitalWrite(11,HIGH);
  digitalWrite(10,LOW); //FULLSTEP
  digitalWrite(9,LOW);

  node.initNode();
  node.subscribe(instruct);
  node.subscribe(motor_info);
  node.subscribe(Wait);
  node.advertise(order);
  node.advertise(hand_alpha);
}

void loop() {
  node.spinOnce();
  delay(1);
}

