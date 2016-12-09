/******************************************************************************/
/*!
\par     Sulfur
\file    sfFbxImporter.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/24/2016

\brief   FBX model importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfFbxImporter.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include <fbxsdk.h>

namespace Sulfur
{

// Fbx memory stream
class FbxMemoryStream : public FbxStream
{

public:
  FbxMemoryStream(FbxManager* pSdkManager, BYTE *buffer, long size)
    : m_buffer(buffer), m_position(0), m_size(size)
  {
    m_readerId = pSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX (*.fbx)");
    m_writerId = pSdkManager->GetIOPluginRegistry()->FindWriterIDByDescription("FBX (*.fbx)");
  }

  ~FbxMemoryStream()
  {
    Close();
  }

  virtual EState GetState()
  {
    return m_buffer == nullptr ? eClosed : eOpen;
  }

  virtual bool Open(void* pStreamData)
  {
    m_position = 0;
    return m_buffer != nullptr;
  }

  virtual bool Close()
  {
    m_position = 0;
    return true;
  }

  virtual bool Flush()
  {
    return true;
  }

  virtual int Write(const void* pData, int pSize)
  {
    if (m_position + pSize > m_size) pSize = m_size - m_position;
    memcpy(m_buffer + m_position, pData, pSize);
    m_position += pSize;
    return pSize;
  }

  virtual int Read(void* pData, int pSize) const
  {
    if (m_position + pSize > m_size) pSize = m_size - m_position;
    memcpy(pData, m_buffer + m_position, pSize);
    const_cast<FbxMemoryStream*>(this)->m_position += pSize;
    return pSize;
  }

  virtual int GetReaderID() const
  {
    return m_readerId;
  }

  virtual int GetWriterID() const
  {
    return m_writerId;
  }

  /** Adjust the current stream position.
  * \param pSeekPos Pre-defined position where offset is added (FbxFile::eBegin, FbxFile::eCurrent:, FbxFile::eEnd)
  * \param pOffset Number of bytes to offset from pSeekPos. */
  virtual void Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos)
  {
    switch (pSeekPos)
    {
    case FbxFile::eBegin:
      m_position = (long)pOffset;
      break;
    case FbxFile::eCurrent:
      m_position += (long)pOffset;
      break;
    case FbxFile::eEnd:
      m_position = m_size - (long)pOffset;
      break;
    }
  }

  virtual long GetPosition() const
  {
    return m_position;
  }

  virtual void SetPosition(long pPosition)
  {
    m_position = pPosition;
  }

  virtual int GetError() const
  {
    return 0;
  }

  virtual void ClearError()
  {
  }

private:
  BYTE *m_buffer;
  long m_size;
  long m_position;

  int m_readerId;
  int m_writerId;

};

FbxAMatrix GeometricTransform(FbxNode *node)
{
  const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
  const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
  const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);

  return FbxAMatrix(lT, lR, lS);
}

FbxVector2 GetUV(FbxMesh *mesh, int index, int controlPoint, int polygonIndex, int positionInPolygon)
{
  FbxGeometryElementUV *uvElement = mesh->GetElementUV(index);
  if (uvElement)
  {
    switch (uvElement->GetMappingMode())
    {
    case FbxLayerElement::eByControlPoint:
      switch (uvElement->GetReferenceMode())
      {
      case FbxLayerElement::eDirect:
        return uvElement->GetDirectArray().GetAt(controlPoint);
      case FbxLayerElement::eIndexToDirect:
        return uvElement->GetDirectArray().GetAt(uvElement->GetIndexArray().GetAt(controlPoint));
      }
      break;

    case FbxLayerElement::eByPolygonVertex:
      return uvElement->GetDirectArray().GetAt(mesh->GetTextureUVIndex(polygonIndex, positionInPolygon));
    }
  }

  return FbxVector2(0.0, 0.0);
}

