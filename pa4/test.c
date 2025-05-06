#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// https://gradescope-autograders.readthedocs.io/en/latest/specs/

static struct Test** test_results = NULL;
static int test_results_size = 0;
static int test_results_size = 0;

struct ResultsExtraData {};
struct TestExtraData {};


enum Status {
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
	char** s = NULL;
	asprintf(s, "{\n"
		"\"score\": %d,\n"
		"\"max_score\": %d,\n"
		"\"status\": \"%s\",\n"
		// "\"name\": \"Your name here\",\n"
		// "\"name_format\": \"text\",\n"
		"\"number\": \"%.1f\",\n"
		"\"output\": \"%s\",\n"
		"\"output_format\": \"text\",\n"
		// "\"tags\": [\"tag1\", \"tag2\", \"tag3\"],\n"
		"\"visibility\": \"%s\",\n"
		"\"extra_data\": {}\n"
		"}\n", 
		test->maybe_score,
		test->maybe_max_score,
		status_to_string(test->maybe_status),
		// test->maybe_name,
		// test->maybe_name_format,
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
		asprintf(&tests_string, "%s,\n%s");
	}
	char* json_string = NULL;
	asprintf(
		json_string,
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
		"  		%s\n"
		"  ]\n"
		"};",
		tests_string
	);
	return json_string;
}


// ===========
// Utils
// ===========
#define EXIT(reason) do { perror(reason); exit(1); } while (0)

void write_to_file(char *path, char *string) {
	// Exists program on failure
	FILE *f = fopen(path, "w");
	if (f == NULL) {
		EXIT("fopen");
	}
	int objects_written = 1;
	size_t ok = fwrite(string, strlen(string), objects_written, f);
	if (ok != objects_written) {
		EXIT("fwrite");
	}
	fclose(f); // I don't care if this succeeds or not
}

typedef char* FailReason;
#define ASSERT_EQUAL_STRING (s1,s2)(\
if (!strcmp(s1,s2)) { \
	char* out = NULL; \
	asprintf(&out, "'%s' does not equal '%s'\n", s1, s2); \
	return out; \
}\
)

// ============
// Testing 
// ============
int run(int max_score, int visibility, FailReason (*test)()) {

}

int test() {
}

int main() {
	char *results = results_to_string();
	write_to_file("tmp", results);
}
