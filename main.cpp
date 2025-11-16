#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <fstream>
#include <limits>
#include <memory>
#include <set>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <thread>
#include <chrono>

using namespace std;

class AnimatedText {
public:
    static void printWithDelay(const string& text, int delayMs = 25) {
        for (char c : text) {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
    }

    static void printWelcomeMessage() {
        cout << "\033[1;32m"; // Green color
        cout << "******************************************************************" << endl;
        printWithDelay("||                 FARMER ADVISORY SYSTEM                  ||\n");
        cout << "||                  & Crop Recommendation               ||" << endl;
        printWithDelay("||                 Made BY:Syed Azan Mehdi                 ||\n");
        cout << "*******************************************************************" << endl;
        cout << "\033[0m"; // Reset color

        this_thread::sleep_for(chrono::milliseconds(500));

        this_thread::sleep_for(chrono::milliseconds(1000));
    }
};

class CropNode {
public:
    string cropName;
    string soilType;
    double minTemp;
    double maxTemp;
    int waterRequirement;
    int maturityDays;
    double yieldPotential;
    string season;
    double fertilizerRequirement;
    unique_ptr<CropNode> left;
    unique_ptr<CropNode> right;

    CropNode(string name, string soil, double minT, double maxT,
             int water, int days, double yield, string cropSeason = "All", double fertilizer = 0.0)
        : cropName(name), soilType(soil), minTemp(minT), maxTemp(maxT),
          waterRequirement(water), maturityDays(days), yieldPotential(yield),
          season(cropSeason), fertilizerRequirement(fertilizer),
          left(nullptr), right(nullptr) {}
};

class CropBST {
private:
    unique_ptr<CropNode> root;
    unordered_map<string, vector<CropNode*>> soilIndex;
    unordered_map<string, vector<CropNode*>> seasonIndex;

    void inOrderTraversal(CropNode* node, vector<CropNode*>& result) const {
        if (!node) return;
        inOrderTraversal(node->left.get(), result);
        result.push_back(node);
        inOrderTraversal(node->right.get(), result);
    }

    void insertHelper(unique_ptr<CropNode>& node, unique_ptr<CropNode> newNode) {
        if (!node) {
            node = move(newNode);
            soilIndex[node->soilType].push_back(node.get());
            seasonIndex[node->season].push_back(node.get());
            return;
        }

        if (newNode->cropName < node->cropName) {
            insertHelper(node->left, move(newNode));
        } else {
            insertHelper(node->right, move(newNode));
        }
    }

public:
    CropBST() : root(nullptr) {}

    void addCrop(string name, string soil, double minT, double maxT,
                int water, int days, double yield, string season = "All", double fertilizer = 0.0) {
        unique_ptr<CropNode> newNode = make_unique<CropNode>(name, soil, minT, maxT, water, days, yield, season, fertilizer);
        insertHelper(root, move(newNode));
    }

    CropNode* getCropInfo(const string& cropName) const {
        CropNode* current = root.get();

        while (current) {
            if (cropName == current->cropName) {
                return current;
            } else if (cropName < current->cropName) {
                current = current->left.get();
            } else {
                current = current->right.get();
            }
        }
        return nullptr;
    }

    vector<CropNode*> getCropsBySoil(const string& soilType) const {
        auto it = soilIndex.find(soilType);
        if (it != soilIndex.end()) {
            return it->second;
        }
        return {};
    }

    vector<CropNode*> getCropsBySeason(const string& cropSeason) const {
        auto it = seasonIndex.find(cropSeason);
        if (it != seasonIndex.end()) {
            return it->second;
        }
        return {};
    }

    bool cropExists(const string& cropName) const {
        return getCropInfo(cropName) != nullptr;
    }

    vector<CropNode*> getAllCrops() const {
        vector<CropNode*> result;
        inOrderTraversal(root.get(), result);
        return result;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    size_t getCropCount() const {
        return getAllCrops().size();
    }

    vector<string> getAllSoilTypes() const {
        vector<string> soilTypes;
        for (const auto& pair : soilIndex) {
            soilTypes.push_back(pair.first);
        }
        return soilTypes;
    }

    vector<string> getAllSeasons() const {
        vector<string> seasons;
        for (const auto& pair : seasonIndex) {
            seasons.push_back(pair.first);
        }
        return seasons;
    }
};

class RegionGraph {
private:
    struct RegionData {
        string name;
        double temperature;
        double rainfall;
        double humidity;
        string soilType;
        double elevation;
        vector<int> neighbors;
    };

    vector<RegionData> regions;
    unordered_map<string, int> regionIndex;
    vector<vector<double>> distanceMatrix;

public:
    RegionGraph() {
        regions.reserve(100);
        distanceMatrix.reserve(100);
    }

    void addRegion(string name, double temp, double rain, string soil, double humidity = 60.0, double elevation = 0.0) {
        int index = regions.size();
        regionIndex[name] = index;
        regions.push_back({name, temp, rain, humidity, soil, elevation, {}});

        for (auto& row : distanceMatrix) {
            row.resize(index + 1, -1.0);
        }
        distanceMatrix.push_back(vector<double>(index + 1, -1.0));
        distanceMatrix[index][index] = 0.0;
    }

