#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <queue>
#include <limits>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ballistics.h"
#include "globalConstants.h"

// default ballistic constructor, when a ballistic object is created
// rounds is resized to hold 16 AmmoRound structs
// set initial current shot to unused.
Ballistic::Ballistic() {
    rounds.resize(MaxAmmo); // resize the vector to the MaxAmmo size. This preallocates AmmoRound structs within the vector
    nodePool.resize(MaxAmmo); // resize the vector to the MaxAmmo size. This preallocates BallisticNode structs within the vector
    currentShotType = UNUSED;
    ammoRound = AmmoRound(); // NOT really needed, only for testing, but this constructs a single ammoRound of type AmmoRound and sets its value to default AmmoRound constructor
    root = nullptr;
    poolUsed = 0;

    //mouse function variables
    isMouseDown = false;
    mousePositionX = 0.0;
    mousePositionY = 0.0;

    holdTime = 0.0;
    spawnCooldown = 0.0;
}

// function to initlaize a shot type with appropriate particle parameters
void Ballistic::initializeParticleForAmmoRound(ShotType currentType) { // pass in the type of shot with ShotType currentType
    // switch used to select how to initlaize a shot
    switch (currentType) {
    case (Ballistic::ShotType::UNUSED):
        std::cout << "Unsed breaking out of initlaizeation" << std::endl;
        break;
    case(Ballistic::ShotType::PISTOL):
        ammoRound.particle.setMass(2.0f);
        ammoRound.particle.setVelocity(0.0f, 0.0f, 35.0f);
        ammoRound.particle.setAcceleration(0.0f, -1.0f, 0.0f);
        ammoRound.particle.setDamping(0.99f);
        ammoRound.particle.setRadius(2.0f);
        break;
    case(Ballistic::ShotType::ARTILLERY):
        ammoRound.particle.setMass(200.0f);
        ammoRound.particle.setVelocity(0.0f, 30.0f, 40.0f);
        ammoRound.particle.setAcceleration(0.0f, -20.0f, 0.0f);
        ammoRound.particle.setDamping(0.99f);
        ammoRound.particle.setRadius(10.0f);
        break;
    case (Ballistic::ShotType::FIREBALL):
        ammoRound.particle.setMass(1.0f);
        ammoRound.particle.setVelocity(0.0f, 0.0f, 10.0f);
        ammoRound.particle.setAcceleration(0.0f, 0.6f, 0.0f);
        ammoRound.particle.setDamping(0.99f);
        ammoRound.particle.setRadius(5.0f);
        break;
    case (Ballistic::ShotType::LASER):
        ammoRound.particle.setMass(0.1f);
        ammoRound.particle.setVelocity(0.0f, 0.0f, 100.0f);
        ammoRound.particle.setAcceleration(0, 0, 0);
        ammoRound.particle.setDamping(0.999f);
        ammoRound.particle.setRadius(.1f);
        break;
    }

    ammoRound.particle.setPosition(SCREENHEIGHT / 2.0f, SCREENWIDTH / 2.0f, 0.0f);
    ammoRound.type = currentType;
}

