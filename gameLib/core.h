//
// Created by elder on 12/17/2025.
//
#pragma once
#include <cmath>

class Vector3 {
public:
    float x;
    float y;
    float z;

private:
    float pad;

public:
    Vector3() : x(0), y(0), z(0) { pad = 0; }
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) { pad = 0; }

    void invert() {
        x = -x;
        y = -y;
        z = -z;
    }

    [[nodiscard]] float magnitude() const {
        return sqrtf(x * x + y * y + z * z);
    }

    [[nodiscard]] float squareMagnitude() const {
        return x * x + y * y + z * z;
    }

    // function to normalize the vector
    void normalize() {
        float length = magnitude();
        if (length > 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    ////////////////////////////////////////////////////////////
    // operator overload to allow use to multiply bty scalr "value" to all values of a Vector3
    // Essentailly sclaing the vector by value
    // this modifies the actual values of the object
    void operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;
    }

    // Operator overload to allow us to mutiply by scalar "value"
    // Essentailly sclaing the vector by value
    // this promises not to modify current object but return a new Vector3 object
    Vector3 operator*(const float value) const {
        return { x * value, y * value, z * value };
    }
    ////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////
    // Operator overload to allow us to do vector addition
    // pass in a const referenced Vector3 called vector
    void operator+=(const Vector3& vector) {
        // add the all components to *this vector object
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }
    //Operator overload to allow us to do vector addition,
    // pass in a const referenced Vector3 called vector
    // this one we are returning a new Vector3 object
    // we are NOT modifying *this Vector3 Object
    Vector3 operator+(const Vector3& vector) const {
        return { x + vector.x, y + vector.y, z + vector.z };
    }
    ////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////
    // Operator overload to allow us to do vector subtraction
    // pass in a const referenced Vector3 called vector
    void operator-=(const Vector3& vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    //Operator overload to allow us to do vector subtraction,
    // pass in a const referenced Vector3 called vector
    // this one we are returning a new Vector3 object
    // we are NOT modifying *this Vector3 Object
    Vector3 operator-(const Vector3& vector) const {
        return { x - vector.x, y - vector.y, z - vector.z };
    }
    ////////////////////////////////////////////////////////////

    // function that modifies this vector3 object
    // pass in a referenced Vector3 object and a scalar
    void addScaledVector(const Vector3& vector, const float scale) {
        // scale this values by vector.x/y/z * scale
        x += vector.x * scale;
        y += vector.y * scale;
        z += vector.z * scale;
    }
    // this DOES modify the *this vector3 object

    // function to do vector * vector arithmatic
    [[nodiscard]] Vector3 componentProduct(const Vector3& vector) const {
        // multiply each value of *this object by each values of passed vector object
        return { x * vector.x, y * vector.y, z * vector.z }; // return new Vector3 object
    }
    // function to do vector * vector arithmatic
    void componentUpdate(const Vector3& vector) {
        // this does inplace vector multiplication to the *this object
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
    }

    // function that will return a float scalar value from a dot b
    [[nodiscard]] float scalarProduct(const Vector3& vector) const {
        return (x * vector.x) + (y * vector.y) + (z * vector.z);
    }

    // allows for scalar through the dot product of a dot b
    // this overloads the * operator to just do <1,2,3> * <4,5,6>
    // it can be confusing since we are already overloading the same operator above
    float operator *(const Vector3& vector) const {
        return (x * vector.x) + (y * vector.y) + (z * vector.z);
    }

    // function that takes in a referenced vector3 object
    // do vector cross product on *this vector with passed vector
    // finally return a new Vector3 object
    // definition would be Vector3 c = a.vectorCrossProduct(b);
    [[nodiscard]] Vector3 vectorCrossProduct(const Vector3& vector) const {
        return {
            ((y * vector.z) - (z * vector.y)),
            ((z * vector.x) - (x * vector.z)),
            ((x * vector.y) - (y * vector.x))
        };
    }

    // In place cross product.
    // First computes the cross product using the current vector (*this)
    // and the passed vector, then assigns the resulting Vector3
    // back to *this.
    // definition would be a %= b
    void operator %=(const Vector3& vector) {
        *this = vectorCrossProduct(vector);
    }

    // same thing as vectorCrossProduct function but here
    // we would need to define as Vector3 c = a % b
    Vector3 operator%(const Vector3& vector) const {
        return {
            ((y * vector.z) - (z * vector.y)),
            ((z * vector.x) - (x * vector.z)),
            ((x * vector.y) - (y * vector.x))
        };
    }

    Vector3 operator/(const Vector3& vector) const {
        return { x / vector.x, y / vector.y, z / vector.z };
    }

    void operator/=(const Vector3& vector) {
        // divide all components to *this vector object
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
    }

    Vector3 operator/(const float &val) const {
        return { x / val, y / val, z / val };
    }

    void makeOrthoNormalBasis(Vector3* a, Vector3* b, Vector3* c) {
        a->normalize();
        (*c) = (*a) % (*b);
        if (c->magnitude() == 0.0) return;

        c->normalize();
        (*b) = (*c) % (*a);
    }
};