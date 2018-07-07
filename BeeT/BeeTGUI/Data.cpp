#include "Data.h"

Data::Data()
{
	root_value = json_value_init_object();
	root = json_value_get_object(root_value);
}

Data::Data(const char * filename)
{
	root_value = json_parse_string(filename);
	root = json_value_get_object(root_value);
}

Data::Data(JSON_Object* root) : root(root)
{}

Data::~Data()
{
	json_value_free(root_value);
}

bool Data::AppendArray(const char * name)
{
	JSON_Value* value = json_value_init_array();
	array = json_value_get_array(value);
	return json_object_set_value(root, name, value) == JSONSuccess;
}

bool Data::AppendArrayValue(const Data & data)
{
	if (!array)
		return false;
	return json_array_append_value(array, json_value_deep_copy(data.root_value)) == JSONSuccess;
}

bool Data::AppendString(const char * name, const char * string)
{
	return json_object_set_string(root, name, string) == JSONSuccess;
}

bool Data::AppendBool(const char * name, bool value)
{
	return json_object_set_boolean(root, name, value) == JSONSuccess;
}

bool Data::AppendUInt(const char * name, unsigned int value)
{
	return json_object_set_number(root, name, (double)value) == JSONSuccess;
}

bool Data::AppendInt(const char * name, int value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}

/*bool Data::AppendMatrix(const char * name, const math::float4x4 & matrix)
{
	JSON_Value* value = json_value_init_array();
	JSON_Array* array = json_value_get_array(value);
	const float* matrix_array = *matrix.v;

	for (int i = 0; i < 16; i++)
		json_array_append_number(array, matrix_array[i]);

	return json_object_set_value(root, name, value) == JSONSuccess;
}*/

bool Data::AppendFloat(const char * name, float value)
{
	return json_object_set_number(root, name, (double)value) == JSONSuccess;
}

bool Data::AppendFloat2(const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 2; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool Data::AppendFloat2(const char * name, const float2 & value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	json_array_append_number(array, value.x);
	json_array_append_number(array, value.y);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool Data::AppendInt2(const char * name, const int * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 2; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}


bool Data::AppendFloat3(const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 3; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool Data::AppendFloat3(const char * name, const float3 & value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	json_array_append_number(array, value.x);
	json_array_append_number(array, value.y);
	json_array_append_number(array, value.z);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool Data::AppendFloat4(const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 4; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
}

bool Data::AppendDouble(const char * name, double value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}

Data Data::AppendJObject(const char * name)
{
	json_object_set_value(root, name, json_value_init_object());
	return GetJObject(name);
}

Data Data::GetJObject(const char* name)const
{
	return Data(json_object_get_object(root, name));
}

const char * Data::GetString(const char* name) const
{
	return json_object_get_string(root, name);
}

int Data::GetInt(const char* name)const
{
	return (int)json_object_get_number(root, name);
}

Data Data::GetArray(const char * name, unsigned int index) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
		return Data(json_array_get_object(j_array, index));
	return Data((JSON_Object*) nullptr);
}

size_t Data::GetArraySize(const char* name) const
{
	size_t size = 0;
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
		size = json_array_get_count(j_array);

	return size;
}

bool Data::GetBool(const char * name) const
{
	int ret = json_object_get_boolean(root, name);
	return (ret > 0) ? true : false;
}

unsigned int Data::GetUInt(const char * name) const
{
	return (unsigned int)json_object_get_number(root, name);
}

/*float4x4 Data::GetMatrix(const char * name) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
	{
		float4x4 ret((float)json_array_get_number(j_array, 0),
			(float)json_array_get_number(j_array, 1),
			(float)json_array_get_number(j_array, 2),
			(float)json_array_get_number(j_array, 3),
			(float)json_array_get_number(j_array, 4),
			(float)json_array_get_number(j_array, 5),
			(float)json_array_get_number(j_array, 6),
			(float)json_array_get_number(j_array, 7),
			(float)json_array_get_number(j_array, 8),
			(float)json_array_get_number(j_array, 9),
			(float)json_array_get_number(j_array, 10),
			(float)json_array_get_number(j_array, 11),
			(float)json_array_get_number(j_array, 12),
			(float)json_array_get_number(j_array, 13),
			(float)json_array_get_number(j_array, 14),
			(float)json_array_get_number(j_array, 15));

		return ret;
	}

	return float4x4::identity;
}*/

float Data::GetFloat(const char * name) const
{
	return (float)json_object_get_number(root, name);
}

float2 Data::GetFloat2(const char * name) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
	{
		float2 ret((float)json_array_get_number(j_array, 0),
			(float)json_array_get_number(j_array, 1));

		return ret;
	}
	return float2(0,0);
}

Point<int> Data::GetInt2(const char * name) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
	{
		Point<int> ret((int)json_array_get_number(j_array, 0),
			(int)json_array_get_number(j_array, 1));

		return ret;
	}
	return Point<int>(0, 0);
}

float3 Data::GetFloat3(const char * name) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
	{
		float3 ret((float)json_array_get_number(j_array, 0),
			(float)json_array_get_number(j_array, 1),
			(float)json_array_get_number(j_array, 2));

		return ret;
	}
	return float3();
}
/*

float4 Data::GetFloat4(const char * name) const
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
	{
		float4 ret((float)json_array_get_number(j_array, 0),
			(float)json_array_get_number(j_array, 1),
			(float)json_array_get_number(j_array, 2),
			(float)json_array_get_number(j_array, 3));

		return ret;
	}
	return float4::zero;
}*/

double Data::GetDouble(const char * name) const
{
	return json_object_get_number(root, name);
}

void Data::LoadArray(const char* name)
{
	JSON_Array* j_array = json_object_get_array(root, name);
	if (j_array)
		array = j_array;
}

size_t Data::Serialize(char ** buffer)
{
	size_t size = json_serialization_size_pretty(root_value);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(root_value, *buffer, size);
	return size;
}

bool Data::IsNull() const
{
	return (root_value == nullptr && root == nullptr && array == nullptr) ? true : false;
}