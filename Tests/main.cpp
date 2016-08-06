#define CATCH_CONFIG_MAIN
#include "catch.hpp"

unsigned int myFun()
{
	return 69;
}

TEST_CASE( "myFun works", "[myFun]" ) {
	REQUIRE( myFun() == 69 );
}
