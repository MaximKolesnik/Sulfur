#include "PixelInputs.hlsli"

float main(DepthPixelIn input) : SV_TARGET
{
  return input.depth;
}
