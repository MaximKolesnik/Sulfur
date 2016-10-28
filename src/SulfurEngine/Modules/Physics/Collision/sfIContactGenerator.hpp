#pragma once

namespace Sulfur
{
  class QueryResults;

  namespace Physics
  {
    struct Contact
    {

    };

    typedef std::vector<Contact> Contacts;

    class IContactGenerator
    {
    public:
      IContactGenerator(void) {}
      virtual ~IContactGenerator(void) {}

      virtual void GenerateContacts(Contacts &contacts,
                                    const QueryResults &possiblePairs) = 0;
    private:
    };
  }
}