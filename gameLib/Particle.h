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
    void setPosition(const Vector3& vector);
    void setPosition(float x, float y, float z);
    Vector3 getPosition() const;
    void printPosition();

    void setVelocity(const Vector3& vector);
    void setVelocity(float x, float y, float z);
    Vector3 getVelocity() const;
    void printVelocity();

    void setKineticEnergy();
    float getKineticEnergy() const;

    void setRadius(float passedRadius);
    float getRadius();

    void setMass(float passedMass);
    float getInverseMass() const;
    float getMass() const;

    void setDamping(float passedDamping);
    float getDamping();

    void setAcceleration(const Vector3& passedAcceleration);
    void setAcceleration(float x, float y, float z);
    Vector3 getAcceleration() const;
    void printAcceleration();

    void addForce(const Vector3& forceVector);
    void addForce(float x, float y, float z);
    void clearAccumulator();

    void clearAllValues();

    void update(float time);
};