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
#include "node/node_cmp.hpp"
#include "node/node_array.hpp"

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
		CHECK(Project::nodes_count == 0);
		CHECK(Project::connections_count == 0);
		CHECK(Project::has_open_proj == false);
		REQUIRE(Project::open("../../demo/name.cn") == RES_SUCCESS);
		CHECK(Project::nodes_count == 6);
		CHECK(Project::connections_count == 10);
		CHECK(Project::has_open_proj == true);
	}

	SUBCASE("Closing valid project")
	{
		Project::close();
		CHECK(Project::nodes_count == 0);
		CHECK(Project::connections_count == 0);
		CHECK(Project::has_open_proj == false);
	}

	SUBCASE("Opening invalid project")
	{
		CHECK(Project::nodes_count == 0);
		CHECK(Project::connections_count == 0);
		CHECK(Project::has_open_proj == false);
		REQUIRE(Project::open("../../demo/invalid.cn") == RES_FAIL);
		CHECK(Project::nodes_count == 0);
		CHECK(Project::connections_count == 0);
		CHECK(Project::has_open_proj == false);
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
			CHECK(std::get<int>(var_a->m_value.data) == 32);
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_int"));
			REQUIRE(var_b != nullptr);
			CHECK(std::get<int>(var_b->m_value.data) == 64);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_int"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<int>(node_var->m_value.data) == 96);
			}

			SUBCASE("Testing Subtraction Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_sub_int"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<int>(node_var->m_value.data) == -32);
			}

			SUBCASE("Testing Multiplication Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_mul_int"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<int>(node_var->m_value.data) == 2048);
			}

			SUBCASE("Testing Division Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_div_int"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<int>(node_var->m_value.data) == 0);
			}
		}

		SUBCASE("FLOAT")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_float"));
			REQUIRE(var_a != nullptr);
			CHECK(std::get<float>(var_a->m_value.data) == 32.0f);
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_float"));
			REQUIRE(var_b != nullptr);
			CHECK(std::get<float>(var_b->m_value.data) == 24.0f);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_float"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<float>(node_var->m_value.data) == 56.0f);
			}

			SUBCASE("Testing Subtraction Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_sub_float"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<float>(node_var->m_value.data) == 8.0f);
			}

			SUBCASE("Testing Multiplication Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_mul_float"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<float>(node_var->m_value.data) == (32.0f * 24.0f));
			}

			SUBCASE("Testing Division Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_div_float"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<float>(node_var->m_value.data) == (32.0f/24.0f));
			}
		}

		SUBCASE("DOUBLE")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_double"));
			REQUIRE(var_a != nullptr);
			CHECK(std::get<double>(var_a->m_value.data) == 32.0);
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_double"));
			REQUIRE(var_b != nullptr);
			CHECK(std::get<double>(var_b->m_value.data) == 24.0);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_double"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<double>(node_var->m_value.data) == 56.0);
			}

			SUBCASE("Testing Subtraction Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_sub_double"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<double>(node_var->m_value.data) == 8.0);
			}

			SUBCASE("Testing Multiplication Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_mul_double"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<double>(node_var->m_value.data) == (32.0 * 24.0));
			}

			SUBCASE("Testing Division Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_div_double"));
				REQUIRE(node_var != nullptr);
				CHECK(std::get<double>(node_var->m_value.data) == (32.0/24.0));
			}
		}

		SUBCASE("STRING")
		{
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_string"));
			REQUIRE(var_a != nullptr);
			CHECK((std::get<std::string>(var_a->m_value.data).compare("hello, ") == 0));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_string"));
			REQUIRE(var_b != nullptr);
			CHECK((std::get<std::string>(var_b->m_value.data)).compare("world!") == 0);
			NodeLogic::process();

			SUBCASE("Testing Addition Logic")
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_add_string"));
				REQUIRE(node_var != nullptr);
				CHECK((std::get<std::string>(node_var->m_value.data)).compare("hello, world!") == 0);
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
			CHECK(std::get<int>(var->m_value.data) == val);
			NodeLogic::process();

			SUBCASE("To BOOL")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_bool"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (bool)val);
			}

			SUBCASE("To FLOAT")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<float>(var2->m_value.data) == (float)val);
			}

			SUBCASE("To DOUBLE")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<double>(var2->m_value.data) == (double)val);
			}

			SUBCASE("To STRING")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_string"));
				REQUIRE(var2 != nullptr);
				CHECK((std::get<std::string>(var2->m_value.data)).compare(std::to_string(val)) == 0);
			}
		}

		SUBCASE("From FLOAT")
		{
			const float val = 16.0f;
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_float"));
			REQUIRE(var != nullptr);
			CHECK(std::get<float>(var->m_value.data) == val);
			NodeLogic::process();

			SUBCASE("To INTEGER")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<int>(var2->m_value.data) == (int)val);
			}

			SUBCASE("To DOUBLE")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<double>(var2->m_value.data) == (double)val);
			}

			SUBCASE("To STRING")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_string"));
				REQUIRE(var2 != nullptr);
				CHECK((std::get<std::string>(var2->m_value.data)).compare(std::to_string(val)) == 0);
			}
		}

		SUBCASE("From DOUBLE")
		{
			const float val = 32.0;
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_double"));
			REQUIRE(var != nullptr);
			CHECK(std::get<double>(var->m_value.data) == val);
			NodeLogic::process();

			SUBCASE("To INTEGER")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<int>(var2->m_value.data) == (int)val);
			}

			SUBCASE("To FLOAT")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<float>(var2->m_value.data) == (float)val);
			}

			SUBCASE("To STRING")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("c_string"));
				REQUIRE(var2 != nullptr);
				CHECK((std::get<std::string>(var2->m_value.data)).compare(std::to_string(val)) == 0);
			}
		}

		SUBCASE("From STRING")
		{
			const std::string val = "48.64";
			NodeVariable* var = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_string"));
			REQUIRE(var != nullptr);
			CHECK((std::get<std::string>(var->m_value.data)).compare(val) == 0);
			NodeLogic::process();

			SUBCASE("To BOOL")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_bool"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == false);
			}

			SUBCASE("To INTEGER")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<int>(var2->m_value.data) == std::stoi(val));;
			}

			SUBCASE("To FLOAT")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<float>(var2->m_value.data) == std::stof(val));;
			}

			SUBCASE("To DOUBLE")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("d_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<double>(var2->m_value.data) == std::stod(val));;
			}
		}
	}

	SUBCASE("Testing Comparison Logic")
	{
		REQUIRE(Project::open("../test_cmp.cn") == RES_SUCCESS);
		NodeLogic::process();

		SUBCASE("BOOL")
		{
			const bool val = true;
			const bool val2 = false;
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_bool"));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_bool"));
			REQUIRE(var_a != nullptr);
			REQUIRE(var_b != nullptr);
			CHECK(std::get<bool>(var_a->m_value.data) == val);
			CHECK(std::get<bool>(var_b->m_value.data) == val2);
			NodeLogic::process();

			SUBCASE("==")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("eq_bool"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val == val2));
			}
			SUBCASE("!=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("neq_bool"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val != val2));
			}
			SUBCASE("||")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("or_bool"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val || val2));
			}
			SUBCASE("&&")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("and_bool"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val && val2));
			}
		}

		SUBCASE("INTEGER")
		{
			const int val = 32;
			const int val2 = 64;
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_int"));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_int"));
			REQUIRE(var_a != nullptr);
			REQUIRE(var_b != nullptr);
			CHECK(std::get<int>(var_a->m_value.data) == val);
			CHECK(std::get<int>(var_b->m_value.data) == val2);
			NodeLogic::process();

			SUBCASE("==")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("eq_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val == val2));
			}
			SUBCASE("!=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("neq_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val != val2));
			}
			SUBCASE("<")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("lt_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val < val2));
			}
			SUBCASE(">")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("gt_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val > val2));
			}
			SUBCASE("<=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("lte_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val <= val2));
			}
			SUBCASE(">=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("gte_int"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val >= val2));
			}
		}

		SUBCASE("FLOAT")
		{
			const float val = 32.323232f;
			const float val2 = 64.646464f;
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_float"));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_float"));
			REQUIRE(var_a != nullptr);
			REQUIRE(var_b != nullptr);
			CHECK(std::get<float>(var_a->m_value.data) == val);
			CHECK(std::get<float>(var_b->m_value.data) == val2);
			NodeLogic::process();

			SUBCASE("==")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("eq_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val == val2));
			}
			SUBCASE("!=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("neq_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val != val2));
			}
			SUBCASE("<")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("lt_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val < val2));
			}
			SUBCASE(">")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("gt_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val > val2));
			}
			SUBCASE("<=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("lte_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val <= val2));
			}
			SUBCASE(">=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("gte_float"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val >= val2));
			}
		}

		SUBCASE("DOUBLE")
		{
			const double val = 32.323232;
			const double val2 = 64.646464;
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_double"));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_double"));
			REQUIRE(var_a != nullptr);
			REQUIRE(var_b != nullptr);
			CHECK(std::get<double>(var_a->m_value.data) == val);
			CHECK(std::get<double>(var_b->m_value.data) == val2);
			NodeLogic::process();

			SUBCASE("==")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("eq_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val == val2));
			}
			SUBCASE("!=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("neq_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val != val2));
			}
			SUBCASE("<")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("lt_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val < val2));
			}
			SUBCASE(">")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("gt_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val > val2));
			}
			SUBCASE("<=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("lte_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val <= val2));
			}
			SUBCASE(">=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("gte_double"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val >= val2));
			}
		}

		SUBCASE("STRING")
		{
			const std::string val = "abc";
			const std::string val2 = "def";
			NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a_string"));
			NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b_string"));
			REQUIRE(var_a != nullptr);
			REQUIRE(var_b != nullptr);
			CHECK(std::get<std::string>(var_a->m_value.data) == val);
			CHECK(std::get<std::string>(var_b->m_value.data) == val2);
			NodeLogic::process();

			SUBCASE("==")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("eq_string"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val == val2));
			}
			SUBCASE("!=")
			{
				NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("neq_string"));
				REQUIRE(var2 != nullptr);
				CHECK(std::get<bool>(var2->m_value.data) == (val != val2));
			}
		}
	}

	SUBCASE("Testing Math Logic")
	{
		REQUIRE(Project::open("../test_math.cn") == RES_SUCCESS);
		const float val = 64.0f;
		const float val2 = 2.0f;
		NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a"));
		NodeVariable* var_b = dynamic_cast<NodeVariable*>(Nodes::find_by_name("b"));
		REQUIRE(var_a != nullptr);
		REQUIRE(var_b != nullptr);
		CHECK(std::get<float>(var_a->m_value.data) == val);
		CHECK(std::get<float>(var_b->m_value.data) == val2);
		NodeLogic::process();

		SUBCASE("ROOT")
		{
			NodeLogic::process();
			const float res = std::pow(val2, 1.0/val);
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("root"));
			REQUIRE(var2 != nullptr);
			CHECK(std::get<float>(var2->m_value.data) == res);
		}

		SUBCASE("POW")
		{
			NodeLogic::process();
			const float res = std::pow(val, val2);
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("pow"));
			REQUIRE(var2 != nullptr);
			CHECK(std::get<float>(var2->m_value.data) == res);
		}

		SUBCASE("SIN")
		{
			NodeLogic::process();
			const float res = std::sin(val);
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("sin"));
			REQUIRE(var2 != nullptr);
			CHECK(std::get<float>(var2->m_value.data) == res);
		}

		SUBCASE("COS")
		{
			NodeLogic::process();
			const float res = std::cos(val);
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("cos"));
			REQUIRE(var2 != nullptr);
			CHECK(std::get<float>(var2->m_value.data) == res);
		}

		SUBCASE("tan")
		{
			NodeLogic::process();
			const float res = std::tan(val);
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("tan"));
			REQUIRE(var2 != nullptr);
			CHECK(std::get<float>(var2->m_value.data) == res);
		}
	}

	SUBCASE("Testing String Logic")
	{
		REQUIRE(Project::open("../test_string.cn") == RES_SUCCESS);
		const std::string val = "Hello, World!";
		NodeVariable* var_a = dynamic_cast<NodeVariable*>(Nodes::find_by_name("a"));
		REQUIRE(var_a != nullptr);
		CHECK((std::get<std::string>(var_a->m_value.data)).compare(val) == 0);
		NodeLogic::process();

		SUBCASE("To Lowercase")
		{
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_tlc"));
			REQUIRE(var2 != nullptr);
			NodeLogic::process();
			std::string res = val;
			std::transform(res.begin(), res.end(), res.begin(),
				[](unsigned char c) { return std::tolower(c); });
			std::string res2 = std::get<std::string>(var2->m_value.data);
			CHECK(res.compare(res2) == 0);
		}

		SUBCASE("To Uppercase")
		{
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_tuc"));
			REQUIRE(var2 != nullptr);
			NodeLogic::process();
			std::string res = val;
			std::transform(res.begin(), res.end(), res.begin(),
				[](unsigned char c) { return std::toupper(c); });
			std::string res2 = std::get<std::string>(var2->m_value.data);
			CHECK(res.compare(res2) == 0);
		}

		SUBCASE("Reverse")
		{
			NodeVariable* var2 = dynamic_cast<NodeVariable*>(Nodes::find_by_name("res_reverse"));
			REQUIRE(var2 != nullptr);
			NodeLogic::process();
			std::string res = val;
			std::reverse(res.begin(), res.end());
			std::string res2 = std::get<std::string>(var2->m_value.data);
			CHECK(res.compare(res2) == 0);
		}

		SUBCASE("To Array")
		{
			NodeArray* var2 = dynamic_cast<NodeArray*>(Nodes::find_by_name("res_array"));
			REQUIRE(var2 != nullptr);
			NodeLogic::process();

			std::vector<NodeValue*> res;
			for (char ch : val)
			{
				NodeValue* node_val = new NodeValue();
				std::string str_ch(1, ch);
				node_val->set((std::string)str_ch);
				res.push_back(node_val);
			}
			REQUIRE(res.size() == var2->m_from_string.size());

			for (int i = 0; i < var2->m_from_string.size(); ++i)
			{
				NodeValue* val = var2->m_from_string[i];
				NodeValue* val2 = res[i];
				std::string a = val->get_value_str();
				std::string b = val2->get_value_str();
				CHECK(a.compare(b) == 0);
			}
		}
	}
}