// function used to initlize a slot within rounds with round types particle parameters
void Ballistic::fire(double &xPosition, double &yPosition) {
    // initial for loop to loop through the rounds vector
    int roundIndex = 0;
    for (; roundIndex < rounds.size(); roundIndex++) {
        if (rounds[roundIndex].type == UNUSED) break; // if the current index.type is unused we have found a valid empty slot to initalize
    }

    // boundry checker to see if curent index is out of bounds of MaxAmmo 
    // if it is we return out of function call before intializeing another round at invalid index
    if (roundIndex >= rounds.size()) {
        // error logs
        std::cout << "Rounds is full" << std::endl;
        std::cout << "Cant initalize any more rounds" << std::endl;
        std::cout << "roundIndex: " << roundIndex << std::endl;
        return;
    }

    // if we find a vaild UNUSED spot initilize that spot with current shotType partucle parametes
    switch (currentShotType) {
    case PISTOL:
        rounds[roundIndex].particle.setMass(2.0f);
        rounds[roundIndex].particle.setVelocity(-10.0f, 35.0f, 0.0f);
        rounds[roundIndex].particle.setAcceleration(0.0f, -1.0f, 0.0f);
        rounds[roundIndex].particle.setDamping(0.99f);
        rounds[roundIndex].particle.setRadius(2.0f);
        rounds[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
        rounds[roundIndex].lifeTime = 8.0f;
        std::cout << "Round at index: " << roundIndex << " has been initlaized to PISTOL" << std::endl;
        break;
    case ARTILLERY:
        rounds[roundIndex].particle.setMass(200.0f);
        rounds[roundIndex].particle.setVelocity(-8.0f, 30.0f, 0.0f);
        rounds[roundIndex].particle.setAcceleration(0.0f, -20.0f, 0.0f);
        rounds[roundIndex].particle.setDamping(0.99f);
        rounds[roundIndex].particle.setRadius(10.0f);
        rounds[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
        rounds[roundIndex].lifeTime = 10.0f;
        std::cout << "Round at index: " << roundIndex << " has been initlaized ARTILLERY" << std::endl;
        break;
    case FIREBALL:
        rounds[roundIndex].particle.setMass(1.0f);
        rounds[roundIndex].particle.setVelocity(-10.0f, 10.0f, 0.0f);
        rounds[roundIndex].particle.setAcceleration(0.0f, -0.6f, 0.0f);
        rounds[roundIndex].particle.setDamping(0.99f);
        rounds[roundIndex].particle.setRadius(5.0f);
        rounds[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
        rounds[roundIndex].lifeTime = 5.0f;
        std::cout << "Round at index: " << roundIndex << " has been initlaized FIREBALL" << std::endl;
        break;
    case LASER:
        rounds[roundIndex].particle.setMass(0.1f);
        rounds[roundIndex].particle.setVelocity(0.0f, 0.0f, 100.0f);
        rounds[roundIndex].particle.setAcceleration(0, 0, 0);
        rounds[roundIndex].particle.setDamping(0.999f);
        rounds[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
        rounds[roundIndex].particle.setRadius(1.1f);
        rounds[roundIndex].lifeTime = 10.0f;
        std::cout << "Round at index: " << roundIndex << " has been initlaized LASER" << std::endl;
        break;
    }

    // set the particles default position and reassign its type to currentType
    rounds[roundIndex].type = currentShotType;
}

void Ballistic::updateRound(double &dt) {

    for (int i = 0; i < rounds.size(); i++) {
        if (rounds[i].type != UNUSED) {

            /*rounds[i].lifeTime -= static_cast<float>(dt);
            if (rounds[i].lifeTime <= 0) {
                rounds[i].particle.clearAccumulator();
                rounds[i].particle.clearAllValues();
                rounds[i].type = UNUSED;
            }*/

            rounds[i].particle.update(dt);
            if (rounds[i].particle.getPosition().y <= 0.0f) {
                std::cout << "Shot at index: " << i << " has hit the ground" << std::endl;
                std::cout << "Setting shot at index: " << i << " to UNUSED" << std::endl;
                rounds[i].particle.clearAccumulator();
                rounds[i].particle.clearAllValues();
                rounds[i].type = UNUSED;
            }
        }
    }
}

// function used to check if all rounds within rounds vector are unused
bool Ballistic::allRoundsUnused() {
    for (int i = 0; i < rounds.size(); i++) {
        if (rounds[i].type != UNUSED) {
            return false;
        }
    }
    return true;
}

void Ballistic::spawnRound(int key) {
    Ballistic::AmmoRound round;
    if (key == 80) { // case for PISTOL
        round.particle.setMass(2.0f);
        round.particle.setVelocity(-10.0f, 35.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -1.0f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(2.0f);
        round.lifeTime = 8;
        currentShotType = PISTOL;
        round.type = currentShotType;
        fire(mousePositionX, mousePositionY);

        std::cout << "PISTOL has been added" << std::endl;
    }

    if (key == 65) { // case for ARTILLERY
        round.particle.setMass(200.0f);
        round.particle.setVelocity(-8.0f, 30.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -20.0f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(10.0f);
        round.lifeTime = 10;
        currentShotType = ARTILLERY;
        round.type = currentShotType;
        fire(mousePositionX, mousePositionY);

        std::cout << "ARTILLERY has been added" << std::endl;
    }

    if (key == 70) { // case for FIREBALL
        round.particle.setMass(1.0f);
        round.particle.setVelocity(-10.0f, 10.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -0.6f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(5.0f);
        round.lifeTime = 5;
        currentShotType = FIREBALL;
        fire(mousePositionX, mousePositionY);

        std::cout << "FIREBALL has been added" << std::endl;
    }
}

void Ballistic:: spawnRoundWithMouse(double xPosition, double yPosition) {
    Ballistic::AmmoRound round;
    int particleType = mouseCount % 3;
    mouseCount++;
    if (particleType == 0) {
        round.particle.setMass(2.0f);
        round.particle.setVelocity(-10.0f, 35.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -1.0f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(2.0f);
        round.lifeTime = 8;
        currentShotType = PISTOL;
        fire(xPosition, yPosition);
        std::cout << "PISTOL has been added" << std::endl;
    }

    if (particleType == 1) {
        round.particle.setMass(200.0f);
        round.particle.setVelocity(-8.0f, 30.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -20.0f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(10.0f);
        round.lifeTime = 10;
        currentShotType = Ballistic::ARTILLERY;
        fire(xPosition, yPosition);

        std::cout << "ARTILLERY has been added" << std::endl;
    }

    if (particleType == 2) {
        round.particle.setMass(1.0f);
        round.particle.setVelocity(-10.0f, 10.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -0.6f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(5.0f);
        round.lifeTime = 5;
        currentShotType = FIREBALL;
        fire(xPosition, yPosition);

        std::cout << "FIREBALL has been added" << std::endl;
    }
}

// function used to add a node to the tree
/* Parameters:
*       BallisticNode struct pointer
* Returns void (nothing)
*/
void Ballistic::addNode(BallisticNode* node) {

    // Sanity check, makes passed node a leaf node
    node->left = nullptr;
    node->right = nullptr;

    // if root is nullptr make passed node root of tree
    if (root == nullptr) {
        root = node;
        return;
    }

    // If above is false make initialize current node pointer to root
    // Initialize depth to 0 to calculate the axis to check 
    BallisticNode* current = root;
    int depth = 0;

    // Essentially a normal tree addition of nodes only we are switching compareision axis values on different depths
    // Even depths we check the x-axis and odd depths we check the y-axis
    // While we dont hit a return statment
    while (true) {
        // x axis check
        if (depth % 2 == 0) {
            if (node->roundNode->particle.getPosition().x >= current->roundNode->particle.getPosition().x) {
                if (current->right == nullptr) {
                    current->right = node;
                    return;
                }
                current = current->right;
            }
            else if (node->roundNode->particle.getPosition().x <= current->roundNode->particle.getPosition().x) {
                if (current->left == nullptr) {
                    current->left = node;
                    return;
                }
                current = current->left;
            }
        }
        // y axis check
        else if (depth % 2 == 1) {
            if (node->roundNode->particle.getPosition().y >= current->roundNode->particle.getPosition().y) {
                if (current->right == nullptr) {
                    current->right = node;
                    return;
                }
                current = current->right;
            }
            else if (node->roundNode->particle.getPosition().y <= current->roundNode->particle.getPosition().y) {
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

// Function used to add a round from a vector to the tree itself
/* Parameters:
*       Vector of AmmoRound structs. We are referencing the values from the vector
*       Returns nothing
*/
void Ballistic::addRoundsFromVectorToTree(std::vector<AmmoRound> &rounds) {
    // for each round in rounds
    for (auto &round : rounds) {
        if (round.type == UNUSED) continue; // if round type is UNUSED(invalid particle) continue to next round
        BallisticNode* newNode = allocateNode(&round); // newNode points to a node within the the nodePool that is now initliazed to have the current rounds value
        if (!newNode) break; // if newNode is nullptr break

        addNode(newNode); // add this node to the tree
    }
}

// Function used to allocate a node using a round struct
/* Parameters:
*       Pointer to an AmmoRound struct
*       Returns a BallisticNode
*/
Ballistic::BallisticNode* Ballistic::allocateNode(AmmoRound* round) {
    if (poolUsed >= nodePool.size()) return nullptr; // if the poolUsed counter is greater than or equal to the nodePoolsize return a nullptr

    BallisticNode* node = &nodePool[poolUsed++]; // reference the "dummy" node within the pool and make node point to that

    node->roundNode = round; // assign node's roundtype to the passed round
    // sanitize the node to make it a leaf node
    node->left = nullptr;
    node->right = nullptr;

    return node; // return the node
}

// function used to print nodes by depth
// no parameters
void Ballistic::printBydepth() {
    if (root == nullptr) {
        return;
    }

    int depth = 0;
    std::queue<BallisticNode*> queue;
    queue.push(root);

    while (!queue.empty()) {
        size_t length = queue.size();
        std::cout << "Depth: " << depth << std::endl;
        for (size_t i = 0; i < length; i++) {
            if (queue.front()->left != nullptr) {
                queue.push(queue.front()->left);
            }
            if (queue.front()->right != nullptr) {
                queue.push(queue.front()->right);
            }

            std::cout << "Type of node: " << queue.front()->roundNode->type << std::endl;
            std::cout << "Position ";
            queue.front()->roundNode->particle.printPosition();
            queue.pop();
        }
        depth++;
        std::cout << std::endl;
    }
}

// Function used to calculated the distance squared between the positions of two nodes
/* Parameters:
*       Two Ballistic nodes passed by pointers
*       Returns a float value (distance square)
*/
float Ballistic::distance2(BallisticNode* node1, BallisticNode* node2) {
    float sumX = node1->roundNode->particle.getPosition().x - node2->roundNode->particle.getPosition().x; // do node1.x - node2.x
    float sumY = node1->roundNode->particle.getPosition().y - node2->roundNode->particle.getPosition().y; // do node1.y - node2.y
    
    // square the sums
    float xSquared = sumX * sumX;
    float ySquared = sumY * sumY;

    // return the sums of the squares
    return xSquared + ySquared;
}


// Function used to find the nearestNeighbor of a target node
/* Parameters:
*       Current node pointer, Target node pointer, Pointer reference of the best node found, Reference of current bestDistance, current depth
*/
void Ballistic::findBestNodeHelper(BallisticNode* current, BallisticNode* target, BallisticNode*& bestNode, float& bestDistance, int depth) {
    // if the current node is nullptr
    // return to previous recursion call
    if (current == nullptr) {
        return;
    }

    // calcuate the current distance on current recursive call
    float distance = distance2(current, target); // pass in the current node we are checking and the target node

    // if the distance is greater than 0.0f(cannot be itsself) AND if the distance is less than the current bestDistance
    if (distance > 0.0f && distance < bestDistance) {
        //swap bestDistance to distance and bestNode to current node
        bestDistance = distance;
        bestNode = current;
    }

    // calculate the axis we are going to check in the tree
    // if axis is 0 we check x-axis if 1 we check y-axis
    int axis = depth % 2;

    // if axis is 0 we the the x position of the current and target nodes
    float currentAxisValue = (axis == 0) ? current->roundNode->particle.getPosition().x : current->roundNode->particle.getPosition().y;
    float targetAxisValue = (axis == 0) ? target->roundNode->particle.getPosition().x : target->roundNode->particle.getPosition().y;

    // if the targetAxis value is less than the currentAxisValue nearChild pointer is set to currents left pointer else the right pointer
    // if the targetAxis value is less than the currentAxisValue farChild pointer is set to currents right pointer else the left pointer
    BallisticNode* nearChild = (targetAxisValue < currentAxisValue) ? current->left : current->right;
    BallisticNode* farChild = (targetAxisValue < currentAxisValue) ? current->right : current->left;

    findBestNodeHelper(nearChild, target, bestNode, bestDistance, depth+1); // recursivly call function on nearChild

    // only enters this portion after we are returing up from the bottom nodes
    float difference = targetAxisValue - currentAxisValue;
    float differenceSquared = difference * difference;

    // if the differneceSquared is less than the curernt bestDistance
    // we potentially have a better out put on the far child
    if (differenceSquared < bestDistance) {
        findBestNodeHelper(farChild, target, bestNode, bestDistance, depth+1); // recursivly call function on farChild
    }
}


// Function that calls findBestNodeHelper and returns the bestNode found
/* Parameters:
*       BallisticNode pointer to target
*/
Ballistic::BallisticNode* Ballistic::findBestNode(BallisticNode* target) {
    // if the root of the tree is null 
    if (root == nullptr) {
        std::cout << "Returning target node" << std::endl;
        return target; // return the target
    }

    // Initialize the best distance to infinity and bestNode to root node
    float bestDistance = std::numeric_limits<float>::infinity();
    BallisticNode* bestNode = root;

    // call recursive helper function
    findBestNodeHelper(root, target, bestNode, bestDistance, 0);

    return bestNode; // return the pointer to the bestNode
}

Ballistic::BallisticNode* Ballistic::getRoot() {
    return root;
}

void Ballistic:: treeReset() {
    root = nullptr;
    poolUsed = 0;
}

//bool Ballistic:: compareNodeDistances(std::pair<float, BallisticNode*> nodeA, std::pair<float, BallisticNode*> nodeB) {
//    return nodeA.first < nodeB.first;
//}
//
//void Ballistic:: findMultipleNNHelper(BallisticNode* node, BallisticNode* target, std::vector<std::pair<float, BallisticNode*>>& bestNodes, int maxBestNodes, int depth) {
//    if (node == nullptr || target == nullptr) return;
//    if (node->roundNode == nullptr || target->roundNode == nullptr) return;
//
//    float distance = distance2(node, target);
//    int axis = depth % 2;
//
//    if (distance > 0.0f && !duplicateCheck(bestNodes, node)) {
//        if (static_cast<int>(bestNodes.size()) < maxBestNodes) {
//            bestNodes.emplace_back(distance, node);
//            std::sort(bestNodes.begin(), bestNodes.end(), compareNodeDistances);
//        }
//        else if (distance < bestNodes.back().first) {
//            bestNodes.back() = { distance, node };
//            std::sort(bestNodes.begin(), bestNodes.end(), compareNodeDistances);
//        }
//    }
//
//    float targetAxisValue = (axis == 0) ? target->roundNode->particle.getPosition().x : target->roundNode->particle.getPosition().y;
//    float nodeAxisValue = (axis == 0) ? node->roundNode->particle.getPosition().x : node->roundNode->particle.getPosition().y;
//
//    BallisticNode* nearChild = (targetAxisValue < nodeAxisValue) ? node->left : node->right;
//    BallisticNode* farChild = (targetAxisValue < nodeAxisValue) ? node->right : node->left;
//
//    findMultipleNNHelper(nearChild, target, bestNodes, maxBestNodes, depth + 1);
//
//    float worstDistance = (bestNodes.size() < maxBestNodes) ? std::numeric_limits<float>::infinity() : bestNodes.back().first;
//
//    float difference = targetAxisValue - nodeAxisValue;
//    float differenceSquared = difference * difference;
//
//    if (differenceSquared < worstDistance) {
//        findMultipleNNHelper(farChild, target, bestNodes, maxBestNodes, depth + 1);
//    }
//
//}
//std::vector<Ballistic:: BallisticNode*> Ballistic:: findMultipleNN(Ballistic::BallisticNode *target, int maxBestNodes) {
//    if (root == nullptr || maxBestNodes <= 0) return {};
//
//    std::vector<BallisticNode*> bestNodes;
//    std::vector<std::pair<float, BallisticNode*>> bestDistancesAndNodes;
//
//    bestDistancesAndNodes.reserve(maxBestNodes);
//
//    findMultipleNNHelper(root, target, bestDistancesAndNodes, maxBestNodes, 0);
//
//    bestNodes.reserve(bestDistancesAndNodes.size());
//
//    for (auto& pair : bestDistancesAndNodes) {
//        if (pair.second == nullptr) continue;
//        bestNodes.emplace_back(pair.second);
//    }
//
//    return bestNodes;
//
//}
//
//bool Ballistic::duplicateCheck(const std::vector<std::pair<float, BallisticNode*>>& nodes, const BallisticNode* node) {
//    bool duplicate = false;
//
//    if (nodes.empty()) return duplicate;
//
//    for (auto& pair : nodes) {
//        if (pair.second == node) duplicate = true;
//    }
//
//    return duplicate;
//}