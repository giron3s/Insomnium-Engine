/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#include "precompiled.h"

namespace Framework
{
    LineVsPlaneClassification LineVsPlane(
        __in const glm::vec3& aP, __in const glm::vec3& aD,
        __in const glm::vec3& aQ, __in const glm::vec3& aN,
        __out glm::vec3& anIntersectionPoint)
    {   // Note: D and N vectors assumed to be normalized
        /* reference formula: https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
                      dot(Q - P, N)
        I  = P + D * ---------------
                        dot(D, N)
        I  - intersection point
        if dot(D,N) == 0 we check if point P belonging to the line satisfies plane equation:
        dot(P,N) - dot(Q,N) = 0. If the equation is satisfied, the line belongs to the plane
        otherwise line is parallel to the plane. */
        ASSERT_vec3_is_normalized(aD);
        ASSERT_vec3_is_normalized(aN);
        float d = glm::dot(aD, aN);
        if ( abs(d) < 1.0e-6f )
        { // intersection point is undefined in this case
            return (glm::dot(aP, aN) - glm::dot(aQ, aN)) < 1.0e-6f ?
                LineVsPlaneClassification::Belong : LineVsPlaneClassification::Parallel;
        }
        anIntersectionPoint = aP + aD * (glm::dot((aQ - aP), aN) / d);
        return LineVsPlaneClassification::Intersect;
    }


    float SignedDistance(
        __in const glm::vec3& aP,
        __in const glm::vec3& aQ, __in const glm::vec3& aN)
    {
        // Note: N vector assumed to be normalized
        ASSERT_vec3_is_normalized(aN);
        return glm::dot(aN, aP - aQ);
    }


    float SignedAngle(const glm::vec3& aD1, const glm::vec3& aD2, const glm::vec3& aN)
    {   // Note: D1 and D2 vectors assumed to be normalized
        ASSERT_vec3_is_normalized(aD1);
        ASSERT_vec3_is_normalized(aD2);
        ASSERT_vec3_is_normalized(aN);
        float a = acosf(glm::dot(aD1, aD2));

        /*INFO(LogLevel::eLEVEL2, "SignedAngle : D1(%f, %f, %f), D2(%f, %f, %f), N(%f, %f, %f)",
            aD1.x, aD1.y, aD1.z,
            aD2.x, aD2.y, aD2.z,
            aN.x, aN.y, aN.z);*/
			
        if ( glm::dot(glm::cross(aD1, aD2), aN) < 0.0f )
            a = -a;
        return a;
    }


    //glm::vec3 GetVectorFromYawPitch(float aYaw, float aPitch)
    //{
    //    return glm::vec3(sin(aYaw) * cos(aPitch), sin(aPitch), cos(aYaw) * cos(aPitch));
    //}

    //void GetYawPitchFromVector(glm::vec3 aVector, float *aYaw, float *aPitch) {
    //    *aYaw = atan2(aVector.x, aVector.z);

    //    // Project v in the plane XZ
    //    glm::vec3 lYzero = glm::vec3(aVector.x, 0.0, aVector.z);
    //    float mdo = glm::length(lYzero);
    //    *aPitch = atan2(aVector.y, mdo);
    //}

    //void GetUnprojectedPosition(float aPosX, float aPosY, glm::mat4 aProjection, float& aOutX, float& aOutY)
    //{
    //    glm::vec4 lPosition(aPosX, aPosY, 0.0f, 1.0f);
    //    lPosition = glm::inverse(aProjection) * lPosition;

    //    aOutX = lPosition.x;
    //    aOutY = lPosition.y;
    //}

    uint32_t Clp2(uint32_t x)
    {
        x--;
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return x + 1;
    }

    float RestrictAngle(float angle)
    {
        if (angle >= 360.0)
            angle -= 360.0;
        else if (angle < 0.0)
            angle += 360.0;
        return angle;
    }

