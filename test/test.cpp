#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "core/app.hpp"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "core/project.hpp"
#include "modules/filesystem.hpp"
#include "modules/input.hpp"
#include "node/connection.hpp"
#include "node/nodes.hpp"
#include "node/node_logic.hpp"
#include "node/node_def.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"

using namespace CodeNect;

bool inited = true;
void init()
{
	if (inited) return;
	App app;
	app.init();
	inited = true;
}

TEST_CASE("Testing Input/Output Module")
{
	REQUIRE(Input::v_keypresses.size() == 0);
	REQUIRE(Input::v_keyreleases.size() == 0);

	Input::register_key_event([](int key, int, int mods)
		{
			if (key == GLFW_KEY_T && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
				return true;
			return false;
		}, true);

	Input::register_key_event([](int key, int, int mods)
		{
			if (key == GLFW_KEY_T && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
				return true;
			return false;
		}, false);

	CHECK(Input::v_keypresses.size() == 1);
	CHECK(Input::v_keyreleases.size() == 1);
}

TEST_CASE("Testing Filesystem Module")
{
	init();

	SUBCASE("Opening valid project")
	{
		REQUIRE(Project::nodes_count == 0);
		REQUIRE(Project::connections_count == 0);
		REQUIRE(Project::has_open_proj == false);
		REQUIRE(Project::open("../../demo/name.cn") == RES_SUCCESS);
		REQUIRE(Project::nodes_count == 6);
		REQUIRE(Project::connections_count == 10);
		REQUIRE(Project::has_open_proj == true);
	}

	SUBCASE("Closing valid project")
	{
		Project::close();
		REQUIRE(Project::nodes_count == 0);
		REQUIRE(Project::connections_count == 0);
		REQUIRE(Project::has_open_proj == false);
	}

	SUBCASE("Opening invalid project")
	{
		REQUIRE(Project::nodes_count == 0);
		REQUIRE(Project::connections_count == 0);
		REQUIRE(Project::has_open_proj == false);
		REQUIRE(Project::open("../../demo/invalid.cn") == RES_FAIL);
		REQUIRE(Project::nodes_count == 0);
		REQUIRE(Project::connections_count == 0);
		REQUIRE(Project::has_open_proj == false);
	}
}

TEST_CASE("Testing Visual Nodes Module")
{
	SUBCASE("Testing Operation Logic")
	{
		REQUIRE(Project::open("../test_op.cn") == RES_SUCCESS);
		NodeLogic::process();
		SUBCASE("INTEGER")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_int"));
			REQUIRE(var_a != nullptr);
			REQUIRE(std::get<int>(var_a->m_value.data) == 32);
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_int"));
			REQUIRE(var_b != nullptr);
			REQUIRE(std::get<int>(var_b->m_value.data) == 64);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_int"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<int>(node_var->m_value.data) == 96);
			}

			SUBCASE("Testing Subtraction Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_sub_int"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<int>(node_var->m_value.data) == -32);
			}

			SUBCASE("Testing Multiplication Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_mul_int"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<int>(node_var->m_value.data) == 2048);
			}

			SUBCASE("Testing Division Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_div_int"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<int>(node_var->m_value.data) == 0);
			}
		}

		SUBCASE("FLOAT")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_float"));
			REQUIRE(var_a != nullptr);
			REQUIRE(std::get<float>(var_a->m_value.data) == 32.0f);
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_float"));
			REQUIRE(var_b != nullptr);
			REQUIRE(std::get<float>(var_b->m_value.data) == 24.0f);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_float"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<float>(node_var->m_value.data) == 56.0f);
			}

			SUBCASE("Testing Subtraction Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_sub_float"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<float>(node_var->m_value.data) == 8.0f);
			}

			SUBCASE("Testing Multiplication Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_mul_float"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<float>(node_var->m_value.data) == (32.0f * 24.0f));
			}

			SUBCASE("Testing Division Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_div_float"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<float>(node_var->m_value.data) == (32.0f/24.0f));
			}
		}

		SUBCASE("DOUBLE")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_double"));
			REQUIRE(var_a != nullptr);
			REQUIRE(std::get<double>(var_a->m_value.data) == 32.0);
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_double"));
			REQUIRE(var_b != nullptr);
			REQUIRE(std::get<double>(var_b->m_value.data) == 24.0);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_double"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<double>(node_var->m_value.data) == 56.0);
			}

			SUBCASE("Testing Subtraction Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_sub_double"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<double>(node_var->m_value.data) == 8.0);
			}

			SUBCASE("Testing Multiplication Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_mul_double"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<double>(node_var->m_value.data) == (32.0 * 24.0));
			}

			SUBCASE("Testing Division Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_div_double"));
				REQUIRE(node_var != nullptr);
				REQUIRE(std::get<double>(node_var->m_value.data) == (32.0/24.0));
			}
		}

		SUBCASE("STRING")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_string"));
			REQUIRE(var_a != nullptr);
			REQUIRE((std::get<std::string>(var_a->m_value.data).compare("hello, ") == 0));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_string"));
			REQUIRE(var_b != nullptr);
			REQUIRE((std::get<std::string>(var_b->m_value.data)).compare("world!") == 0);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_string"));
				REQUIRE(node_var != nullptr);
				REQUIRE((std::get<std::string>(node_var->m_value.data)).compare("hello, world!") == 0);
			}
		}
	}

	SUBCASE("Testing Cast Logic")
	{
		REQUIRE(Project::open("../test_cast.cn") == RES_SUCCESS);
		NodeLogic::process();

		SUBCASE("From INTEGER")
		{
			const int val = 8;
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_int"));
			REQUIRE(var != nullptr);
			REQUIRE(std::get<int>(var->m_value.data) == val);
			NodeLogic::process();

			SUBCASE("To BOOL")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_bool"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<bool>(var2->m_value.data) == (bool)val);
			}

			SUBCASE("To FLOAT")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_float"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<float>(var2->m_value.data) == (float)val);
			}

			SUBCASE("To DOUBLE")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_double"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<double>(var2->m_value.data) == (double)val);
			}

			SUBCASE("To STRING")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_string"));
				REQUIRE(var2 != nullptr);
				REQUIRE((std::get<std::string>(var2->m_value.data)).compare(std::to_string(val)) == 0);
			}
		}

		SUBCASE("From FLOAT")
		{
			const float val = 16.0f;
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_float"));
			REQUIRE(var != nullptr);
			REQUIRE(std::get<float>(var->m_value.data) == val);
			NodeLogic::process();

			SUBCASE("To INTEGER")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_int"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<int>(var2->m_value.data) == (int)val);
			}

			SUBCASE("To DOUBLE")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_double"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<double>(var2->m_value.data) == (double)val);
			}

			SUBCASE("To STRING")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_string"));
				REQUIRE(var2 != nullptr);
				REQUIRE((std::get<std::string>(var2->m_value.data)).compare(std::to_string(val)) == 0);
			}
		}

		SUBCASE("From DOUBLE")
		{
			const float val = 32.0;
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_double"));
			REQUIRE(var != nullptr);
			REQUIRE(std::get<double>(var->m_value.data) == val);
			NodeLogic::process();

			SUBCASE("To INTEGER")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_int"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<int>(var2->m_value.data) == (int)val);
			}

			SUBCASE("To FLOAT")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_float"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<float>(var2->m_value.data) == (float)val);
			}

			SUBCASE("To STRING")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_string"));
				REQUIRE(var2 != nullptr);
				REQUIRE((std::get<std::string>(var2->m_value.data)).compare(std::to_string(val)) == 0);
			}
		}

		SUBCASE("From STRING")
		{
			const std::string val = "48.64";
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_string"));
			REQUIRE(var != nullptr);
			REQUIRE((std::get<std::string>(var->m_value.data)).compare(val) == 0);
			NodeLogic::process();

			SUBCASE("To BOOL")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_bool"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<bool>(var2->m_value.data) == false);
			}

			SUBCASE("To INTEGER")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_int"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<int>(var2->m_value.data) == std::stoi(val));;
			}

			SUBCASE("To FLOAT")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_float"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<float>(var2->m_value.data) == std::stof(val));;
			}

			SUBCASE("To DOUBLE")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_double"));
				REQUIRE(var2 != nullptr);
				REQUIRE(std::get<double>(var2->m_value.data) == std::stod(val));;
			}
		}
	}
}
