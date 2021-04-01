#include "node/node_val.hpp"

#include "ppk_assert.h"
#include "imgui.h"
#include "plog/Log.h"

namespace CodeNect
{
void NodeValue::add(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: std::get<int>(data) += std::get<int>(other.data); break;
		case NODE_SLOT::FLOAT: std::get<float>(data) += std::get<float>(other.data); break;
		case NODE_SLOT::DOUBLE: std::get<double>(data) += std::get<double>(other.data); break;
		case NODE_SLOT::STRING:
		{
			std::string str_a = std::get<std::string>(data);
			std::string str_b = std::get<std::string>(other.data);
			std::string str = str_a + str_b;
			data = str_a + str_b;
			break;
		}
	}
}

void NodeValue::sub(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: std::get<int>(data) -= std::get<int>(other.data); break;
		case NODE_SLOT::FLOAT: std::get<float>(data) -= std::get<float>(other.data); break;
		case NODE_SLOT::DOUBLE: std::get<double>(data) -= std::get<double>(other.data); break;
		case NODE_SLOT::STRING: break;
	}
}

void NodeValue::mul(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			std::get<int>(data) *= std::get<int>(other.data);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			std::get<float>(data) *= std::get<float>(other.data);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			std::get<double>(data) *= std::get<double>(other.data);
			break;
		}
		case NODE_SLOT::STRING: break;
	}
}

void NodeValue::div(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			std::get<int>(data) /= std::get<int>(other.data);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			std::get<float>(data) /= std::get<float>(other.data);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			std::get<double>(data) /= std::get<double>(other.data);
			break;
		}
		case NODE_SLOT::STRING: break;
	}
}

void NodeValue::mod(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			std::get<int>(data) %= std::get<int>(other.data);
			break;
		}
		case NODE_SLOT::FLOAT: break;
		case NODE_SLOT::DOUBLE: break;
		case NODE_SLOT::STRING: break;
	}
}

void NodeValue::cast_from(NodeValue& from_val)
{
	switch (from_val.m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::STRING: break;
		case NODE_SLOT::BOOL:
		{
			bool from_v = std::get<bool>(from_val.data);

			if (m_slot == +NODE_SLOT::INTEGER)
				this->set(from_v ? 1 : 0);
			else if (m_slot == +NODE_SLOT::STRING)
			{
				this->set(std::string(from_v == true ? "true" : "false"));
			}
			break;
		}
		case NODE_SLOT::INTEGER:
		{
			int from_v = std::get<int>(from_val.data);

			if (m_slot == +NODE_SLOT::FLOAT)
				this->set((float)from_v);
			else if (m_slot == +NODE_SLOT::DOUBLE)
				this->set((double)from_v);
			else if (m_slot == +NODE_SLOT::STRING)
				this->set(std::to_string(from_v));
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			float from_v = std::get<float>(from_val.data);

			if (m_slot == +NODE_SLOT::INTEGER)
				this->set((int)from_v);
			else if (m_slot == +NODE_SLOT::DOUBLE)
				this->set((double)from_v);
			else if (m_slot == +NODE_SLOT::STRING)
				this->set(std::to_string(from_v));
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			double from_v = std::get<double>(from_val.data);

			if (m_slot == +NODE_SLOT::INTEGER)
				this->set((int)from_v);
			else if (m_slot == +NODE_SLOT::FLOAT)
				this->set((float)from_v);
			else if (m_slot == +NODE_SLOT::STRING)
				this->set(std::to_string(from_v));
			break;
		}
	}
}

bool NodeValue::is_eq_to(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL:
		{
			return std::get<bool>(data) == std::get<bool>(other.data);
			break;
		}
		case NODE_SLOT::INTEGER:
		{
			return std::get<int>(data) == std::get<int>(other.data);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			return std::get<float>(data) == std::get<float>(other.data);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			return std::get<double>(data) == std::get<double>(other.data);
			break;
		}
		case NODE_SLOT::STRING:
		{
			return std::get<std::string>(data).compare(std::get<std::string>(other.data)) == 0;
			break;
		}
	}
	return false;
}

bool NodeValue::is_neq_to(NodeValue& other)
{
	return is_eq_to(other) == false;
}

bool NodeValue::is_lt_to(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			return std::get<int>(data) < std::get<int>(other.data);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			return std::get<float>(data) < std::get<float>(other.data);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			return std::get<double>(data) < std::get<double>(other.data);
			break;
		}
		case NODE_SLOT::STRING: break;
	}
	return false;
}

bool NodeValue::is_gt_to(NodeValue& other)
{
	return is_lt_to(other) == false && is_eq_to(other) == false;
}

bool NodeValue::is_lte_to(NodeValue& other)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			return std::get<int>(data) <= std::get<int>(other.data);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			return std::get<float>(data) <= std::get<float>(other.data);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			return std::get<double>(data) <= std::get<double>(other.data);
			break;
		}
		case NODE_SLOT::STRING: break;
	}
	return false;
}

