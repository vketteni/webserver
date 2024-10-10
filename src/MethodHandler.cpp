#include "../incl/MethodHandler.hpp"


AbstractMethodHandler* getHandlerForMethod(const std::string& method)
{
    if (method == "GET")
	{
        return new GetRequestHandler();
    }
	else if (method == "POST")
	{
        return new PostRequestHandler();
    }
	else if (method == "DELETE")
	{
        return new DeleteRequestHandler();
    }
    
    return NULL; 
}