static const float3 c_dielectricSpecular = float3(0.2f, 0.2f, 0.2f);

float pow2(float num)
{
  return num * num;
}

float pow3(float num)
{
  return num * num * num;
}

float pow4(float num)
{
  return num * num * num * num;
}

float pow5(float num)
{
  return num * num * num * num * num;
}

float3 SchlickFresnel(in float VoH, in float3 F0)
{
  return F0 + (1.0f - F0) * pow5(1.0f - VoH);
}

float3 SphericalGaussianFresnel(in float NoV, in float3 F0)
{
  return F0 + (1.0f - F0) * pow(2.0f, (-5.55473f * NoV - 6.98316f) * NoV);
}

float ImplicitGeometry(in float NoL, in float NoV)
{
  return NoL * NoV;
}

float NewmannGeometry(in float NoL, in float NoV)
{
  return (NoL * NoV) / max(NoL, NoV);
}

float CookTorrenceGeometry(in float NoL, in float NoV, in float NoH, in float VoH)
{
  return min(1.0f, min(2.0f * NoV * NoH / VoH, 2.0f * NoL * NoH / VoH));
}

float SmithGeometry(in float rough2, in float NoV)
{
  float k = sqrt(2 * pow2(rough2) / 3.14159f);
  return NoV / (NoV * (1.0f - k) + k);
}

float PhongDistribution(in float rough2, in float NoH)
{
  if (NoH <= 0.0f) return 0;
  
  float ap = 2.0f / pow2(rough2) - 2.0f;
  return (ap + 2.0f) / (2.0f * 3.14159f) * pow(NoH, ap);
}

float BeckmannDistribution(in float rough2, in float NoH)
{
  float NoH2 = NoH * NoH;
  float NoH3 = NoH2 * NoH;
  float a2 = rough2 * rough2;

  return 1.0f / (3.14159f * a2 * NoH3) * pow(2.718f, (NoH2 - 1.0f) / (NoH2 * a2));
}

float GgxDistribution(in float rough2, in float NoH)
{
  float a2 = pow2(rough2);
  return a2 / (3.14159f * pow2(1.0f + pow2(NoH) * (a2 - 1.0f)));
}

float MetallicToSemiconductorRange(in float metallic)
{
  return smoothstep(0.2f, 0.45f, metallic);
}

float3 CalculateSpecularColor(in float3 albedo, in float metallic)
{
  return lerp(c_dielectricSpecular, albedo, MetallicToSemiconductorRange(metallic));
}

float3 BlendDiffuseSpecular(in float3 diffuse, in float3 specular, in float3 albedo, in float metallic)
{
  float3 dielectric = diffuse + specular;
  float3 metal = specular * albedo;
  return lerp(dielectric, metal, MetallicToSemiconductorRange(metallic));
}

