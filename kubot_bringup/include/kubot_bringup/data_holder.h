#ifndef KUBOT_DATA_HOLDER_H_
#define KUBOT_DATA_HOLDER_H_

#include <string.h>

#pragma pack(1)

using namespace std;

typedef int int32;
typedef short int16;
typedef unsigned short uint16;

struct Robot_firmware
{
   char version[16]; //控制板版本
   char time[16];    //建構時間
};

enum IMU_TYPE
{
   IMU_TYPE_MPU6050 = 65,
   IMU_TYPE_GY85 = 69,
   IMU_TYPE_GY87 = 71,
   IMU_TYPE_MPU9250 = 92,
   IMU_TYPE_BMI160 = 16,
};

enum MODEL_TYPE
{
   MODEL_TYPE_2WD_DIFF = 1,
   MODEL_TYPE_4WD_DIFF = 2,
   MODEL_TYPE_3WD_OMNI = 101,
   MODEL_TYPE_4WD_OMNI = 102,
   MODEL_TYPE_4WD_MECANUM = 201,
};

struct Robot_parameter
{
   union
   {
      char buff[64]; //總長度64位元
      struct
      {
			unsigned short wheel_diameter;     // 輪子直徑(mm)
         unsigned short wheel_track;        //差分:兩輪中心距、三輪全向:輪心直徑、四輪全向:前後輪中心距+左右輪中心距 (mm)
         unsigned short encoder_resolution; // 編碼器解析度
         unsigned char do_pid_interval;     // PID間隔 (ms)
         unsigned short kp;
         unsigned short ki;
         unsigned short kd;
         unsigned short ko;              // PID比例
         unsigned short cmd_last_time;   // 命令維持時間(ms)，超過該時間會自動停止運動
         unsigned short max_v_liner_x;   // 最大X方向線速度
         unsigned short max_v_liner_y;   // 最大Y方向線速度 (差分輪為0)
         unsigned short max_v_angular_z; // 最大旋轉速度
         unsigned char imu_type;         // IMU類型
         unsigned short motor_ratio;     // 馬達減速比
         unsigned char model_type;       // 運動模型類型
         unsigned short sona_distance;   // 超音波感測距離
        };
    };
};

struct Robot_velocity
{
   short v_liner_x;   // 線速度       前>0 cm/s
   short v_liner_y;   // 差分輪為0  cm/s      全向左為>0
   short v_angular_z; // 角速度 左>0 0.01rad/s  100 means 1 rad/s
};

struct Robot_odom
{
   short v_liner_x;   // 線速度 前>0 後<0  cm/s
   short v_liner_y;   // 差分輪 为0        cm/s
   short v_angular_z; // 角速度 左>0 右<0  0.01rad/s   100 means 1 rad/s
   int32 x;           // 里程計座標x       cm (long 4個字節)
   int32 y;           // 里程計座標y       cm (long 4個字節)
   short yaw;         // 里程計航角        0.01rad     100 means 1 rad
};

struct Robot_pid_data
{
   int32 input[4];  //各輪子输入值
   int32 output[4]; //各輪子输出值
};

struct Robot_imu
{
    union
    {
        float imu_data[9];
        struct
        {
            float ax; // 加速度x m/s^2
            float ay; // 加速度y m/s^2
            float az; // 加速度z m/s^2
            float gx; // 角速度x rad/s^2
            float gy; // 角速度y rad/s^2
            float gz; // 角速度z rad/s^2
            float mx; // 磁力x mGA
            float my; // 磁力y mGA
            float mz; // 磁力z mGA
        } imu;
    };
};

struct Robot_status
{
   bool bumper_status;         //防撞條狀態

	union 
	{
   	unsigned char sona_data[8]; //sona raw_data
		struct
		{
			unsigned char sona1_dis;
			unsigned char sona2_dis;			
			unsigned char sona3_dis;
			unsigned char sona4_dis;	
			unsigned char sona5_dis;					
			unsigned char sona6_dis;
			unsigned char sona7_dis;	
			unsigned char sona8_dis;		
		};
	};
};

struct Robot_lcd_status
{
    //    char name[16];         //機器人名字
    char robot_ip[16]; //設定顯示於下位機之機器人ip
};

struct Robot_mcu_voltage
{
    unsigned short mcu_voltage; // 下位機電壓
};

#pragma pack(0)

class Data_holder
{
public:
   static Data_holder *get()
    {
        static Data_holder dh;
        return &dh;
    }

    void load_parameter();

    void save_parameter();

    static void dump_params(struct Robot_parameter *params);

private:
    Data_holder()
    {
      memset(&firmware_info, 0, sizeof(struct Robot_firmware));
      memset(&parameter, 0, sizeof(struct Robot_parameter));
      memset(&velocity, 0, sizeof(struct Robot_velocity));
      memset(&odom, 0, sizeof(struct Robot_odom));
      memset(&pid_data, 0, sizeof(struct Robot_pid_data));
      memset(&robot_status, 0, sizeof(struct Robot_status));
      memset(&lcd_status, 0, sizeof(struct Robot_lcd_status));
      memset(&mcu_voltage, 0, sizeof(struct Robot_mcu_voltage));
      memset(&imu_data, 0, sizeof(imu_data));
		memset(&sona_data, 0, sizeof(sona_data));
    }

public:
   struct Robot_firmware firmware_info;
   struct Robot_parameter parameter;
   struct Robot_velocity velocity;
   struct Robot_odom odom;
   struct Robot_pid_data pid_data;
   struct Robot_status robot_status;
   struct Robot_lcd_status lcd_status;
   struct Robot_mcu_voltage mcu_voltage;
    
   float imu_data[9];
	unsigned char sona_data[8];
};

#endif

// KUBOT_DATA_HOLDER_H_