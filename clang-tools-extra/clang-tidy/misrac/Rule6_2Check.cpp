//===--- Rule6_2Check.cpp - clang-tidy ------------------------------------===//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//===----------------------------------------------------------------------===//
// This file is mostly copied from
// https://github.com/rettichschnidi/clang-tidy-misra/blob/master/src/rules/c2012/6.2.cpp
// except the FixItHint

#include "Rule6_2Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misrac {

void Rule6_2Check::registerMatchers(MatchFinder *Finder) {
  using namespace ast_matchers;
  Finder->addMatcher(fieldDecl(hasType(isInteger())).bind("FieldDecl"), this);
}

void Rule6_2Check::check(const MatchFinder::MatchResult &Result) {
  auto *MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("FieldDecl");

  if (!MatchedDecl->isBitField()) {
    return;
  }

  if (MatchedDecl->getBitWidthValue(*Result.Context) != 1) {
    return;
  }

  if (MatchedDecl->isUnnamedBitfield()) {
    return;
  }

  if (MatchedDecl->getType().getTypePtr()->isSignedIntegerType()) {
    diag(MatchedDecl->getTypeSpecStartLoc(), "1 bit field should be unsigned")
      << FixItHint::CreateInsertion(MatchedDecl->getTypeSpecStartLoc(), "unsigned ");
  }

}
}// namespace clang::tidy::misrac