    void addConnection(string region1, string region2, double distance) {
        if (regionIndex.find(region1) != regionIndex.end() &&
            regionIndex.find(region2) != regionIndex.end()) {
            int idx1 = regionIndex[region1];
            int idx2 = regionIndex[region2];

            regions[idx1].neighbors.push_back(idx2);
            regions[idx2].neighbors.push_back(idx1);

            distanceMatrix[idx1][idx2] = distance;
            distanceMatrix[idx2][idx1] = distance;
        }
    }

    vector<string> getNearbyRegions(string regionName, double maxDistance) const {
        vector<string> nearby;
        auto it = regionIndex.find(regionName);
        if (it == regionIndex.end()) return nearby;

        int startIdx = it->second;
        for (int neighborIdx : regions[startIdx].neighbors) {
            if (distanceMatrix[startIdx][neighborIdx] <= maxDistance && distanceMatrix[startIdx][neighborIdx] > 0) {
                nearby.push_back(regions[neighborIdx].name);
            }
        }
        return nearby;
    }

    RegionData* getRegionInfo(string regionName) {
        auto it = regionIndex.find(regionName);
        if (it != regionIndex.end()) {
            return &regions[it->second];
        }
        return nullptr;
    }

    const RegionData* getRegionInfo(string regionName) const {
        auto it = regionIndex.find(regionName);
        if (it != regionIndex.end()) {
            return &regions[it->second];
        }
        return nullptr;
    }

    vector<string> findSimilarRegions(string regionName, double tempTolerance, double rainTolerance) const {
        vector<string> similar;
        auto it = regionIndex.find(regionName);
        if (it == regionIndex.end()) return similar;

        const RegionData* target = &regions[it->second];
        for (const auto& region : regions) {
            if (region.name != regionName) {
                if (abs(region.temperature - target->temperature) <= tempTolerance &&
                    abs(region.rainfall - target->rainfall) <= rainTolerance) {
                    similar.push_back(region.name);
                }
            }
        }
        return similar;
    }

    bool regionExists(const string& regionName) const {
        return regionIndex.find(regionName) != regionIndex.end();
    }

    vector<string> getAllRegionNames() const {
        vector<string> names;
        names.reserve(regions.size());
        for (const auto& region : regions) {
            names.push_back(region.name);
        }
        return names;
    }

    vector<RegionData> getAllRegions() const {
        return regions;
    }

    const unordered_map<string, int>& getRegionIndex() const {
        return regionIndex;
    }

    size_t getRegionCount() const {
        return regions.size();
    }

    bool isEmpty() const {
        return regions.empty();
    }

    vector<string> getRegionsBySoilType(const string& soilType) const {
        vector<string> result;
        for (const auto& region : regions) {
            if (region.soilType == soilType) {
                result.push_back(region.name);
            }
        }
        return result;
    }
};

struct CropScore {
    string cropName;
    double score;
    string soilType;
    double tempSuitability;
    string season;
    double estimatedYield;

    CropScore(string name, double s, string soil, double temp, string cropSeason = "All", double yield = 0.0)
        : cropName(name), score(s), soilType(soil), tempSuitability(temp), season(cropSeason), estimatedYield(yield) {}

    bool operator<(const CropScore& other) const {
        return score > other.score;
    }
};

class StringUtils {
public:
    static vector<string> splitString(const string& str, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(str);

        while (getline(tokenStream, token, delimiter)) {
            string trimmed = trim(token);
            if (!trimmed.empty()) {
                tokens.push_back(trimmed);
            }
        }
        return tokens;
    }

    static string trim(const string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");

        if (start == string::npos) return "";
        return str.substr(start, end - start + 1);
    }

