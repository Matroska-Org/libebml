// Copyright © 2023 Steve Lhomme.
// SPDX-License-Identifier: ISC

#include <ebml/EbmlHead.h>

#include <cassert>
#include <string>

using namespace libebml;

static void FindAllMissingElements(const EbmlMaster *pThis, std::vector<std::string> & missingElements)
{
  const auto & MasterContext = pThis->ContextMaster();
  for (auto childElement : pThis->GetElementList()) {
    if (!childElement->ValueIsSet()) {
      std::string missingValue;
      missingValue = "The Child Element \"";
      missingValue.append(EBML_NAME(childElement));
      missingValue.append("\" of EbmlMaster \"");
      missingValue.append(EBML_NAME(pThis));
      missingValue.append("\", does not have a value set.");
      missingElements.push_back(std::move(missingValue));
    }

    if (childElement->IsMaster()) {
      const auto childMaster = static_cast<const EbmlMaster*>(childElement);

      FindAllMissingElements(childMaster, missingElements);
    }
  }
  unsigned int EltIdx;
  for (EltIdx = 0; EltIdx < EBML_CTX_SIZE(MasterContext); EltIdx++) {
    if (EBML_CTX_IDX(MasterContext,EltIdx).IsMandatory()) {
      if (pThis->FindFirstElt(EBML_CTX_IDX_INFO(MasterContext,EltIdx)) == nullptr) {
        std::string missingElement;
        missingElement = "Missing element \"";
        missingElement.append(EBML_INFO_NAME(EBML_CTX_IDX_INFO(MasterContext,EltIdx)));
        missingElement.append("\" in EbmlMaster \"");
        missingElement.append(EBML_INFO_NAME(*EBML_CTX_MASTER(MasterContext)));
        missingElement.append("\"");
        missingElements.push_back(std::move(missingElement));
      }
    }
  }

}

int main(void)
{
    EbmlHead TestHead;
    const EbmlSemanticContextMaster & MasterContext = TestHead.ContextMaster();

    assert(MasterContext.GetSize() != 0);

    std::vector<std::string> missingElements;
    FindAllMissingElements(&TestHead, missingElements);

    const auto & tstStatic = EbmlHead::GetContextMaster();
    assert(EBML_CTX_SIZE(tstStatic) != 0);

    return 0;
}
