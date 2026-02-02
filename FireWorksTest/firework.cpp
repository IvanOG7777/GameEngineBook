//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <queue>
#include <limits>

#include "firework.h"

Firework::Firework() {
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
}

/*
* initialization of rules for the particles
*
* Parameters: NONE
* return type void
*
* Function only initalizes the rules for specific particles values
*/
// for now particles will expand into a coneish shape, later will implement
void Firework::initFireworkRules() {
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
    rules[2].payloads[0].set(Firework::SMALL, 2);

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

void Firework::spawnFirework(int key) {
    FireworkNode firework;

    // Spawn Extra large key F
    if (key == 70) {
        currentFireworkType = EXTRALARGE;
        fire(currentFireworkType, mousePositionX, mousePositionY);
    }
}

void Firework::fire(FireworkSizeType type, double &xPosition, double &yPosition) {
    size_t roundIndex = 0;
    for (; roundIndex < activeFireworks.size(); roundIndex++) {
        if (activeFireworks[roundIndex] == nullptr) break;
        if (activeFireworks[roundIndex]->type == UNUSED) break;
    }

    if (roundIndex >= activeFireworks.size()) {
        // error logs
        std::cout << "Rounds is full" << std::endl;
        std::cout << "Cant initalize any more fireworks" << std::endl;
        std::cout << "roundIndex: " << roundIndex << std::endl;
        return;
    }

    if (activeFireworks[roundIndex] == nullptr) {
        std::string name;

        switch (type) {
            case SMALL:
                name = "SMALL";
                break;
            case MEDIUM:
                name = "MEDIUM";
                break;
            case LARGE:
                name = "LARGE";
                break;
            case EXTRALARGE:
                name = "EXTRALARGE";
                break;
            default:
                name = "UNKNOWN";
                break;
        }
        activeFireworks[roundIndex] = std::make_shared<FireworkNode>();
        activeFireworks[roundIndex]->name = name;
    }

    switch (type) {
        case Firework::UNUSED:
            break;
        case SMALL:
            activeFireworks[roundIndex]->type = type;
            activeFireworks[roundIndex]->particle.setMass(2.0f);
            activeFireworks[roundIndex]->particle.setVelocity(0.0f, 35.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setDamping(0.99f);
            activeFireworks[roundIndex]->particle.setRadius(2.0f);
            activeFireworks[roundIndex]->particle.setPosition(static_cast<float>(xPosition),
                                                              static_cast<float>(yPosition), 0.0f);
            std::cout << "Fireworks at index " << roundIndex << " has been initalized to SMALL\n";
            break;
        case MEDIUM:
            activeFireworks[roundIndex]->type = type;
            activeFireworks[roundIndex]->particle.setMass(5.5f);
            activeFireworks[roundIndex]->particle.setVelocity(0.0f, 30.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setDamping(0.99f);
            activeFireworks[roundIndex]->particle.setRadius(10.0f);
            activeFireworks[roundIndex]->particle.setPosition(static_cast<float>(xPosition),
                                                              static_cast<float>(yPosition), 0.0f);
            std::cout << "Fireworks at index " << roundIndex << " has been initalized to MEDIUM\n";
            break;
        case LARGE:
            activeFireworks[roundIndex]->type = type;
            activeFireworks[roundIndex]->particle.setMass(11.0f);
            activeFireworks[roundIndex]->particle.setVelocity(0.0f, 30.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setDamping(0.99f);
            activeFireworks[roundIndex]->particle.setRadius(10.0f);
            activeFireworks[roundIndex]->particle.setPosition(static_cast<float>(xPosition),
                                                              static_cast<float>(yPosition), 0.0f);
            std::cout << "Fireworks at index " << roundIndex << " has been initalized to LARGE \n";
            break;
        case EXTRALARGE:
            activeFireworks[roundIndex]->type = type;
            activeFireworks[roundIndex]->particle.setMass(15.0f);
            activeFireworks[roundIndex]->particle.setVelocity(0.0f, 30.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setAcceleration(0.0f, 35.0f, 0.0f);
            activeFireworks[roundIndex]->particle.setDamping(0.99f);
            activeFireworks[roundIndex]->particle.setRadius(12.5f);
            activeFireworks[roundIndex]->particle.setPosition(static_cast<float>(xPosition),
                                                              static_cast<float>(yPosition), 0.0f);
            std::cout << "Fireworks at index " << roundIndex << " has been initalized to EXTRALARGE \n";
            break;
        default:
            break;
    }
}

// since each particle will have its own amount of payloads maybye pass in the node to initlaize its payload amount?
void Firework::FireworkRule::init(unsigned int payloadCount) {
    FireworkRule::payloadCount = payloadCount;
    payloads.clear();
    FireworkRule::payloads.resize(payloadCount);
}

// function used to update particles position/velocity/acceleration
void Firework::updateFireworks(double dt) {
    for (auto &node: activeFireworks) {
        if (node == nullptr) continue;
        if (node->type == Firework::UNUSED) continue;

        node->particle.update(dt);
        if (node->particle.getPosition().y <= 0.0f) {
            node->particle.clearAccumulator();
            node->particle.clearAllValues();
            node->type = UNUSED;
        }
    }
}

void Firework::allocateNode(std::string name, unsigned int type) {
    if (nodeCount >= activeFireworks.size()) {
        std::cout << "Nodes is full no space for new node. Returning" << std::endl;
        return;
    }

    auto newNode = std::make_shared<FireworkNode>();
    newNode->name = name;
    newNode->type = type;

    std::cout << "Adding: " << newNode->name << " to the vector of active nodes" << '\n';

    activeFireworks[nodeCount++] = newNode;
    activeNodeCount++;
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
                    std::cout << node->name << " has been added to tree" << std::endl;
                    return;
                }
                current = current->right;
            } else if (node->particle.getPosition().x <= current->particle.getPosition().x) {
                if (current->left == nullptr) {
                    current->left = node;
                    std::cout << node->name << " has been added to tree" << std::endl;
                    return;
                }
                current = current->left;
            }
        } else if (depth % 2 == 1) {
            if (node->particle.getPosition().y >= current->particle.getPosition().y) {
                if (current->right == nullptr) {
                    current->right = node;
                    std::cout << node->name << " has been added to tree" << std::endl;
                    return;
                }
                current = current->right;
            } else if (node->particle.getPosition().y <= current->particle.getPosition().y) {
                if (current->left == nullptr) {
                    current->left = node;
                    std::cout << node->name << " has been added to tree" << std::endl;
                    return;
                }
                current = current->left;
            }
        }
        depth++;
    }
}

void Firework::treeReset() {
    if (root != nullptr) {
        std::cout << "Address of root: " << root->name << " " << root.get() << '\n';
    }
    else {
        std::cout << "Root is already null" << '\n';
    }
    root = nullptr;
    std:: cout << "Set root back to null" << '\n';
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

    std::weak_ptr<FireworkNode> bestNode = root;
    std::weak_ptr<FireworkNode> current = root;
    std::weak_ptr<FireworkNode> targetPtr = target;

    findNearestNeighborHelper(current, targetPtr, bestNode, bestDistance, 0);

    return bestNode;
}

Firework::FireworkNode *Firework::getRoot() {
    return root.get();
}

void Firework::printByDepth() {
    if (root == nullptr) {
        std::cerr << "Tree is empty!" << std::endl;
        return;
    }

    std::weak_ptr<FireworkNode> current = root;
    int depth = 0;
    std::queue<std::weak_ptr<FireworkNode> > nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty()) {
        size_t length = nodeQueue.size();
        std::cout << "Depth: " << depth << std::endl;
        auto node = nodeQueue.front().lock();
        for (size_t i = 0; i < length; i++) {
            if (node->left != nullptr) {
                nodeQueue.push(node->left);
            }
            if (node->right != nullptr) {
                nodeQueue.push(node->right);
            }
            std::cout << " " << node->name << " ";
            nodeQueue.pop();
        }
        depth++;
        std::cout << '\n';
    }
}