    static string toLower(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    static string toUpper(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    static bool containsIgnoreCase(const string& str, const string& substr) {
        string strLower = toLower(str);
        string substrLower = toLower(substr);
        return strLower.find(substrLower) != string::npos;
    }
};

class FileHandler {
private:
    static const string CROP_DATA_FILE;
    static const string REGION_DATA_FILE;
    static const string CONNECTION_DATA_FILE;
    static const string RECOMMENDATION_FILE;
    static const string SOIL_MAPPING_FILE;

public:
    static bool saveCropData(const CropBST& cropDB) {
        ofstream file(CROP_DATA_FILE);
        if (!file.is_open()) {
            return false;
        }

        auto crops = cropDB.getAllCrops();
        file << "CropName,SoilType,MinTemp,MaxTemp,WaterRequirement,MaturityDays,YieldPotential,Season,FertilizerRequirement\n";

        for (const auto& crop : crops) {
            file << crop->cropName << ","
                 << crop->soilType << ","
                 << crop->minTemp << ","
                 << crop->maxTemp << ","
                 << crop->waterRequirement << ","
                 << crop->maturityDays << ","
                 << crop->yieldPotential << ","
                 << crop->season << ","
                 << crop->fertilizerRequirement << "\n";
        }
        file.close();
        return true;
    }

    static bool loadCropData(CropBST& cropDB) {
        ifstream file(CROP_DATA_FILE);
        if (!file.is_open()) {
            return false;
        }

        string line;
        int loadedCount = 0;
        bool firstLine = true;

        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            vector<string> tokens = StringUtils::splitString(line, ',');
            if (tokens.size() >= 7) {
                try {
                    string season = tokens.size() > 7 ? tokens[7] : "All";
                    double fertilizer = tokens.size() > 8 ? stod(tokens[8]) : 0.0;

                    cropDB.addCrop(
                        tokens[0],
                        tokens[1],
                        stod(tokens[2]),
                        stod(tokens[3]),
                        stoi(tokens[4]),
                        stoi(tokens[5]),
                        stod(tokens[6]),
                        season,
                        fertilizer
                    );
                    loadedCount++;
                } catch (const exception& e) {
                    continue;
                }
            }
        }
        file.close();
        return true;
    }

    static bool saveRegionData(const RegionGraph& regionGraph) {
        ofstream file(REGION_DATA_FILE);
        if (!file.is_open()) {
            return false;
        }

        auto regions = regionGraph.getAllRegions();
        file << "RegionName,Temperature,Rainfall,SoilType,Humidity,Elevation\n";

        for (const auto& region : regions) {
            file << region.name << ","
                 << region.temperature << ","
                 << region.rainfall << ","
                 << region.soilType << ","
                 << region.humidity << ","
                 << region.elevation << "\n";
        }
        file.close();
        return true;
    }

    static bool loadRegionData(RegionGraph& regionGraph) {
        ifstream file(REGION_DATA_FILE);
        if (!file.is_open()) {
            return false;
        }

        string line;
        int loadedCount = 0;
        bool firstLine = true;

        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            vector<string> tokens = StringUtils::splitString(line, ',');
            if (tokens.size() >= 4) {
                try {
                    double humidity = tokens.size() > 4 ? stod(tokens[4]) : 60.0;
                    double elevation = tokens.size() > 5 ? stod(tokens[5]) : 0.0;

                    regionGraph.addRegion(
                        tokens[0],
                        stod(tokens[1]),
                        stod(tokens[2]),
                        tokens[3],
                        humidity,
                        elevation
                    );
                    loadedCount++;
                } catch (const exception& e) {
                    continue;
                }
            }
        }
        file.close();
        return true;
    }

    static bool saveSoilMapping(const unordered_map<string, vector<string>>& soilToCrops) {
        ofstream file(SOIL_MAPPING_FILE);
        if (!file.is_open()) {
            return false;
        }

        file << "SoilType,Crop1,Crop2,Crop3,...\n";

        for (const auto& entry : soilToCrops) {
            file << entry.first;
            for (const auto& crop : entry.second) {
                file << "," << crop;
            }
            file << "\n";
        }
        file.close();
        return true;
    }

    static bool loadSoilMapping(unordered_map<string, vector<string>>& soilToCrops) {
        ifstream file(SOIL_MAPPING_FILE);
        if (!file.is_open()) {
            return false;
        }

        string line;
        bool firstLine = true;

        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            vector<string> tokens = StringUtils::splitString(line, ',');
            if (tokens.size() >= 2) {
                string soilType = tokens[0];
                vector<string> crops(tokens.begin() + 1, tokens.end());
                soilToCrops[soilType] = crops;
            }
        }
        file.close();
        return true;
    }

    static void saveRecommendation(const vector<CropScore>& recommendations,
                                  const string& region, double temp, double rain,
                                  const string& farmerName = "Unknown",
                                  const string& season = "Current") {
        ofstream file(RECOMMENDATION_FILE, ios::app);
        if (!file.is_open()) {
            return;
        }

        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());

        file << "\n=== Farmer: " << farmerName << " ===\n";
        file << "Date: " << getCurrentDateTime() << "\n";
        file << "Region: " << region << " | Season: " << season << "\n";
        file << "Conditions - Temperature: " << temp << "C, Rainfall: " << rain << "mm\n";
        file << "Top Recommendations:\n";

        for (size_t i = 0; i < min(recommendations.size(), size_t(5)); i++) {
            file << i+1 << ". " << recommendations[i].cropName
                 << " | Score: " << fixed << setprecision(1) << (recommendations[i].score * 100)
                 << "% | Soil: " << recommendations[i].soilType
                 << " | Season: " << recommendations[i].season
                 << " | Est. Yield: " << recommendations[i].estimatedYield << "t/ha\n";
        }
        file << "================================\n";
        file.close();
    }

