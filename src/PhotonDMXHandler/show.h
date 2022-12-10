#include <string>
#include <vector>
#include "fixture.h"
#include "cue.h"
#include "cuelist.h"
#include <json/json.hpp>
using json = nlohmann::json;

class Show
{
    static std::string fileName;
    static std::map<std::string, Fixture *> fixMap;
    static std::vector<Fixture *> fix_list;
    static std::map<std::string, Cuelist *> cuelists;
    static void loadCuelists(json cuelists);
    static void loadFixtures(json fixtures);
    static void loadLayouts(json layouts);
    static json serializeFixtures();
    static json serializeCuelists();
    static json serializeLayouts();
    static std::map<std::string, std::vector<std::map<std::string, std::pair<float, float>>>> layouts_map;
    static std::string showName;
    static float liveTime;

public:
    static void loadShowFromFile(std::string filename);
    static void saveShowToFile(std::string filename = "");
    static Cuelist *getCuelist(std::string id);
    static std::vector<Cuelist *> getCuelists();
    static void addCuelist(Cuelist *cuelist, std::string id);
    static Fixture *getFixtureById(std::string id);
    static std::vector<Fixture *> getFixtureList();
};