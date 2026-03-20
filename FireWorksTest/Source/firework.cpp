//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <queue>
#include <limits>
#include <random>
#include <cassert>

#include "../Header/firework.h"

Firework::Firework() : mtSeed(time(nullptr)) {
    activeFireworks.reserve(maxFireworks);
    activeFireworks.resize(maxFireworks);

    currentFireworkType = UNUSED;
    /*std::string name = "Single firework node";
    singleFireWork = FireworkNode(name);*/
    root = nullptr;
    nodeCount = 0;
    activeNodeCount = 0;
    mousePositionX = 0.0;
    mousePositionY = 0.0;
    rebuildTree = false;
}

/*
* initialization of rules for the particles
*
* Parameters: NONE
* return type void
*
* Function only initializes the rules for specific particles values
*/
// for now particles will expand into a coneish shape, later will implement
void Firework::fireworkBurstRules() {
    rules.clear();
    rules.resize(5);

    rules[0].type = UNUSED;
    rules[0].damping = 1.0f;
    rules[0].init(0);

    rules[1].type = SMALL;
    rules[1].damping = 0.99f;
    rules[1].maxAge = 1.5f;
    rules[1].minAge = 0.5f;
    rules[1].maxVelocity = {115.0f, 40.0f, 0.0f};
    rules[1].minVelocity = {-115.0f, -40.0f, 0.0f};
    rules[1].init(0);

    rules[2].type = MEDIUM;
    rules[2].damping = 0.99f;
    rules[2].maxAge = 1.5f;
    rules[2].minAge = 0.5f;
    rules[2].maxVelocity = {80.0f, 50.0f, 0.0f};
    rules[2].minVelocity = {-80.0f, -10.0f, 0.0f};
    rules[2].init(1);
    rules[2].payloads[0].set(Firework::SMALL, 8);

    rules[3].type = LARGE;
    rules[3].damping = 0.99f;
    rules[3].maxAge = 1.5f;
    rules[3].minAge = 0.5f;
    rules[3].maxVelocity = {40.0f, 75.0f, 0.0f};
    rules[3].minVelocity = {40.0f, 20.0f, 0.0f};
    rules[3].init(1);
    rules[3].payloads[0].set(Firework::MEDIUM, 4);

    rules[4].type = EXTRALARGE;
    rules[4].damping = 0.99f;
    rules[4].maxAge = 1.5f;
    rules[4].minAge = 0.5f;
    rules[4].maxVelocity = {5.0f, 90.0f, 0.0f};
    rules[4].minVelocity = {-5.0f, 60.0f, 0.0f};
    rules[4].init(2);
    rules[4].payloads[0].set(Firework::LARGE, 2);
    rules[4].payloads[1].set(Firework::MEDIUM, 4);
}

void Firework::fountainBurst() {

}

void Firework::rocketBurstRules() {
    rules.clear();
    rules.resize(5);

    rules[0].type = UNUSED;
    rules[0].damping = 1.0f;
    rules[0].init(0);

    rules[1].type = SMALL;
    rules[1].damping = 0.80f;
    rules[1].maxAge = 1.0f;
    rules[1].minAge = 0.5f;
    rules[1].minVelocity = {-300.0f, -200.0f, 0.0f};
    rules[1].maxVelocity = {300.0f, -100.0f, 0.0f};
    rules[1].init(0);
    rules[1].spawnBudget = 0;

    rules[2].type = MEDIUM;
    rules[2].damping = 0.85f;
    rules[2].maxAge = 2.5f;
    rules[2].minAge = 0.5f;
    rules[2].minVelocity = {-200.0f, -100.0f, 0.0f};
    rules[2].maxVelocity = {200.0f, -50.0f, 0.0f};
    rules[2].init(1);
    rules[2].payloads[0].set(Firework::SMALL, 10);
    rules[2].spawnBudget = 0;

    rules[3].type = LARGE;
    rules[3].damping = 0.95f;
    rules[3].maxAge = 3.5f;
    rules[3].minAge = 1.5f;
    rules[3].minVelocity = {-100.0f, -120.0f, 0.0f};
    rules[3].maxVelocity = {100.0f, -75.0f, 0.0f};
    rules[3].init(1);
    rules[3].payloads[0].set(Firework::MEDIUM, 5);
    rules[3].spawnBudget = 0;

    rules[4].type = EXTRALARGE;
    rules[4].damping = 0.99f;
    rules[4].maxAge = 5.5f;
    rules[4].minAge = 4.5f;
    rules[4].minVelocity = {-5.0f, 90.0f, 0.0f};
    rules[4].maxVelocity = {5.0f, 120.0f, 0.0f};
    rules[4].init(2);
    rules[4].payloads[0].set(Firework::LARGE, 3);
    rules[4].spawnBudget = 0;
}