    static void viewRecommendationHistory() {
        ifstream file(RECOMMENDATION_FILE);
        if (!file.is_open()) {
            cout << "No recommendation history found." << endl;
            return;
        }

        cout << "\n=== RECOMMENDATION HISTORY ===\n";
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    static string getCurrentDateTime() {
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);

        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

const string FileHandler::CROP_DATA_FILE = "crops.dat";
const string FileHandler::REGION_DATA_FILE = "regions.dat";
const string FileHandler::CONNECTION_DATA_FILE = "connections.dat";
const string FileHandler::RECOMMENDATION_FILE = "recommendations.log";
const string FileHandler::SOIL_MAPPING_FILE = "soil_mapping.dat";

class InputValidator {
public:
    static bool validateTemperature(double temp) {
        return temp >= -50.0 && temp <= 60.0;
    }

    static bool validateRainfall(double rain) {
        return rain >= 0.0 && rain <= 5000.0;
    }

    static bool validateHumidity(double humidity) {
        return humidity >= 0.0 && humidity <= 100.0;
    }

    static bool validatePositiveNumber(double num) {
        return num >= 0.0;
    }

    static bool validatePositiveInteger(int num) {
        return num > 0;
    }

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static string getTrimmedInput() {
        string input;
        getline(cin, input);
        return StringUtils::trim(input);
    }

    static string getStringInput(const string& prompt) {
        cout << prompt;
        return getTrimmedInput();
    }

    static double getDoubleInput(const string& prompt, double minVal, double maxVal) {
        double value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail() || value < minVal || value > maxVal) {
                clearInput();
                cout << "Please enter a valid number between " << minVal << " and " << maxVal << ": ";
            } else {
                clearInput();
                return value;
            }
        }
    }

    static int getIntegerInput(const string& prompt, int minVal, int maxVal) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail() || value < minVal || value > maxVal) {
                clearInput();
                cout << "Please enter a valid number between " << minVal << " and " << maxVal << ": ";
            } else {
                clearInput();
                return value;
            }
        }
    }
};

class RecommendationEngine {
private:
    const CropBST& cropDatabase;
    const RegionGraph& regionNetwork;

public:
    RecommendationEngine(const CropBST& cropDB, const RegionGraph& regionNet)
        : cropDatabase(cropDB), regionNetwork(regionNet) {}

    double calculateCropScore(CropNode* crop, double currentTemp, double rainfall,
                             const string& soil, const string& season, double humidity = 60.0) const {
        double score = 0.0;

        double tempOptimal = (crop->minTemp + crop->maxTemp) / 2.0;
        double tempDeviation = abs(currentTemp - tempOptimal);
        double tempRange = crop->maxTemp - crop->minTemp;

        if (tempDeviation <= tempRange / 2.0) {
            score += (1.0 - (tempDeviation / (tempRange / 2.0))) * 0.4;
        }

        if (crop->soilType == soil) {
            score += 0.25;
        } else {
            vector<string> similarSoils = getSimilarSoils(soil);
            if (find(similarSoils.begin(), similarSoils.end(), crop->soilType) != similarSoils.end()) {
                score += 0.15;
            }
        }

        double waterRatio = min(rainfall / max(crop->waterRequirement, 1), 1.5);
        score += min(waterRatio, 1.0) * 0.2;

        if (crop->season == "All" || crop->season == season || season == "All") {
            score += 0.1;
        }

        score += (crop->yieldPotential / 100.0) * 0.05;

        return min(max(score, 0.0), 1.0);
    }

    vector<string> getSimilarSoils(const string& soilType) const {
        static unordered_map<string, vector<string>> soilGroups = {
            {"Sandy", {"Sandy Loam", "Loamy Sand"}},
            {"Sandy Loam", {"Sandy", "Loamy"}},
            {"Loamy", {"Sandy Loam", "Clay Loam"}},
            {"Clay Loam", {"Loamy", "Clay"}},
            {"Clay", {"Clay Loam", "Silty Clay"}}
        };

        auto it = soilGroups.find(soilType);
        if (it != soilGroups.end()) {
            return it->second;
        }
        return {};
    }

    vector<CropScore> generateRecommendations(const string& regionName,
                                             double currentTemp,
                                             double expectedRainfall,
                                             const string& season = "Current",
                                             double humidity = 60.0) const {
        vector<CropScore> recommendations;

        if (!regionNetwork.regionExists(regionName)) {
            return recommendations;
        }

        auto regionInfo = regionNetwork.getRegionInfo(regionName);
        auto allCrops = cropDatabase.getAllCrops();

        for (auto crop : allCrops) {
            double score = calculateCropScore(crop, currentTemp, expectedRainfall,
                                            regionInfo->soilType, season, humidity);
            double estimatedYield = crop->yieldPotential * score;

            if (score > 0.2) {
                recommendations.emplace_back(crop->cropName, score, crop->soilType,
                                           (crop->minTemp + crop->maxTemp) / 2.0,
                                           crop->season, estimatedYield);
            }
        }

        sort(recommendations.begin(), recommendations.end());
        return recommendations;
    }

