#include "BeeT_Serializer.h"

BeeT_Serializer::BeeT_Serializer()
{
	root_value = json_value_init_object();
	root = json_value_get_object(root_value);
}

BeeT_Serializer::BeeT_Serializer(const char * filename)
{
	root_value = json_parse_string(filename);
	root = json_value_get_object(root_value);
}

BeeT_Serializer::BeeT_Serializer(JSON_Object* root) : root(root)
{}

BeeT_Serializer::~BeeT_Serializer()
{
	json_value_free(root_value);
}

bool BeeT_Serializer::AppendArray(const char * name)
{
	JSON_Value* value = json_value_init_array();
	array = json_value_get_array(value);
	return json_object_set_value(root, name, value) == JSONSuccess;
}

bool BeeT_Serializer::AppendArrayValue(const BeeT_Serializer & BeeT_Serializer)
{
	if (!array)
		return false;
	return json_array_append_value(array, json_value_deep_copy(BeeT_Serializer.root_value)) == JSONSuccess;
}

bool BeeT_Serializer::AppendString(const char * name, const char * string)
{
	return json_object_set_string(root, name, string) == JSONSuccess;
}

bool BeeT_Serializer::AppendBool(const char * name, bool value)
{
	return json_object_set_boolean(root, name, value) == JSONSuccess;
}

bool BeeT_Serializer::AppendUInt(const char * name, unsigned int value)
{
	return json_object_set_number(root, name, (double)value) == JSONSuccess;
}

bool BeeT_Serializer::AppendInt(const char * name, int value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}

/*bool BeeT_Serializer::AppendMatrix(const char * name, const math::float4x4 & matrix)
{
	JSON_Value* value = json_value_init_array();
	JSON_Array* array = json_value_get_array(value);
	const float* matrix_array = *matrix.v;

	for (int i = 0; i < 16; i++)
		json_array_append_number(array, matrix_array[i]);

	return json_object_set_value(root, name, value) == JSONSuccess;
}*/

bool BeeT_Serializer::AppendFloat(const char * name, float value)
{
	return json_object_set_number(root, name, (double)value) == JSONSuccess;
}

bool BeeT_Serializer::AppendFloat2(const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 2; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool BeeT_Serializer::AppendInt2(const char * name, const int * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 2; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}


bool BeeT_Serializer::AppendFloat3(const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 3; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool BeeT_Serializer::AppendFloat4(const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 4; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool BeeT_Serializer::AppendDouble(const char * name, double value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}

BeeT_Serializer BeeT_Serializer::AppendJObject(const char * name)
{
	json_object_set_value(root, name, json_value_init_object());
	return GetJObject(name);
}

BeeT_Serializer BeeT_Serializer::GetJObject(const char* name)const
{
	return BeeT_Serializer(json_object_get_object(root, name));
}

const char * BeeT_Serializer::GetString(const char* name) const
{
	return json_object_get_string(root, name);
}

int BeeT_Serializer::GetInt(const char* name)const
{
	return (int)json_object_get_number(root, name);
}

BeeT_Serializer BeeT_Serializer::GetArray(const char * name, unsigned int index) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
		return BeeT_Serializer(json_array_get_object(j_array, index));
	return BeeT_Serializer((JSON_Object*) nullptr);
}

size_t BeeT_Serializer::GetArraySize(const char* name) const
{
	size_t size = 0;
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
		size = json_array_get_count(j_array);

	return size;
}

bool BeeT_Serializer::GetBool(const char * name) const
{
	int ret = json_object_get_boolean(root, name);
	return (ret > 0) ? true : false;
}

unsigned int BeeT_Serializer::GetUInt(const char * name) const
{
	return (unsigned int)json_object_get_number(root, name);
}

float BeeT_Serializer::GetFloat(const char * name) const
{
	return (float)json_object_get_number(root, name);
}

double BeeT_Serializer::GetDouble(const char * name) const
{
	return json_object_get_number(root, name);
}

void BeeT_Serializer::LoadArray(const char* name)
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
		array = j_array;
}

size_t BeeT_Serializer::Serialize(char ** buffer)
{
	size_t size = json_serialization_size_pretty(root_value);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(root_value, *buffer, size);
	return size;
}

bool BeeT_Serializer::IsNull() const
{
	return (root_value == nullptr && root == nullptr && array == nullptr) ? true : false;
}