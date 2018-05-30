#ifndef __SHARED_ENUMS_H__
#define __SHARED_ENUMS_H__

//--------------------------------------------------------------------
// Enums shared between the library and the editor
//--------------------------------------------------------------------

typedef enum PacketType 
{
	PT_CONNECTION_ACK,
	PT_BT_FILE, // Contains the whole BT to be recreated in the Editor
	PT_BT_FILE_ACK,
	PT_BT_UPDATE,
	PT_BT_UPDATE_ACK,
	PT_NULL_STATE
}PacketType;

#define MAX_PACKET_SIZE 512
#define PACKET_HEADER_SIZE (sizeof(int)*3)

typedef enum SampleType SampleType;
enum SampleType
{
	BBVAR_CHANGED,
	NODE_RETURNS,
	NEW_CURRENT_NODE,
	DECORATOR_CONDITION
};

typedef enum BBVarType BBVarType;
enum BBVarType
{
	BV_BOOL,
	BV_INT,
	BV_FLOAT,
	BV_STRING
};

typedef enum NodeStatus
{
	NS_INVALID,
	NS_SUCCESS,
	NS_FAILURE,
	NS_RUNNING,
	NS_SUSPENDED
}NodeStatus;


#endif // !__SHARED_ENUMS_H__
