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
#include <cstddef>

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
unsigned int EBML_DLL_API CodedSizeLength(std::uint64_t Value, unsigned int MinSizeLength, bool bSizeIsFinite = true);

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
class EbmlSemanticContextMaster;
class EbmlElement;

#define DEFINE_xxx_CONTEXT(x,global) \
    const libebml::EbmlSemanticContextMaster Context_##x = libebml::EbmlSemanticContextMaster(countof(ContextList_##x), ContextList_##x, nullptr, global, nullptr); \

#define DEFINE_xxx_MASTER(x,id,parent,infinite,name,versions,global) \
    DEFINE_xxx_MASTER_CONS(x,id,parent,infinite,name,versions,global) \
    x::x() :libebml::EbmlMaster(x::ClassInfos) {}

// define a master class with a custom constructor
#define DEFINE_xxx_MASTER_CONS(x,id,parent,infinite,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    constexpr const libebml::EbmlSemanticContextMaster x::SemanticContext = libebml::EbmlSemanticContextMaster(countof(ContextList_##x), ContextList_##x, &parent::SemanticContext, global, &EBML_INFO(x)); \
    constexpr const libebml::EbmlCallbacksMaster x::ClassInfos(x::Create, Id_##x, infinite, name, x::SemanticContext, versions); \

// define a master class with no parent class (can be used globally)
#define DEFINE_xxx_MASTER_ORPHAN(x,id,infinite,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    constexpr const libebml::EbmlSemanticContextMaster x::SemanticContext = libebml::EbmlSemanticContextMaster(countof(ContextList_##x), ContextList_##x, nullptr, global, &EBML_INFO(x)); \
    constexpr const libebml::EbmlCallbacksMaster x::ClassInfos(x::Create, Id_##x, infinite, name, x::SemanticContext, versions); \

#define DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    static constexpr const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext( &parent::SemanticContext, global, &EBML_INFO(x));

#define DEFINE_xxx_CLASS_BASE(x,BaseClass,id,parent,name,versions,global) \
    DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, false, false, name, Context_##x, versions); \
    x::x() : BaseClass(x::ClassInfos) {}

#define DEFINE_xxx_CLASS_BASE_DEFAULT(x,BaseClass,id,parent,name,global,StorageType,defval,versions) \
    DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    constexpr const libebml::EbmlCallbacksWithDefault<StorageType> x::ClassInfos(x::Create, Id_##x, defval, name, Context_##x, versions); \
    x::x() : BaseClass(x::ClassInfos) {}

#define DEFINE_xxx_CLASS_BASE_NODEFAULT(x,BaseClass,id,parent,name,global,StorageType,versions) \
    DEFINE_xxx_CLASS_CONS(x,id,parent,name,global) \
    constexpr const libebml::EbmlCallbacksDefault<StorageType> x::ClassInfos(x::Create, Id_##x, name, Context_##x, versions); \
    x::x() : BaseClass(x::ClassInfos) {}

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

// define a class with no parent class (can be used globally)
#define DEFINE_xxx_CLASS_ORPHAN(x,id,name,versions,global) \
    static constexpr const libebml::EbmlId Id_##x    {id}; static_assert(libebml::EbmlId::IsValid(Id_##x .GetValue()), "invalid id for " name ); \
    static constexpr const libebml::EbmlSemanticContext Context_##x = libebml::EbmlSemanticContext(nullptr, global, nullptr); \
    constexpr const libebml::EbmlCallbacks x::ClassInfos(x::Create, Id_##x, false, false, name, Context_##x, versions); \

#define DEFINE_EBML_CONTEXT(x)                             DEFINE_xxx_CONTEXT(x,GetEbmlGlobal_Context)

#define DEFINE_EBML_MASTER(x,id,parent,infinite,name,versions)  DEFINE_xxx_MASTER(x,id,parent,infinite,name,versions,GetEbmlGlobal_Context)
#define DEFINE_EBML_MASTER_ORPHAN(x,id,infinite,name,versions)  DEFINE_xxx_MASTER_ORPHAN(x,id,infinite,name,versions,GetEbmlGlobal_Context)
#define DEFINE_EBML_CLASS_ORPHAN(x,id,name,versions)            DEFINE_xxx_CLASS_ORPHAN(x,id,name,versions,GetEbmlGlobal_Context)
#define DEFINE_EBML_UINTEGER_DEF(x,id,parent,name,val,versions) DEFINE_xxx_UINTEGER_DEF(x,id,parent,name,versions,GetEbmlGlobal_Context,val)
#define DEFINE_EBML_STRING_DEF(x,id,parent,name,val,versions)   DEFINE_xxx_STRING_DEF(x,id,parent,name,versions,GetEbmlGlobal_Context,val)

#define DEFINE_SEMANTIC_CONTEXT(x)
#define DEFINE_START_SEMANTIC(x)     static constexpr const libebml::EbmlSemantic ContextList_##x[] = {
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

#define DECLARE_xxx_BASE_MASTER(x, DllApi, BaseClass) \
class DllApi x : public BaseClass { \
  private: \
    static const libebml::EbmlCallbacksMaster ClassInfos; \
  public: \
    const libebml::EbmlCallbacksMaster & ElementSpec() const override { return ClassInfos; } \
    static constexpr const libebml::EbmlCallbacksMaster & GetElementSpec() { return ClassInfos; } \
    x();

#define DECLARE_xxx_BASE(x, DllApi, BaseClass) \
class DllApi x : public BaseClass { \
  private: \
    static const libebml::EbmlCallbacks ClassInfos; \
  public: \
    static constexpr const libebml::EbmlCallbacks & GetElementSpec() { return ClassInfos; } \
    x();

#define DECLARE_xxx_MASTER(x,DllApi)    \
  DECLARE_xxx_BASE_MASTER(x, DllApi, libebml::EbmlMaster) \
  static const libebml::EbmlSemanticContextMaster SemanticContext; \
  static const libebml::EbmlSemanticContextMaster & GetContextMaster() { return SemanticContext; }

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
        libebml::EbmlElement * Clone() const override { return new Type(*this); } \
    static libebml::EbmlElement & Create() {return *(new Type);} \
        static constexpr const libebml::EbmlCallbacks & ClassInfo() {return ClassInfos;} \

#define EBML_INFO(ref)             ref::GetElementSpec()
#define EBML_ID(ref)               EBML_INFO_ID(EBML_INFO(ref))
#define EBML_CLASS_SEMCONTEXT(ref) Context_##ref
#define EBML_CONTEXT(e)            tEBML_CONTEXT(e)
#define EBML_NAME(e)               tEBML_NAME(e)
#define EBML_SPEC(e)               tEBML_SPEC(e)

#define EBML_INFO_ID(cb)      tEBML_INFO_ID(cb)
#define EBML_INFO_NAME(cb)    tEBML_INFO_NAME(cb)
#define EBML_INFO_CREATE(cb)  tEBML_INFO_CREATE(cb)
#define EBML_INFO_CONTEXT(cb) tEBML_INFO_CONTEXT(cb)

#define EBML_SEM_SPECS(s)   tEBML_SEM_SPECS(s)
#define EBML_SEM_CONTEXT(s) EBML_INFO_CONTEXT(EBML_SEM_SPECS(s))
#define EBML_SEM_CREATE(s)  EBML_INFO_CREATE(EBML_SEM_SPECS(s))

#define EBML_CTX_SIZE(c)       tEBML_CTX_SIZE(c)
#define EBML_CTX_MASTER(c)     tEBML_CTX_MASTER(c)
#define EBML_CTX_PARENT(c)     tEBML_CTX_PARENT(c)
#define EBML_CTX_IDX(c,i)      tEBML_CTX_IDX(c,i)
#define EBML_CTX_IDX_INFO(c,i) EBML_SEM_SPECS(EBML_CTX_IDX(c,i))
#define EBML_CTX_IDX_ID(c,i)   EBML_INFO_ID(EBML_CTX_IDX_INFO(c,i))

#define EBML_CLASS_CONTEXT(ref)    (EBML_INFO(ref)).GetContextMaster()

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
    constexpr EbmlDocVersion(const std::string_view &name_space, version_type min = 0, version_type max = ANY_VERSION)
      : minver(min)
      , maxver(max)
      , ns(name_space)
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

    constexpr const std::string_view & GetNameSpace() const { return ns; }

    /// @brief constant value to indicate the maximum version matches all versions or the minimum version matches no version
    static const version_type ANY_VERSION = std::numeric_limits<version_type>::max();

  private:
    // the minimum DocType version this element is allowed in
    // ANY_VERSION if the element is never supported
    const version_type minver;
    // the maximum DocType version this element is allowed in
    // ANY_VERSION if the element is supported in all (known) version
    const version_type maxver;

    const std::string_view ns;
};

// functions for generic handling of data (should be static to all classes)
class EBML_DLL_API EbmlCallbacks {
  public:
    using CreateOperator = EbmlElement & (*)(void);

    constexpr EbmlCallbacks(const CreateOperator & Creator, const EbmlId & aGlobalId, bool aCanInfinite, bool aHasDefault, const char * aDebugName, const EbmlSemanticContext & aContext,
                            const EbmlDocVersion & aVersion)
      :Create(Creator)
      ,GlobalId(aGlobalId)
      ,CanInfinite(aCanInfinite)
      ,hasDefault(aHasDefault)
      ,Version(aVersion)
      ,DebugName(aDebugName)
      ,Context(aContext)
    {
    }

        inline constexpr const EbmlId & ClassId() const { return GlobalId; }
        inline constexpr const EbmlSemanticContext & GetContext() const { return Context; }
        inline constexpr const char * GetName() const { return DebugName; }
        inline EbmlElement & NewElement() const { return Create(); }
        /// is infinite/unknown size allowed
        inline constexpr bool CanHaveInfiniteSize() const { return CanInfinite; }
        inline constexpr bool HasDefault() const { return hasDefault; }
        // get information about supported version for this element
        inline constexpr const EbmlDocVersion & GetVersions() const { return Version; }

    private:
    const CreateOperator Create;
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
                                   const EbmlDocVersion & aVersion, bool aHasDefault = false)
      :EbmlCallbacks(Creator, aGlobalId, false, aHasDefault, aDebugName, aContext, aVersion)
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

static inline constexpr const EbmlId & tEBML_INFO_ID(const EbmlCallbacks & cb)
{
  return cb.ClassId();
}

static inline constexpr const char * tEBML_INFO_NAME(const EbmlCallbacks & cb)
{
  return cb.GetName();
}

static inline EbmlElement & tEBML_INFO_CREATE(const EbmlCallbacks & cb)
{
  return cb.NewElement();
}

static inline constexpr const EbmlSemanticContext & tEBML_INFO_CONTEXT(const EbmlCallbacks & cb)
{
  return cb.GetContext();
}

/*!
  \brief contains the semantic informations for a given level and all sublevels
  \todo move the ID in the element class
*/
class EBML_DLL_API EbmlSemantic {
  public:
    constexpr EbmlSemantic(bool aMandatory, bool aUnique, const EbmlCallbacks & aCallbacks)
      :Mandatory(aMandatory), Unique(aUnique), Callbacks(aCallbacks) {}

        inline constexpr bool IsMandatory() const { return Mandatory; }
        inline constexpr bool IsUnique() const { return Unique; }
        inline constexpr EbmlCallbacks const &GetCallbacks() const { return Callbacks; }

    private:
    const bool Mandatory; ///< whether the element is mandatory in the context or not
    const bool Unique;
    const EbmlCallbacks & Callbacks;
};

static inline constexpr const EbmlCallbacks & tEBML_SEM_SPECS(const EbmlSemantic & s)
{
  return s.GetCallbacks();
}

using _GetSemanticContext = const EbmlSemanticContextMaster &(*)();

/*!
  Context of the element
  \todo allow more than one parent ?
*/
class EBML_DLL_API EbmlSemanticContext {
  protected:
    constexpr EbmlSemanticContext(std::size_t aSize,
      const EbmlSemanticContext *aUpTable,
      const _GetSemanticContext aGetGlobalContext,
      const EbmlCallbacks *aMasterElt)
      : GetGlobalContext(aGetGlobalContext), Size(aSize),
        UpTable(aUpTable), MasterElt(aMasterElt) {}

  public:
    constexpr EbmlSemanticContext(
      const EbmlSemanticContext *aUpTable,
      const _GetSemanticContext aGetGlobalContext,
      const EbmlCallbacks *aMasterElt)
      : GetGlobalContext(aGetGlobalContext), Size(0),
        UpTable(aUpTable), MasterElt(aMasterElt) {}

    bool operator!=(const EbmlSemanticContext & aElt) const {
      return ((Size != aElt.Size) ||
        (UpTable != aElt.UpTable) || (GetGlobalContext != aElt.GetGlobalContext) ||
        (MasterElt != aElt.MasterElt));
    }

        inline constexpr std::size_t GetSize() const { return Size; }
        inline constexpr const EbmlCallbacks* GetMaster() const { return MasterElt; }
        inline constexpr const EbmlSemanticContext* Parent() const { return UpTable; }

    const _GetSemanticContext GetGlobalContext; ///< global elements supported at this level

    private:
    const std::size_t Size;          ///< number of elements in the table
    const EbmlSemanticContext *UpTable; ///< Parent element
    /// \todo replace with the global context directly
    const EbmlCallbacks *MasterElt;
};

static inline constexpr size_t tEBML_CTX_SIZE(const EbmlSemanticContext & c)
{
  return c.GetSize();
}

static inline constexpr const EbmlCallbacks * tEBML_CTX_MASTER(const EbmlSemanticContext & c)
{
  return c.GetMaster();
}

static inline constexpr const EbmlSemanticContext * tEBML_CTX_PARENT(const EbmlSemanticContext & c)
{
  return c.Parent();
}

class EBML_DLL_API EbmlSemanticContextMaster : public EbmlSemanticContext {
  public:
    constexpr EbmlSemanticContextMaster(std::size_t aSize,
      const EbmlSemantic *aMyTable,
      const EbmlSemanticContext *aUpTable,
      const _GetSemanticContext aGetGlobalContext,
      const EbmlCallbacks *aMasterElt)
      : EbmlSemanticContext(aSize, aUpTable, aGetGlobalContext, aMasterElt)
      , MyTable(aMyTable)
    {}

    bool operator!=(const EbmlSemanticContext & aElt) const {
      if (EbmlSemanticContext::operator!=(aElt))
        return true;

      if (MyTable != reinterpret_cast<const EbmlSemanticContextMaster &>(aElt).MyTable)
        return true;

      return false;
    }

        const EbmlSemantic & GetSemantic(std::size_t i) const;

    private:
        const EbmlSemantic *MyTable; ///< First element in the table
};

static inline const EbmlSemantic & tEBML_CTX_IDX(const EbmlSemanticContextMaster & c, std::size_t i)
{
  return c.GetSemantic(i);
}

class EBML_DLL_API EbmlCallbacksMaster : public EbmlCallbacks {
  public:
    constexpr EbmlCallbacksMaster(EbmlElement & (*Creator)(), const EbmlId & aGlobalId, bool aCanInfinite,
                                  const char * aDebugName, const EbmlSemanticContextMaster & aContext,
                                  const EbmlDocVersion & aVersion)
      :EbmlCallbacks(Creator, aGlobalId, aCanInfinite, false, aDebugName, aContext, aVersion)
    {
    }

    inline constexpr const EbmlSemanticContextMaster & GetContextMaster() const {
      return static_cast<const EbmlSemanticContextMaster &>(GetContext());
    }
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
      if (elt.IsDefaultValue())
        return false;
      return !elt.ElementSpec().GetVersions().IsAlwaysDeprecated();
    }

    // write all elements except deprecated ones
    static bool WriteAll(const EbmlElement & elt) {
      return !elt.ElementSpec().GetVersions().IsAlwaysDeprecated();
    }

    explicit EbmlElement(const EbmlCallbacks &, std::uint64_t aDefaultSize, bool bValueSet = false);
    virtual ~EbmlElement() = default;
    EbmlElement& operator=(const EbmlElement&) = delete;

    virtual const EbmlCallbacks & ElementSpec() const { return ClassInfo; }

    /// Set the minimum length that will be used to write the element size (-1 = optimal)
    void SetSizeLength(unsigned int NewSizeLength) {SizeLength = NewSizeLength;}
    unsigned int GetSizeLength() const {return SizeLength;}

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

    virtual EbmlId const &GetClassId() const {return EBML_INFO_ID(ClassInfo);}
    virtual explicit operator const EbmlId &() const { return GetClassId(); }
    constexpr const char *DebugName() const {return ClassInfo.GetName();}
    constexpr const EbmlSemanticContext &Context() const {return ClassInfo.GetContext();}
    EbmlElement & CreateElement() const { return EBML_INFO_CREATE(ClassInfo); }

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

    std::uint64_t GetDataStart() const {
      return GetElementPosition() + HeadSize();
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
    std::size_t HeadSize() const {
      return EBML_ID_LENGTH(static_cast<const EbmlId&>(*this)) + CodedSizeLength(Size, SizeLength, bSizeIsFinite);
    } /// return the size of the head, on reading/writing

    std::uint64_t Size;        ///< the size of the data to write
    std::uint64_t DefaultSize; ///< Minimum data size to fill on rendering (0 = optimal)
    unsigned int SizeLength{0}; /// the minimum size on which the size will be written (0 = optimal)
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
    using EbmlElementDefault<T>::EbmlElementDefault;
    using EbmlElementDefault<T>::operator const EbmlId &;

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
    using EbmlElementDefault<T>::EbmlElementDefault;
    using EbmlElementDefault<T>::operator const EbmlId &;

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


static inline constexpr const EbmlSemanticContext & tEBML_CONTEXT(const EbmlElement * e)
{
  return e->Context();
}

static inline const EbmlCallbacks & tEBML_SPEC(const EbmlElement & e)
{
  return e.ElementSpec();
}

static inline constexpr const char * tEBML_NAME(const EbmlElement * e)
{
  return e->DebugName();
}


} // namespace libebml

#define EBML_WRITE_FILTER 1

#endif // LIBEBML_ELEMENT_H
