#ifndef __BEET_SERIALIZER_H__
#define __BEET_SERIALIZER_H__

#include "ThirdParty/parson.h"

typedef enum{false, true} bool;

//Wrapper for Parson (JSON) library

typedef struct BeeT_Serializer BeeT_Serializer;

BeeT_Serializer* BeeT_Serializer__Create();
BeeT_Serializer* BeeT_Serializer__Create(const char* filename);
BeeT_Serializer* BeeT_Serializer__Create(JSON_Object* root);

void BeeT_Serializer__Destroy(BeeT_Serializer* self);

// Append types
bool BeeT_Serializer__AppendArray(BeeT_Serializer* self, const char* name);
bool BeeT_Serializer__AppendArrayValue(BeeT_Serializer* self, const BeeT_Serializer* data);
bool BeeT_Serializer__AppendString(BeeT_Serializer* self, const char* name, const char* string);
bool BeeT_Serializer__AppendBool(BeeT_Serializer* self, const char* name, bool value);
bool BeeT_Serializer__AppendUInt(BeeT_Serializer* self, const char* name, unsigned int value);
bool BeeT_Serializer__AppendInt(BeeT_Serializer* self, const char* name, int value);
bool BeeT_Serializer__AppendFloat(BeeT_Serializer* self, const char* name, float value);
bool BeeT_Serializer__AppendFloat2(BeeT_Serializer* self, const char * name, const float * value);
bool BeeT_Serializer__AppendInt2(BeeT_Serializer* self, const char* name, const int* value);
bool BeeT_Serializer__AppendFloat3(BeeT_Serializer* self, const char* name, const float* value);
bool BeeT_Serializer__AppendFloat4(BeeT_Serializer* self, const char* name, const float* value);
bool BeeT_Serializer__AppendDouble(BeeT_Serializer* self, const char* name, double value);
BeeT_Serializer* AppendJObject(BeeT_Serializer* self, const char* name);

// Get types
BeeT_Serializer*	BeeT_Serializer__GetJObject(const BeeT_Serializer* self, const char* name);
const char*			BeeT_Serializer__GetString(const BeeT_Serializer* self, const char* name);
int					BeeT_Serializer__GetInt(const BeeT_Serializer* self, const char* name);
BeeT_Serializer*	BeeT_Serializer__GetArray(const BeeT_Serializer* self, const char* name, unsigned int index);
size_t				BeeT_Serializer__GetArraySize(const BeeT_Serializer* self, const char* name);
bool				BeeT_Serializer__GetBool(const BeeT_Serializer* self, const char* name);
unsigned int		BeeT_Serializer__GetUInt(const BeeT_Serializer* self, const char* name);
float				BeeT_Serializer__GetFloat(const BeeT_Serializer* self, const char* name);
double				BeeT_Serializer__GetDouble(const BeeT_Serializer* self, const char* name);

//Loading an array into the data
void BeeT_Serializer__LoadArray(BeeT_Serializer* self, const char* name);

//Save
size_t BeeT_Serializer__Serialize(BeeT_Serializer* self, char** buffer);

bool BeeT_Serializer__IsNull(const BeeT_Serializer* self);

#endif // !__DATA_H__