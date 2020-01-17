#include <Arduino.h>
#include <ros.h>
#include <Encoder.h>
//header file for cmd_subscribing to "cmd_vel"
#include "geometry_msgs/Twist.h"
#include <config.h>
void velCallback(const geometry_msgs::Twist &vel);

ros::NodeHandle nh;
ros::Subscriber<geometry_msgs::Twist> cmd_sub("/cmd_vel", velCallback);
Encoder motor_enconder_L(21, 20);
Encoder motor_enconder_R(23, 22);

void setup()
{
  // put your setup code here, to run once:
  nh.getHardware()->setBaud(115200); //setting burdrate usb
  nh.initNode();                     // start node
  nh.subscribe(cmd_sub);
  pinMode(PWM_MOTOR_L, OUTPUT);
  pinMode(PWM_MOTOR_R, OUTPUT);
  pinMode(MOTOR_IN1_L, OUTPUT);
  pinMode(MOTOR_IN2_R, OUTPUT);
  // pinMode(MOTOR_IN3_R, OUTPUT);
  // pinMode(MOTOR_IN4_R, OUTPUT);
}
void loop()
{
  // put your main code here, to run repeatedly:
  long newLeft, newRight;
  newLeft = motor_enconder_L.read();
  newRight = motor_enconder_R.read();
  if (newLeft != positionLeft || newRight != positionRight)
  {

    positionLeft = newLeft;
    positionRight = newRight;
    char buffer[50];
    sprintf(buffer, "motor_enconder_L : %ld", newLeft);
    nh.loginfo(buffer);
    sprintf(buffer, "motor_enconder_R  : %ld", newRight);
    nh.loginfo(buffer);
  }
  nh.spinOnce();
}

void velCallback(const geometry_msgs::Twist &vel)
{
  //  nh.loginfo("In Callback Function");
  vel_linear_x = vel.linear.x;
  vel_angular_z = vel.angular.z;
  //  g_prev_command_time = 0;
  printDebug();
  runMove();
  g_prev_command_time = millis();
}
void printDebug()
{
  char buffer[50];
  sprintf(buffer, "linear.x  : %.2f", vel_linear_x);
  nh.loginfo(buffer);
  sprintf(buffer, "angular.z  : %.2f", vel_angular_z);
  nh.loginfo(buffer);
}

void runMove()
{
  if (vel_linear_x > 0 && vel_angular_z == 0)
  {
    analogWrite(PWM_MOTOR_R, vel_linear_x * 100);
    analogWrite(PWM_MOTOR_L, vel_linear_x * 100);
    digitalWrite(MOTOR_IN1_L, HIGH);
    digitalWrite(MOTOR_IN2_R, HIGH);
  }
  if (vel_linear_x < 0 && vel_angular_z == 0)
  {

    analogWrite(PWM_MOTOR_R, abs(vel_linear_x) * 100);
    analogWrite(PWM_MOTOR_L, abs(vel_linear_x) * 100);
    digitalWrite(MOTOR_IN1_L, LOW);
    digitalWrite(MOTOR_IN2_R, LOW);
  }
  if (vel_angular_z > 0 && vel_linear_x == 0)
  {

    analogWrite(PWM_MOTOR_L, vel_angular_z * 100);
    analogWrite(PWM_MOTOR_R, vel_angular_z * 100);
    digitalWrite(MOTOR_IN2_R, HIGH);
    digitalWrite(MOTOR_IN1_L, LOW);
  }
  if (vel_angular_z < 0 && vel_linear_x == 0)
  {

    analogWrite(PWM_MOTOR_L, abs(vel_angular_z) * 100);
    analogWrite(PWM_MOTOR_R, abs(vel_angular_z) * 100);
    digitalWrite(MOTOR_IN2_R, LOW);
    digitalWrite(MOTOR_IN1_L, HIGH);
  }
  if (vel_angular_z > 0 && vel_linear_x > 0)
  {
    analogWrite(PWM_MOTOR_L, vel_linear_x * 100);
    analogWrite(PWM_MOTOR_R, vel_angular_z * 100);
    digitalWrite(MOTOR_IN2_R, LOW);
    digitalWrite(MOTOR_IN1_L, LOW);
  }
  if (vel_angular_z < 0 && vel_linear_x > 0)
  {
    analogWrite(PWM_MOTOR_L, abs(vel_angular_z) * 100);
    analogWrite(PWM_MOTOR_R, vel_linear_x * 100);
    digitalWrite(MOTOR_IN2_R, LOW);
    digitalWrite(MOTOR_IN1_L, LOW);
  }
  if (vel_angular_z == 0 && vel_linear_x == 0)
  {
    analogWrite(PWM_MOTOR_L, 0);
    analogWrite(PWM_MOTOR_R, 0);
    digitalWrite(MOTOR_IN2_R, LOW);
    digitalWrite(MOTOR_IN1_L, LOW);
  }
}