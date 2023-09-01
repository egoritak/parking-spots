# This script sets up clang-tidy globally through CMAKE_CXX_CLANG_TIDY
# It must be included BEFORE any target you want to run checks on is declared

# When checking header files, only do so if the header is from src/ directory to avoid checks in submodules
set(HEADER_FILTER "${CMAKE_CURRENT_SOURCE_DIR}/src/.*")

# Cherry-picked list of clang-tidy checks from https://releases.llvm.org/13.0.0/tools/clang/tools/extra/docs/clang-tidy/checks/list.html
set(CLANG_TIDY_CHECKS
        bugprone-bool-pointer-implicit-conversion
        bugprone-branch-clone
        bugprone-copy-constructor-init
        bugprone-dynamic-static-initializers
        bugprone-exception-escape
        bugprone-forward-declaration-namespace
        bugprone-inaccurate-erase
        bugprone-infinite-loop
        bugprone-integer-division
        bugprone-lambda-function-name
        bugprone-macro-parentheses
        bugprone-macro-repeated-side-effects
        bugprone-misplaced-operator-in-strlen-in-alloc
        bugprone-misplaced-pointer-arithmetic-in-alloc
        bugprone-multiple-statement-macro
        bugprone-not-null-terminated-result
        bugprone-parent-virtual-call
        bugprone-posix-return
        bugprone-redundant-branch-condition
        bugprone-reserved-identifier
        bugprone-signed-char-misuse
        bugprone-sizeof-container
        bugprone-sizeof-expression
        bugprone-string-constructor
        bugprone-string-integer-assignment
        bugprone-string-literal-with-embedded-nul
        bugprone-stringview-nullptr
        bugprone-suspicious-memory-comparison
        bugprone-suspicious-missing-comma
        bugprone-suspicious-semicolon
        bugprone-suspicious-string-compare
        bugprone-terminating-continue
        bugprone-throw-keyword-missing
        bugprone-use-after-move

        readability-const-return-type
        readability-duplicate-include
        readability-misleading-indentation
        readability-misplaced-array-index
        readability-qualified-auto
        cppcoreguidelines-narrowing-conversions

        # TODO cppcoreguidelines-pro-type-member-init detects uninitialized member fields,
        #  but gives warnings on [out] variables like Messgen::info or sockaddr_in sock_addr
        # cppcoreguidelines-pro-type-member-init
)

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
if(CLANG_TIDY_COMMAND)
    string(REPLACE ";" "," CLANG_TIDY_CHECKS_STRING "${CLANG_TIDY_CHECKS}")
    set(CLANG_TIDY_CHECKS_STRING "-*,${CLANG_TIDY_CHECKS_STRING}")

    set(CMAKE_CXX_CLANG_TIDY
            ${CLANG_TIDY_COMMAND};
            -header-filter=${HEADER_FILTER}
            -checks=${CLANG_TIDY_CHECKS_STRING};
            -warnings-as-errors=*
       )
else()
    message(FATAL_ERROR "clang-tidy binary not found. Install with:\n\tsudo apt-get install clang-tidy")
endif()