    vector<CropScore> generateSeasonalRecommendations(const string& regionName, const string& season) const {
        auto regionInfo = regionNetwork.getRegionInfo(regionName);
        if (!regionInfo) {
            return {};
        }

        double seasonalTemp = regionInfo->temperature;
        double seasonalRainfall = regionInfo->rainfall;

        if (season == "Winter") {
            seasonalTemp -= 5.0;
            seasonalRainfall *= 0.7;
        } else if (season == "Summer") {
            seasonalTemp += 5.0;
            seasonalRainfall *= 1.3;
        } else if (season == "Monsoon") {
            seasonalRainfall *= 2.0;
        }

        return generateRecommendations(regionName, seasonalTemp, seasonalRainfall, season);
    }
};

class DataManager {
private:
    CropBST& cropDatabase;
    RegionGraph& regionNetwork;
    unordered_map<string, vector<string>>& soilToCrops;
    bool& dataModified;

public:
    DataManager(CropBST& cropDB, RegionGraph& regionNet,
                unordered_map<string, vector<string>>& soilMap, bool& modified)
        : cropDatabase(cropDB), regionNetwork(regionNet),
          soilToCrops(soilMap), dataModified(modified) {}

    void initializeDefaultData() {
        if (cropDatabase.isEmpty()) {
            vector<tuple<string, string, double, double, int, int, double, string, double>> crops = {
                {"Wheat", "Loamy", 10.0, 25.0, 500, 120, 3.5, "Winter", 120.0},
                {"Cotton", "Sandy Loam", 20.0, 35.0, 700, 180, 2.0, "Summer", 150.0},
                {"Sugarcane", "Clay Loam", 15.0, 32.0, 1500, 300, 70.0, "All", 300.0},
                {"Rice", "Clay", 20.0, 35.0, 1200, 140, 4.0, "Monsoon", 100.0},
                {"Maize", "Loamy", 15.0, 30.0, 600, 90, 5.0, "Summer", 120.0},
                {"Sunflower", "Sandy", 18.0, 32.0, 400, 100, 1.5, "All", 80.0},
                {"Gram", "Sandy Loam", 15.0, 28.0, 350, 110, 1.2, "Winter", 60.0},
                {"Millet", "Sandy", 20.0, 35.0, 300, 85, 1.0, "Monsoon", 50.0},
                {"Potato", "Loamy", 15.0, 25.0, 500, 90, 25.0, "Winter", 150.0},
                {"Tomato", "Loamy", 18.0, 30.0, 600, 85, 30.0, "All", 100.0}
            };

            for (const auto& crop : crops) {
                cropDatabase.addCrop(
                    get<0>(crop), get<1>(crop), get<2>(crop), get<3>(crop),
                    get<4>(crop), get<5>(crop), get<6>(crop), get<7>(crop), get<8>(crop)
                );
            }
            dataModified = true;
        }

        if (regionNetwork.isEmpty()) {
            vector<tuple<string, double, double, string, double, double>> regions = {
                {"Mianwali City", 28.5, 250.0, "Loamy", 65.0, 210.0},
                {"Isa Khel", 29.0, 200.0, "Sandy Loam", 60.0, 190.0},
                {"Kundian", 27.5, 280.0, "Clay Loam", 70.0, 200.0},
                {"Piplan", 28.0, 230.0, "Sandy", 55.0, 205.0},
                {"Kalabagh", 30.0, 180.0, "Sandy", 50.0, 220.0}
            };

            for (const auto& region : regions) {
                regionNetwork.addRegion(
                    get<0>(region), get<1>(region), get<2>(region),
                    get<3>(region), get<4>(region), get<5>(region)
                );
            }
            dataModified = true;
        }

        if (soilToCrops.empty()) {
            soilToCrops["Loamy"] = {"Wheat", "Maize", "Potato", "Tomato"};
            soilToCrops["Sandy Loam"] = {"Cotton", "Gram", "Sunflower"};
            soilToCrops["Clay Loam"] = {"Sugarcane", "Rice", "Wheat"};
            soilToCrops["Clay"] = {"Rice", "Sugarcane"};
            soilToCrops["Sandy"] = {"Millet", "Sunflower", "Gram"};
            dataModified = true;
        }
    }

    void saveAllData() {
        FileHandler::saveCropData(cropDatabase);
        FileHandler::saveRegionData(regionNetwork);
        FileHandler::saveSoilMapping(soilToCrops);
        dataModified = false;
    }

    void addNewCrop() {
        cout << "\n--- Add New Crop ---\n";

        string name = InputValidator::getStringInput("Crop name: ");
        if (cropDatabase.cropExists(name)) {
            cout << "Crop already exists in database.\n";
            return;
        }

        string soil = InputValidator::getStringInput("Soil type: ");
        double minTemp = InputValidator::getDoubleInput("Minimum temperature (C): ", -50.0, 60.0);
        double maxTemp = InputValidator::getDoubleInput("Maximum temperature (C): ", -50.0, 60.0);

        if (minTemp >= maxTemp) {
            cout << "Minimum temperature must be less than maximum temperature.\n";
            return;
        }

        int water = InputValidator::getIntegerInput("Water requirement (mm): ", 0, 10000);
        int days = InputValidator::getIntegerInput("Maturity days: ", 1, 365);
        double yield = InputValidator::getDoubleInput("Yield potential (tons/ha): ", 0.0, 1000.0);

        string season = InputValidator::getStringInput("Growing season (Winter/Summer/Monsoon/All): ");
        double fertilizer = InputValidator::getDoubleInput("Fertilizer requirement (kg/ha): ", 0.0, 1000.0);

        cropDatabase.addCrop(name, soil, minTemp, maxTemp, water, days, yield, season, fertilizer);
        dataModified = true;
        cout << "Crop added successfully!\n";
    }

