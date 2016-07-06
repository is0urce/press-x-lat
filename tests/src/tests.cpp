// tests.cpp : Defines the entry point for the console application.
//

#define CATCH_CONFIG_RUNNER // providing own main
#include <catch.hpp>

//int _tmain(int argc, _TCHAR* argv[])
int _tmain(int argc, char* const argv[])
{
	Catch::Session().run(argc, argv);
	return 0;
}

