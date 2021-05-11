#ifndef _TEMPLATES_HPP
#define _TEMPLATES_HPP

#include "fmt/format.h"

namespace CodeNect::Templates
{
inline static const char* incl_tcc = "#include <tcclib.h>\n"
	"//for assert for checking array out of bounds\n"
	"#include <assert.h>\n"
	"//for using tolower, toupper\n"
	"#include <ctype.h>\n";
inline static const char* incl_stdio = "//for using input and output functions like printf and getline\n"
	"#include <stdio.h>\n";
inline static const char* incl_stdlib = "//for assert for checking array out of bounds\n"
	"#include <assert.h>\n"
	"//for using malloc, realloc\n"
	"#include <stdlib.h>\n";
inline static const char* incl_math = "//for using math functions like sin, cos, tan, pow, and more\n"
	"#include <math.h>\n";
inline static const char* incl_string = "//for using strcmp and other string-related functions\n"
	"#include <string.h>\n";
inline static const char* incl_bool = "//C doesn't have native bool, so we implement our own\n"
	"#define bool int\n"
	"#define true 1\n"
	"#define false 0\n";

inline static const char* s_incl_section = "//START OF INCLUDES SECTION\n";
inline static const char* e_incl_section = "//END OF INCLUDES SECTION\n\n";
inline static const char* s_structs_section = "//START OF STRUCTS SECTION\n";
inline static const char* e_structs_section = "//END OF STRUCTS SECTION\n\n";

inline static const char* string_methods = "//START OF STRING METHODS\n"
	"char* string_to_lowercase(char* str)\n"
	"{\n"
	"  char* buffer = malloc(sizeof(char) * strlen(str));\n"
	"  strcpy(buffer, str);\n"
	"  for (int i = 0; i < strlen(buffer); i++)\n"
	"  {\n"
	"    buffer[i] = tolower(buffer[i]);\n"
	"  }\n"
	"  return buffer;\n"
	"}\n"
	"char* string_to_uppercase(char* str)\n"
	"{\n"
	"  char* buffer = malloc(sizeof(char) * strlen(str));\n"
	"  strcpy(buffer, str);\n"
	"  for (int i = 0; i < strlen(buffer); i++)\n"
	"  {\n"
	"    buffer[i] = toupper(buffer[i]);\n"
	"  }\n"
	"  return buffer;\n"
	"}\n"
	"char* reverse_string(char* str)\n"
	"{\n"
	"  char* buffer = malloc(sizeof(char) * strlen(str));\n"
	"  strcpy(buffer, str);\n"
	"  int start = 0;\n"
	"  int end = strlen(buffer) - 1;\n"
	"  char ch;\n"
	"  while (start < end)\n"
	"  {\n"
	"    ch = buffer[start];\n"
	"    buffer[start] = buffer[end];\n"
	"    buffer[end] = ch;\n"
	"    start++;\n"
	"    end--;\n"
	"  }\n"
	"  return buffer;\n"
	"}\n"
	"//END OF STRING METHODS\n";

inline static const char* struct_prompt = "//START OF PROMPT STRUCT\n"
	"typedef struct Prompt\n"
	"{\n"
	"  char* buffer;\n"
	"  int max_size;\n"
	"  int used;\n"
	"} Prompt;\n\n"
	"void init_prompt(Prompt* prompt, int max_size)\n"
	"{\n"
	"  prompt->max_size = max_size;\n"
	"  prompt->buffer = (char*)malloc(max_size * sizeof(char));\n"
	"}\n\n"
	"void run_prompt(Prompt* prompt, const char* msg, bool is_newline)\n"
	"{\n"
	"  if (is_newline)\n"
	"    printf(\"%s\\n\", msg);\n"
	"  else\n"
	"    printf(\"%s\", msg);\n"
	"  if (!fgets(prompt->buffer, prompt->max_size, stdin))\n"
	"  {\n"
	"    printf(\"Error in reading input\\n\");\n"
	"    exit(1);\n"
	"  }\n"
	"  prompt->buffer[strlen(prompt->buffer) - 1] = '\\0';\n"
	"}\n"
	"//END OF PROMPT STRUCT\n";

inline static const char* def_cast = "//START OF METHODS FOR CAST\n"
	"bool string_to_bool(char* str)\n"
	"{\n"
	"  if (strcmp(str, \"true\") == 0)\n"
	"    return true;\n"
	"  if (strcmp(str, \"false\") == 0)\n"
	"    return false;\n"
	"  if (strcmp(str, \"1\") == 0)\n"
	"    return true;\n"
	"  if (strcmp(str, \"0\") == 0)\n"
	"    return false;\n"
	"  return false;\n"
	"}\n\n"
	"const char* bool_to_string(bool b)\n"
	"{\n"
	"  return b ? \"true\" : \"false\";\n"
	"}\n\n"
	"bool int_to_bool(int n)\n"
	"{\n"
	"  return (n != 0);\n"
	"}\n"
	"//END OF METHODS FOR CAST\n";

inline static const char* tpl_dynamic_arr = "//START OF DYNAMIC ARRAY METHOD FOR {type}\n"
	"typedef struct {name}\n"
	"{{\n"
	"  {type}* array;\n"
	"  int used;\n"
	"  int size;\n"
	"}} {name};\n\n"
	"void d_init_arr_{type_name}({name}* array, int size)\n"
	"{{\n"
	"  array->array = malloc(size * sizeof({type}));\n"
	"  array->used = 0;\n"
	"  array->size = size;\n"
	"}}\n\n"
	"void d_insert_{type_name}({name}* array, {type} value)\n"
	"{{\n"
	"  if (array->used == array->size)\n"
	"  {{\n"
	"    array->size *= 2;\n"
	"    array->array = realloc(array->array, array->size * sizeof({type}));\n"
	"  }}\n"
	"  array->array[array->used++] = value;\n"
	"}}\n\n"
	"void d_insert_{type_name}_array({name}* array, {type}* elements, int size)\n"
	"{{\n"
	"  for (int i = 0; i < size; i++)\n"
	"  {{\n"
	"    d_insert_{type_name}(array, elements[i]);\n"
	"  }}\n"
	"}}\n\n"
	"void d_free_{type_name}({name}* array)\n"
	"{{\n"
	"  free(array->array);\n"
	"  array->array = NULL;\n"
	"  array->used = array->size = 0;\n"
	"}}\n"
	"//END OF DYNAMIC ARRAY STRUCT FOR {type}\n";

inline static const char* tpl_fixed_arr = "//START OF FIXED ARRAY METHOD FOR {type}\n"
	"typedef struct {name}\n"
	"{{\n"
	"  {type}* array;\n"
	"  int used;\n"
	"  int size;\n"
	"}} {name};\n\n"
	"void f_init_arr_{type_name}({name}* array, int size)\n"
	"{{\n"
	"  array->array = malloc(size * sizeof({type}));\n"
	"  array->used = 0;\n"
	"  array->size = size;\n"
	"}}\n\n"
	"void f_insert_{type_name}({name}* array, {type} value)\n"
	"{{\n"
	"  assert((++array->used > array->size) && \"Out of bounds error. Can't insert element to FixedArray as it exceeds max size\");\n"
	"  array->array[array->used++] = value;\n"
	"}}\n\n"
	"void f_insert_{type_name}_array({name}* array, {type}* elements, int size)\n"
	"{{\n"
	"  for (int i = 0; i < size; i++)\n"
	"  {{\n"
	"    f_insert_{type_name}(array, elements[i]);\n"
	"  }}\n"
	"}}\n\n"
	"void f_free_{type_name}({name}* array)\n"
	"{{\n"
	"  free(array->array);\n"
	"  array->array = NULL;\n"
	"  array->used = array->size = 0;\n"
	"}}\n"
	"//END OF FIXED ARRAY STRUCT FOR {type}\n";

//Dynamic
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

//Fixed
inline static const std::string f_arr_bool = fmt::format(tpl_fixed_arr,
	fmt::arg("type", "bool"), fmt::arg("name", "FixedArrayBool"), fmt::arg("type_name", "bool"));
inline static const std::string f_arr_int = fmt::format(tpl_fixed_arr,
	fmt::arg("type", "int"), fmt::arg("name", "FixedArrayInt"), fmt::arg("type_name", "int"));
inline static const std::string f_arr_float = fmt::format(tpl_fixed_arr,
	fmt::arg("type", "float"), fmt::arg("name", "FixedArrayFloat"), fmt::arg("type_name", "float"));
inline static const std::string f_arr_double = fmt::format(tpl_fixed_arr,
	fmt::arg("type", "double"), fmt::arg("name", "FixedArrayDouble"), fmt::arg("type_name", "double"));
inline static const std::string f_arr_string = fmt::format(tpl_fixed_arr,
	fmt::arg("type", "const char*"), fmt::arg("name", "FixedArrayString"), fmt::arg("type_name", "string"));
}

#endif //_TEMPLATES_HPP