    void addNewRegion() {
        cout << "\n--- Add New Region ---\n";

        string name = InputValidator::getStringInput("Region name: ");
        if (regionNetwork.regionExists(name)) {
            cout << "Region already exists in database.\n";
            return;
        }

        double temp = InputValidator::getDoubleInput("Average temperature (C): ", -50.0, 60.0);
        double rain = InputValidator::getDoubleInput("Average rainfall (mm): ", 0.0, 5000.0);
        string soil = InputValidator::getStringInput("Soil type: ");
        double humidity = InputValidator::getDoubleInput("Average humidity (%): ", 0.0, 100.0);
        double elevation = InputValidator::getDoubleInput("Elevation (meters): ", -100.0, 10000.0);

        regionNetwork.addRegion(name, temp, rain, soil, humidity, elevation);
        dataModified = true;
        cout << "Region added successfully!\n";
    }

    void searchCrops() {
        cout << "\n--- Search Crops ---\n";
        string query = InputValidator::getStringInput("Enter crop name or soil type to search: ");

        auto allCrops = cropDatabase.getAllCrops();
        vector<CropNode*> results;

        for (auto crop : allCrops) {
            if (StringUtils::containsIgnoreCase(crop->cropName, query) ||
                StringUtils::containsIgnoreCase(crop->soilType, query)) {
                results.push_back(crop);
            }
        }

        if (results.empty()) {
            cout << "No crops found matching your search.\n";
        } else {
            cout << "\nFound " << results.size() << " matching crops:\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->cropName
                     << " (Soil: " << results[i]->soilType
                     << ", Season: " << results[i]->season << ")\n";
            }
        }
    }
};

