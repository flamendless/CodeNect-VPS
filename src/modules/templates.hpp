#ifndef _TEMPLATES_HPP
#define _TEMPLATES_HPP

#include "fmt/format.h"

namespace CodeNect::Templates
{
inline static const char* incl_tcc = "#include <tcclib.h>";
inline static const char* incl_stdio = "//for using input and output functions like printf and getline\n"
	"#include <stdio.h>\n";
inline static const char* incl_stdlib = "//for using malloc, realloc\n"
	"#include <stdlib.h>\n";
inline static const char* incl_math = "//for using math functions like sin, cos, tan, pow, and more\n"
	"#include <math.h>\n";

inline static const char* s_incl_section = "//START OF INCLUDES SECTION\n";
inline static const char* e_incl_section = "//END OF INCLUDES SECTION\n\n";
inline static const char* s_structs_section = "//START OF STRUCTS SECTION\n";
inline static const char* e_structs_section = "//END OF STRUCTS SECTION\n\n";

inline static const char* struct_prompt = "//start of prompt struct\n"
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
	"void run_prompt(Prompt* prompt, const char* msg)\n"
	"{\n"
	"  printf(\"%s\", msg);\n"
	"  if (!fgets(prompt->buffer, prompt->max_size, stdin))\n"
	"  {\n"
	"    printf(\"Error in reading input\n\");\n"
	"    exit(1);\n"
	"  }\n"
	"}\n"
	"//end of prompt struct\n";

inline static const char* tpl_dynamic_arr = "//start of dynamic array struct for {type}\n"
	"typedef struct {name}\n"
	"{{\n"
	"  {type}* array;\n"
	"  int used;\n"
	"  int size;\n"
	"}} {name};\n\n"
	"void init_d_arr_{type_name}({name}* array, int size)\n"
	"{{\n"
	"  array->array = malloc(size * sizeof({type}));\n"
	"  array->used = 0;\n"
	"  array->size = size;\n"
	"}}\n\n"
	"void insert_{type_name}({name}* array, {type} value)\n"
	"{{\n"
	"  if (array->used == array->size)\n"
	"  {{\n"
	"    array->size *= 2;\n"
	"    array->array = realloc(array->array, array->size * sizeof({type}));\n"
	"  }}\n"
	"  array->array[array->used++] = value;\n"
	"}}\n\n"
	"void insert_{type_name}_array({name}* array, {type}* elements, int size)\n"
	"{{\n"
	"  for (int i = 0; i < size; i++)\n"
	"  {{\n"
	"    insert_{type_name}(array, elements[i]);\n"
	"  }}\n"
	"}}\n\n"
	"void free_{type_name}({name}* array)\n"
	"{{\n"
	"  free(array->array);\n"
	"  array->array = NULL;\n"
	"  array->used = array->size = 0;\n"
	"}}\n"
	"//end of dynamic array struct for {type}\n";

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
