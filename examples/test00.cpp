// Copyright © 2002-2004 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
    \file
    \author Steve Lhomme     <robux4 @ users.sf.net>
    Test the writing a basic EBML file
*/

#include <ebml/StdIOCallback.h>
#include <ebml/EbmlHead.h>
#include <ebml/EbmlStream.h>
#include <ebml/EbmlSubHead.h>
#include <ebml/EbmlUnicodeString.h>
#include <ebml/EbmlVoid.h>
#include <ebml/EbmlContexts.h>
#include <ebml/EbmlVersion.h>

#include <cstdio>
#include <string>

using namespace libebml;

class UniStringWithoutDefault : public EbmlUnicodeString {
    public: UniStringWithoutDefault(EBML_EXTRA_PARAM); 
    UniStringWithoutDefault(const UniStringWithoutDefault & ElementToClone) : EbmlUnicodeString(ElementToClone) {} 
    EBML_CONCRETE_CLASS(UniStringWithoutDefault)
};

DEFINE_xxx_CLASS(UniStringWithoutDefault, 0x4123, 2, EbmlHead, "UniStringWithoutDefault", GetEbmlGlobal_Context)

const char RW_FILENAME[] = "test.ebml";

int main(void)
{
  printf("Test00 :\nEBML %s\n", EbmlCodeVersion.c_str());
  StdIOCallback Ebml_file(RW_FILENAME, ::MODE_CREATE);

  ///// Writing test
  EbmlHead TestHead;

  printf("Write EBML + Matroska elements\n");
  EDocType & MyDocType = GetChild<EDocType>(TestHead);
  MyDocType.SetValue("matroska");

  EDocTypeVersion & MyDocTypeVer = GetChild<EDocTypeVersion>(TestHead);
  MyDocTypeVer.SetValue(1);

  EDocTypeReadVersion & MyDocTypeReadVer = GetChild<EDocTypeReadVersion>(TestHead);
  MyDocTypeReadVer.SetValue(1);

  auto & MyString = GetChild<UniStringWithoutDefault>(TestHead);
  MyDocType.SetValue("éé");

  TestHead.Render(Ebml_file);

  Ebml_file.close();

  ///// Reading test
  StdIOCallback Ebml_Wfile(RW_FILENAME, ::MODE_READ);

  // example 1 skip all the elements found
  EbmlStream aStream(Ebml_Wfile);
  EbmlElement * ElementLevel0;
  EbmlElement * ElementLevel1;
  EbmlElement * ElementLevel2;
  EbmlElement * ElementLevel3;

  // read the data until a possible element is found (valid ID + size combination)
  printf("Read EBML elements & skip data\n");
  // find the EBML head in the file
  ElementLevel0 = aStream.FindNextID(EBML_INFO(EbmlHead), 0xFFFFFFFFL);
  if (ElementLevel0 != NULL)
  {
    printf("ID : ");
    for (unsigned int i=EbmlId(*ElementLevel0).GetLength(); i; i++)
    {
      printf("[%02X]", (EbmlId(*ElementLevel0).GetValue() >> ((i-1)*8)) & 0xFF);
    }
    printf("\n");

    ElementLevel0->SkipData(aStream, EBML_CLASS_CONTEXT(EbmlHead));
    if (ElementLevel0 != NULL)
      delete ElementLevel0;
  }


  Ebml_Wfile.close();

  return 0;
}
