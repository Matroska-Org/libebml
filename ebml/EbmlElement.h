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

#include <cassert>
#include <functional>
#include <string>
#include <limits>

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

#define DEFINE_xxx_CONTEXT(x,global) \
    const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, nullptr, global, nullptr); \

#define DEFINE_xxx_MASTER(x,id,parent,infinite,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, &Context_##parent, global, &EBML_INFO(x)); \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, infinite, false, name, Context_##x, versions); \
    x::x() :libebml::EbmlMaster(x::ClassInfos) {}

// define a master class with a custom constructor
#define DEFINE_xxx_MASTER_CONS(x,id,parent,infinite,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, &Context_##parent, global, &EBML_INFO(x)); \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, infinite, false, name, Context_##x, versions);

// define a master class with no parent class
#define DEFINE_xxx_MASTER_ORPHAN(x,id,infinite,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(countof(ContextList_##x), ContextList_##x, nullptr, global, &EBML_INFO(x)); \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, infinite, false, name, Context_##x, versions); \

#define DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(0, nullptr, &Context_##parent, global, &EBML_INFO(x));

#define DEFINE_xxx_CLASS_BASE(x,BaseClass,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, false, false, name, Context_##x, versions); \
    x::x() :libebml::BaseClass(x::ClassInfos) {}