class DisplayManager {
public:
    static void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "                   MAIN MENU" << endl;
        cout << string(60, '=') << endl;
        cout << "1.  Get Crop Recommendations" << endl;
        cout << "2.  Seasonal Planning" << endl;
        cout << "3.  View Crop Details" << endl;
        cout << "4.  Browse All Crops" << endl;
        cout << "5.  Search Crops" << endl;
        cout << "6.  Soil-Based Suggestions" << endl;
        cout << "7.  Regional Analysis" << endl;
        cout << "8.  Add New Crop" << endl;
        cout << "9.  Add New Region" << endl;
        cout << "10. View All Regions" << endl;
        cout << "11. View History" << endl;
        cout << "12. System Info" << endl;
        cout << "13. Save & Exit" << endl;
        cout << string(60, '=') << endl;
        cout << "Enter your choice (1-13): ";
    }

    static void displayRecommendations(const vector<CropScore>& recommendations,
                                      const string& region, double temp, double rain,
                                      const string& farmerName, const string& season = "Current") {
        cout << "\n" << string(70, '=') << endl;
        cout << "CROP RECOMMENDATIONS FOR: " << region << endl;
        cout << "Farmer: " << farmerName << " | Season: " << season << endl;
        cout << string(70, '=') << endl;
        cout << "Current Conditions: " << temp << "C, " << rain << "mm rainfall\n\n";

        if (recommendations.empty()) {
            cout << "No suitable crops found for current conditions.\n";
            return;
        }

        cout << "TOP RECOMMENDATIONS:" << endl;
        cout << string(70, '-') << endl;
        cout << left << setw(4) << "Rank" << setw(15) << "Crop"
             << setw(12) << "Suitability" << setw(15) << "Soil Type"
             << setw(12) << "Season" << setw(15) << "Est. Yield" << endl;
        cout << string(70, '-') << endl;

        for (size_t i = 0; i < min(recommendations.size(), size_t(8)); i++) {
            string suitability;
            if (recommendations[i].score >= 0.8) suitability = "Excellent";
            else if (recommendations[i].score >= 0.6) suitability = "Very Good";
            else if (recommendations[i].score >= 0.4) suitability = "Good";
            else suitability = "Moderate";

            cout << left << setw(4) << i+1
                 << setw(15) << recommendations[i].cropName
                 << setw(12) << suitability
                 << setw(15) << recommendations[i].soilType
                 << setw(12) << recommendations[i].season
                 << setw(15) << fixed << setprecision(1) << recommendations[i].estimatedYield << " t/ha" << endl;
        }

        if (!recommendations.empty()) {
            cout << "\nRECOMMENDATION: " << recommendations[0].cropName
                 << " is the most suitable crop (" << (recommendations[0].score * 100) << "% match)" << endl;
        }
    }

    static void displayCropDetails(CropNode* crop) {
        if (!crop) {
            cout << "Crop not found in database.\n";
            return;
        }

        cout << "\n" << string(50, '=') << endl;
        cout << "CROP DETAILS: " << crop->cropName << endl;
        cout << string(50, '=') << endl;
        cout << "Soil Type:          " << crop->soilType << endl;
        cout << "Temperature Range:  " << crop->minTemp << "C - " << crop->maxTemp << "C" << endl;
        cout << "Water Requirement:  " << crop->waterRequirement << " mm/season" << endl;
        cout << "Maturity Period:    " << crop->maturityDays << " days" << endl;
        cout << "Yield Potential:    " << crop->yieldPotential << " tons/ha" << endl;
        cout << "Growing Season:     " << crop->season << endl;
        cout << "Fertilizer Need:    " << crop->fertilizerRequirement << " kg/ha" << endl;
    }

    static void displayAllCrops(const vector<CropNode*>& crops) {
        cout << "\n" << string(80, '=') << endl;
        cout << "                      ALL CROPS IN DATABASE" << endl;
        cout << string(80, '=') << endl;
        cout << left << setw(4) << "No." << setw(15) << "Crop" << setw(15) << "Soil Type"
             << setw(15) << "Temp Range" << setw(10) << "Water" << setw(8) << "Days"
             << setw(10) << "Yield" << setw(10) << "Season" << endl;
        cout << string(80, '-') << endl;

        for (size_t i = 0; i < crops.size(); i++) {
            string tempRange = to_string(crops[i]->minTemp) + "-" + to_string(crops[i]->maxTemp);
            cout << left << setw(4) << i+1
                 << setw(15) << crops[i]->cropName
                 << setw(15) << crops[i]->soilType
                 << setw(15) << tempRange
                 << setw(10) << crops[i]->waterRequirement
                 << setw(8) << crops[i]->maturityDays
                 << setw(10) << crops[i]->yieldPotential
                 << setw(10) << crops[i]->season << endl;
        }
    }

    static void displayAllRegions(const vector<string>& regionNames, const RegionGraph& regionNetwork) {
        cout << "\n" << string(70, '=') << endl;
        cout << "                   ALL REGIONS" << endl;
        cout << string(70, '=') << endl;
        cout << left << setw(20) << "Region" << setw(12) << "Temperature"
             << setw(10) << "Rainfall" << setw(12) << "Humidity" << setw(15) << "Soil Type" << endl;
        cout << string(70, '-') << endl;

        for (const auto& region : regionNames) {
            auto info = regionNetwork.getRegionInfo(region);
            cout << left << setw(20) << region
                 << setw(12) << info->temperature
                 << setw(10) << info->rainfall
                 << setw(12) << info->humidity
                 << setw(15) << info->soilType << endl;
        }
    }

    static void displaySystemInfo(const CropBST& cropDatabase,
                                 const RegionGraph& regionNetwork,
                                 const unordered_map<string, vector<string>>& soilToCrops) {
        auto crops = cropDatabase.getAllCrops();
        auto regions = regionNetwork.getAllRegionNames();

        cout << "\n" << string(50, '=') << endl;
        cout << "              SYSTEM INFORMATION" << endl;
        cout << string(50, '=') << endl;
        cout << "Crops in Database:      " << crops.size() << endl;
        cout << "Regions Registered:     " << regions.size() << endl;
        cout << "Soil Types Supported:   " << soilToCrops.size() << endl;
        cout << "System Status:          Operational" << endl;
        cout << string(50, '=') << endl;
    }
};

class FarmerAdvisorySystem {
private:
    CropBST cropDatabase;
    RegionGraph regionNetwork;
    unordered_map<string, vector<string>> soilToCrops;
    bool dataModified;

    DataManager dataManager;
    RecommendationEngine recommendationEngine;

    void getCropRecommendations() {
        cout << "\n--- Crop Recommendation Engine ---\n";

        string farmerName = InputValidator::getStringInput("Enter your name: ");
        string regionName = InputValidator::getStringInput("Enter your region: ");

        if (!regionNetwork.regionExists(regionName)) {
            cout << "Region not found. Available regions:\n";
            auto regions = regionNetwork.getAllRegionNames();
            for (const auto& region : regions) {
                cout << " - " << region << "\n";
            }
            return;
        }

        double currentTemp = InputValidator::getDoubleInput("Current temperature (C): ", -50.0, 60.0);
        double expectedRainfall = InputValidator::getDoubleInput("Expected rainfall (mm): ", 0.0, 5000.0);
        string season = InputValidator::getStringInput("Current season (Winter/Summer/Monsoon/Current): ");

        auto recommendations = recommendationEngine.generateRecommendations(
            regionName, currentTemp, expectedRainfall, season);

        DisplayManager::displayRecommendations(recommendations, regionName,
                                             currentTemp, expectedRainfall,
                                             farmerName, season);

        if (!recommendations.empty()) {
            FileHandler::saveRecommendation(recommendations, regionName,
                                          currentTemp, expectedRainfall,
                                          farmerName, season);
            dataModified = true;
        }
    }

