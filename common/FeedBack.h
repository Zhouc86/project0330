#pragma once
#include <stdint.h>
#include <mutex>
#include <thread>
#include "comm.h"

// typedef struct
// {
//     union 
//     {
//         struct
//         {
//             //头 数据长度 1个字节 
//             uint8_t header;
//             // canid 4个字节
//             uint32_t canid;
//             // 8字节数据
//             uint8_t data1;
//             uint8_t data2;
//             uint8_t data3;
//             uint8_t data4;
//             uint8_t data5;
//             uint8_t data6;
//             uint8_t data7;
//             uint8_t data8;
//         } msg;
//         uint8_t rawMsg[13];
//     } data;
// } CANFrame;

// typedef enum
// {
//     MsgNone = 0x00,
//     PDO200Msg = 0x200,
//     Feedback2Msg = 0x300,
//     Feedback3Msg = 0x400,
//     Feedback4Msg = 0x500,
// } MsgType;

//反馈信息表, 一共有4条信息
//第一条 CANid 0x200+node id
// typedef struct {
//     union
//     {
//         struct
//         {
//             struct
//             {
//                 unsigned water_temp_alarm; //水温高报警
//                 unsigned oil_pressure_low_alarm; //机油压力低报警
//                 unsigned air_filter_alarm; //空滤堵塞报警
//                 unsigned left_oil_filter_alarm; //左行走补油滤网堵塞报警
//                 unsigned right_oil_filter_alarm; //右行走补油滤网堵塞报警
//                 unsigned oil_loop_filter_alarm; //回油滤网堵塞报警
//                 unsigned hydr_pressure_low_alarm; //液压油箱位低报警
//                 unsigned reserved7;
//             } group1;
//             uint8_t reserved1;
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } engine_rpm; //发动机转速
//             uint8_t oil_pressure; //机油压力
//             uint8_t water_temperature; //水温
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } left_MA_pressure; //左行走MA压力
//         } status;
//         uint8_t raw[8];
//     } data;
// } Feedback1Message;

// //第二条 CANid 0x300+node id
// typedef struct {
//     union
//     {
//         struct
//         {
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } left_MB_pressure; //左行走MB压力
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } right_MA_pressure; //右行走MA压力
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } right_MB_pressure; //右行走MB压力
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } pump_pressure; // 工作泵压力
//         } status;
//         uint8_t raw[8];
//     } data;
// } Feedback2Message;

// //第三条 CANid 0x400+node id
// typedef struct {
//     union
//     {
//         struct
//         {
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } left_motor_rpm; //左马达转速
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } right_motor_rpm; //右马达转速
//             uint8_t hydr_oil_height;  //液压油液位
//             uint8_t disel_oil_height;  //柴油油位
//             struct
//             {
//                 uint8_t high_byte;
//                 uint8_t low_byte;
//             } up_time;  //工作小时
//         } status;
//         uint8_t raw[8];
//     } data;
// } Feedback3Message;

// //第四条 CANid 0x500+node id
// typedef struct {
//     union
//     {
//         struct
//         {
//             uint8_t votage; // 电压
//             uint8_t reserved1;
//             uint8_t reserved2;
//             uint8_t reserved3;
//             uint8_t reserved4;
//             uint8_t reserved5;
//             uint8_t reserved6;
//             uint8_t reserved7;            
//         } status;
//         uint8_t raw[8];
//     } data;
// } Feedback4Message;

// typedef struct 
// {
//     MsgType type;
//     union {
//         Feedback1Message feedback_1_msg;
//         Feedback2Message feedback_2_msg;
//         Feedback3Message feedback_3_msg;
//         Feedback4Message feedback_4_msg;
//     } body;
// } CANMsg;

struct FeedbackData
{
    struct ControlState
    {
        uint8_t main_start = 0;
        uint8_t horn = 0;
        uint8_t front_light = 0;
        uint8_t rear_light = 0;
        uint8_t front_rear_light = 0;
        uint8_t enable = 0;
        uint8_t brake_release = 0;
        uint8_t connect = 0;
        uint8_t connect_start = 0;
        uint8_t pilot_valve = 0;
        uint8_t em_stop = 0;
        uint8_t em_stop_inverted = 0;
        int8_t shovel_loader = 0;
        int8_t shovel_arm = 0;
        int8_t direction = 0;
        int8_t moverment = 0;
    };

    struct Feedback
    {
        uint8_t water_temp_alarm = 0;
        uint8_t oil_pressure_low_alarm = 0;
        uint8_t air_filter_alarm = 0;
        uint8_t left_oil_filter_alarm = 0;
        uint8_t right_oil_filter_alarm = 0;
        uint8_t oil_loop_filter_alarm = 0;
        uint8_t hydr_pressure_low_alarm = 0;
        int32_t engine_rpm = 0;
        uint8_t oil_pressure = 0;
        uint8_t water_temperature = 0;
        int32_t left_MA_pressure = 0;
        int32_t left_MB_pressure = 0;
        int32_t right_MA_pressure = 0;
        int32_t right_MB_pressure = 0;
        int32_t pump_pressure = 0;
        int32_t left_motor_rpm = 0;
        int32_t right_motor_rpm = 0;
        uint8_t hydr_oil_height = 0;
        uint8_t disel_oil_height = 0;
        int32_t up_time = 0;
        uint8_t votage = 0;
    };

    ControlState controlStat;
    Feedback feedback;
};

 
typedef  struct  _CAN_OBJ{
	uint32_t	ID;
	uint8_t		NodeId;
	uint32_t	TimeStamp;
	uint8_t		TimeFlag;
	uint8_t		SendType;
	uint8_t		RemoteFlag; //是否是远程帧
	uint8_t		ExternFlag; //是否是扩展帧
	uint8_t		DataLen;
	uint8_t		Data[8];
}CAN_OBJ;

class FeedbackStat {
    public:
        FeedbackStat();
        ~FeedbackStat();
        // static void UpdateState(const CANMsg &can_msg, FeedbackState &feedBackState);

        FeedbackData GetFeedback();
        FeedbackData feedBackState_;
       
        void receiveCanData();
        void dealCanData(CAN_OBJ candata);

        static volatile int32_t num_of_rec;
        static CAN_OBJ can_receive_struct[100];

        void dealPDO200Status(CAN_OBJ candata);
        void dealPDO300Status(CAN_OBJ candata);
        void dealPDO400Status(CAN_OBJ candata);
        void dealPDO500Status(CAN_OBJ candata);

        static volatile FeedbackData feedBackData;

    private:
        std::mutex feedback_state_mutex_;
};
