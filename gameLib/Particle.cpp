//
// Created by elder on 12/17/2025.
//

#include "Particle.h"
#include <cassert>
#include <iostream>

void Particle::integrate(float duration) {
    if (inverseMass <= 0.0f) return;

    assert(duration > 0.0f);
    
    // Add the small g to current acceleration in particle
    Vector3 resultingAcceleration = acceleration + forceAccumulator * inverseMass; // This is equation g = F/m
    std::cout << "Force Accumulator:" << forceAccumulator.x << ", " << forceAccumulator.y << ", " << forceAccumulator.z << "\n";
    std::cout << "Resulting Acceleration: " << resultingAcceleration.x << ", " << resultingAcceleration.y << ", " << resultingAcceleration.z << "\n";

    std::cout << "Velocity before acceleration change: " << velocity.x << ", " << velocity.y << ", " << velocity.z << "\n";
    velocity.addScaledVector(resultingAcceleration, duration); // scale acceleration by change in time then add to velocity vector
    std::cout << "Velocity after acceleration change: " << velocity.x << ", " << velocity.y << ", " << velocity.z << "\n";
    std::cout << std::endl;

    std::cout << "Position before velocity change: " << position.x << ", " << position.y << ", " << position.z << "\n";
    position.addScaledVector(velocity, duration); // scale velocity by change in time then add to position vector
    std::cout << "Position after velocity change: " << position.x << ", " << position.y << ", " << position.z << "\n";
    std::cout << std::endl;

    std::cout << "Velocity before before change: " << velocity.x << ", " << velocity.y << ", " << velocity.z << "\n";
    velocity *= powf(damping, duration); // dampen vellocity a bit if damp is less than 1 but grater than 0
    std::cout << "Velocity after damping change: " << velocity.x << ", " << velocity.y << ", " << velocity.z << "\n";

    std::cout << "\n";
    clearAccumulator();
}

////////////////////////////////////////////////////////////
void Particle::setPosition(const Vector3& passedPosition) {
    position = passedPosition;
}
void Particle::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

Vector3 Particle::getPosition() const {
    return position;
}
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
void Particle::setVelocity(const Vector3& passedVelocity) {
    velocity = passedVelocity;
}
void Particle::setVelocity(float x, float y, float z) {
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}
Vector3 Particle::getVelocity() const {
    return velocity;
}
////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
void Particle::setAcceleration(const Vector3& passedAcceleration) {
    acceleration = passedAcceleration;
}
void Particle::setAcceleration(float x, float y, float z) {
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

Vector3 Particle::getAcceleration() const {
    return acceleration;
}
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
void Particle::setDamping(const float passedDamping) {
    damping = passedDamping;
}
float Particle::getDamping() {
    return damping;
}

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
void Particle::setMass(const float passedMass) {
    if (passedMass <= 0.0f) {
        inverseMass = 0;
        mass = 0;
    }
    else {
        inverseMass = 1 / passedMass;
        mass = passedMass;
    }
}

float Particle::getInverseMass() const {
    return inverseMass;
}

float Particle::getMass() const {
    return mass;
}
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Particle::setKineticEnergy() {
    if (inverseMass == 0) {
        kinetic = 0;
    }
    else {
        float velocityMagnitude = velocity.magnitude();
        kinetic = (powf(velocityMagnitude, 2)) / (2 * inverseMass);
    }
}
float Particle::getKineticEnergy() const {
    return kinetic;
}
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
void Particle::setRadius(float passedRadius) {
    radius = passedRadius;
}
float Particle::getRadius() {
    return radius;
}
////////////////////////////////////////////////////////////

void Particle::addForce(const Vector3& forceVector) {
    forceAccumulator += forceVector;
}

void Particle::addForce(float x, float y, float z) {
    Vector3 forceVector(x, y, z);
    forceAccumulator += forceVector;
}
void Particle::clearAccumulator() {
    forceAccumulator.x = 0;
    forceAccumulator.y = 0;
    forceAccumulator.z = 0;
}

void Particle::update(float time) {
    integrate(time);
}