bool NodeValue::is_gte_to(NodeValue& other)
{
	return is_eq_to(other) == true || is_lt_to(other) == false;
}

bool NodeValue::is_or_to(NodeValue& other)
{
	bool this_bool = std::get<bool>(data);
	bool other_bool = std::get<bool>(other.data);
	return this_bool || other_bool;
}

bool NodeValue::is_and_to(NodeValue& other)
{
	bool this_bool = std::get<bool>(data);
	bool other_bool = std::get<bool>(other.data);
	return this_bool && other_bool;
}

void NodeValue::set(bool arg){ data = arg; m_slot = NODE_SLOT::BOOL; }
void NodeValue::set(int arg){ data = arg; m_slot = NODE_SLOT::INTEGER; }
void NodeValue::set(float arg){ data = arg; m_slot = NODE_SLOT::FLOAT; }
void NodeValue::set(double arg){ data = arg; m_slot = NODE_SLOT::DOUBLE; }
void NodeValue::set(std::string arg){ data = arg; m_slot = NODE_SLOT::STRING; }

void NodeValue::copy(NodeValue& val)
{
	switch (val.m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: set(std::get<bool>(val.data)); break;
		case NODE_SLOT::INTEGER: set(std::get<int>(val.data)); break;
		case NODE_SLOT::FLOAT: set(std::get<float>(val.data)); break;
		case NODE_SLOT::DOUBLE: set(std::get<double>(val.data)); break;
		case NODE_SLOT::STRING: set(std::get<std::string>(val.data)); break;
	}
}

void NodeValue::copy(NODE_SLOT& slot)
{
	switch (slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: set(true); break;
		case NODE_SLOT::INTEGER: set(0); break;
		case NODE_SLOT::FLOAT: set((float)0); break;
		case NODE_SLOT::DOUBLE: set((double)0); break;
		case NODE_SLOT::STRING: set(std::string()); break;
	}
}

void NodeValue::to_bool(const char* str)
{
	if (std::strcmp(str, "true") == 0)
		set(true);
	else if (std::strcmp(str, "false") == 0)
		set(false);
	else
		PLOGE << "unable to convert '" << str << "' to bool";
}

void NodeValue::to_int(const char* str)
{
	int i = std::stoi(str);
	set(i);
}

void NodeValue::to_float(const char* str)
{
	float i = std::stof(str);
	set(i);
}

void NodeValue::to_double(const char* str)
{
	double i = std::stod(str);
	set(i);
}

void* NodeValue::get_value()
{
	// void* d = node.m_value.get_value();
	// const char* str = static_cast<const char*>(d);
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: return nullptr; break;
		case NODE_SLOT::BOOL: return &std::get<bool>(data); break;
		case NODE_SLOT::INTEGER: return &std::get<int>(data); break;
		case NODE_SLOT::FLOAT: return &std::get<float>(data); break;
		case NODE_SLOT::DOUBLE: return &std::get<double>(data); break;
		case NODE_SLOT::STRING: return (void*)std::get<std::string>(data).c_str(); break;
	}

	PPK_ASSERT(false, "This should not be reached");
	return nullptr;
}

const std::string NodeValue::get_value_str(void)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: return "empty"; break;
		case NODE_SLOT::BOOL: return std::get<bool>(data) ? "true" : "false"; break;
		case NODE_SLOT::INTEGER: return std::to_string(std::get<int>(data)); break;
		case NODE_SLOT::FLOAT: return std::to_string(std::get<float>(data)); break;
		case NODE_SLOT::DOUBLE: return std::to_string(std::get<double>(data)); break;
		case NODE_SLOT::STRING: return std::get<std::string>(data).c_str(); break;
	}
}

const std::string NodeValue::get_value_str_ex(void)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: return "empty"; break;
		case NODE_SLOT::BOOL: return std::get<bool>(data) ? "true" : "false"; break;
		case NODE_SLOT::INTEGER: return std::to_string(std::get<int>(data)); break;
		case NODE_SLOT::FLOAT: return std::to_string(std::get<float>(data)); break;
		case NODE_SLOT::DOUBLE: return std::to_string(std::get<double>(data)); break;
		case NODE_SLOT::STRING:
		{
			std::string str;
			str.append("\"");
			str.append(std::get<std::string>(data));
			str.append("\"");
			return str.c_str();
			break;
		}
	}

	PPK_ASSERT(false, "This should not be reached");
	return "";
}

const char* NodeValue::get_spec(void)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: return nullptr; break;
		case NODE_SLOT::BOOL: return "%d"; break;
		case NODE_SLOT::INTEGER: return "%d"; break;
		case NODE_SLOT::FLOAT: return "%f"; break;
		case NODE_SLOT::DOUBLE: return "%lf"; break;
		case NODE_SLOT::STRING: return "%s"; break;
	}

	PPK_ASSERT(false, "This should not be reached");
	return "";
}
}