template <typename ReturnType, typename ElementType>
ReturnType GetElement(ElementType *element, int controlPoint, int vertexIndex)
{
  if (element)
  {
    switch (element->GetMappingMode())
    {
    case FbxLayerElement::eByControlPoint:
      switch (element->GetReferenceMode())
      {
      case FbxLayerElement::eDirect:
        return element->GetDirectArray().GetAt(controlPoint);
      case FbxLayerElement::eIndexToDirect:
        return element->GetDirectArray().GetAt(element->GetIndexArray().GetAt(controlPoint));
      }
      break;

    case FbxLayerElement::eByPolygonVertex:
      switch (element->GetReferenceMode())
      {
      case FbxLayerElement::eDirect:
        return element->GetDirectArray().GetAt(vertexIndex);
      case FbxLayerElement::eIndexToDirect:
        return element->GetDirectArray().GetAt(element->GetIndexArray().GetAt(vertexIndex));
      }
      break;
    }
  }

  return ReturnType();
}

FbxImporter::FbxImporter()
  : m_manager(nullptr)
{
}

FbxImporter::~FbxImporter()
{
  if (m_manager) m_manager->Destroy();
}

void FbxImporter::Load(BYTE *buffer, UINT32 bufferSize, Mesh *resource)
{
  if (m_manager == nullptr) m_manager = FbxManager::Create();

  resource->Init(GraphicsManager::Instance()->GetDevice());

  FbxScene *scene = FbxScene::Create(m_manager, "Scene");
  ::FbxImporter *importer = ::FbxImporter::Create(m_manager, "");

  FbxMemoryStream memoryStream(m_manager, buffer, (long)bufferSize);
  importer->Initialize(&memoryStream, nullptr, -1, m_manager->GetIOSettings());
  importer->Import(scene);

  double scale = scene->GetGlobalSettings().GetSystemUnit().GetConversionFactorTo(FbxSystemUnit::m);
  LoadMesh(scene, scene->GetRootNode(), resource, scale);

  importer->Destroy();
  scene->Destroy();
}

void FbxImporter::LoadMesh(FbxScene *scene, FbxNode *node, Mesh *resource, double scale)
{
  FbxMesh *mesh = node->GetMesh();
  const char *nodeName = node->GetName();

  if (mesh)
  {
    int vertexCounter = 0;
    int polygonCount = mesh->GetPolygonCount();
    for (int p = 0; p < polygonCount; ++p)
    {
      int triangleCount = mesh->GetPolygonSize(p) - 2;
      for (int t = 0; t < triangleCount; ++t)
      {
        UINT32 indices[3];
        for (int i = 0; i < 3; ++i)
        {
          int positionInPolygon = i == 0 ? 0 : t + i;
          int controlPointIndex = mesh->GetPolygonVertex(p, positionInPolygon);

          FbxVector4 position = mesh->GetControlPointAt(controlPointIndex);
          FbxVector2 uv0 = GetUV(mesh, 0, controlPointIndex, p, positionInPolygon);
          FbxVector4 normal = GetElement<FbxVector4>(mesh->GetElementNormal(), controlPointIndex, vertexCounter);
          FbxVector4 tangent = GetElement<FbxVector4>(mesh->GetElementTangent(), controlPointIndex, vertexCounter);
          FbxVector4 binormal = GetElement<FbxVector4>(mesh->GetElementBinormal(), controlPointIndex, vertexCounter);
          FbxColor color = GetElement<FbxColor>(mesh->GetElementVertexColor(), controlPointIndex, vertexCounter);

          indices[i] = resource->AddUniqueVertex(Vertex(
            Vector4((Real)position.mData[0], (Real)position.mData[1], (Real)position.mData[2], 1.0f),
            Vector2((Real)uv0.mData[0], (Real)uv0.mData[1]),
            Vector3((Real)normal.mData[0], (Real)normal.mData[1], (Real)normal.mData[2]),
            Vector3((Real)tangent.mData[0], (Real)tangent.mData[1], (Real)tangent.mData[2]),
            Vector3((Real)binormal.mData[0], (Real)binormal.mData[1], (Real)binormal.mData[2]),
            Vector4((Real)color.mRed, (Real)color.mGreen, (Real)color.mBlue, (Real)color.mAlpha)
            ));
          ++vertexCounter;
        }

        resource->AddTriangle(indices[0], indices[1], indices[2]);
      }
    }
  }

  int childCount = node->GetChildCount();
  for (int i = 0; i < childCount; ++i)
    LoadMesh(scene, node->GetChild(i), resource, scale);
}

}
