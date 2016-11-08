#include "sfIConstraintSolver.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void IConstraintSolver::AddContact(Contact *contact)
    {
      ContactKey key(contact->m_colliderA, contact->m_colliderB);

      auto it = m_contacts.find(key);

      if (it != m_contacts.end())
      {
        //Update
      }
      else
      {
        m_contacts[key] = *contact;
      }
    }

    void IConstraintSolver::_UpdateEntry(Contact &contact)
    {
      
    }
  }
}