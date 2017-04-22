/******************************************************************************/
/*!
\par     Sulfur
\file    Blur.hlsli
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Blur constants.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
static const float GaussianOffset[3] = {
  0.0, 1.3846153846, 3.2307692308
};

static const float GaussianKernel[3] = {
  0.2270270270, 0.3162162162, 0.0702702703
};

static const float GaussianKernel3[3] = {
  0.27901f, 0.44198f, 0.27901f
};

static const float GaussianKernel5[5] = {
  0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f
};

static const float GaussianKernel7[7] = {
  0.00598f, 0.060626f, 0.241843f, 0.383103f, 0.241843f, 0.060626f, 0.00598f
};

static const float GaussianKernel9[9] = {
  0.000229f, 0.005977f, 0.060598f, 0.241732f, 0.382928f, 0.241732f, 0.060598f, 0.005977f, 0.000229f
};

static const float GaussianKernel11[11] = {
  0.000003f, 0.000229f, 0.005977f, 0.060598f, 0.24173f, 0.382925f, 0.24173f, 0.060598f, 0.005977f, 0.000229f, 0.000003
};