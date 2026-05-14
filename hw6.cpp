#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class CPBuilder {
public:
    virtual ~CPBuilder() = default;
    virtual void addMandatory(const std::string& name, double lat, double lon) = 0;
    virtual void addOptional(const std::string& name, double lat, double lon, double penalty) = 0;
};

class TextReportBuilder : public CPBuilder {
private:
    int index = 1;
    std::ostringstream report;

public:
    void addMandatory(const std::string& name, double lat, double lon) override {
        report << index++ << "; " << name << "; " 
               << lat << ", " << lon << "; незачёт СУ\n";
    }

    void addOptional(const std::string& name, double lat, double lon, double penalty) override {
        report << index++ << "; " << name << "; " 
               << lat << ", " << lon << "; " << penalty << " ч\n";
    }

    std::string getResult() const {
        return report.str();
    }
};

class TotalPenaltyBuilder : public CPBuilder {
private:
    double totalPenalty = 0.0;

public:
    void addMandatory(const std::string&, double, double) override {}

    void addOptional(const std::string&, double, double, double penalty) override {
        totalPenalty += penalty;
    }

    double getResult() const {
        return totalPenalty;
    }
};

struct Checkpoint {
    std::string name;
    double lat;
    double lon;
    bool is_mandatory;
    double penalty;
};

class RouteDirector {
private:
    std::vector<Checkpoint> route;

public:
    void addMandatoryCP(const std::string& name, double lat, double lon) {
        route.push_back({name, lat, lon, true, 0.0});
    }

    void addOptionalCP(const std::string& name, double lat, double lon, double penalty) {
        route.push_back({name, lat, lon, false, penalty});
    }

    void process(CPBuilder& builder) const {
        for (const auto& cp : route) {
            if (cp.is_mandatory) {
                builder.addMandatory(cp.name, cp.lat, cp.lon);
            } else {
                builder.addOptional(cp.name, cp.lat, cp.lon, cp.penalty);
            }
        }
    }
};

int main() {
    RouteDirector director;
    
    director.addMandatoryCP("Start", 55.7558, 37.6173);
    director.addOptionalCP("Point A", 55.8000, 37.6000, 1.5);
    director.addOptionalCP("Point B", 55.9000, 37.7000, 2.0);
    director.addMandatoryCP("Finish", 56.0000, 37.8000);

    TextReportBuilder textBuilder;
    director.process(textBuilder);
    std::cout << textBuilder.getResult() << "\n";

    TotalPenaltyBuilder penaltyBuilder;
    director.process(penaltyBuilder);
    std::cout << "Total penalty: " << penaltyBuilder.getResult() << " h\n";

    return 0;
}