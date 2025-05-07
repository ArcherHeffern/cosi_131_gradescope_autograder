#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Test Test;
void append_test_result(struct Test *test_result);

// https://gradescope-autograders.readthedocs.io/en/latest/specs/
#ifdef __APPLE__
	#define DEV
#endif

#ifdef DEV
	#define RESULTS_DEST "./results.json"
#else
	#define RESULTS_DEST "/autograder/results/results.json"
#endif

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

char* status_to_string(enum Status status) {
	switch (status) {
	case PASSED:
		return "passed";
	case FAILED:
		return "failed";
	default:
		return "?";
	}
}

enum Visibility {
	VISIBLE,
	HIDDEN,
	AFTER_DUE_DATE,
	AFTER_PUBLISHED,
};

char* visibility_to_string(enum Visibility visibility) {
	switch (visibility) {
	case (VISIBLE):
		return "visible";
	case (AFTER_PUBLISHED):
		return "after_published";
	case (AFTER_DUE_DATE):
		return "after_due_date";
	case (HIDDEN):
		return "hidden";
	default:
		return "?";
	}
}

enum OutputFormat {
	TEXT,
	HTML,
	SIMPLE_FORMAT,
	MD,
	ANSI,
};

char* output_format_to_string(enum OutputFormat output_format) {
	switch (output_format) {
	case (TEXT):
		return "text";
	case (HTML):
		return "html";
	case (SIMPLE_FORMAT):
		return "simple_format";
	case (MD):
		return "md";
	case (ANSI):
		return "ansi";
	default:
		return "?";
	}
}

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

char* test_to_string(struct Test *test) {
	// TODO: Does not yet handle optional fields
	char* s = NULL;
	asprintf(&s, "\t\t{\n"
		"\t\t\t\"score\": %.1f,\n"
		"\t\t\t\"max_score\": %.1f,\n"
		"\t\t\t\"status\": \"%s\",\n"
		"\t\t\t\"name\": \"%s\",\n"
		"\t\t\t\"name_format\": \"%s\",\n"
		"\t\t\t\"number\": \"%.1f\",\n"
		"\t\t\t\"output\": \"%s\",\n"
		"\t\t\t\"output_format\": \"text\",\n"
		//\t\t\t "\"tags\": [\"tag1\", \"tag2\", \"tag3\"],\n"
		"\t\t\t\"visibility\": \"%s\",\n"
		"\t\t\t\"extra_data\": {}\n"
		"\t\t}\n", 
		test->maybe_score,
		test->maybe_max_score,
		status_to_string(test->maybe_status),
		test->maybe_name,
		output_format_to_string(test->maybe_name_format),
		test->maybe_number,
		test->maybe_output,
		// test->maybe_output_format,
		// test->maybe_tags, // Convert array to string
		visibility_to_string(test->maybe_visibility)
		// test->maybe_extra_data, // Convert to string version
	);
	return s;
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

// struct Result* results_create() {
// 	struct Result *result = malloc(sizeof(struct Result));
// 	memset(result, 0, sizeof(struct Result));
// 	result->maybe_score = 0;
// 	result->maybe_execution_time = 0;
// 	result->maybe_output = NULL;
// 	result->maybe_output_format = SIMPLE_FORMAT;
// 	result->maybe_test_output_format = TEXT;
// 	result->maybe_test_name_format = TEXT;
// 	result->maybe_visibility = AFTER_DUE_DATE;
// 	result->maybe_stdout_visibility = VISIBLE;
// 	result->maybe_extra_data = NULL;
// 	result->maybe_tests = NULL;
// 	return result;
// }

char* results_to_string() {
	// TODO: Take result object as input
	if (test_results_size == 0) {
		return "";
	}
	char* tests_string = test_to_string(test_results[0]);
	for (int i = 1; i < test_results_size; i++) {
		asprintf(&tests_string, "%s,\n%s", tests_string, test_to_string(test_results[i]));
	}
	char* json_string = NULL;
	asprintf(
		&json_string,
		"{\n"
		// "  \"score\": 44.0,\n"
		// "  \"execution_time\": 136,\n"
		// "  \"output\": \"Text relevant to the entire submission\",\n"
		// "  \"output_format\": \"simple_format\",\n"
		"  \"test_output_format\": \"text\",\n"
		"  \"test_name_format\": \"text\",\n"
		"  \"visibility\": \"after_due_date\",\n"
		"  \"stdout_visibility\": \"visible\",\n"
		"  \"extra_data\": {},\n"
		"  \"tests\": [\n"
		"%s\n"
		"  ]\n"
		"}",
		tests_string
	);
	return json_string;
}


// ===========
// Utils
// ===========
#define EXIT(reason) do { perror(reason); exit(1); } while (0)

int min_int(int i1, int i2) {
	return i1 > i2 ? i2: i1;
}

int max_int(int i1, int i2) {
	return i1 > i2 ? i1: i2;
}

void write_to_file(char *path, char *string) {
	// Exists program on failure
	FILE *f = fopen(path, "w");
	if (f == NULL) {
		EXIT("fopen");
	}
	int objects_written = 1;
	int string_length = strlen(string);
	if (string_length != 0) {
		size_t ok = fwrite(string, string_length * sizeof(char), objects_written, f);
		if (ok != objects_written) {
			EXIT("fwrite");
		}
	}
	fclose(f); // I don't care if this succeeds or not
}

void file_read_lines(char *path, char ***lines, int *count_lines) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		EXIT("fopen");
	}

	*lines = NULL;
	*count_lines = 0;
	size_t capacity = 0;
	char *line = NULL;
	size_t len = 0;

	while (getline(&line, &len, file) != -1) {
		if (*count_lines == capacity) {
			capacity = capacity == 0 ? 10 : capacity * 2;
			*lines = realloc(*lines, capacity * sizeof(char *));
			if (*lines == NULL) {
				EXIT("realloc");
			}
		}
		(*lines)[*count_lines] = strdup(line);
		if ((*lines)[*count_lines] == NULL) {
			EXIT("strdup");
		}
		(*count_lines)++;
	}

	free(line);
	fclose(file);
}

