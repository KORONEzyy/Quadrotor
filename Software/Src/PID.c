#include "stm32f4xx.h"                  // Device header
#include "GY86.h"
#include "PID.h"

#define PWM_IN_TO_OUT 0.05427f

//积分限制
#define MAX_INTEGRATE_NUM 10

/*
	串级PID：
		外层PID——位置：角度、高度
		内层PID——位置单位积分：角速度
		内快外慢
		
		----------------------------------------------------------
		|            |        内层        |         外层         |
		|---------------------------------------------------------
		|YAW偏航角   |         √          |          √           | 
		|---------------------------------------------------------
		|Roll翻滚角  |         √          |          √           |
		|---------------------------------------------------------
		|Pitch俯仰角 |         √          |          √           |
		|---------------------------------------------------------
		|Height高度  |         X          |          √           |
		|--------------------------------------------------------|
		
		TODO_01: 无法测得偏航角Yaw角度，致使只有Yaw只进行内层PID？
			已改：将Yaw也加入串级PID
*/

PID_t outerPitch,outerRoll,outerHeight;
PID_t innerPitch,innerRoll,innerYaw;
PID_t outerYaw;
/*
	Gyro      -> GY86测得角速度
	Angle     -> 姿态解算得到的角度
	expect    -> 遥控器传入的期望角度 
	motor     -> 四个电机传入PWM
	PWM_IN[4] -> 捕获的PWM波
	TODO_02: 不同文件之间的转换
*/



/*
	TODO_03: 外层追求简单可靠，故只采用Kp比例计算
*/

//统一配置参数





////PWM周期->外层周期

//void Get_T(float * T){
//	float last_t = 0; 
//	float now_t = 0;
//	
//	now_t = TIM1->CNT;
//	
//	* T = now_t / 84000000  - last_t / 84000000;
//	
//	last_t = now_t;
//}


void Single_PID_Init(PID_t* pid)
{
	pid->e = 0;
	pid->e_pre = 0;
	pid->e_sum = 0;
	pid->Kp = 0;
	pid->Ki = 0;
	pid->Kd = 0;
	pid->output = 0;
	pid->T = 0;
}

void PID_Init()
{
	Single_PID_Init(&outerHeight);
	outerHeight.Kp = 0.7;
	outerHeight.Kd = 0.1;
	
	Single_PID_Init(&outerPitch);
	outerPitch.Kp = 0.7;
	
	Single_PID_Init(&outerRoll);
	outerRoll.Kp = 0.1;
	
	Single_PID_Init(&innerPitch);
	innerPitch.Kp = 0.7;
	innerPitch.Ki = 0.1;
	innerPitch.Kd = 0.01;
	
	Single_PID_Init(&innerRoll);
	innerRoll.Kp = 0.7;
	innerRoll.Ki = 0.1;
	innerRoll.Kd = 0.01;
	
	Single_PID_Init(&innerYaw);
	innerYaw.Kp = 0.7;
	innerYaw.Ki = 0.1;
	innerYaw.Kd = 0.01;
	
	Single_PID_Init(&outerYaw);
	outerYaw.Kp = 0.7;
	outerYaw.Ki = 0.1;
	outerYaw.Kd = 0.01;
}


float PID_Calc(PID_t* inner, PID_t* outer, float outerError, float innerStatus){
	//改变周期
	float Kp = inner->Kp;
	float Ki = inner->Ki;
	float Kd = inner->Kd;

	// 均进行内外层PID运算：Roll Picth Yaw
	if((inner != 0) && (outer != 0))
	{
		//外层进进行P比例运算
		outer->e = outerError;
		outer->output = outer->Kp * outer->e;
		outer->e_pre = outer->e;
		
		//外层输出作为内层输入
		inner->e = outer->output - innerStatus;
		
		//对e_sum积分进行限制 浮点数的比较
		if(inner->e_sum - MAX_INTEGRATE_NUM <= 0.00001 && inner->e_sum + MAX_INTEGRATE_NUM >= 0.00001)
		{  
			inner->e_sum += inner->e;
		}
		
		inner->output = Kp * inner->e + Ki * inner->e_sum + Kd * (inner->e - inner->e_pre);
		inner->e_pre = inner->e;
		return inner->output;
	}
	
}

