
#ifndef __TEST_H_
#define __TEST_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "test.h"

// ============
// Assertion Library 
// ============
#define RUN_TEST(max_score, visibility, test_fn) \
do { \
	__run_test(max_score, visibility, #test_fn, test_fn); \
} while (0) 

#define ASSERT_EQUAL_STRING(s1, s2) 									\
do { 																	\
if (strcmp((s1),(s2)) != 0) { 													\
	char *out = NULL; 													\
	asprintf(&out, "'%s' does not equal '%s'", (s1), (s2)); 				\
	return out; 														\
}																		\
} while (0)

#define ASSERT_EQUAL_LINES(actual_lines, actual_lines_count, expected_lines, expected_lines_count) do { \
	char* error = __assert_equal_lines((actual_lines), (actual_lines_count), (expected_lines), (expected_lines_count)); \
	if (error) return error; \
} while (0)

#define ASSERT_LINE_CONTAINS(line, substring) do { \
	if (strstr(line, substring) == NULL) { \
		char* error = NULL; \
		asprintf(&error, "Expected to find substring '%s' in '%s'", (substring), (line)); \
	}\
} while (0)\

#define ASSERT_FILE_EXISTS(path) do { \
if (!file_exists((path))) { \
	char* error = NULL; \
	asprintf(&error, "Expected %s to exist as a file", (path)); \
	return error; \
} while (0) 

// ############
// Ignore the rest
// ############

typedef struct Test Test;

// https://gradescope-autograders.readthedocs.io/en/latest/specs/
#ifdef __APPLE__
	#define DEV
#endif

#ifdef DEV
	#define RESULTS_DEST "./results.json"
#else
	#define RESULTS_DEST "/autograder/results/results.json"
#endif

// External Interface
typedef char* FailReason;

// Internal Interface

static struct Test** test_results = NULL;
static int test_results_size = 0;
static int test_results_capacity = 0;
struct ResultsExtraData {};
struct TestExtraData {};


enum Status {
	NOT_YET_RUN,
	PASSED,
	FAILED,
};

enum Visibility {
	VISIBLE,
	HIDDEN,
	AFTER_DUE_DATE,
	AFTER_PUBLISHED,
};

enum OutputFormat {
	TEXT,
	HTML,
	SIMPLE_FORMAT,
	MD,
	ANSI,
};

struct Test {
	float maybe_score; // Required if no score on `struct Results`
	float maybe_max_score;
	enum Status maybe_status;
	char *maybe_name;
	enum OutputFormat maybe_name_format;
	float maybe_number; // 0 if not exists
	char *maybe_output;
	enum OutputFormat maybe_output_format;
	char **maybe_tags;
	enum Visibility maybe_visibility;
	struct TestExtraData *maybe_extra_data;
};

// struct Result {
// 	float maybe_score; // Required if not on each test case
// 	float maybe_execution_time;
// 	char *maybe_output;
// 	enum OutputFormat maybe_output_format;
// 	enum OutputFormat maybe_test_output_format;
// 	enum OutputFormat maybe_test_name_format;
// 	char *maybe_visibility;
// 	enum Visibility maybe_stdout_visibility;
// 	struct ExtraData *maybe_extra_data;
// 	struct Test *maybe_tests; // Required if no top level scoring
// };

// Printers
char* status_to_string(enum Status status);
char* visibility_to_string(enum Visibility visibility);
char* output_format_to_string(enum OutputFormat output_format);
char* test_to_string(struct Test *test);
char* result_to_string();

// Utils
#define EXIT(reason) do { perror(reason); exit(1); } while (0)
char* escape_as_json(char* s);
int min_int(int i1, int i2);
int max_int(int i1, int i2);
bool file_exists(char *path);
void write_to_file(char *path, char *string);
void file_read_lines(char *path, char ***lines, int *count_lines);

// Assertion Library Internals
void __run_test(float max_score, enum Visibility visibility, char* test_name, FailReason (*test_fn)());
void append_test_result(struct Test *test_result);
struct Test* create_not_yet_run_test(float max_score, enum Visibility visibility, char* test_name);
char* __assert_equal_lines(char** actual_lines, int actual_lines_count, char** expected_lines, int expected_lines_count);

#endif // __TEST_H_