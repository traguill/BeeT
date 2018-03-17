#include "BeeT_Serializer.h"

struct BeeT_Serializer
{
		JSON_Value* root_value;
		JSON_Object* root;
		JSON_Array* array;
};

BeeT_Serializer * BeeT_Serializer__Create()
{
	BeeT_Serializer* result = (BeeT_Serializer*)malloc(sizeof(BeeT_Serializer));
	result->array = NULL;
	result->root_value = json_value_init_object();
	result->root = json_value_get_object(result->root_value);
	return result;
}

BeeT_Serializer * BeeT_Serializer__CreateFromBuffer(const char * buf)
{
	BeeT_Serializer* result = (BeeT_Serializer*)malloc(sizeof(BeeT_Serializer));
	result->array = NULL;
	result->root_value = json_parse_string(buf);
	result->root = json_value_get_object(result->root_value);
	return result;
}

BeeT_Serializer * BeeT_Serializer__CreateFromObject(JSON_Object * root)
{
	BeeT_Serializer* result = (BeeT_Serializer*)malloc(sizeof(BeeT_Serializer));
	result->array = NULL;
	result->root_value = NULL;
	result->root = root;
	return result;
}

void BeeT_Serializer__Destroy(BeeT_Serializer * self)
{
	json_value_free(self->root_value);
	free(self);
}

BEET_bool BeeT_Serializer__AppendArray(BeeT_Serializer* self, const char * name)
{
	JSON_Value* value = json_value_init_array();
	self->array = json_value_get_array(value);
	return json_object_set_value(self->root, name, value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendArrayValue(BeeT_Serializer* self, const BeeT_Serializer* data)
{
	if (!self->array)
		return BEET_FALSE;
	return json_array_append_value(self->array, json_value_deep_copy(data->root_value)) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendString(BeeT_Serializer* self, const char * name, const char * string)
{
	return json_object_set_string(self->root, name, string) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendBool(BeeT_Serializer* self, const char * name, BEET_bool value)
{
	return json_object_set_boolean(self->root, name, value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendUInt(BeeT_Serializer* self, const char * name, unsigned int value)
{
	return json_object_set_number(self->root, name, (double)value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendInt(BeeT_Serializer* self, const char * name, int value)
{
	return json_object_set_number(self->root, name, value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendFloat(BeeT_Serializer* self, const char * name, float value)
{
	return json_object_set_number(self->root, name, (double)value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendFloat2(BeeT_Serializer* self, const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 2; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(self->root, name, j_value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendInt2(BeeT_Serializer* self, const char * name, const int * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 2; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(self->root, name, j_value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendFloat3(BeeT_Serializer* self, const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 3; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(self->root, name, j_value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendFloat4(BeeT_Serializer* self, const char * name, const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 4; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(self->root, name, j_value) == JSONSuccess;
}

BEET_bool BeeT_Serializer__AppendDouble(BeeT_Serializer* self, const char * name, double value)
{
	return json_object_set_number(self->root, name, value) == JSONSuccess;
}

BeeT_Serializer* BeeT_Serializer__AppendJObject(BeeT_Serializer* self, const char * name)
{
	json_object_set_value(self->root, name, json_value_init_object());
	return BeeT_Serializer__GetJObject(self, name);
}

BeeT_Serializer* BeeT_Serializer__GetJObject(const BeeT_Serializer* self, const char* name)
{
	return BeeT_Serializer__Create(json_object_get_object(self->root, name));
}

const char * BeeT_Serializer__GetString(const BeeT_Serializer* self, const char* name) 
{
	return json_object_get_string(self->root, name);
}

int BeeT_Serializer__GetInt(const BeeT_Serializer* self, const char* name)
{
	return (int)json_object_get_number(self->root, name);
}

BeeT_Serializer* BeeT_Serializer__GetArray(const BeeT_Serializer* self, const char * name, unsigned int index) 
{
	JSON_Array* j_array = json_object_get_array(self->root, name);
	if (j_array)
		return BeeT_Serializer__Create(json_array_get_object(j_array, index));
	return BeeT_Serializer__Create((JSON_Object*) NULL);
}

size_t BeeT_Serializer__GetArraySize(const BeeT_Serializer* self, const char* name) 
{
	size_t size = 0;
	JSON_Array* j_array = json_object_get_array(self->root, name);
	if (j_array)
		size = json_array_get_count(j_array);

	return size;
}

BEET_bool BeeT_Serializer__GetBool(const BeeT_Serializer* self, const char * name)
{
	int ret = json_object_get_boolean(self->root, name);
	return (ret > 0) ? BEET_TRUE : BEET_FALSE;
}

unsigned int BeeT_Serializer__GetUInt(const BeeT_Serializer* self, const char * name)
{
	return (unsigned int)json_object_get_number(self->root, name);
}

float BeeT_Serializer__GetFloat(const BeeT_Serializer* self, const char * name)
{
	return (float)json_object_get_number(self->root, name);
}

double BeeT_Serializer__GetDouble(const BeeT_Serializer* self, const char * name)
{
	return json_object_get_number(self->root, name);
}

void BeeT_Serializer__LoadArray(BeeT_Serializer* self, const char* name)
{
	JSON_Array* j_array = json_object_get_array(self->root, name);
	if (j_array)
		self->array = j_array;
}

size_t BeeT_Serializer__Serialize(BeeT_Serializer* self, char ** buffer)
{
	size_t size = json_serialization_size_pretty(self->root_value);
	*buffer = malloc(size);
	json_serialize_to_buffer_pretty(self->root_value, *buffer, size);
	return size;
}

BEET_bool BeeT_Serializer__IsNull(const BeeT_Serializer* self)
{
	return (self->root_value == NULL && self->root == NULL && self->array == NULL) ? BEET_TRUE : BEET_FALSE;
}