    void seasonalPlanning() {
        cout << "\n--- Seasonal Crop Planning ---\n";

        string regionName = InputValidator::getStringInput("Enter region: ");
        if (!regionNetwork.regionExists(regionName)) {
            cout << "Region not found.\n";
            return;
        }

        string season = InputValidator::getStringInput("Planning season (Winter/Summer/Monsoon): ");

        auto recommendations = recommendationEngine.generateSeasonalRecommendations(regionName, season);

        auto regionInfo = regionNetwork.getRegionInfo(regionName);
        DisplayManager::displayRecommendations(recommendations, regionName,
                                             regionInfo->temperature,
                                             regionInfo->rainfall,
                                             "Seasonal Planning", season);
    }

    void viewCropDetails() {
        cout << "\n--- Crop Information ---\n";
        string cropName = InputValidator::getStringInput("Enter crop name: ");

        auto crop = cropDatabase.getCropInfo(cropName);
        DisplayManager::displayCropDetails(crop);
    }

    void browseAllCrops() {
        auto crops = cropDatabase.getAllCrops();
        DisplayManager::displayAllCrops(crops);
    }

    void searchCrops() {
        dataManager.searchCrops();
    }

    void soilSuggestions() {
        cout << "\n--- Soil-Based Crop Suggestions ---\n";
        string soilType = InputValidator::getStringInput("Enter soil type: ");

        auto it = soilToCrops.find(soilType);
        if (it != soilToCrops.end()) {
            cout << "\nRecommended crops for " << soilType << " soil:\n";
            for (size_t i = 0; i < it->second.size(); i++) {
                cout << i+1 << ". " << it->second[i] << "\n";
            }
        } else {
            cout << "No data available for " << soilType << " soil.\n";
        }
    }

    void regionalAnalysis() {
        cout << "\n--- Regional Climate Analysis ---\n";
        string regionName = InputValidator::getStringInput("Enter region name: ");

        if (!regionNetwork.regionExists(regionName)) {
            cout << "Region not found.\n";
            return;
        }

        auto regionInfo = regionNetwork.getRegionInfo(regionName);
        auto similarRegions = regionNetwork.findSimilarRegions(regionName, 2.5, 40.0);

        cout << "\nClimate Profile for " << regionName << ":\n";
        cout << "Temperature: " << regionInfo->temperature << "C\n";
        cout << "Rainfall: " << regionInfo->rainfall << "mm\n";
        cout << "Humidity: " << regionInfo->humidity << "%\n";
        cout << "Soil Type: " << regionInfo->soilType << "\n";

        if (!similarRegions.empty()) {
            cout << "\nRegions with similar climate:\n";
            for (const auto& region : similarRegions) {
                cout << " - " << region << "\n";
            }
        }
    }

    void viewAllRegions() {
        auto regions = regionNetwork.getAllRegionNames();
        DisplayManager::displayAllRegions(regions, regionNetwork);
    }

    void showSystemInfo() {
        DisplayManager::displaySystemInfo(cropDatabase, regionNetwork, soilToCrops);
    }

public:
    FarmerAdvisorySystem()
        : dataModified(false),
          dataManager(cropDatabase, regionNetwork, soilToCrops, dataModified),
          recommendationEngine(cropDatabase, regionNetwork) {

        FileHandler::loadCropData(cropDatabase);
        FileHandler::loadRegionData(regionNetwork);
        FileHandler::loadSoilMapping(soilToCrops);

        dataManager.initializeDefaultData();

        if (dataModified) {
            dataManager.saveAllData();
            dataModified = false;
        }
    }

    ~FarmerAdvisorySystem() {
        if (dataModified) {
            dataManager.saveAllData();
        }
    }

    void run() {
        AnimatedText::printWelcomeMessage();

        int choice;
        while (true) {
            DisplayManager::displayMenu();
            cin >> choice;

            if (cin.fail()) {
                InputValidator::clearInput();
                cout << "Invalid input. Please enter a number between 1-13.\n";
                continue;
            }

            InputValidator::clearInput();

            switch (choice) {
                case 1: getCropRecommendations(); break;
                case 2: seasonalPlanning(); break;
                case 3: viewCropDetails(); break;
                case 4: browseAllCrops(); break;
                case 5: searchCrops(); break;
                case 6: soilSuggestions(); break;
                case 7: regionalAnalysis(); break;
                case 8: dataManager.addNewCrop(); break;
                case 9: dataManager.addNewRegion(); break;
                case 10: viewAllRegions(); break;
                case 11: FileHandler::viewRecommendationHistory(); break;
                case 12: showSystemInfo(); break;
                case 13:
                    dataManager.saveAllData();
                    cout << "\nThank you for using Farmer Advisory System!\n";
                    return;
                default:
                    cout << "Please enter a valid option (1-13).\n";
            }
        }
    }
};

int main() {
    try {
        FarmerAdvisorySystem system;
        system.run();
    } catch (const exception& e) {
        cout << "System error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
