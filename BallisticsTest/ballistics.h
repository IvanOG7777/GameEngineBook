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
        uint32_t startTime = 0;
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

public:
    Ballistic(); // default Ballistic object constructor, no parameters needed
    AmmoRound ammoRound; // used for now only to define a single round. Not neccescary. I will use mainly for testing
    ShotType currentShotType; // set the current type of shot for when use use fire() it will fire that current shot within rounds
    std::vector<AmmoRound> rounds; // vector of AmmoRound Structs, can hold multiple types of shots PISTOL,ARTILERY...
    std::vector<BallisticNode> nodePool; // vector that will hold BallisticNodes
    int poolUsed;

    void initializeParticleForAmmoRound(ShotType currentType); // function used to initalize an AmmoRound struct, function is really only used for testing since its not cycling through all elements of rounds

    // function used to initalize rounds within rounds vector
    void fire();
    // function used to updated rounds within rounds vector, acceleration, velocity, postion...
    void updateRound(double& dt);
    // function used to check if slots within rounds vector are all UNUSED
    bool allRoundsUnused();

    void spawnRound(int key);

    void addNode(BallisticNode* node);

    void addNodesFromVectorToTree(std::vector<AmmoRound> &rounds);

    void printBydepth();

    float distance2(BallisticNode* node1, BallisticNode* node2);

    void findBestNodeHelper(BallisticNode* current, BallisticNode* target, BallisticNode*& bestNode, float& bestDistance, int depth);

    BallisticNode* findBestNode(BallisticNode* tagetNode);

    BallisticNode* getRoot();

    void treeReset();

    BallisticNode* allocateNode(AmmoRound* round);
};