#define DEFINE_xxx_CLASS_BASE_DEFAULT(x,BaseClass,id,parent,name,global,StorageType,defval,versions) \
    DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    constexpr const libebml::EbmlCallbacksWithDefault<StorageType> x::ClassInfos(x::Create, Id_##x, defval, name, Context_##x, versions); \
    x::x() :libebml::BaseClass(x::ClassInfos) {}

#define DEFINE_xxx_CLASS_BASE_NODEFAULT(x,BaseClass,id,parent,name,global,StorageType,versions) \
    DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    constexpr const libebml::EbmlCallbacksDefault<StorageType> x::ClassInfos(x::Create, Id_##x, name, Context_##x, versions); \
    x::x() :libebml::BaseClass(x::ClassInfos) {}

#define DEFINE_xxx_UINTEGER(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE_NODEFAULT(x,EbmlUInteger,id,parent,name,global,std::uint64_t,versions)

#define DEFINE_xxx_SINTEGER(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE_NODEFAULT(x,EbmlSInteger,id,parent,name,global,std::int64_t,versions)

#define DEFINE_xxx_STRING(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE_NODEFAULT(x,EbmlString,id,parent,name,global,const char *,versions)

#define DEFINE_xxx_UNISTRING(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE_NODEFAULT(x,EbmlUnicodeString,id,parent,name,global, const wchar_t *,versions)

#define DEFINE_xxx_FLOAT(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE_NODEFAULT(x,EbmlFloat,id,parent,name,global,double,versions)

#define DEFINE_xxx_DATE(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE_NODEFAULT(x,EbmlDate,id,parent,name,global,std::int64_t,versions)

#define DEFINE_xxx_BINARY(x,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_BASE(x,EbmlBinary,id,parent,name,versions,global)

#define DEFINE_xxx_UINTEGER_DEF(x,id,parent,name,versions,global,defval) \
    DEFINE_xxx_CLASS_BASE_DEFAULT(x,EbmlUInteger,id,parent,name,global,std::uint64_t,defval, versions)

#define DEFINE_xxx_SINTEGER_DEF(x,id,parent,name,versions,global,defval) \
    DEFINE_xxx_CLASS_BASE_DEFAULT(x,EbmlSInteger,id,parent,name,global,std::int64_t,defval, versions)

#define DEFINE_xxx_STRING_DEF(x,id,parent,name,versions,global,defval) \
    DEFINE_xxx_CLASS_BASE_DEFAULT(x,EbmlString,id,parent,name,global,const char *,defval, versions)

#define DEFINE_xxx_UNISTRING_DEF(x,id,parent,name,versions,global,defval) \
    DEFINE_xxx_CLASS_BASE_DEFAULT(x,EbmlUnicodeString,id,parent,name,global,const wchar_t*,defval, versions)

#define DEFINE_xxx_FLOAT_DEF(x,id,parent,name,versions,global,defval) \
    DEFINE_xxx_CLASS_BASE_DEFAULT(x,EbmlFloat,id,parent,name,global,double,defval, versions)

#define DEFINE_xxx_DATE_DEF(x,id,parent,name,versions,global,defval) \
    DEFINE_xxx_CLASS_BASE_DEFAULT(x,EbmlDate,id,parent,name,versions,global,defval, versions)

#define DEFINE_xxx_CLASS_ORPHAN(x,id,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(0, nullptr, nullptr, global, nullptr); \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, false, false, name, Context_##x, versions); \

#define DEFINE_EBML_CONTEXT(x)                             DEFINE_xxx_CONTEXT(x,GetEbmlGlobal_Context)

#define DEFINE_EBML_MASTER(x,id,parent,infinite,name,versions)  DEFINE_xxx_MASTER(x,id,parent,infinite,name,versions,GetEbmlGlobal_Context)
#define DEFINE_EBML_MASTER_ORPHAN(x,id,infinite,name,versions)  DEFINE_xxx_MASTER_ORPHAN(x,id,infinite,name,versions,GetEbmlGlobal_Context)
#define DEFINE_EBML_CLASS_ORPHAN(x,id,name,versions)            DEFINE_xxx_CLASS_ORPHAN(x,id,name,versions,GetEbmlGlobal_Context)
#define DEFINE_EBML_UINTEGER_DEF(x,id,parent,name,val,versions) DEFINE_xxx_UINTEGER_DEF(x,id,parent,name,versions,GetEbmlGlobal_Context,val)
#define DEFINE_EBML_STRING_DEF(x,id,parent,name,val,versions)   DEFINE_xxx_STRING_DEF(x,id,parent,name,versions,GetEbmlGlobal_Context,val)

#define DEFINE_SEMANTIC_CONTEXT(x)
#define DEFINE_START_SEMANTIC(x)     static const libebml::EbmlSemantic ContextList_##x[] = {
#define DEFINE_END_SEMANTIC(x)       };
#define DEFINE_SEMANTIC_ITEM(m,u,c)  libebml::EbmlSemantic(m, u, EBML_INFO(c)),
#define DEFINE_SEMANTIC_ITEM_UINT(m,u,d,c)      EbmlSemantic(m, u, static_cast<std::uint64_t>(d), EBML_INFO(c)),
#define DEFINE_SEMANTIC_ITEM_SINT(m,u,d,c)      EbmlSemantic(m, u, static_cast<std::int64_t>(d),  EBML_INFO(c)),
#define DEFINE_SEMANTIC_ITEM_DATE(m,u,d,c)      EbmlSemantic(m, u, static_cast<std::int64_t>(d,true), EBML_INFO(c)),
#define DEFINE_SEMANTIC_ITEM_FLOAT(m,u,d,c)     EbmlSemantic(m, u, static_cast<double>(d),        EBML_INFO(c)),
#define DEFINE_SEMANTIC_ITEM_STRING(m,u,d,c)    EbmlSemantic(m, u, static_cast<const char*>(d),    EBML_INFO(c)),
#define DEFINE_SEMANTIC_ITEM_UTF8(m,u,d,c)      EbmlSemantic(m, u, static_cast<const wchar_t*>(d), EBML_INFO(c)),

#define DECLARE_xxx_BASE_DEFAULT(x, DllApi, BaseClass, StorageType) \
class DllApi x : public BaseClass { \
  private: \
    static const libebml::EbmlCallbacksWithDefault<StorageType> ClassInfos; \
  public: \
    const libebml::EbmlCallbacksWithDefault<StorageType> & ElementSpec() const override { return ClassInfos; } \
    static constexpr const libebml::EbmlCallbacksWithDefault<StorageType> & GetElementSpec() { return ClassInfos; } \
    x();

#define DECLARE_xxx_BASE_NODEFAULT(x, DllApi, BaseClass, StorageType) \
class DllApi x : public BaseClass { \
  private: \
    static const libebml::EbmlCallbacksDefault<StorageType> ClassInfos; \
  public: \
    const libebml::EbmlCallbacksDefault<StorageType> & ElementSpec() const override { return ClassInfos; } \
    static constexpr const libebml::EbmlCallbacksDefault<StorageType> & GetElementSpec() { return ClassInfos; } \
    x();

#define DECLARE_xxx_BASE(x, DllApi, BaseClass) \
class DllApi x : public BaseClass { \
  private: \
    static const libebml::EbmlCallbacks ClassInfos; \
  public: \
    x();

#define DECLARE_xxx_MASTER(x,DllApi)    \
  DECLARE_xxx_BASE(x, DllApi, libebml::EbmlMaster)

#define DECLARE_xxx_BINARY(x,DllApi)    \
  DECLARE_xxx_BASE(x, DllApi, libebml::EbmlBinary)

#define DECLARE_xxx_BINARY_LENGTH(x,len,DllApi)    \
  DECLARE_xxx_BASE(x, DllApi, libebml::EbmlBinary) \
  bool SizeIsValid(std::uint64_t size) const override {return size == len;}

#define DECLARE_xxx_UINTEGER(x,DllApi)  \
  DECLARE_xxx_BASE_NODEFAULT(x, DllApi, libebml::EbmlUInteger, std::uint64_t)

#define DECLARE_xxx_SINTEGER(x,DllApi)  \
  DECLARE_xxx_BASE_NODEFAULT(x, DllApi, libebml::EbmlSInteger, std::int64_t)

#define DECLARE_xxx_STRING(x,DllApi)    \
  DECLARE_xxx_BASE_NODEFAULT(x, DllApi, libebml::EbmlString, const char *)

#define DECLARE_xxx_UNISTRING(x,DllApi) \
  DECLARE_xxx_BASE_NODEFAULT(x, DllApi, libebml::EbmlUnicodeString, const wchar_t *)

#define DECLARE_xxx_DATE(x,DllApi)      \
  DECLARE_xxx_BASE_NODEFAULT(x, DllApi, libebml::EbmlDate, std::int64_t)

#define DECLARE_xxx_FLOAT(x,DllApi)     \
  DECLARE_xxx_BASE_NODEFAULT(x, DllApi, libebml::EbmlFloat, double)

#define DECLARE_xxx_UINTEGER_DEF(x,DllApi)  \
  DECLARE_xxx_BASE_DEFAULT(x, DllApi, libebml::EbmlUInteger, std::uint64_t)

#define DECLARE_xxx_SINTEGER_DEF(x,DllApi)  \
  DECLARE_xxx_BASE_DEFAULT(x, DllApi, libebml::EbmlSInteger, std::int64_t)

#define DECLARE_xxx_STRING_DEF(x,DllApi)    \
  DECLARE_xxx_BASE_DEFAULT(x, DllApi, libebml::EbmlString, const char *)

#define DECLARE_xxx_UNISTRING_DEF(x,DllApi) \
  DECLARE_xxx_BASE_DEFAULT(x, DllApi, libebml::EbmlUnicodeString, const wchar_t *)

#define DECLARE_xxx_DATE_DEF(x,DllApi)      \
  DECLARE_xxx_BASE_DEFAULT(x, DllApi, libebml::EbmlDate, std::int64_t)

#define DECLARE_xxx_FLOAT_DEF(x,DllApi)     \
  DECLARE_xxx_BASE_DEFAULT(x, DllApi, libebml::EbmlFloat, double)

#define DECLARE_EBML_MASTER(x)    DECLARE_xxx_MASTER(  x,EBML_DLL_API)
#define DECLARE_EBML_UINTEGER_DEF(x)  DECLARE_xxx_UINTEGER_DEF(x,EBML_DLL_API)
#define DECLARE_EBML_STRING_DEF(x)    DECLARE_xxx_STRING_DEF(  x,EBML_DLL_API)
#define DECLARE_EBML_BINARY(x)    DECLARE_xxx_BINARY(  x,EBML_DLL_API)
#define DECLARE_EBML_BINARY_LENGTH(x,len)    DECLARE_xxx_BINARY_LENGTH(x,len,EBML_DLL_API)

#define EBML_CONCRETE_CLASS(Type) \
    public: \
        libebml::EbmlElement & CreateElement() const override {return Create();} \
        libebml::EbmlElement * Clone() const override { return new Type(*this); } \
    static libebml::EbmlElement & Create() {return *(new Type);} \
        static constexpr const libebml::EbmlCallbacks & ClassInfo() {return ClassInfos;} \
        static const libebml::EbmlId & ClassId() {return ClassInfos.ClassId();} \

#define EBML_INFO(ref)             ref::ClassInfo()
#define EBML_ID(ref)               ref::ClassId()
#define EBML_CLASS_SEMCONTEXT(ref) Context_##ref
#define EBML_CLASS_CONTEXT(ref)    ref::ClassInfo().GetContext()
#define EBML_CONTEXT(e) (e)->Context()
#define EBML_NAME(e)    (e)->DebugName()

#define EBML_INFO_ID(cb)      (cb).ClassId()
#define EBML_INFO_NAME(cb)    (cb).GetName()
#define EBML_INFO_CREATE(cb)  (cb).NewElement()

#define EBML_SEM_CONTEXT(s) ((const libebml::EbmlCallbacks &)(s)).GetContext()
#define EBML_SEM_CREATE(s)  (s).Create()

#define EBML_CTX_SIZE(c)       (c).GetSize()
#define EBML_CTX_MASTER(c)     (c).GetMaster()
#define EBML_CTX_PARENT(c)     (c).Parent()
#define EBML_CTX_IDX(c,i)      (c).GetSemantic(i)
#define EBML_CTX_IDX_INFO(c,i) (const libebml::EbmlCallbacks &)((c).GetSemantic(i))
#define EBML_CTX_IDX_ID(c,i)   ((const libebml::EbmlCallbacks &)((c).GetSemantic(i))).ClassId()

#if !defined(INVALID_FILEPOS_T)
#define INVALID_FILEPOS_T 0
#endif

class EBML_DLL_API EbmlDocVersion {
  public:
    using version_type = unsigned int;

    // EbmlDocVersion constructor
    //
    // \param min the minimum supported version, or ANY_VERSION to make the element never available
    // \param max the maximum supported version, 0 to make the element never available or ANY_VERSION to be supported on any known and unknown doctype version
    constexpr EbmlDocVersion(version_type min = 0, version_type max = ANY_VERSION)
      : minver(min)
      , maxver(max)
    {}

    // the element with this EbmlDocVersion should never be used if this methods return true
    bool IsAlwaysDeprecated() const {
      return minver == ANY_VERSION;
    }

    // return true if the element with this EbmlDocVersion can be used with the given doctype_version
    bool IsValidInVersion(version_type doctype_version) const {
      if (minver == ANY_VERSION)
        return false;
      if (doctype_version < minver)
        return false;
      return doctype_version <= maxver;
    }

    // the minimum DocType version the element with this EbmlDocVersion is allowed in,
    // ANY_VERSION if the element is never supported
    version_type GetMinVersion() const { return minver; }

    // the maximum DocType version the element with this EbmlDocVersion is allowed in,
    // ANY_VERSION if the element is supported in all (known) version
    version_type GetMaxVersion() const { return maxver; }

    /// @brief constant value to indicate the maximum version matches all versions or the minimum version matches no version
    static const version_type ANY_VERSION = std::numeric_limits<version_type>::max();

  private:
    // the minimum DocType version this element is allowed in
    // ANY_VERSION if the element is never supported
    const version_type minver;
    // the maximum DocType version this element is allowed in
    // ANY_VERSION if the element is supported in all (known) version
    const version_type maxver;
};

// functions for generic handling of data (should be static to all classes)
/*!
  \todo Handle default value
*/
class EBML_DLL_API EbmlCallbacks {
  public:
    constexpr EbmlCallbacks(EbmlElement & (*Creator)(), const EbmlId & aGlobalId, bool aCanInfinite, bool aHasDefault, const char * aDebugName, const EbmlSemanticContext & aContext,
                            const EbmlDocVersion & aVersion)
      :Create(Creator)
      ,GlobalId(aGlobalId)
      ,CanInfinite(aCanInfinite)
      ,hasDefault(aHasDefault)
      ,Version(aVersion)
      ,DebugName(aDebugName)
      ,Context(aContext)
    {
      assert(Creator!=nullptr);
    }

        inline const EbmlId & ClassId() const { return GlobalId; }
        inline constexpr const EbmlSemanticContext & GetContext() const { return Context; }
        inline const char * GetName() const { return DebugName; }
        inline EbmlElement & NewElement() const { return Create(); }
        /// is infinite/unknown size allowed
        inline bool CanHaveInfiniteSize() const { return CanInfinite; }
        bool HasDefault() const { return hasDefault; }
        // get information about supported version for this element
        inline const EbmlDocVersion & GetVersions() const { return Version; }

    private:
    EbmlElement & (* const Create)();
    const EbmlId & GlobalId;
    const bool CanInfinite;
    const bool hasDefault;
    const EbmlDocVersion &Version;
    const char * DebugName;
    const EbmlSemanticContext & Context;
};

template<typename T>
class EBML_DLL_API EbmlCallbacksDefault : public EbmlCallbacks {
  public:
    constexpr EbmlCallbacksDefault(EbmlElement & (*Creator)(), const EbmlId & aGlobalId, const char * aDebugName, const EbmlSemanticContext & aContext,
                                   const EbmlDocVersion & aVersion, bool hasDefault = false)
      :EbmlCallbacks(Creator, aGlobalId, false, hasDefault, aDebugName, aContext, aVersion)
    {
    }
};

template<typename T>
class EBML_DLL_API EbmlCallbacksWithDefault : public EbmlCallbacksDefault<T> {
  public:
    constexpr EbmlCallbacksWithDefault(EbmlElement & (*Creator)(), const EbmlId & aGlobalId, const T &def, const char * aDebugName, const EbmlSemanticContext & aContext,
                                       const EbmlDocVersion & aVersion)
      :EbmlCallbacksDefault<T>(Creator, aGlobalId, aDebugName, aContext, aVersion, true)
      ,defaultValue(def)
    {
    }

    inline const T & DefaultValue() const { return defaultValue; }

    private:
    const T defaultValue;
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
    constexpr EbmlSemanticContext(std::size_t aSize,
      const EbmlSemantic *aMyTable,
      const EbmlSemanticContext *aUpTable,
      const _GetSemanticContext aGetGlobalContext,
      const EbmlCallbacks *aMasterElt)
      : GetGlobalContext(aGetGlobalContext), MyTable(aMyTable), Size(aSize),
        UpTable(aUpTable), MasterElt(aMasterElt) {}

    bool operator!=(const EbmlSemanticContext & aElt) const {
      return ((Size != aElt.Size) || (MyTable != aElt.MyTable) ||
        (UpTable != aElt.UpTable) || (GetGlobalContext != aElt.GetGlobalContext) ||
        (MasterElt != aElt.MasterElt));
    }

        inline std::size_t GetSize() const { return Size; }
        inline const EbmlCallbacks* GetMaster() const { return MasterElt; }
        inline const EbmlSemanticContext* Parent() const { return UpTable; }
        const EbmlSemantic & GetSemantic(std::size_t i) const;

    const _GetSemanticContext GetGlobalContext; ///< global elements supported at this level

    private:
        const EbmlSemantic *MyTable; ///< First element in the table
    const std::size_t Size;          ///< number of elements in the table
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
    // callback to tell if the element should be written or not
    // \return true if the element should be written
    using ShouldWrite = std::function<bool(const EbmlElement &)>;

    // write only elements that don't have their default value set
    static bool WriteSkipDefault(const EbmlElement &elt) {
      return !elt.IsDefaultValue();
    }

    static bool WriteAll(const EbmlElement &) {
      return true;
    }

    explicit EbmlElement(const EbmlCallbacks &, std::uint64_t aDefaultSize, bool bValueSet = false);
    virtual ~EbmlElement() = default;
    EbmlElement& operator=(const EbmlElement&) = delete;

    virtual const EbmlCallbacks & ElementSpec() const { return ClassInfo; }

    /// Set the minimum length that will be used to write the element size (-1 = optimal)
    void SetSizeLength(int NewSizeLength) {SizeLength = NewSizeLength;}
    int GetSizeLength() const {return SizeLength;}

    static EbmlElement * FindNextElement(IOCallback & DataStream, const EbmlSemanticContext & Context, int & UpperLevel, std::uint64_t MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel = 1);
    static EbmlElement * FindNextID(IOCallback & DataStream, const EbmlCallbacks & ClassInfos, std::uint64_t MaxDataSize);

    /*!
      \brief find the next element with the same ID
    */
    EbmlElement * SkipData(EbmlStream & DataStream, const EbmlSemanticContext & Context, EbmlElement * TestReadElt = nullptr, bool AllowDummyElt = false);

    /*!
      \brief Give a copy of the element, all data inside the element is copied
      \return nullptr if there is not enough memory
    */
    virtual EbmlElement * Clone() const = 0;

    virtual EbmlId const &GetClassId() const {return ClassInfo.ClassId();}
    virtual explicit operator const EbmlId &() const { return GetClassId(); }
    const char *DebugName() const {return ClassInfo.GetName();}
    const EbmlSemanticContext &Context() const {return ClassInfo.GetContext();}
        virtual EbmlElement & CreateElement() const = 0;

    /*!
     * \brief Set whether the size is finite
     * \note by default only allow to set element as finite (override when needed)
     * The size is known in advance when writing, or infinite size is not known on writing.
     * Setting false always works.
     * \return true if setting the value worked
     */
    bool SetSizeInfinite(bool bIsInfinite = true) {
        if (ClassInfo.CanHaveInfiniteSize())
        {
          bSizeIsFinite = !bIsInfinite;
          return true;
        }
        return false;
    }

    virtual bool SizeIsValid(std::uint64_t) const = 0;
    bool ValidateSize() const { return SizeIsValid(GetSize()); }

    std::uint64_t GetElementPosition() const {
      return ElementPosition;
    }

    std::uint64_t ElementSize(const ShouldWrite& writeFilter = WriteSkipDefault) const; /// return the size of the header+data, before writing

    filepos_t Render(IOCallback & output, const ShouldWrite& writeFilter = WriteSkipDefault, bool bKeepPosition = false, bool bForceRender = false);

    virtual filepos_t UpdateSize(const ShouldWrite & writeFilter = WriteSkipDefault, bool bForceRender = false) = 0; /// update the Size of the Data stored
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
    std::uint64_t VoidMe(IOCallback & output, const ShouldWrite& writeFilter = WriteSkipDefault) const;

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

    virtual bool CanWrite(const ShouldWrite & writeFilter) const {
      return writeFilter(*this);
    }

  protected:
    /*!
      \brief find any element in the stream
      \return a DummyRawElement if the element is unknown or nullptr if the element dummy is not allowed
    */
    static EbmlElement *CreateElementUsingContext(const EbmlId & aID, const EbmlSemanticContext & Context, int & LowLevel, bool IsGlobalContext,
                                                  bool AsInfiniteSize,
                                                  bool bAllowDummy = false, unsigned int MaxLowerLevel = 1);

    filepos_t RenderHead(IOCallback & output, bool bForceRender, const ShouldWrite& writeFilter = WriteSkipDefault, bool bKeepPosition = false);
    filepos_t MakeRenderHead(IOCallback & output, bool bKeepPosition);

    /*!
      \brief prepare the data before writing them (in case it's not already done by default)
    */
    virtual filepos_t RenderData(IOCallback & output, bool bForceRender, const ShouldWrite & writeFilter = WriteSkipDefault) = 0;

    /*!
      \brief special constructor for cloning
    */
    EbmlElement(const EbmlElement & ElementToClone) = default;

        inline std::uint64_t GetDefaultSize() const {return DefaultSize;}
        inline void SetSize_(std::uint64_t aSize) {Size = aSize;}
        inline void SetValueIsSet(bool Set = true) {bValueIsSet = Set;}
        inline std::uint64_t GetSizePosition() const {return SizePosition;}

  protected:
    const EbmlCallbacks & ClassInfo;

  private:
    std::uint64_t Size;        ///< the size of the data to write
    std::uint64_t DefaultSize; ///< Minimum data size to fill on rendering (0 = optimal)
    int SizeLength{0}; /// the minimum size on which the size will be written (0 = optimal)
    bool bSizeIsFinite{true};
    std::uint64_t ElementPosition{0};
    std::uint64_t SizePosition{0};
    bool bValueIsSet;
};

/*!
 * \class EbmlElementDefault
 * \brief EbmlElement class that may contain default values
 */
template<typename T>
class EBML_DLL_API EbmlElementDefault : public EbmlElement {
  public:
    explicit EbmlElementDefault(const libebml::EbmlCallbacksDefault<T> &classInfo, std::uint64_t aDefaultSize)
      :EbmlElement(classInfo, aDefaultSize, false)
    {}

    bool IsDefaultValue() const final {
      if (!ElementSpec().HasDefault())
        return false;
      if (!ValueIsSet())
        return true;
      const auto & spec = static_cast<const EbmlCallbacksWithDefault<T> &>(ElementSpec());
      return *this == spec.DefaultValue();
    }

    virtual bool operator==(const T &) const = 0;
    virtual EbmlElementDefault<T> & SetValue(const T &) = 0;
};


template<typename T>
class EBML_DLL_API EbmlElementDefaultSameStorage : public EbmlElementDefault<T> {
  public:
    explicit EbmlElementDefaultSameStorage(const EbmlCallbacksDefault<T> &classInfo, std::uint64_t aDefaultSize)
      :EbmlElementDefault<T>(classInfo, aDefaultSize)
    {}

    EbmlElementDefaultSameStorage<T> & SetValue(const T & NewValue) override
    {
      Value = NewValue;
      EbmlElement::SetValueIsSet();
      return *this;
    }

    bool operator==(const T & test) const override
    {
      return Value == test;
    }

    T GetValue() const
    {
      return Value;
    }

    bool IsSmallerThan(const EbmlElement *Cmp) const override
    {
      if (EbmlId(*this) != EbmlId(*Cmp))
        return false;

      return this->Value < static_cast<const EbmlElementDefaultSameStorage<T> *>(Cmp)->Value;
    }

    explicit operator T() const { return Value; }

  private:
    T Value;
};


template<typename T, typename S>
class EBML_DLL_API EbmlElementDefaultStorage : public EbmlElementDefault<T> {
  public:
    explicit EbmlElementDefaultStorage(const EbmlCallbacksDefault<T> &classInfo, std::uint64_t aDefaultSize)
      :EbmlElementDefault<T>(classInfo, aDefaultSize)
    {}

    EbmlElementDefaultStorage<T,S> & SetValue(const T & NewValue) override
    {
      Value = static_cast<S>(NewValue);
      EbmlElement::SetValueIsSet();
      return *this;
    }

    EbmlElementDefaultStorage<T,S> & SetValue(const S & NewValue)
    {
      Value = NewValue;
      EbmlElement::SetValueIsSet();
      return *this;
    }

    const S & GetValue() const
    {
      return Value;
    }

    explicit operator const S &() const { return Value; }

  protected:
    S Value;
};

} // namespace libebml

#define EBML_WRITE_FILTER 1

#endif // LIBEBML_ELEMENT_H
