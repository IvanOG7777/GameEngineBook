#pragma once
#include "Particle.h"
#include <cstdint>
#include <vector>

class Ballistic {
public:
    enum ShotType {
        UNUSED = 0,
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
    };

    struct AmmoRound {
        float lifeTime = 0;
        Particle particle;
        ShotType type = UNUSED;
    };

    // Ballistic Node struct for theKDTree
    struct BallisticNode {
        AmmoRound *roundNode = nullptr; // make sure the round we want is a pointer to the actualy address of a AmmoRound
        // initialze left and right nodes to null
        BallisticNode* left = nullptr;
        BallisticNode* right = nullptr;

        // Constructor for a BallisticNode struct
        // Pass in a type AmmoRound round through a pointer to its address
        BallisticNode() : roundNode(nullptr), left(nullptr), right(nullptr) {}
        BallisticNode(AmmoRound *round) : roundNode(round) {} // initialze BallisticNodes roundNode to round
    };

    static constexpr unsigned MaxAmmo = 1000;

private:
    // Give Ballistic class a root node
    BallisticNode* root;
    float distance2(BallisticNode* node1, BallisticNode* node2);
    void findBestNodeHelper(BallisticNode* current, BallisticNode* target, BallisticNode*& bestNode, float& bestDistance, int depth);
    BallisticNode* allocateNode(AmmoRound* round);
    void addNode(BallisticNode* node);

public:
    Ballistic(); // default Ballistic object constructor, no parameters needed
    AmmoRound ammoRound; // used for now only to define a single round. Not neccescary. I will use mainly for testing
    ShotType currentShotType; // set the current type of shot for when use use fire() it will fire that current shot within rounds
    std::vector<AmmoRound> rounds; // vector of AmmoRound Structs, can hold multiple types of shots PISTOL,ARTILERY...
    std::vector<BallisticNode> nodePool; // vector that will hold BallisticNodes
    int poolUsed; // counter
    int mouseCount = 0;

    void initializeParticleForAmmoRound(ShotType currentType); // function used to initalize an AmmoRound struct, function is really only used for testing since its not cycling through all elements of rounds

    // function used to initalize rounds within rounds vector
    void fire(double &xPosition, double &yPosition);
    // function used to updated rounds within rounds vector, acceleration, velocity, postion...
    void updateRound(double& dt);
    // function used to check if slots within rounds vector are all UNUSED
    bool allRoundsUnused();

    void spawnRound(int key);
    void spawnRoundWithMouse(double xPosition, double yPosition);

    void addRoundsFromVectorToTree(std::vector<AmmoRound> &rounds);

    void printBydepth();

    BallisticNode* findBestNode(BallisticNode* tagetNode);

    BallisticNode* getRoot();
    void treeReset();

    /*static bool duplicateCheck(const  std::vector<std::pair<float, BallisticNode*>>& nodes, const BallisticNode* node);
    void findMultipleNNHelper(BallisticNode* node, BallisticNode* target, std::vector<std::pair<float, BallisticNode*>>& bestNodes, int maxBestNodes, int depth);
    std::vector<BallisticNode*> findMultipleNN(Ballistic::BallisticNode* target, int maxBestNodes);
    static bool compareNodeDistances(std::pair<float, BallisticNode*> nodeA, std::pair<float, BallisticNode*> nodeB);*/
};