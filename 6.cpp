#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

enum class CheckpointType {
    Mandatory,
    Optional
};

struct Coordinates {
    double latitude;
    double longitude;
};

struct Checkpoint {
    std::string name;
    Coordinates coord;
    CheckpointType type;
    double penalty; // только для optional
};

class Builder{
public:
    virtual ~Builder() = default;
    virtual void reset() = 0;
    virtual void addCheckpoint(const Checkpoint& cp, int index) = 0;
};

class TextBuilder: public Builder {
private:
    std::ostringstream oss;
public:
    void reset() override {
        oss.str("");
        oss.clear();
    };

    void addCheckpoint(const Checkpoint& cp, int index) override {
        oss << index << ". " << cp.name << "( " 
            << std::fixed << std::setprecision(4) << cp.coord.latitude
            << ", " << cp.coord.longitude << ") - ";

        if (cp.type == CheckpointType::Mandatory)
            oss << "незачет СУ";
        else
            oss << "штраф: " << cp.penalty << " часов";
        oss << "\n";
    }

    std::string getResult() const {
        return oss.str();
    }

};

class PenaltySumBuilder : public Builder {
private:
    double total_penalty = 0;
public:
    void reset() override {
        total_penalty = 0;
    }

    void addCheckpoint(const Checkpoint& cp, int index) override {
        if (cp.type == CheckpointType::Optional)
        {
            total_penalty += cp.penalty;
        }
    }

    double getResult() const
    {
        return total_penalty;
    }
};

class Director
{
public:
    void setBuilder(Builder* b){
        builder = b;
    }

    void process(const std::vector<Checkpoint>& checkpoints)
    {
        builder->reset();
        int index = 1;
        for (const auto& cp: checkpoints) {
            builder->addCheckpoint(cp, index);
            index++;
        }
    }

private:
    Builder* builder;
};

int main() 
{
    std::vector<Checkpoint> checkpoints = {
        {"КП1", {55.7558, 37.6176}, CheckpointType::Mandatory, 0.0},
        {"КП2", {56.2341, 38.1234}, CheckpointType::Optional, 1.5},
        {"КП3", {57.6789, 39.8765}, CheckpointType::Optional, 2.0},
        {"КП4", {58.0000, 40.0000}, CheckpointType::Mandatory, 0.0},
        {"КП5", {58.9876, 40.4567}, CheckpointType::Optional, 0.75},
        {"КП6", {59.1111, 41.1111}, CheckpointType::Mandatory, 0.0},
        {"КП7", {60.5432, 42.3210}, CheckpointType::Optional, 1.0},
        {"КП8", {61.2222, 43.3333}, CheckpointType::Optional, 2.5}
    };


    Director d;

    TextBuilder textBuilder;
    d.setBuilder(&textBuilder);
    d.process(checkpoints);
    std::cout << textBuilder.getResult() << "\n";

    PenaltySumBuilder psd;
    d.setBuilder(&psd);
    d.process(checkpoints);
    std::cout << "суммарный штраф: "<< psd.getResult() << "\n";
}