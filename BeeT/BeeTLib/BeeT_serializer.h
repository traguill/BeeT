#ifndef __BEET_SERIALIZER_H__
#define __BEET_SERIALIZER_H__

#include "ThirdParty/parson.h"

//Wrapper for Parson (JSON) library

class BeeT_Serializer
{
public:
	BeeT_Serializer();
	BeeT_Serializer(const char* filename);
	BeeT_Serializer(JSON_Object* root);
	~BeeT_Serializer();

	//Append
	bool AppendArray(const char* name);
	bool AppendArrayValue(const BeeT_Serializer& data);
	bool AppendString(const char* name, const char* string);
	bool AppendBool(const char* name, bool value);
	bool AppendUInt(const char* name, unsigned int value);
	bool AppendInt(const char* name, int value);
	bool AppendFloat(const char* name, float value);
	bool AppendFloat2(const char * name, const float * value);
	bool AppendInt2(const char* name, const int* value);
	bool AppendFloat3(const char* name, const float* value);
	bool AppendFloat4(const char* name, const float* value);
	bool AppendDouble(const char* name, double value);
	BeeT_Serializer AppendJObject(const char* name);

	//Gets
	BeeT_Serializer GetJObject(const char* name)const;
	const char* GetString(const char* name)const;
	int GetInt(const char* name)const;
	BeeT_Serializer GetArray(const char* name, unsigned int index)const;
	size_t GetArraySize(const char* name)const;
	bool GetBool(const char* name)const;
	unsigned int GetUInt(const char* name)const;
	float GetFloat(const char* name)const;
	double GetDouble(const char* name)const;

	//Loading an array into the data
	void LoadArray(const char* name);

	//Save
	size_t Serialize(char** buffer);

	bool IsNull()const;

private:
	JSON_Value* root_value = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;

};

#endif // !__DATA_H__