//
// Created by elder on 12/17/2025.
//

#pragma once
#include "core.h"

class Particle {
protected:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 forceAccumulator;

    float damping;
    float inverseMass;
    float kinetic;
    float mass;
    float radius;

    void integrate(float duration);

public:
    Particle();

    // Acceleration values
    void setAcceleration(const Vector3& passedAcceleration);
    void setAcceleration(float x, float y, float z);
    const Vector3& getAcceleration() const;
    void printAcceleration();
    //

    // Velocity values
    void setVelocity(const Vector3& vector);
    void setVelocity(float x, float y, float z);
    const Vector3& getVelocity() const;
    void printVelocity();
    //

    // Position values
    void setPosition(const Vector3& vector);
    void setPosition(float x, float y, float z);
    const Vector3& getPosition() const;
    void printPosition();
    //

    // Kinetic values
    void setKineticEnergy();
    const float& getKineticEnergy() const;
    //

    //Radius values
    void setRadius(float passedRadius);
    const float& getRadius();
    //

    // Mass values
    void setMass(float passedMass);
    const float& getInverseMass() const;
    const float& getMass() const;
    //

    // Damping value
    void setDamping(float passedDamping);
    const float& getDamping();
    //

    // Force values
    void addForce(const Vector3& forceVector);
    void addForce(float x, float y, float z);
    void clearAccumulator();
    void clearAllValues();
    //

    void update(float time);
};