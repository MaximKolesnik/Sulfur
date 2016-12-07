/******************************************************************************/
/*!
\par     Sulfur
\file    sfDebug.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfDebug.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Components\sfRigidBody.hpp"

namespace Sulfur
{
  namespace Physics
  {
    static void DrawSphere(const ColliderData *data)
    {
      Matrix4 m;
      Vector3 translation;
      Quaternion orientation;
      Real radius;
      Transform *trans = SF_GET_COMP_TYPE(Transform, data->m_transformHndl);

      SF_ASSERT(trans, "Transform is not set");
      
      if (data->m_rbData)
      {
        translation = data->m_rbData->m_position + data->m_offset;
        radius = trans->GetScale().MaxAxisValue() * data->m_radius;
        orientation = data->m_rbData->m_orientation;
      }
      else
      {
        translation = trans->GetTranslation() + data->m_offset;
        radius = trans->GetScale().MaxAxisValue() * data->m_radius;
        orientation = trans->GetRotation();
      }
      m.SetTransformation(orientation, Vector3(Real(1.0), Real(1.0), Real(1.0)),
        translation);

      DebugDraw::Instance()->DrawSphere(m, radius);
    }

    static void DrawBox(const ColliderData *data)
    {
      Matrix4 m;
      Vector3 translation;
      Quaternion orientation;
      Vector3 scale;

      Transform *trans = SF_GET_COMP_TYPE(Transform, data->m_transformHndl);

      SF_ASSERT(trans, "Transform is not set");

      if (data->m_rbData)
      {
        translation = data->m_rbData->m_position + data->m_offset;
        scale = trans->GetScale() * data->m_scale;
        orientation = data->m_rbData->m_orientation;
      }
      else
      {
        translation = trans->GetTranslation() + data->m_offset;
        scale = trans->GetScale() * data->m_scale;
        orientation = trans->GetRotation();
      }
      m.SetTransformation(orientation, scale,
        translation);

      DebugDraw::Instance()->DrawBox(m);
    }

    static void DrawCapsule(const ColliderData *data)
    {
      Matrix4 m;
      Vector3 translation;
      Quaternion orientation;
      Real radius;
      Real centerLine;

      Transform *trans = SF_GET_COMP_TYPE(Transform, data->m_transformHndl);

      SF_ASSERT(trans, "Transform is not set");

      if (data->m_rbData)
      {
        translation = data->m_rbData->m_position + data->m_offset;
        radius = trans->GetScale().MaxAxisValue() * data->m_radius;
        centerLine = trans->GetScale().MaxAxisValue() * data->m_lineLength;
        orientation = data->m_rbData->m_orientation;
      }
      else
      {
        translation = trans->GetTranslation() + data->m_offset;
        radius = trans->GetScale().MaxAxisValue() * data->m_radius;
        centerLine = trans->GetScale().MaxAxisValue() * data->m_lineLength;
        orientation = trans->GetRotation();
      }

      Vector3 leftSpherePos(-centerLine / 2, Real(0.0), Real(0.0));
      Vector3 rightSpherePos(centerLine / 2, Real(0.0), Real(0.0));
      Matrix4 helper;
      m.SetTransformation(Quaternion(), Vector3(Real(1.0), Real(1.0), Real(1.0)),
        leftSpherePos);
      helper.SetRotation(orientation);
      m = helper * m;
      helper.SetTranslation(translation[0], translation[1], translation[2]);
      m = helper * m;
      
      DebugDraw::Instance()->DrawSphere(m, radius, true, Vector4(0, 0, 1, 1));
      m.SetTransformation(Quaternion(), Vector3(Real(1.0), Real(1.0), Real(1.0)),
        rightSpherePos);
      helper.SetRotation(orientation);
      m = helper * m;
      helper.SetTranslation(translation[0], translation[1], translation[2]);
      m = helper * m;

      DebugDraw::Instance()->DrawSphere(m, radius, true, Vector4(0, 0, 1, 1));

      m.SetTransformation(orientation, Vector3(Real(1.0), Real(1.0), Real(1.0)), translation);

      Vector3 p1 = leftSpherePos + Vector3(Real(0.0), radius, Real(0.0));
      Vector3 p2 = rightSpherePos + Vector3(Real(0.0), radius, Real(0.0));

      for (float angle = 0; angle <= SF_2PI; angle += SF_PI / 6)
      {
        Quaternion rot;
        Vector3 temp1, temp2;
        rot.SetEuler(angle, Real(0.0), Real(0.0));

        temp1 = rot.Rotated(p1);
        temp2 = rot.Rotated(p2);
        temp1 = orientation.Rotated(temp1);
        temp2 = orientation.Rotated(temp2);
        DebugDraw::Instance()->DrawLine(temp1 + translation, temp2 + translation, 
          true, Vector4(0, 0, 1, 1));
      }
    }

    void DrawColliders(const PhysicsWorld::ColliderList &colliderList)
    {
      for (auto &it : colliderList)
      {
        ColliderData *data = it.second;

        switch (data->m_type)
        {
        case CT_SPHERE:
          DrawSphere(data);
          break;
        case CT_BOX:
          DrawBox(data);
          break;
        case CT_CAPSULE:
          DrawCapsule(data);
          break;
        default:
          SF_CRITICAL_ERR("Unrecognized collider type");
        }
      }
    }
  }
}