void Firework::sphereBurst() {

}

void Firework::commetBurst() {

}

void Firework::fireworkLoop(const float &dt) {
    for (auto &node : activeFireworks) {
        if (node == nullptr) continue;
        if (node->type == UNUSED) continue;

        node->age -= dt;

        if (node->age <= 0.0f) {
            for (size_t j = 0; j < rules[node->type].payloads.size(); j++) {
                for (size_t k = 0; k < rules[node->type].payloads[j].count; k++) {
                    unsigned int currentType = rules[node->type].payloads[j].type;
                    fire(static_cast<FireworkSizeType>(currentType), node->particle.getPosition().x, node->particle.getPosition().y);
                }
            }
            node->type = Firework::UNUSED;
            rebuildTree = true;
        }
    }
}

void Firework::rocketBurstLoop(const float &dt) {
    for (auto &node : activeFireworks) {
        if (node == nullptr) continue;
        if (node->type == UNUSED) continue;

        node->age -= dt;
        node->emissionAccumulator += dt;

        if (node->emissionAccumulator >= node->emissionInterval) {
            for (size_t j = 0; j < rules[node->type].payloads.size(); j++) {
                for (size_t k = 0; k < rules[node->type].payloads[j].count; k++) {
                    unsigned int currentType = rules[node->type].payloads[j].type;
                    fireRocket(static_cast<FireworkSizeType>(currentType), node->particle.getPosition().x, node->particle.getPosition().y);
                }
            }
            node->emissionAccumulator -= node->emissionInterval;
        }

        if (node->age <= 0.0f) {
            node->type = Firework::UNUSED;
            rebuildTree = true;
        }
    }
}

void Firework::spawnFirework(const int key) {
    FireworkNode firework;

    constexpr int maxSpawn = 10;
    constexpr int minSpawn = 3;

    std:: uniform_int_distribution<> particleDistribution(minSpawn, maxSpawn);
    int randParticles = particleDistribution(mtSeed);

    // Spawn Extra large key F
    if (key == 70) {
        currentFireworkType = EXTRALARGE;
        fireRocket(currentFireworkType, mousePositionX, mousePositionY);

        for (size_t i = 0; i < randParticles; i++) {
            fire(currentFireworkType, mousePositionX, mousePositionY);
        }
    }
}

void Firework::fire(FireworkSizeType type, const float &xPosition, const float &yPosition) {
    rebuildTree = true;
    size_t roundIndex = 0;
    for (; roundIndex < activeFireworks.size(); roundIndex++) {
        if (activeFireworks[roundIndex] == nullptr) break;
        if (activeFireworks[roundIndex]->type == UNUSED) break;
    }

    if (roundIndex >= activeFireworks.size()) {
        // error logs
        std::cout << "Rounds is full" << std::endl;
        std::cout << "Cant initialize any more fireworks" << std::endl;
        std::cout << "roundIndex: " << roundIndex << std::endl;
        return;
    }

    auto newNode = allocateNode(type);
    assert(newNode != nullptr);
    std:: cout << newNode->type << std:: endl;

    float minAge = rules[newNode->type].minAge;
    float maxAge = rules[newNode->type].maxAge;
    float damping = rules[newNode->type].damping;
    Vector3 minSpawnVelocity = rules[newNode->type].minVelocity;
    Vector3 maxSpawnVelocity = rules[newNode->type].maxVelocity;

    std::uniform_real_distribution<float> velocityXDistribution(minSpawnVelocity.x, maxSpawnVelocity.x);
    std::uniform_real_distribution<float> velocityYDistribution(minSpawnVelocity.y, maxSpawnVelocity.y);
    std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);

    float randAge = ageDistribution(mtSeed);
    float randXVelocity = velocityXDistribution(mtSeed);
    float randYVelocity = velocityYDistribution(mtSeed);

    switch (type) {
        case UNUSED:
            break;
        case SMALL:
            newNode->name = "SMALL";
            newNode->age = randAge;
            newNode->particle.setMass(2.0f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0.0f);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(2.0f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[4].payloads.size()) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;
        case MEDIUM:
            newNode->name = "MEDIUM";
            newNode->age = randAge;
            newNode->particle.setMass(5.5f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0.0f);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(5.0f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[4].payloads.size()) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;
        case LARGE:
            newNode->name = "LARGE";
            newNode->age = randAge;
            newNode->particle.setMass(11.0f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0.0f);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(10.0f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[4].payloads.size()) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;
        case EXTRALARGE:
            newNode->name = "EXTRALARGE";
            newNode->age = randAge;
            newNode->particle.setMass(15.0f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0.0f);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(12.5f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[4].payloads[0].count + rules[4].payloads[1].count) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;
        default:
            break;
    }
}

