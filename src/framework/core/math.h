/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits>

#include "logger.h"
#include "macros.h"
#include "glm/glm.hpp"

#define GLM_FORCE_RADIANS


namespace Framework
{
    const float PI              = 3.14159265358979323846264338327950288419716939937510f;
    const float PI2             = PI * 2.0f;

#define ASSERT_vec3_is_normalized(v) ASSERT(abs ( 1.0f - glm::dot(v, v)) < 1.0e-6f)
    
    /* find intersection of a line (P,D) with plane (Q,N)
    where: P - a point on the line, D - directional vector of the line,
    Q - a point on the plane, N plane normal vector */
    enum class LineVsPlaneClassification { Intersect, Belong, Parallel };
    LineVsPlaneClassification LineVsPlane(
        __in const glm::vec3& aP, __in const glm::vec3& aD,
        __in const glm::vec3& aQ, __in const glm::vec3& aN,
        __out glm::vec3& anIntersectionPoint);

    /* find a signed distance from a point P and a plane (Q,N)
    where: P - a point,
    Q - a point on the plane, N plane normal vector */
    float SignedDistance(
        __in const glm::vec3& aP,
        __in const glm::vec3& aQ, __in const glm::vec3& aN);

    /* find angle between vectors D1 and D2 according to normal N
    if D1 is parallel to D2, angle is 0,
    if <D1,D2,N> is a right hand system, angle is > 0,
    if <D1,D2,N> is a left hand system, angle is < 0 */
    float SignedAngle(const glm::vec3& aD1, const glm::vec3& aD2, const glm::vec3& aN);

    template<class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
      equal_with_epsilon(T x, T y, T epsilon)
    {
      return std::abs(x - y) < epsilon
          || std::abs(x - y) < std::numeric_limits<T>::min();
    }

    //From http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
    template<class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
      almost_equal(T x, T y, int ulp)
    {
      return equal_with_epsilon(x, y, std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp);
    }

    template <typename T>
    T WiseMax(T aParam1, T aParam2)
    {
        if (aParam1.length() != aParam2.length())
        {
            CRASH("Different length of aParam1(%d) and aParam2(%d)\n", aParam1.length(), aParam2.length());
        }

        T aResult;
        for(int i = 0; i < aParam1.length(); ++i)
            aResult[i] = std::max(aParam1[i],aParam2[i]);
        return aResult;
    }

    template <typename T>
    T WiseMin(T aParam1, T aParam2)
    {
        if (aParam1.length() != aParam2.length())
        {
            CRASH("Different length of aParam1(%d) and aParam2(%d)\n", aParam1.length(), aParam2.length());
        }

        T aResult;
        for(int i = 0; i < aParam1.length(); ++i)
            aResult[i] = std::min(aParam1[i],aParam2[i]);
        return aResult;
    }


    uint32_t Clp2(uint32_t x);
    float RestrictAngle(float angle);

    ///**
    //* Perlin noise implementation mostly based on:
    //*
    //* https://flafla2.github.io/2014/08/09/perlinnoise.html
    //*
    //* including octaves noise
    //*/
    //class Perlin
    //{
    //public:
    //    /**
    //    * Returns the perlin noise at the specified coordinate
    //    *
    //    * A 2D slice can be obtained by fixing one of the coordinates
    //    *
    //    * @param x  X coordinate
    //    * @param y  Y coordinate
    //    * @param z  Z coordinate
    //    *
    //    * @return Value between 0.0 and 1.0
    //    */
    //    static double Noise(double x, double y, double z);
    //
    //    /**
    //    * Returns the perlin noise at the specified location after sampling
    //    * different octaves for the same coordinate. The number of octaves to
    //    * be samples are indicated by 'octaves' parameter. The persistence of each
    //    * octave is determined by the 'persistence' parameter. If 'persistence' is less than
    //    * 1.0 higher frequency octaves will have less impact in the resulting noise. If
    //    * 'persistence' is greater than 1.0 then lower frequency octaves will have less
    //    * impact in the resulting noise
    //    *
    //    * @param x            X coordinate
    //    * @param y            Y coordinate
    //    * @param z            Z coordinate
    //    * @param octaves      Number of octaves to sample, up to 255
    //    * @param persistence  Persistance of each subsequent octave being sampled,
    //    *                     with a value greater than 0.0
    //    *
    //    * @return Value between 0.0 and 1.0
    //    */
    //    static double Octave(double x, double y, double z, uint8_t octaves, double persistence);
    //
    //private:
    //    /**
    //    * Singleton accessor
    //    */
    //    static Perlin *GetPerlin();
    //
    //    /**
    //    * Constructor
    //    */
    //    Perlin();
    //
    //    double Noise(double x, double y, double z);
    //    double Octave(double x, double y, double z, uint8_t octaves, double persistence);
    //    double Grad(uint8_t hash, double x, double y, double z);
    //    double Fade(double v);
    //    uint8_t *mPerms;
    //};



    //glm::vec3 GetVectorFromYawPitch(float aYaw, float aPitch);

    //void GetYawPitchFromVector(glm::vec3 aVector, float *aYaw, float *aPitch);

    //void GetUnprojectedPosition(float aPosX, float aPosY, glm::mat4 aProjection, float& aOutX, float& aOutY);
};
