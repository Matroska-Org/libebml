// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_ELEMENT_H
#define LIBEBML_ELEMENT_H

#include "EbmlTypes.h"
#include "EbmlId.h"
#include "IOCallback.h"

namespace libebml {

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
    return N;
}

/*!
  \brief The size of the EBML-coded integer
  \param Value value to encode as EBML integer
  \param MinSizeLength minimum size length to encode the value (0 for shortest size)
  \param bSizeIsFinite whether the size will be written as an infinite EBML size or not
*/
int EBML_DLL_API CodedSizeLength(std::uint64_t Value, unsigned int MinSizeLength, bool bSizeIsFinite = true);

/*!
  \brief The coded value of the EBML-coded integer
  \param Value value to encode as EBML integer
  \param CodedSize amount of octets to use to write the integer
  \param OutBuffer buffer to write the EBML-coded integer
  \note The size of OutBuffer must be at least CodedSize octets big
*/
int EBML_DLL_API CodedValueLength(std::uint64_t Value, int CodedSize, binary * OutBuffer);

/*!
  \brief Read an EBML-coded value from a buffer
  \return the value read
*/
std::uint64_t EBML_DLL_API ReadCodedSizeValue(const binary * InBuffer, std::uint32_t & BufferSize, std::uint64_t & SizeUnknown);

class EbmlStream;
class EbmlSemanticContext;
class EbmlElement;

extern const EbmlSemanticContext Context_EbmlGlobal;

#define DEFINE_xxx_CONTEXT(x,global) \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, nullptr, global, nullptr); \

