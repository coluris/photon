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
    static float liveTime;

public:
    static void loadShowFromFile(std::string filename);
    static void saveShowToFile(std::string filename);
    static Cuelist *getCuelist(std::string id);
    static std::vector<Cuelist *> getCuelists();
    static void addCuelist(Cuelist *cuelist, std::string id);
    static Fixture *getFixtureById(std::string id);
    static std::vector<Fixture *> getFixtureList();
};