#include "PixelInputs.hlsli"

cbuffer PickingData
{
  uint ObjectHandle;
};

uint main(PixelIn input) : SV_TARGET
{
  return ObjectHandle;
}
