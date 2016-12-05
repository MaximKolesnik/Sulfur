cbuffer TonemapData
{
  float Threshold;
  float WhiteLevel;
  float Saturation;
  float Bias;
  float Exposure;
  float KeyValue;
  float AutoExposure;
  float LuminanceMipLevel;
};

#define TONEMAP TonemapExp

float Luminance(float3 color)
{
  return max(color.r * 0.299f + color.g * 0.587f + color.b * 0.114f, 0.0001f);
}

float AverageLuminance(in Texture2D tex, in SamplerState ss, in float2 texCoords)
{
  return exp(tex.SampleLevel(ss, texCoords, LuminanceMipLevel).r);
}

float3 TonemapNone(float3 color)
{
  return color;
}

float3 TonemapLog(float3 color)
{
  float lum = Luminance(color);
  float mappedLum = log10(1.0f + lum) / log10(1.0f + WhiteLevel);
  return mappedLum * pow(color / lum, Saturation);
}

float3 TonemapDragoLog(float3 color)
{
  float lum = Luminance(color);
  float mappedLum = log10(1.0f + lum);
  mappedLum /= log10(1.0f + WhiteLevel);
  mappedLum /= log10(2.0f + 8.0f * ((lum / WhiteLevel) * log10(Bias) / log10(0.5f)));
  return mappedLum * pow(color / lum, Saturation);
}

float3 TonemapExp(float3 color)
{
  float lum = Luminance(color);
  float mappedLum = 1.0f - exp(-lum / WhiteLevel);
  return mappedLum * pow(color / lum, Saturation);
}

float3 TonemapReinhard(float3 color)
{
  float lum = Luminance(color);
  float mappedLum = lum / (lum + 1.0f);
  return mappedLum * pow(color / lum, Saturation);
}

float3 TonemapReinhardMod(float3 color)
{
  float lum = Luminance(color);
  float mappedLum = lum * (1.0f + lum / (WhiteLevel * WhiteLevel)) / (lum + 1.0f);
  return mappedLum * pow(color / lum, Saturation);
}

float3 TonemapFilmicALU(float3 color)
{
  color = max(0.0f, color - 0.004f);
  color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f) + 0.06f);
  return pow(color, 2.2f);
}

float4 ExposedColor(float3 color, float averageLum)
{
  color = max(0.0f, color);
  float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);

  if (AutoExposure >= 1.0f && AutoExposure <= 2.0f)
  {
    averageLum = max(averageLum, 0.001f);

    float key = 0.0f;
    if (AutoExposure == 1) key = KeyValue;
    else if (AutoExposure == 2) key = 1.03f - (2.0f / (2.0f + log10(averageLum + 1.0f)));

    float linearExp = key / averageLum;
    output.a = log2(max(linearExp, 0.0001f));
  }
  else
  {
    output.a = Exposure;
  }

  output.a -= Threshold;
  output.rgb = exp2(output.a) * color;
  return output;
}