void Firework::findNearestNeighborHelper(std::weak_ptr<FireworkNode> &current, std::weak_ptr<FireworkNode> &target,
                                         std::weak_ptr<FireworkNode> &bestNode, float &bestDistance, int depth) {
    auto sharedPtrCurrent = current.lock();
    auto sharedPtrTarget = target.lock();
    auto sharedPtrBestNode = bestNode.lock();

    if (sharedPtrCurrent == nullptr) return;

    int axis = depth % 2;
    float currentDistance = distance2(current, target);

    if (currentDistance > 0.0f && currentDistance < bestDistance) {
        bestDistance = currentDistance;
        bestNode = current;
    }

    float targetValueAxis = (axis % 2 == 0)
                                ? sharedPtrTarget->particle.getPosition().x
                                : sharedPtrTarget->particle.getPosition().y;
    float currentValueAxis = (axis % 2 == 0)
                                 ? sharedPtrCurrent->particle.getPosition().x
                                 : sharedPtrCurrent->particle.getPosition().y;

    std::weak_ptr<FireworkNode> nearChild = (targetValueAxis < currentValueAxis)
                                                ? sharedPtrCurrent->left
                                                : sharedPtrCurrent->right;
    std::weak_ptr<FireworkNode> farChild = (targetValueAxis < currentValueAxis)
                                               ? sharedPtrCurrent->right
                                               : sharedPtrCurrent->left;

    findNearestNeighborHelper(nearChild, target, bestNode, bestDistance, depth + 1);

    float difference = targetValueAxis - currentValueAxis;
    float differenceSquared = difference * difference;

    if (differenceSquared < bestDistance) {
        findNearestNeighborHelper(farChild, target, bestNode, bestDistance, depth + 1);
    }
}

float Firework::distance2(std::weak_ptr<FireworkNode> &node1, std::weak_ptr<FireworkNode> &node2) {
    auto nodePtr1 = node1.lock();
    auto nodePtr2 = node2.lock();

    if (nodePtr1 == nullptr || nodePtr2 == nullptr) {
        return 0.0f;
    }

    float xSum = nodePtr1->particle.getPosition().x - nodePtr2->particle.getPosition().x;
    float ySum = nodePtr1->particle.getPosition().y - nodePtr2->particle.getPosition().y;

    float xSquared = xSum * xSum;
    float ySquared = ySum * ySum;

    return xSquared + ySquared;
}
