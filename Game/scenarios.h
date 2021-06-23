#include <map>
#include <vector>
#include <string>

class scenarios {
	std::vector<std::string> objectList;
	std::vector<std::string> interactibleList;
	std::map<int, void (*)()> inputFunctions;
	void (*initFunction)();
	void (*updateFunction)();
};