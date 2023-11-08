//===--- LinuxKernelTidyModule.cpp - clang-tidy----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "Rule17_7Check.h"

#include "Rule6_2Check.h"
#include "Rule6_2Check.h"





namespace clang::tidy {
namespace misrac {

/// This module is for checks specific to the Linux kernel.
class MisracModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    /*CheckFactories.registerCheck<MustCheckErrsCheck>(
      "Misra C");*/

    CheckFactories.registerCheck<Rule17_7Check>(
        "misrac-rule17_7");
    CheckFactories.registerCheck<Rule6_2Check>(
        "misrac-rule6_2");
  }
};
// Register the Misra C using this statically initialized
// variable.
static ClangTidyModuleRegistry::Add<MisracModule>
    X("misrac-module", "Adds checks specific to the Misrac.");
} // namespace linuxkernel

// This anchor is used to force the linker to link in the generated object file
// and thus register the MisracModule.
volatile int MisracModuleAnchorSource = 0;

} // namespace clang::tidy