/*
	TODO4: 动能分配需要检查 计算结果可能反了
*/
/*
	动能分配：
						    Motor2 顺  O      ^      O Motor1 逆
													   \	  |    /
															 \ 	|  /                 ^
																 \|/                   |
										     <--------O---------    左 <---| 前
																 /|\                   |
															 /	|  \                 |
														 / 		|    \             
                 Motor3 逆 O             O Motor4 顺
	
          	|------------------------------------------------|
            |  操作    |   改变角   |     +      |     -     |
          	|------------------------------------------------|
            |  左倾    |   Roll     | Motor1+4   | Motor2+3  |
          	|------------------------------------------------|
            |  前俯    |   Pitch    | Motor3+4   | Motor1+2  |
          	|------------------------------------------------|
            |  左偏    |   Yaw      | Motor2+ 3  | Motor1+4  |
          	|------------------------------------------------|
						|  上飞    |     -      |Motor1+2+3+4|    -      |
						|------------------------------------------------|

	左倾、前俯、左偏时，PID返回结果roll pitch yaw为负：
	pwm_out1 = -roll + pitch + yaw + thr
	pwm_out2 = +roll + pitch - yaw + thr 
	pwm_out3 = +roll - pitch + yaw + thr
	pwm_out4 = -roll - pitch - yaw + thr
*/

void Motor_Calc(gy_t gy86_data, float pitch , float roll ,float yaw , float expectRoll, float expectPicth, float expectYaw, float expectThr, float *motor1, float * motor2, float *motor3, float *motor4){
	
	float GyroRoll = gy86_data.gx;
    float GyroPitch = gy86_data.gy; 
	float GyroYaw = gy86_data.gz;
	
	float AngleYaw = yaw;
	float AngleRoll = roll;
	float AnglePitch = pitch;	
	
	
	//存储pid计算结果
	float pid_yaw, pid_roll, pid_pitch, pid_thr;
	
	
	pid_pitch = PID_Calc(&innerPitch, &outerPitch, expectPicth - AnglePitch, GyroPitch);
	pid_roll  = PID_Calc(&innerRoll, &outerRoll, expectRoll - AngleRoll, GyroRoll);
	pid_yaw   = PID_Calc(&innerYaw, &outerYaw, expectYaw - AngleYaw, GyroYaw);
	pid_thr = expectThr;
	
	//pid_thr   = PID_Calc(0,&outerHeight, expectThr, 0);
	
	//运算得到期望的电机速度motor = 三个自由度 + 油门叠加(pid_thr)
	*motor1 = -(-pid_roll + pid_pitch + pid_yaw + pid_thr) * PWM_IN_TO_OUT;
	*motor2 = -(+pid_roll + pid_pitch - pid_yaw + pid_thr) * PWM_IN_TO_OUT;
	*motor3 = -(-pid_roll - pid_pitch + pid_yaw + pid_thr) * PWM_IN_TO_OUT;
	*motor4 = -(+pid_roll - pid_pitch - pid_yaw + pid_thr) * PWM_IN_TO_OUT;	
}



//通过PWM捕获到的占空比来计算用户遥控器传来的期望角度值
void Expect_Angle_Calc(float *PWM_IN , float * roll, float *pitch, float *yaw, float *thr){
	  //TODO6: 四个通道是否匹配三个自由度以及油门 公式仍然需要调试
	
    * roll = (float)((PWM_IN[3] - 1500) * 0.03f);  //roll
    *pitch = (float)((PWM_IN[1] - 1500) * 0.04f); //pitch
    *yaw = (float)((PWM_IN[0] - 1500) * 0.02f); 
    *thr = PWM_IN[2];
	
}

//TODO7: 周期如何安排 
/*
	周期采用通过PID周期与PWM周期的比例 影响Ki与Kd
*/
