#ifndef _TEMPLATES_HPP
#define _TEMPLATES_HPP

#include "fmt/format.h"

namespace CodeNect::Templates
{
inline static const char* tpl_dynamic_arr = "//start of dynamic array for {type}\n"
	"typedef struct\n"
	"{{\n"
	"  {type}* array;\n"
	"  int used;\n"
	"  int size;\n"
	"}} {name};\n"
	"void init_d_arr_{type}({name}* array, int size)\n"
	"{{\n"
	"  array->array = malloc(size * sizeof({type}));\n"
	"  array->used = 0;\n"
	"  array->size = size;\n"
	"}}\n"
	"void insert_{type_name}({name}* array, {type} value)\n"
	"{{\n"
	"  if (array->used == array->size)\n"
	"  {{\n"
	"    array->size *= 2;\n"
	"    array->array = realloc(array->array, array->size * sizeof({type}));\n"
	"  }}\n"
	"  array->array[array->used++] = value;\n"
	"}}\n"
	"void insert_{type_name}_array({name}* array, {type} elements, int size)\n"
	"{{\n"
	"  for (int i = 0; i < size; i++)\n"
	"  {{\n"
	"    insert_{type}(&array, elements[i]);\n"
	"  }}\n"
	"}}\n"
	"void free_{type_name}({name}* array)\n"
	"{{\n"
	"  free(array->array);\n"
	"  array->array = NULL;\n"
	"  array->used = array->size = 0;\n"
	"}}\n"
	"//end of dynamic array for {type}\n";

inline static const std::string d_arr_bool = fmt::format(tpl_dynamic_arr,
	fmt::arg("type", "bool"), fmt::arg("name", "DynamicArrayBool"), fmt::arg("type_name", "bool"));

inline static const std::string d_arr_int = fmt::format(tpl_dynamic_arr,
	fmt::arg("type", "int"), fmt::arg("name", "DynamicArrayInt"), fmt::arg("type_name", "int"));

inline static const std::string d_arr_float = fmt::format(tpl_dynamic_arr,
	fmt::arg("type", "float"), fmt::arg("name", "DynamicArrayFloat"), fmt::arg("type_name", "float"));

inline static const std::string d_arr_double = fmt::format(tpl_dynamic_arr,
	fmt::arg("type", "double"), fmt::arg("name", "DynamicArrayDouble"), fmt::arg("type_name", "double"));

inline static const std::string d_arr_string = fmt::format(tpl_dynamic_arr,
	fmt::arg("type", "const char*"), fmt::arg("name", "DynamicArrayString"), fmt::arg("type_name", "string"));
}

#endif //_TEMPLATES_HPP
