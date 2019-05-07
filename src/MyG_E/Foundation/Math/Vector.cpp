#include "Vector.h"


Vector<float, 3> Cross(const Vector<float, 3>& vector1, const Vector<float, 3>& vector2) {
Vector<float, 3> result;
result[0] = (vector1[1] * vector2[2] - vector1[2] * vector2[1]);
result[1] = (vector1[2] * vector2[0] - vector1[0] * vector2[2]);
result[2] = (vector1[0] * vector2[1] - vector1[1] * vector2[0]);
return result;
}