#define DEFINE_xxx_MASTER(x,id,idl,parent,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \
    x::x() :EbmlMaster(Context_##x) {}

#define DEFINE_xxx_MASTER_CONS(x,id,idl,parent,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x);

#define DEFINE_xxx_MASTER_ORPHAN(x,id,idl,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, nullptr, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \

#define DEFINE_xxx_CLASS(x,id,idl,parent,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \
    x::x() {}

#define DEFINE_xxx_CLASS_CONS(x,id,idl,parent,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x);

#define DEFINE_xxx_UINTEGER_DEF(x,id,idl,parent,name,global,defval) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \
    x::x() :EbmlUInteger(defval) {}

#define DEFINE_xxx_SINTEGER_DEF(x,id,idl,parent,name,global,defval) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \
    x::x() :EbmlSInteger(defval) {}

#define DEFINE_xxx_STRING_DEF(x,id,idl,parent,name,global,defval) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \
    x::x() :EbmlString(defval) {}

#define DEFINE_xxx_FLOAT_DEF(x,id,idl,parent,name,global,defval) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x)); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \
    x::x() :EbmlFloat(defval) {}

#define DEFINE_xxx_CLASS_GLOBAL(x,id,idl,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_EbmlGlobal); \

#define DEFINE_xxx_CLASS_ORPHAN(x,id,idl,name,global) \
    const EbmlId Id_##x    (id, idl); \
    const EbmlSemanticContext Context_##x = EbmlSemanticContext(0, nullptr, nullptr, global, nullptr); \
    const EbmlCallbacks x::ClassInfos(x::Create, Id_##x, name, Context_##x); \

#define DEFINE_EBML_CONTEXT(x)                             DEFINE_xxx_CONTEXT(x,GetEbmlGlobal_Context)
#define DEFINE_EBML_MASTER(x,id,idl,parent,name)           DEFINE_xxx_MASTER(x,id,idl,parent,name,GetEbmlGlobal_Context)
#define DEFINE_EBML_MASTER_ORPHAN(x,id,idl,name)           DEFINE_xxx_MASTER_ORPHAN(x,id,idl,name,GetEbmlGlobal_Context)
#define DEFINE_EBML_CLASS(x,id,idl,parent,name)            DEFINE_xxx_CLASS(x,id,idl,parent,name,GetEbmlGlobal_Context)
#define DEFINE_EBML_CLASS_GLOBAL(x,id,idl,name)            DEFINE_xxx_CLASS_GLOBAL(x,id,idl,name,GetEbmlGlobal_Context)
#define DEFINE_EBML_CLASS_ORPHAN(x,id,idl,name)            DEFINE_xxx_CLASS_ORPHAN(x,id,idl,name,GetEbmlGlobal_Context)
#define DEFINE_EBML_UINTEGER_DEF(x,id,idl,parent,name,val) DEFINE_xxx_UINTEGER_DEF(x,id,idl,parent,name,GetEbmlGlobal_Context,val)
#define DEFINE_EBML_STRING_DEF(x,id,idl,parent,name,val)   DEFINE_xxx_STRING_DEF(x,id,idl,parent,name,GetEbmlGlobal_Context,val)
#define DEFINE_EBML_BINARY_CONS(x,id,idl,parent,name)      DEFINE_xxx_CLASS_CONS(x,id,idl,parent,name,GetEbmlGlobal_Context)

#define DEFINE_SEMANTIC_CONTEXT(x)
#define DEFINE_START_SEMANTIC(x)     static const EbmlSemantic ContextList_##x[] = {
#define DEFINE_END_SEMANTIC(x)       };
#define DEFINE_SEMANTIC_ITEM(m,u,c)  EbmlSemantic(m, u, EBML_INFO(c)),

#define DECLARE_EBML_MASTER(x)  class EBML_DLL_API x : public EbmlMaster { \
  public: \
    x();
#define DECLARE_EBML_UINTEGER(x)  class EBML_DLL_API x : public EbmlUInteger { \
  public: \
    x();
#define DECLARE_EBML_SINTEGER(x)  class EBML_DLL_API x : public EbmlSInteger { \
  public: \
    x();
#define DECLARE_EBML_STRING(x)    class EBML_DLL_API x : public EbmlString { \
  public: \
    x();
#define DECLARE_EBML_BINARY(x)    class EBML_DLL_API x : public EbmlBinary { \
  public: \
    x();
#define DECLARE_EBML_DATE(x)  class EBML_DLL_API x : public EbmlDate { \
  public: \
    x();
#define DECLARE_EBML_FLOAT(x)  class EBML_DLL_API x : public EbmlFloat { \
  public: \
    x();

#define EBML_CONCRETE_CLASS(Type) \
    public: \
        const EbmlSemanticContext &Context() const override {return ClassInfos.GetContext();} \
        const char *DebugName() const override {return ClassInfos.GetName();} \
        EbmlId const &GetClassId() const override {return ClassInfos.ClassId();} \
        EbmlElement & CreateElement() const override {return Create();} \
        EbmlElement * Clone() const override { return new Type(*this); } \
    static EbmlElement & Create() {return *(new Type);} \
        static constexpr const EbmlCallbacks & ClassInfo() {return ClassInfos;} \
        static const EbmlId & ClassId() {return ClassInfos.ClassId();} \
    private: \
    static const EbmlCallbacks ClassInfos; \

#define EBML_INFO(ref)             ref::ClassInfo()
#define EBML_ID(ref)               ref::ClassId()
#define EBML_CLASS_SEMCONTEXT(ref) Context_##ref
#define EBML_CLASS_CONTEXT(ref)    ref::ClassInfo().GetContext()
#define EBML_CONTEXT(e) (e)->Context()
#define EBML_NAME(e)    (e)->DebugName()

#define EBML_INFO_ID(cb)      (cb).ClassId()
#define EBML_INFO_NAME(cb)    (cb).GetName()
#define EBML_INFO_CREATE(cb)  (cb).NewElement()

#define EBML_SEM_CONTEXT(s) ((const EbmlCallbacks &)(s)).GetContext()
#define EBML_SEM_CREATE(s)  (s).Create()

#define EBML_CTX_SIZE(c)       (c).GetSize()
#define EBML_CTX_MASTER(c)     (c).GetMaster()
#define EBML_CTX_PARENT(c)     (c).Parent()
#define EBML_CTX_IDX(c,i)      (c).GetSemantic(i)
#define EBML_CTX_IDX_INFO(c,i) (const EbmlCallbacks &)((c).GetSemantic(i))
#define EBML_CTX_IDX_ID(c,i)   ((const EbmlCallbacks &)((c).GetSemantic(i))).ClassId()

#if !defined(INVALID_FILEPOS_T)
#define INVALID_FILEPOS_T 0
#endif

// functions for generic handling of data (should be static to all classes)
/*!
  \todo Handle default value
*/
class EBML_DLL_API EbmlCallbacks {
  public:
    EbmlCallbacks(EbmlElement & (*Creator)(), const EbmlId & aGlobalId, const char * aDebugName, const EbmlSemanticContext & aContext);

        inline const EbmlId & ClassId() const { return GlobalId; }
        inline const EbmlSemanticContext & GetContext() const { return Context; }
        inline const char * GetName() const { return DebugName; }
        inline EbmlElement & NewElement() const { return Create(); }

    private:
    EbmlElement & (* const Create)();
    const EbmlId & GlobalId;
    const char * DebugName;
    const EbmlSemanticContext & Context;
};

/*!
  \brief contains the semantic informations for a given level and all sublevels
  \todo move the ID in the element class
*/
class EBML_DLL_API EbmlSemantic {
  public:
    constexpr EbmlSemantic(bool aMandatory, bool aUnique, const EbmlCallbacks & aCallbacks)
      :Mandatory(aMandatory), Unique(aUnique), Callbacks(aCallbacks) {}

        inline bool IsMandatory() const { return Mandatory; }
        inline bool IsUnique() const { return Unique; }
        inline EbmlElement & Create() const { return EBML_INFO_CREATE(Callbacks); }
        inline explicit operator const EbmlCallbacks &() const { return Callbacks; }
        inline EbmlCallbacks const &GetCallbacks() const { return Callbacks; }

    private:
    const bool Mandatory; ///< whether the element is mandatory in the context or not
    const bool Unique;
    const EbmlCallbacks & Callbacks;
};

using _GetSemanticContext = const class EbmlSemanticContext &(*)();

/*!
  Context of the element
  \todo allow more than one parent ?
*/
class EBML_DLL_API EbmlSemanticContext {
  public:
    EbmlSemanticContext(std::size_t aSize,
      const EbmlSemantic *aMyTable,
      const EbmlSemanticContext *aUpTable,
      const _GetSemanticContext aGetGlobalContext,
      const EbmlCallbacks *aMasterElt)
      : GetGlobalContext(aGetGlobalContext), MyTable(aMyTable), Size(aSize),
        UpTable(aUpTable), MasterElt(aMasterElt) {}

    bool operator!=(const EbmlSemanticContext & aElt) const {
      return ((Size != aElt.Size) || (MyTable != aElt.MyTable) ||
        (UpTable != aElt.UpTable) || (GetGlobalContext != aElt.GetGlobalContext) |
        (MasterElt != aElt.MasterElt));
    }

        inline std::size_t GetSize() const { return Size; }
        inline const EbmlCallbacks* GetMaster() const { return MasterElt; }
        inline const EbmlSemanticContext* Parent() const { return UpTable; }
        const EbmlSemantic & GetSemantic(std::size_t i) const;

    const _GetSemanticContext GetGlobalContext; ///< global elements supported at this level

    private:
        const EbmlSemantic *MyTable; ///< First element in the table
    std::size_t Size;          ///< number of elements in the table
    const EbmlSemanticContext *UpTable; ///< Parent element
    /// \todo replace with the global context directly
    const EbmlCallbacks *MasterElt;
};

/*!
  \class EbmlElement
  \brief Hold basic informations about an EBML element (ID + length)
*/
class EBML_DLL_API EbmlElement {
  public:
    explicit EbmlElement(std::uint64_t aDefaultSize, bool bValueSet = false);
    virtual ~EbmlElement() = default;
    EbmlElement& operator=(const EbmlElement&) = delete;

    /// Set the minimum length that will be used to write the element size (-1 = optimal)
    void SetSizeLength(int NewSizeLength) {SizeLength = NewSizeLength;}
    int GetSizeLength() const {return SizeLength;}

    static EbmlElement * FindNextElement(IOCallback & DataStream, const EbmlSemanticContext & Context, int & UpperLevel, std::uint64_t MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel = 1);
    static EbmlElement * FindNextID(IOCallback & DataStream, const EbmlCallbacks & ClassInfos, std::uint64_t MaxDataSize);

    /*!
      \brief find the next element with the same ID
    */
    EbmlElement * FindNext(IOCallback & DataStream, std::uint64_t MaxDataSize);

    EbmlElement * SkipData(EbmlStream & DataStream, const EbmlSemanticContext & Context, EbmlElement * TestReadElt = nullptr, bool AllowDummyElt = false);

    /*!
      \brief Give a copy of the element, all data inside the element is copied
      \return nullptr if there is not enough memory
    */
    virtual EbmlElement * Clone() const = 0;

    virtual EbmlId const &GetClassId() const = 0;
    virtual explicit operator const EbmlId &() const { return GetClassId(); }
        virtual const char *DebugName() const = 0;
        virtual const EbmlSemanticContext &Context() const = 0;
        virtual EbmlElement & CreateElement() const = 0;

    // by default only allow to set element as finite (override when needed)
    virtual bool SetSizeInfinite(bool bIsInfinite = true) {return !bIsInfinite;}

    virtual bool ValidateSize() const = 0;

    std::uint64_t GetElementPosition() const {
      return ElementPosition;
    }

    std::uint64_t ElementSize(bool bWithDefault = false) const; /// return the size of the header+data, before writing

    filepos_t Render(IOCallback & output, bool bWithDefault = false, bool bKeepPosition = false, bool bForceRender = false);

    virtual filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) = 0; /// update the Size of the Data stored
    virtual filepos_t GetSize() const {return Size;} /// return the size of the data stored in the element, on reading

    virtual filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) = 0;
    virtual void Read(EbmlStream & inDataStream, const EbmlSemanticContext & Context, int & UpperEltFound, EbmlElement * & FoundElt, bool AllowDummyElt = false, ScopeMode ReadFully = SCOPE_ALL_DATA);

    /*!
      \brief default comparison for elements that can't be compared
    */
    virtual bool IsSmallerThan(const EbmlElement *Cmp) const;
    static bool CompareElements(const EbmlElement *A, const EbmlElement *B);

    virtual bool IsDummy() const {return false;}
    virtual bool IsMaster() const {return false;}

    std::size_t HeadSize() const {
      return EBML_ID_LENGTH((const EbmlId&)*this) + CodedSizeLength(Size, SizeLength, bSizeIsFinite);
    } /// return the size of the head, on reading/writing

    /*!
      \brief Force the size of an element
      \warning only possible if the size is "undefined"
    */
    bool ForceSize(std::uint64_t NewSize);

    filepos_t OverwriteHead(IOCallback & output, bool bKeepPosition = false);
    filepos_t OverwriteData(IOCallback & output, bool bKeepPosition = false);

    /*!
      \brief void the content of the element (replace by EbmlVoid)
    */
    std::uint64_t VoidMe(IOCallback & output, bool bWithDefault = false) const;

    bool DefaultISset() const {return DefaultIsSet;}
    void ForceNoDefault() {SetDefaultIsSet(false);}
    virtual bool IsDefaultValue() const = 0;
    bool IsFiniteSize() const {return bSizeIsFinite;}

    /*!
      \brief set the default size of an element
    */
    virtual void SetDefaultSize(std::uint64_t aDefaultSize) {DefaultSize = aDefaultSize;}

    bool ValueIsSet() const {return bValueIsSet;}

    inline std::uint64_t GetEndPosition() const {
      assert(bSizeIsFinite); // we don't know where the end is
      return SizePosition + CodedSizeLength(Size, SizeLength, bSizeIsFinite) + Size;
    }

  protected:
    /*!
      \brief find any element in the stream
      \return a DummyRawElement if the element is unknown or nullptr if the element dummy is not allowed
    */
    static EbmlElement *CreateElementUsingContext(const EbmlId & aID, const EbmlSemanticContext & Context, int & LowLevel, bool IsGlobalContext, bool bAllowDummy = false, unsigned int MaxLowerLevel = 1);

    filepos_t RenderHead(IOCallback & output, bool bForceRender, bool bWithDefault = false, bool bKeepPosition = false);
    filepos_t MakeRenderHead(IOCallback & output, bool bKeepPosition);

    /*!
      \brief prepare the data before writing them (in case it's not already done by default)
    */
    virtual filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) = 0;

    /*!
      \brief special constructor for cloning
    */
    EbmlElement(const EbmlElement & ElementToClone) = default;

        inline std::uint64_t GetDefaultSize() const {return DefaultSize;}
        inline void SetSize_(std::uint64_t aSize) {Size = aSize;}
        inline void SetValueIsSet(bool Set = true) {bValueIsSet = Set;}
        inline void SetDefaultIsSet(bool Set = true) {DefaultIsSet = Set;}
        inline void SetSizeIsFinite(bool Set = true) {bSizeIsFinite = Set;}
        inline std::uint64_t GetSizePosition() const {return SizePosition;}

  private:
    std::uint64_t Size;        ///< the size of the data to write
    std::uint64_t DefaultSize; ///< Minimum data size to fill on rendering (0 = optimal)
    int SizeLength{0}; /// the minimum size on which the size will be written (0 = optimal)
    bool bSizeIsFinite{true};
    std::uint64_t ElementPosition{0};
    std::uint64_t SizePosition{0};
    bool bValueIsSet;
    bool DefaultIsSet{false};
};

} // namespace libebml

#endif // LIBEBML_ELEMENT_H