    ///**
    //* Perlin noise implementation
    //*/
    //double Perlin::Noise(double x, double y, double z) { return GetPerlin()->Noise(x, y, z); }
    //double Perlin::Octave(double x, double y, double z, uint8_t octaves, double persistence)
    //{
    //    return GetPerlin()->Octave(x, y, z, octaves, persistence);
    //}
    //
    //Perlin *Perlin::GetPerlin()
    //{
    //    static Perlin *_perlin = NULL;
    //
    //    if (_perlin == NULL)
    //    {
    //        _perlin = new Perlin();
    //    }
    //    return _perlin;
    //}
    //
    //Perlin::Perlin()
    //{
    //    uint8_t perlinPerms[] = {
    //        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240,
    //        21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
    //        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111,
    //        229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,
    //        209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217,
    //        226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,
    //        223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
    //        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241,
    //        81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
    //        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180 };
    //
    //    mPerms = new uint8_t[2 * sizeof perlinPerms / sizeof *perlinPerms];
    //
    //    for (int i = 0; i < sizeof perlinPerms / sizeof *perlinPerms; ++i) {
    //        mPerms[i] = mPerms[i + 256] = perlinPerms[i];
    //    }
    //}
    //
    //double Perlin::Noise(double x, double y, double z)
    //{
    //    int32_t xi = (int32_t)x & 255;
    //    int32_t yi = (int32_t)y & 255;
    //    int32_t zi = (int32_t)z & 255;
    //
    //    x -= xi;
    //    y -= yi;
    //    z -= zi;
    //
    //    double u = Fade(x);
    //    double v = Fade(y);
    //    double w = Fade(z);
    //
    //    uint8_t px0 = mPerms[xi] + yi;
    //    uint8_t px1 = mPerms[xi + 1] + yi;
    //    uint8_t px0y0 = mPerms[px0] + zi;
    //    uint8_t px0y1 = mPerms[px0 + 1] + zi;
    //    uint8_t px1y0 = mPerms[px1] + zi;
    //    uint8_t px1y1 = mPerms[px1 + 1] + zi;
    //
    //    return (glm::lerp(
    //        glm::lerp(glm::lerp(Grad(mPerms[px0y0], x, y, z), Grad(mPerms[px0y0 + 1], x, y, z - 1.0), w),
    //            glm::lerp(_grad(Perms[px0y1], x, y - 1.0, z), _grad(mPerms[px0y1 + 1], x, y - 1.0, z - 1.0), w), v),
    //        glm::lerp(glm::lerp(_grad(mPerms[px1y0], x - 1.0, y, z), _grad(mPerms[px1y0 + 1], x - 1.0, y, z - 1.0), w),
    //            glm::lerp(_grad(mPerms[px1y1], x - 1.0, y - 1.0, z), _grad(mPerms[px1y1 + 1], x - 1.0, y - 1.0, z - 1.0), w), v),
    //        u) +
    //        1.0) /
    //        2.0;
    //}
    //
    //double Perlin::Octave(double x, double y, double z, uint8_t octaves, double persistence)
    //{
    //    double frequency = 1.0;
    //    double amplitude = 1.0;
    //    double maxValue = 0.0;
    //
    //    double v = 0.0;
    //
    //    for (int i = 0; i < octaves; ++i) {
    //        v += amplitude * Noise(x * frequency, y * frequency, z * frequency);
    //
    //        maxValue += amplitude;
    //
    //        amplitude *= persistence;
    //        frequency *= 2.0;
    //    }
    //
    //    return v / maxValue;
    //}
    //
    //double Perlin::Grad(uint8_t hash, double x, double y, double z)
    //{
    //    // clang-format off
    //    switch (hash & 0xF) {
    //    case 0x0: return  x + y;
    //    case 0x1: return -x + y;
    //    case 0x2: return  x - y;
    //    case 0x3: return -x - y;
    //    case 0x4: return  x + z;
    //    case 0x5: return -x + z;
    //    case 0x6: return  x - z;
    //    case 0x7: return -x - z;
    //    case 0x8: return  y + z;
    //    case 0x9: return -y + z;
    //    case 0xA: return  y - z;
    //    case 0xB: return -y - z;
    //    case 0xC: return  y + x;
    //    case 0xD: return -y + z;
    //    case 0xE: return  y - x;
    //    case 0xF: return -y - z;
    //    default: return 0;  // never happens
    //    }
    //    // clang-format on
    //}
    //
    //double Perlin::Fade(double v) { return v * v * v * (v * (v * 6 - 15) + 10); }
}