typedef char* FailReason;

// ============
// ASSERTIONS LIBRARY
// ============

// TODO: Convert to function. And wrap with macro to get auto returning behavior on failures. 
#define ASSERT_EQUAL_STRING(s1, s2) 									\
do { 																	\
if (strcmp((s1),(s2)) != 0) { 													\
	char *out = NULL; 													\
	asprintf(&out, "'%s' does not equal '%s'", (s1), (s2)); 				\
	return out; 														\
}																		\
} while (0)

#define ASSERT_EQUAL_LINES(actual_lines, actual_lines_count, expected_lines, expected_lines_count) do { \
	char *error = NULL; \
	size_t error_size = 0; \
	int min_lines = min_int(actual_lines_count, expected_lines_count); \
	for (int i = 0; i < min_lines; i++) { \
		if (strcmp(actual_lines[i], expected_lines[i]) != 0) { \
			char *line_error = NULL; \
			asprintf(&line_error, "Expected '%s' on line %d, but found '%s'.\n", expected_lines[i], i + 1, actual_lines[i]); \
			if (line_error) { \
				size_t len = strlen(line_error); \
				error = realloc(error, error_size + len + 1); \
				memcpy(error + error_size, line_error, len); \
				error_size += len; \
				error[error_size] = '\0'; \
				free(line_error); \
			} \
		} \
	} \
	if (actual_lines_count > expected_lines_count) { \
		for (int i = expected_lines_count; i < actual_lines_count; i++) { \
			char *line_error = NULL; \
			asprintf(&line_error, "Extra line in actual output at line %d: '%s'.\n", i + 1, actual_lines[i]); \
			if (line_error) { \
				size_t len = strlen(line_error); \
				error = realloc(error, error_size + len + 1); \
				memcpy(error + error_size, line_error, len); \
				error_size += len; \
				error[error_size] = '\0'; \
				free(line_error); \
			} \
		} \
	} else if (expected_lines_count > actual_lines_count) { \
		for (int i = actual_lines_count; i < expected_lines_count; i++) { \
			char *line_error = NULL; \
			asprintf(&line_error, "Missing line in actual output at line %d: '%s'.\n", i + 1, expected_lines[i]); \
			if (line_error) { \
				size_t len = strlen(line_error); \
				error = realloc(error, error_size + len + 1); \
				memcpy(error + error_size, line_error, len); \
				error_size += len; \
				error[error_size] = '\0'; \
				free(line_error); \
			} \
		} \
	} \
	if (error) return error; \
} while (0)

