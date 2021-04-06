#include "App.hpp"
#include <cstdlib>
#include <ctime>

int main(int argc, char * argv[]) {

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	
	CApplication::setApplicationName("Circles");
	CApplication::setOrganizationName("Frachop");
	CApplication::setOrganizationDomain("frachop.in");
	CApplication::setApplicationVersion("0.1");
	CApplication::setQuitOnLastWindowClosed(true);
	CApplication app(argc, argv);
	return app.exec();
}
