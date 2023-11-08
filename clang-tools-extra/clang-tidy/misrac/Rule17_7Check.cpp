//===--- Rule17_7Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// most copied from UnusedReturnValueCheck.cpp
// clang-tidy -checks='-*,misrac-rule17_7' -fix ./17.7.c 
//
// long func(long para1)
//{
//        return para1;
//}

//void discarded(long para2)
//{
//        func(para2);
//}

#include "Rule17_7Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misrac {

void Rule17_7Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  auto MatchedDirectCallExpr =
    expr(callExpr(callee(functionDecl(
				      unless(returns(voidType())))))
	 .bind("match"));
  auto CheckCastToVoid =
    castExpr(unless(hasCastKind(CK_ToVoid)));
  auto MatchedCallExpr = expr(
      anyOf(MatchedDirectCallExpr,
            explicitCastExpr(unless(cxxFunctionalCastExpr()), CheckCastToVoid,
                             hasSourceExpression(MatchedDirectCallExpr))));

  auto UnusedInCompoundStmt =
      compoundStmt(forEach(MatchedCallExpr),
                   // The checker can't currently differentiate between the
                   // return statement and other statements inside GNU statement
                   // expressions, so disable the checker inside them to avoid
                   // false positives.
                   unless(hasParent(stmtExpr())));
  auto UnusedInIfStmt =
      ifStmt(eachOf(hasThen(MatchedCallExpr), hasElse(MatchedCallExpr)));
  auto UnusedInWhileStmt = whileStmt(hasBody(MatchedCallExpr));
  auto UnusedInDoStmt = doStmt(hasBody(MatchedCallExpr));
  auto UnusedInForStmt =
      forStmt(eachOf(hasLoopInit(MatchedCallExpr),
                     hasIncrement(MatchedCallExpr), hasBody(MatchedCallExpr)));
  auto UnusedInRangeForStmt = cxxForRangeStmt(hasBody(MatchedCallExpr));
  auto UnusedInCaseStmt = switchCase(forEach(MatchedCallExpr));

  Finder->addMatcher(
      stmt(anyOf(UnusedInCompoundStmt, UnusedInIfStmt, UnusedInWhileStmt,
                 UnusedInDoStmt, UnusedInForStmt, UnusedInRangeForStmt,
                 UnusedInCaseStmt)),
      this);
}

void Rule17_7Check::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Matched = Result.Nodes.getNodeAs<CallExpr>("match")) {
    diag(Matched->getBeginLoc(),
         "the value returned by this function should not be disregarded; "
         "neglecting it may lead to errors")
      << FixItHint::CreateInsertion(Matched->getBeginLoc(), "( void )");
  }
}

} // namespace clang::tidy::misrac