// ============
// Testing 
// ============

struct Test* create_not_yet_run_test(float max_score, enum Visibility visibility, char* test_name) {
	// Must fill out the following during test runtime
	// - maybe_score
	// - maybe_status
	// - maybe_output
	static int test_number = 1;
	struct Test *test = malloc(sizeof(struct Test));
	test->maybe_score = 0;
	test->maybe_max_score = max_score;
	test->maybe_status = NOT_YET_RUN;
	test->maybe_name = test_name;
	test->maybe_name_format = TEXT;
	test->maybe_number = test_number++;
	test->maybe_output = "";
	test->maybe_tags = NULL;
	test->maybe_visibility = visibility;
	test->maybe_extra_data = NULL;

	return test;
}

#define RUN_TEST(max_score, visibility, test_fn) \
do { \
	__run_test(max_score, visibility, #test_fn, test_fn); \
} while (0) 

void __run_test(float max_score, enum Visibility visibility, char* test_name, FailReason (*test_fn)()) {
	struct Test *test = create_not_yet_run_test(max_score, visibility, test_name);
	FailReason fail_reason = test_fn();
	if (fail_reason) {
		test->maybe_score = 0;
		test->maybe_status = FAILED;
		test->maybe_output = fail_reason;
	} else {
		test->maybe_score = max_score;
		test->maybe_status = PASSED;
	}

	append_test_result(test);
}

void append_test_result(struct Test *test_result) {
	if (test_results_size == test_results_capacity) {
		test_results_size *= 2;
		struct Test **new_test_results = malloc(sizeof(struct Test*) * test_results_capacity);
		for (int i = 0; i < test_results_size; i++) {
			new_test_results[i] = test_results[i];
		}
		free(test_results);
		test_results = new_test_results;
	}
	test_results[test_results_size++] = test_result;
}

// ============
// TESTS
// ============
FailReason test_strings__equal() {
	char* s1 = "hello";
	ASSERT_EQUAL_STRING(s1, s1);
	return NULL;
}

FailReason test_strings__not_equal() {
	ASSERT_EQUAL_STRING("hello", "world");
	return NULL;
}

FailReason test_lines__equal() {
	char **lines1 = (char *[]) {
		"hello",
		"how",
		"are"
	};
	ASSERT_EQUAL_LINES(lines1, 3, lines1, 3);
	return NULL;
}

FailReason test_lines__same_length_but_not_equal() {
	char **actual = (char *[]) {
		"hello",
		"how",
		"are",
		"yoo"

	};
	char **expected = (char *[]) {
		"hello",
		"hw",
		"are"
		"you"
	};
	ASSERT_EQUAL_LINES(actual, 4, expected, 4);
	return NULL;
}

FailReason test_lines__actual_missing_lines() {
	char **actual = (char *[]) {
		"hello",
	};
	char **expected = (char *[]) {
		"hello",
		"how",
		"are",
		"you"
	};
	ASSERT_EQUAL_LINES(actual, 1, expected, 4);
	return NULL;
}

FailReason test_lines__actual_has_too_many_lines() {
	char **actual = (char *[]) {
		"hello",
		"how",
		"are",
		"you",
		"doing"
	};
	char **expected = (char *[]) {
		"hello",
		"how",
	};
	ASSERT_EQUAL_LINES(actual, 5, expected, 2);
	return NULL;
}

int main() {

	RUN_TEST(2, VISIBLE, test_strings__equal);
	RUN_TEST(2, VISIBLE, test_strings__not_equal);
	RUN_TEST(2, VISIBLE, test_lines__equal);
	RUN_TEST(2, VISIBLE, test_lines__same_length_but_not_equal);
	RUN_TEST(2, VISIBLE, test_lines__actual_missing_lines);
	RUN_TEST(2, VISIBLE, test_lines__actual_has_too_many_lines);

	char* results = results_to_string();
	printf("%s\n", results);
	write_to_file(RESULTS_DEST, results); // Required
}
