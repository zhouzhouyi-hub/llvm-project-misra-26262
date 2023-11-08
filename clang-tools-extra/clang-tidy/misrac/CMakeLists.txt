set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_library(clangTidyMisracModule
  MisracTidyModule.cpp
  Rule17_7Check.cpp
  Rule6_2Check.cpp


  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  ClangDriverOptions
  )

clang_target_link_libraries(clangTidyMisracModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
