#ifndef __BEET_SERIALIZER_H__
#define __BEET_SERIALIZER_H__

#include "ThirdParty/parson.h"

#include "beet_std.h"

//Wrapper for Parson (JSON) library

typedef struct BeeT_Serializer BeeT_Serializer;

BeeT_Serializer* BeeT_Serializer_Create();
BeeT_Serializer* BeeT_Serializer_CreateFromBuffer(const char* buf);
BeeT_Serializer* BeeT_Serializer_CreateFromObject(JSON_Object* root);

void BeeT_Serializer_Destroy(BeeT_Serializer* self);

// Append types
BEET_bool BeeT_Serializer_AppendArray(BeeT_Serializer* self, const char* name);
BEET_bool BeeT_Serializer_AppendArrayValue(BeeT_Serializer* self, const BeeT_Serializer* data);
BEET_bool BeeT_Serializer_AppendString(BeeT_Serializer* self, const char* name, const char* string);
BEET_bool BeeT_Serializer_AppendBool(BeeT_Serializer* self, const char* name, BEET_bool value);
BEET_bool BeeT_Serializer_AppendUInt(BeeT_Serializer* self, const char* name, unsigned int value);
BEET_bool BeeT_Serializer_AppendInt(BeeT_Serializer* self, const char* name, int value);
BEET_bool BeeT_Serializer_AppendFloat(BeeT_Serializer* self, const char* name, float value);
BEET_bool BeeT_Serializer_AppendFloat2(BeeT_Serializer* self, const char * name, const float * value);
BEET_bool BeeT_Serializer_AppendInt2(BeeT_Serializer* self, const char* name, const int* value);
BEET_bool BeeT_Serializer_AppendFloat3(BeeT_Serializer* self, const char* name, const float* value);
BEET_bool BeeT_Serializer_AppendFloat4(BeeT_Serializer* self, const char* name, const float* value);
BEET_bool BeeT_Serializer_AppendDouble(BeeT_Serializer* self, const char* name, double value);
BeeT_Serializer* BeeT_Serializer_AppendJObject(BeeT_Serializer* self, const char* name);

// Get types
BeeT_Serializer*	BeeT_Serializer_GetJObject(const BeeT_Serializer* self, const char* name);
const char*			BeeT_Serializer_GetString(const BeeT_Serializer* self, const char* name);
int					BeeT_Serializer_GetInt(const BeeT_Serializer* self, const char* name);
BeeT_Serializer*	BeeT_Serializer_GetArray(const BeeT_Serializer* self, const char* name, unsigned int index);
size_t				BeeT_Serializer_GetArraySize(const BeeT_Serializer* self, const char* name);
BEET_bool			BeeT_Serializer_GetBool(const BeeT_Serializer* self, const char* name);
unsigned int		BeeT_Serializer_GetUInt(const BeeT_Serializer* self, const char* name);
float				BeeT_Serializer_GetFloat(const BeeT_Serializer* self, const char* name);
double				BeeT_Serializer_GetDouble(const BeeT_Serializer* self, const char* name);
float2				BeeT_Serializer_GetFloat2(const BeeT_Serializer* self, const char* name);
float3				BeeT_Serializer_GetFloat3(const BeeT_Serializer* self, const char* name);

//Loading an array into the data
void BeeT_Serializer_LoadArray(BeeT_Serializer* self, const char* name);

//Save
size_t BeeT_Serializer_Serialize(BeeT_Serializer* self, char** buffer);

BEET_bool BeeT_Serializer_IsNull(const BeeT_Serializer* self);

#endif // !__DATA_H__