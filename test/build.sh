test_name=CodeNectTest
dir_test=build_test
dir_test_install=bin_test

function rebuild()
{
	cmake -G Ninja -B${dir_test} -H. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PWD/${dir_test_install}
}

function compile()
{
	cmake --build ${dir_test} --target install
}

function run()
{
	cd ${dir_test_install} && ./${test_name} "$@"
}

function test_detailed()
{
	compile && cd ${dir_test_install} && ./${test_name} --success -o=../report --duration=true
	cat ../report
}

function test_summary()
{
	compile && cd ${dir_test_install} && ./${test_name} -o=../summary --duration=true
	cat ../summary
}

if [ $# -eq 0 ]; then
	echo "Must pass command: rebuild, compile, run, test"
else
	"$@"
fi