void Firework:: fireRocket(FireworkSizeType type, const float &xPosition, const float &yPosition) {
    rebuildTree = true;
    size_t roundIndex = 0;
    for (; roundIndex < activeFireworks.size(); roundIndex++) {
        if (activeFireworks[roundIndex] == nullptr) break;
        if (activeFireworks[roundIndex]->type == UNUSED) break;
    }

    if (roundIndex >= activeFireworks.size()) {
        // error logs
        std::cout << "Rounds is full" << std::endl;
        std::cout << "Cant initialize any more fireworks" << std::endl;
        std::cout << "roundIndex: " << roundIndex << std::endl;
        return;
    }

    auto newNode = allocateNode(type);
    assert(newNode != nullptr);
    std:: cout << newNode->type << std:: endl;

    // std::mt19937 ageGen(time(nullptr));
    // std::mt19937 velGen(time(nullptr));

    float minAge = rules[newNode->type].minAge;
    float maxAge = rules[newNode->type].maxAge;
    Vector3 minVel = rules[newNode->type].minVelocity;
    Vector3 maxVel = rules[newNode->type].maxVelocity;
    float damping = rules[newNode->type].damping;

    std::uniform_real_distribution<float> velocityXDistribution(minVel.x, maxVel.x);
    std::uniform_real_distribution<float> velocityYDistribution(minVel.y, maxVel.y);
    std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);

    float randAge = ageDistribution(mtSeed);
    float randXVelocity = velocityXDistribution(mtSeed);
    float randYVelocity = velocityYDistribution(mtSeed);

    switch (type) {
        case UNUSED:
            break;

        case SMALL:
            newNode->name = "SMALL";
            newNode->age = randAge;
            newNode->particle.setMass(2.0f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(2.0f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            activeFireworks[roundIndex] = newNode;
            break;

        case MEDIUM:
            newNode->name = "MEDIUM";
            newNode->age = randAge;
            newNode->particle.setMass(5.5f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(5.0f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[2].payloads[0].count) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;

        case LARGE:
            newNode->name = "LARGE";
            newNode->age = randAge;
            newNode->particle.setMass(11.0f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(10.0f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[3].payloads[0].count) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;

        case EXTRALARGE:
            newNode->name = "EXTRALARGE";
            newNode->age = randAge;
            newNode->particle.setMass(15.0f);
            newNode->particle.setVelocity(randXVelocity, randYVelocity, 0);
            newNode->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            newNode->particle.setDamping(damping);
            newNode->particle.setRadius(12.5f);
            newNode->particle.setPosition(xPosition, yPosition, 0.0f);
            newNode->emissionRate = static_cast<float>(rules[4].payloads[0].count) / randAge;
            newNode->emissionInterval = 1 / newNode->emissionRate;
            activeFireworks[roundIndex] = newNode;
            break;
        default:
            break;
    }
}

// since each particle will have its own amount of payloads maybye pass in the node to initlaize its payload amount?
void Firework::FireworkRule::init(unsigned int payloadCount) {
    FireworkRule::payloadCount = payloadCount;
    payloads.clear();
    payloads.resize(payloadCount);
}

// function used to update particles position/velocity/acceleration
void Firework::updateFireworks(const double dt) {
    for (auto &node: activeFireworks) {
        if (node == nullptr) continue;
        if (node->type == Firework::UNUSED) continue;

        node->particle.update(static_cast<float>(dt));
        if (node->particle.getPosition().y <= 0.0f) {
            node->particle.clearAccumulator();
            node->particle.clearAllValues();
            node->type = UNUSED;
        }
    }
}

std::shared_ptr<Firework::FireworkNode> Firework::allocateNode(unsigned int type) {
    auto newNode = std::make_shared<FireworkNode>();
    newNode->type = type;

    std::cout << "Allocated: " << newNode->type << '\n';

    return newNode;
}

void Firework::addNode(std::shared_ptr<FireworkNode> &node) {
    if (node == nullptr) {
        std::cout << "Node is null invalid returning from addNode function" << '\n';
        return;
    }

    node->left = nullptr;
    node->right = nullptr;

    if (root == nullptr) {
        root = node;
        std::cout << "Root was null " << node->name << " is now root" << '\n';
        return;
    }

    int depth = 0;
    std::shared_ptr<FireworkNode> current = root;

    while (true) {
        if (depth % 2 == 0) {
            if (node->particle.getPosition().x >= current->particle.getPosition().x) {
                if (current->right == nullptr) {
                    current->right = node;
                    return;
                }
                current = current->right;
            } else if (node->particle.getPosition().x <= current->particle.getPosition().x) {
                if (current->left == nullptr) {
                    current->left = node;
                    return;
                }
                current = current->left;
            }
        } else if (depth % 2 == 1) {
            if (node->particle.getPosition().y >= current->particle.getPosition().y) {
                if (current->right == nullptr) {
                    current->right = node;
                    return;
                }
                current = current->right;
            } else if (node->particle.getPosition().y <= current->particle.getPosition().y) {
                if (current->left == nullptr) {
                    current->left = node;
                    return;
                }
                current = current->left;
            }
        }
        depth++;
    }
}

void Firework::treeReset() {
    for (auto &node : activeFireworks) {
        if (node == nullptr) continue;
        node->left = nullptr;
        node->right = nullptr;
    }
    root = nullptr;
    std:: cout << "Root is null" << '\n';
}

void Firework::addFireworksFromVectorToTree() {
    if (activeFireworks.empty()) return;

    for (auto &node: activeFireworks) {
        if (node == nullptr) continue;
        if (node->type == UNUSED) continue;

        addNode(node);
    }
}


std::weak_ptr<Firework::FireworkNode> Firework::findBestNode(std::shared_ptr<FireworkNode> &target) {
    if (root == nullptr) {
        std::cout << "No nodes returning target" << std::endl;
        return target;
    }

    float bestDistance = std::numeric_limits<float>::infinity();

    std::shared_ptr<FireworkNode> bestNode = root;
    std::shared_ptr<FireworkNode> current = root;
    std::shared_ptr<FireworkNode> targetPtr = target;

    findNearestNeighborHelper(current, targetPtr, bestNode, bestDistance, 0);

    return bestNode;
}

Firework::FireworkNode *Firework::getRoot() const {
    return root.get();
}

void Firework::findNearestNeighborHelper(std::shared_ptr<FireworkNode> &current, std::shared_ptr<FireworkNode> &target,
                                         std::shared_ptr<FireworkNode> &bestNode, float &bestDistance, int depth) {

    if (target == nullptr) return;;
    if (current == nullptr) return;

    int axis = depth % 2;
    float currentDistance = distanceSquared(current, target);

    if (currentDistance > 0.0f && currentDistance < bestDistance) {
        bestDistance = currentDistance;
        bestNode = current;
    }

    float targetValueAxis = (axis % 2 == 0)
                                ? target->particle.getPosition().x
                                : target->particle.getPosition().y;
    float currentValueAxis = (axis % 2 == 0)
                                 ? current->particle.getPosition().x
                                 : current->particle.getPosition().y;

    std::shared_ptr<FireworkNode> nearChild = (targetValueAxis < currentValueAxis)
                                                ? current->left
                                                : current->right;
    std::shared_ptr<FireworkNode> farChild = (targetValueAxis < currentValueAxis)
                                               ? current->right
                                               : current->left;

    findNearestNeighborHelper(nearChild, target, bestNode, bestDistance, depth + 1);

    float difference = targetValueAxis - currentValueAxis;
    float differenceSquared = difference * difference;

    if (differenceSquared < bestDistance) {
        findNearestNeighborHelper(farChild, target, bestNode, bestDistance, depth + 1);
    }
}

float Firework::distanceSquared(std::shared_ptr<FireworkNode> &node1, std::shared_ptr<FireworkNode> &node2) {

    if (node1 == nullptr || node2 == nullptr) {
        return 0.0f;
    }

    float distanceX = node1->particle.getPosition().x - node2->particle.getPosition().x;
    float distanceY = node1->particle.getPosition().y - node2->particle.getPosition().y;

    float distanceXSquared = distanceX * distanceX;
    float distanceYSquared = distanceY * distanceY;

    return distanceXSquared + distanceYSquared;
}