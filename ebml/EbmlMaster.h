/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2010 Steve Lhomme.  All rights reserved.
**
** This file is part of libebml.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** See http://www.gnu.org/licenses/lgpl-2.1.html for LGPL licensing information.
**
** Contact license@matroska.org if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

/*!
  \file
  \version \$Id: EbmlMaster.h 1232 2005-10-15 15:56:52Z robux4 $
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_MASTER_H
#define LIBEBML_MASTER_H

#include <string>
#include <vector>

#include "EbmlTypes.h"
#include "EbmlElement.h"
#include "EbmlCrc32.h"

#define EBML_MASTER_ITERATOR  std::vector<EbmlElement *>::iterator
#define EBML_MASTER_CONST_ITERATOR  std::vector<EbmlElement *>::const_iterator
#define EBML_MASTER_RITERATOR std::vector<EbmlElement *>::reverse_iterator
#define EBML_MASTER_CONST_RITERATOR std::vector<EbmlElement *>::const_reverse_iterator

namespace libebml {

const bool bChecksumUsedByDefault = false;

/*!
    \class EbmlMaster
    \brief Handle all operations on an EBML element that contains other EBML elements
*/
class EBML_DLL_API EbmlMaster : public EbmlElement {
  public:
    explicit EbmlMaster(const EbmlSemanticContext & aContext, bool bSizeIsKnown = true);
    EbmlMaster(const EbmlMaster & ElementToClone);
    EbmlMaster& operator=(const EbmlMaster&) = delete;
    bool ValidateSize() const override {return true;}
    /*!
      \warning be carefull to clear the memory allocated in the ElementList elsewhere
    */
    ~EbmlMaster() override;

    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    /*!
      \brief Set wether the size is finite (size is known in advance when writing, or infinite size is not known on writing)
    */
    bool SetSizeInfinite(bool aIsInfinite = true) override {SetSizeIsFinite(!aIsInfinite); return true;}

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
    filepos_t WriteHead(IOCallback & output, int SizeLength, bool bWithDefault = false);

    void EnableChecksum(bool bIsEnabled = true) { bChecksumUsed = bIsEnabled; }
    bool HasChecksum() const {return bChecksumUsed;}
    bool VerifyChecksum() const;
    std::uint32_t GetCrc32() const {return Checksum.GetCrc32();}
    void ForceChecksum(std::uint32_t NewChecksum) {
      Checksum.ForceCrc32(NewChecksum);
      bChecksumUsed = true;
    }

    /*!
      \brief drill down all sub-elements, finding any missing elements
    */
    std::vector<std::string> FindAllMissingElements() const;

    private:
    std::vector<EbmlElement *> ElementList;

    const EbmlSemanticContext & Context;

    bool      bChecksumUsed;
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
