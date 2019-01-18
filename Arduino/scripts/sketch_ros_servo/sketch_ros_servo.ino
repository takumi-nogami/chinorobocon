#include <ros.h>

#include <std_msgs/Int32.h>
#include <VarSpeedServo.h>

VarSpeedServo myservo_dodai;
VarSpeedServo myservo_joint;

ros::NodeHandle node;

void hand_alpha_Callback(const std_msgs::Int32 &hand_alpha_msg){
  if(hand_alpha_msg.data == 0){ //catchball
    myservo_joint.write(155, 64, true); // OPEN
    delay(1500);
    myservo_dodai.write(0, 256, true);  // DOWN
    delay(1500);
    myservo_joint.write(89, 64, true);  //CLOSE
    delay(1500);
    myservo_dodai.write(150, 64, true); // RAISE
    delay(1500);
  }else if(hand_alpha_msg.data == 1){   //releaseball
    myservo_joint.write(155, 256, true); // OPEN
    delay(1500);
  }
}

ros::Subscriber<std_msgs::Int32>hand_alpha("/mtoh/cmd_hand_alpha", &hand_alpha_Callback);

void setup() { 
  myservo_dodai.attach(10);
  myservo_joint.attach(5);
  node.initNode();
  node.subscribe(hand_alpha);
}

void loop() {
  node.spinOnce();
  delay(1);
}
