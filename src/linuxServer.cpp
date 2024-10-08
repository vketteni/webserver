# include "../incl/Webserver.hpp"

int main()
{
	using namespace http;
	Webserver server = Webserver("0.0.0.0", 8080);
	return (0);
}

