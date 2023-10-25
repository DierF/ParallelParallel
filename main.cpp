#include <iostream>

#include "Application/Application.h"

int main(int argc, char* argv[])
{
	PParallel::Application app;

	app.init();

	app.run();

	return EXIT_SUCCESS;
}
