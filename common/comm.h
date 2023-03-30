#pragma once
#include <unordered_map>
#include <vector>


template<typename T>
struct FVector3
{
	T X;
	T Y;
	T Z;
};
 
enum  RadarPosition :int32_t
{
	RADAR_LEFT,
	RADAR_RIGHT,
	RADAR_REAR,
	RADAR_ALL
};
enum  RenderPosition :int32_t
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	DASHBOARD,
	ALL
};
enum EgoType :int32_t
{
	None,
	User,
	 
	Car
};
enum ChannelType:int32_t
{
	CHANNEL_CAR,
	CHANNEL_IMU,
	CHANNEL_RADAR,
#ifdef LIDAR_SENSOR
    CHANNEL_LEFT_LIDAR,
	CHANNEL_RIGHT_LIDAR,
#endif
	CHANNEL_ALL,
};

 
enum UserState
{
	Offline,
	Online,
	Busy
};

struct UserCamera
{
	int32_t uid;
	EgoType type;
	UserState state;
	std::string name;
 
};
struct OfferDesc
{
	char type[16];
	char sdp[4096 * 3-16];
};
struct AnswerDesc
{
	char type[16];
	char sdp[4096 * 3 - 16];
};
struct CandidateDesc
{
	char  candidate[4096];
	char sdp_mid[4096];
	int32_t sdp_mline_index;
};
#pragma pack(1)
#define CAN_MSG_LEN  13
struct cannet_frame
{
	int8_t dlc;
	int32_t canid;
	int8_t data[8];
};
#pragma pack()
enum RobotStatus
{
	 OK,
	 Fail,
};