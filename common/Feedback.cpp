 
#include "FeedBack.h"
#include <string>
#include <cstring>
#include <iostream>

FeedbackStat::FeedbackStat()
{

}

FeedbackStat::~FeedbackStat()
{

}

void FeedbackStat::receiveCanData()
{
    int i;

    for(i=0; i<FeedbackStat::num_of_rec; i++)
    {
        dealCanData(FeedbackStat::can_receive_struct[i]);
    }
}

void FeedbackStat::dealCanData(CAN_OBJ candata)
{
    uint32_t id_normal = candata.ID;

    if(id_normal == 0x200)
    {
        dealPDO200Status(candata);
    } else if (id_normal == 0x300)
    {
        dealPDO300Status(candata);
    } else if (id_normal == 0x400)
    {
        dealPDO400Status(candata);
    } else if (id_normal == 0x500)
    {
        dealPDO500Status(candata);
    }
}

FeedbackData FeedbackStat::GetFeedback()
{
    std::lock_guard<std::mutex> guard(feedback_state_mutex_);
    return feedBackState_;
}

void FeedbackStat::dealPDO200Status(CAN_OBJ candata)
{
    std::lock_guard<std::mutex> guard(feedback_state_mutex_);

    FeedbackStat::feedBackData.feedback.water_temp_alarm = static_cast<uint8_t>(candata.Data[0] & 0b01);
    FeedbackStat::feedBackData.feedback.oil_pressure_low_alarm = static_cast<uint8_t>((candata.Data[0] & 0b10) >> 2);
    FeedbackStat::feedBackData.feedback.air_filter_alarm = static_cast<uint8_t>((candata.Data[0] & 0b100) >> 3);
    FeedbackStat::feedBackData.feedback.right_oil_filter_alarm = static_cast<uint8_t>((candata.Data[0] & 0b1000) >> 4);
    FeedbackStat::feedBackData.feedback.left_oil_filter_alarm = static_cast<uint8_t>((candata.Data[0] & 0b10000) >> 5);
    FeedbackStat::feedBackData.feedback.oil_loop_filter_alarm = static_cast<uint8_t>((candata.Data[0] & 0b100000) >> 6);
    FeedbackStat::feedBackData.feedback.hydr_pressure_low_alarm = static_cast<uint8_t>((candata.Data[0] & 0b1000000) >> 7);

    FeedbackStat::feedBackData.feedback.engine_rpm = ((uint8_t)candata.Data[2]<<8) + (uint8_t)candata.Data[3];
    FeedbackStat::feedBackData.feedback.oil_pressure = static_cast<uint8_t>((candata.Data[4]));
    FeedbackStat::feedBackData.feedback.water_temperature = static_cast<uint8_t>((candata.Data[5]));
    FeedbackStat::feedBackData.feedback.left_MA_pressure = ((uint8_t)candata.Data[6]<<8) + (uint8_t)candata.Data[7];
}

void FeedbackStat::dealPDO300Status(CAN_OBJ candata)
{
    std::lock_guard<std::mutex> guard(feedback_state_mutex_);

    FeedbackStat::feedBackData.feedback.left_MB_pressure = ((uint8_t)candata.Data[0]<<8) + (uint8_t)candata.Data[1];
    FeedbackStat::feedBackData.feedback.right_MA_pressure = ((uint8_t)candata.Data[2]<<8) + (uint8_t)candata.Data[3];
    FeedbackStat::feedBackData.feedback.right_MB_pressure = ((uint8_t)candata.Data[4]<<8) + (uint8_t)candata.Data[5];
    FeedbackStat::feedBackData.feedback.pump_pressure = ((uint8_t)candata.Data[6]<<8) + (uint8_t)candata.Data[7];
}

void FeedbackStat::dealPDO400Status(CAN_OBJ candata)
{
    std::lock_guard<std::mutex> guard(feedback_state_mutex_);

    FeedbackStat::feedBackData.feedback.left_motor_rpm = ((uint8_t)candata.Data[0]<<8) + (uint8_t)candata.Data[1];
    FeedbackStat::feedBackData.feedback.right_motor_rpm = ((uint8_t)candata.Data[2]<<8) + (uint8_t)candata.Data[3];

    FeedbackStat::feedBackData.feedback.hydr_oil_height = (uint8_t)candata.Data[4];
    FeedbackStat::feedBackData.feedback.disel_oil_height = (uint8_t)candata.Data[5];

    FeedbackStat::feedBackData.feedback.up_time = ((uint8_t)candata.Data[6]<<8) + (uint8_t)candata.Data[7];
}

void FeedbackStat::dealPDO500Status(CAN_OBJ candata)
{
    std::lock_guard<std::mutex> guard(feedback_state_mutex_);

    FeedbackStat::feedBackData.feedback.votage = (uint8_t)candata.Data[0];
}

CAN_OBJ FeedbackStat::can_receive_struct[100];
volatile int32_t FeedbackStat::num_of_rec = 0;
volatile FeedbackData FeedbackStat::feedBackData;