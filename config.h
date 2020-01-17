#define  PWM_MOTOR_L 23
#define  PWM_MOTOR_R 22
#define  MOTOR_IN1_L 2 
#define  MOTOR_IN2_R 3
// #define  MOTOR_IN3_R 4 
// #define  MOTOR_IN4_R 5
double vel_linear_x;
double vel_angular_z;
unsigned long g_prev_command_time = 0;
long positionLeft  = -999;
long positionRight = -999;

void printDebug();
void runMove();