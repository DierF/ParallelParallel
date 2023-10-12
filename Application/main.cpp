#include <iostream>

#include "Application.h"

int
main()
{
	PParallel::Application app;

	try
	{
		app.run();
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Hi" << std::endl;

	return EXIT_SUCCESS;
}