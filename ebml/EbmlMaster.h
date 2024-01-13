// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_MASTER_H
#define LIBEBML_MASTER_H

#include <vector>

#include "EbmlTypes.h"
#include "EbmlElement.h"
#include "EbmlCrc32.h"

#define EBML_MASTER_ITERATOR  std::vector<EbmlElement *>::iterator
#define EBML_MASTER_CONST_ITERATOR  std::vector<EbmlElement *>::const_iterator
#define EBML_MASTER_RITERATOR std::vector<EbmlElement *>::reverse_iterator
#define EBML_MASTER_CONST_RITERATOR std::vector<EbmlElement *>::const_reverse_iterator

namespace libebml {

constexpr const bool bChecksumUsedByDefault = false;

/*!
    \class EbmlMaster
    \brief Handle all operations on an EBML element that contains other EBML elements
*/
class EBML_DLL_API EbmlMaster : public EbmlElement {
  public:
    explicit EbmlMaster(const EbmlCallbacks &, const EbmlSemanticContext & aContext, bool bSizeIsKnown = true);
    EbmlMaster(const EbmlMaster & ElementToClone);
    EbmlMaster& operator=(const EbmlMaster&) = delete;
    bool ValidateSize() const override {return true;}
    /*!
      \warning be carefull to clear the memory allocated in the ElementList elsewhere
    */
    ~EbmlMaster() override;

    filepos_t RenderData(IOCallback & output, bool bForceRender, ShouldWrite writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully) override;
    filepos_t UpdateSize(ShouldWrite writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    bool PushElement(EbmlElement & element);
    std::uint64_t GetSize() const override {
      if (IsFiniteSize())
        return EbmlElement::GetSize();
      return (0-1);
    }

    std::uint64_t GetDataStart() const {
      return GetElementPosition() + EBML_ID_LENGTH((const EbmlId&)*this) + CodedSizeLength(EbmlElement::GetSize(), GetSizeLength(), IsFiniteSize());
    }

    /*!
      \brief find the element corresponding to the ID of the element, NULL if not found
    */
    EbmlElement *FindElt(const EbmlCallbacks & Callbacks) const;
    /*!
      \brief find the first element corresponding to the ID of the element
    */
    EbmlElement *FindFirstElt(const EbmlCallbacks & Callbacks, bool bCreateIfNull);
    EbmlElement *FindFirstElt(const EbmlCallbacks & Callbacks) const;

    /*!
      \brief find the element of the same type of PasElt following in the list of elements
    */
    EbmlElement *FindNextElt(const EbmlElement & PastElt, bool bCreateIfNull);
    EbmlElement *FindNextElt(const EbmlElement & PastElt) const;
    EbmlElement *AddNewElt(const EbmlCallbacks & Callbacks);

    /*!
      \brief add an element at a specified location
    */
    bool InsertElement(EbmlElement & element, std::size_t position = 0);
    bool InsertElement(EbmlElement & element, const EbmlElement & before);

    /*!
      \brief Read the data and keep the known children
    */
    void Read(EbmlStream & inDataStream, const EbmlSemanticContext & Context, int & UpperEltFound, EbmlElement * & FoundElt, bool AllowDummyElt, ScopeMode ReadFully = SCOPE_ALL_DATA) override;

    /*!
      \brief sort Data when they can
    */
    void Sort();

    std::size_t ListSize() const {return ElementList.size();}
    std::vector<EbmlElement *> const &GetElementList() const {return ElementList;}
    std::vector<EbmlElement *> &GetElementList() {return ElementList;}

        inline EBML_MASTER_ITERATOR begin() {return ElementList.begin();}
        inline EBML_MASTER_ITERATOR end() {return ElementList.end();}
        inline EBML_MASTER_RITERATOR rbegin() {return ElementList.rbegin();}
        inline EBML_MASTER_RITERATOR rend() {return ElementList.rend();}
        inline EBML_MASTER_CONST_ITERATOR begin() const {return ElementList.begin();}
        inline EBML_MASTER_CONST_ITERATOR end() const {return ElementList.end();}
        inline EBML_MASTER_CONST_RITERATOR rbegin() const {return ElementList.rbegin();}
        inline EBML_MASTER_CONST_RITERATOR rend() const {return ElementList.rend();}

    EbmlElement * operator[](unsigned int position) {return ElementList[position];}
    const EbmlElement * operator[](unsigned int position) const {return ElementList[position];}

    bool IsDefaultValue() const override {
      return (ElementList.empty());
    }
    bool IsMaster() const override {return true;}

    /*!
      \brief verify that all mandatory elements are present
      \note usefull after reading or before writing
    */
    bool CheckMandatory() const;

    /*!
      \brief Remove an element from the list of the master
    */
    void Remove(std::size_t Index);
    void Remove(EBML_MASTER_ITERATOR & Itr);
    void Remove(EBML_MASTER_RITERATOR & Itr);

    /*!
      \brief remove all elements, even the mandatory ones
    */
    void RemoveAll() {ElementList.clear();}

    /*!
      \brief facility for Master elements to write only the head and force the size later
    */
    filepos_t WriteHead(IOCallback & output, int SizeLength, ShouldWrite writeFilter = WriteSkipDefault);

    void EnableChecksum(bool bIsEnabled = true) { bChecksumUsed = bIsEnabled; }
    bool HasChecksum() const {return bChecksumUsed;}
    bool VerifyChecksum() const;
    std::uint32_t GetCrc32() const {return Checksum.GetCrc32();}
    void ForceChecksum(std::uint32_t NewChecksum) {
      Checksum.ForceCrc32(NewChecksum);
      bChecksumUsed = true;
    }

    private:
    std::vector<EbmlElement *> ElementList;

    const EbmlSemanticContext & MasterContext;

    bool      bChecksumUsed = bChecksumUsedByDefault;
    EbmlCrc32 Checksum;

  private:
    /*!
      \brief Add all the mandatory elements to the list
    */
    bool ProcessMandatory();
};

///< \todo add a restriction to only elements legal in the context
template <typename Type>
Type & GetChild(EbmlMaster & Master)
{
  return *(static_cast<Type *>(Master.FindFirstElt(EBML_INFO(Type), true)));
}
// call with
// MyDocType = GetChild<EDocType>(TestHead);

template <typename Type>
Type * FindChild(EbmlMaster & Master)
{
  return static_cast<Type *>(Master.FindFirstElt(EBML_INFO(Type), false));
}

template <typename Type>
Type & GetNextChild(EbmlMaster & Master, const Type & PastElt)
{
  return *(static_cast<Type *>(Master.FindNextElt(PastElt, true)));
}

template <typename Type>
Type * FindNextChild(EbmlMaster & Master, const Type & PastElt)
{
  return static_cast<Type *>(Master.FindNextElt(PastElt, false));
}

template <typename Type>
Type & AddNewChild(EbmlMaster & Master)
{
  return *(static_cast<Type *>(Master.AddNewElt(EBML_INFO(Type))));
}

} // namespace libebml

#endif // LIBEBML_MASTER_H
