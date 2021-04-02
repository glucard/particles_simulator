#include "mymath.hpp"

float norm(float x, float y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}

float calculateSine(float x, float y) {
    float distance = norm(x, y);
    return x / distance;
}
float calculateCossine(float x, float y) {
    float distance = norm(x, y);
    return y / distance;
}