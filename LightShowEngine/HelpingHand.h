#ifndef HELPING_HAND_H
#define HELPING_HAND_H
#include <Bullet/LinearMath/btTransform.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

/****************************
*hh stands for helping hand.*
*****************************/

#define property(Type, Variable)                              \
private:                                                      \
    Type Variable;                                            \
                                                              \
public:                                                       \
    const Type##& get##Variable() const { return Variable; }; \
    void set##Variable(const Type& Variable##_) { Variable = Variable##_; }

//here is how you use the property definition

//class abc
//{
//	//this automattically creates a getter, setter & a private variable with the type of std::string, with the name 'name' for the private variable, and 'getname' for the getter and 'setname' for the setter.
//	property(std::string, name);
//
//	void modifyProperty()
//	{
//		setname("hello!");
//		std::string test = getname();
//	}
//
//};

namespace hh {

int getRandomRange(int minInclusive, int maxExclusive);
float getRandomRange(float minInclusive, float maxExclusive);

template <typename T>
bool compareFloatingPoint(T a, T b, T epsilon) {
    return (glm::abs<T>(a - b)) < epsilon;
}

btVector3 toBtVec3(const glm::vec3&);
glm::quat toQuaternion(const glm::vec3&);
glm::vec3 toGlmVec3(const btVector3&);
glm::quat toGlmQuat(const btQuaternion&);

template <typename T, typename A, typename B>
std::vector<T> combineVectors(std::vector<A>& v1, std::vector<B>& v2) {
    std::vector<T> AB;
    AB.reserve(v1.size() + v2.size()); // preallocate memory
    AB.insert(AB.end(), v1.begin(), v1.end());
    AB.insert(AB.end(), v2.begin(), v2.end());
    return AB;
}
template <typename T, typename A>
std::vector<T> combineVectors(std::vector<A>& v1, std::vector<A>& v2) {
    std::vector<T> AB;
    AB.reserve(v1.size() + v2.size()); // preallocate memory
    AB.insert(AB.end(), v1.begin(), v1.end());
    AB.insert(AB.end(), v2.begin(), v2.end());
    return AB;
}
template <typename T>
std::vector<T> combineVectors(std::vector<T>& v1, std::vector<T>& v2) {
    std::vector<T> AB;
    AB.reserve(v1.size() + v2.size()); // preallocate memory
    AB.insert(AB.end(), v1.begin(), v1.end());
    AB.insert(AB.end(), v2.begin(), v2.end());
    return AB;
}

glm::quat eularToQuat(const glm::vec3& pitchYawRoll);

float signedRadiansBetweenVectors(glm::vec3 vec1, glm::vec3 vec2);

glm::vec3 sphericalToCartisean(float theta, float phi, float radius);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Check if a point is within a cone
bool isPointWithinCone(const glm::vec3& coneTipPosition, const glm::vec3& coneCenterLine, const glm::vec3& point, const float fovRadians);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Check if a point is within a cone (the fov passed into this function is expected to be cos(radians))
bool isPointWithinConePreCos(const glm::vec3& coneTipPosition, const glm::vec3& coneCenterLine, const glm::vec3& point, const float fovRadiansPreCos);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Check if a point is within a cone (within range)
bool isPointWithinCone(const glm::vec3& coneTipPosition, const glm::vec3& coneCenterLine, const glm::vec3& point, const float fovRadians, const float withinDistance);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Check if a point is within a cone (the fov passed into this function is expected to be cos(radians)) (within range)
bool isPointWithinConePreCos(const glm::vec3& coneTipPosition, const glm::vec3& coneCenterLine, const glm::vec3& point, const float fovRadiansPreCos, const float withinDistance);

//For a quad with texture coords
const float quadVertices[20] = {
    //top left
    -1.0f, 1.0f, -1.0f,

    0, 1,

    -1.0f, -1.0f, -1.0f,

    //bottom left
    0, 0,

    1.0f, 1.0f, -1.0f,

    // top right
    1, 1,

    // bottom right
    1.0f, -1.0f, -1.0f,

    1, 0
};

template <class T>
void swapVal(T& a, T& b) {
    T c(a);
    a = b;
    b = c;
}

const glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);

//clearVectorOfPointers will delete each element inside of a vector, then clear the vector of those pointers.
template <class C>
void clearVectorOfPointers(C& cntr) {
    for (typename C::iterator it = cntr.begin();
         it != cntr.end(); ++it) {
        delete *it;
    }
    cntr.clear();
}

//Used for linear interpolation.
template <typename T>
T lerp(const T& a, const T& b, float t) {
    return (1.0f - t) * a + t * b;
}

template <typename T>
void moveItemToBack(std::vector<T>& v, size_t itemIndex) {
    auto it = v.begin() + itemIndex;
    std::rotate(it, it + 1, v.end());
}

//Binary search will return the position inside of the vector where the key value is located. It the value is not there, it will return -1.
template <typename T>
int binarySearchVector(const std::vector<T>& sorted_vec, const T& key) {
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = sorted_vec.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (key > sorted_vec.at(mid)) {
            left = mid + 1;
        } else if (key < sorted_vec.at(mid)) {
            right = mid;
        } else {
            return mid;
        }
    }

    return -1;
}

static int isPowerOfTwo(unsigned int x) {
    return (
        x == 1 || x == 2 || x == 4 || x == 8 || x == 16 || x == 32 || x == 64 || x == 128 || x == 256 || x == 512 || x == 1024 || x == 2048 || x == 4096 || x == 8192);
}

};

#endif
