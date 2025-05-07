#include "test.h"

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
		"are",
	};
	ASSERT_EQUAL_LINES(lines1, 3, lines1, 3);
	return NULL;
}

FailReason test_lines__same_length_but_not_equal() {
	char **actual = (char *[]) {
		"hello",
		"how",
		"are",
		"yoo",

	};
	char **expected = (char *[]) {
		"hello",
		"hw",
		"are",
		"you",
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
		"you",
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
		"doing",
	};
	char **expected = (char *[]) {
		"hello",
		"how",
	};
	ASSERT_EQUAL_LINES(actual, 5, expected, 2);
	return NULL;
}


// FailReason test_files__match() {
// 	char** actual_lines = NULL;
// 	int num_actual_lines = 0;
// 	char** expected_lines = NULL;
// 	int num_expected_lines = 0;
// 	// do stuff that results in a file being created...
// 	file_read_lines(ACTUAL_OUTPUT_FILE, &actual_lines, &num_actual_lines);
// 	file_read_lines(EXPECTED_OUTPUT_FILE, &expected_lines, &num_expected_lines);
// 	ASSERT_EQUAL_LINES(actual_lines, num_actual_lines, expected_lines, num_expected_lines);
// 	return NULL;
// }


#include "paging.h"
#include "readFile.h"
#define ACTUAL_OUTPUT_FILE "actual_output.txt"
#define EXPECTED_OUTPUT_FILE "expected_output.txt"
#define FILENAME "processes.txt"
int generateRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}
void __test_student() {
    // get the processes from the file
    unsigned int num_processes = countLines(FILENAME);
    char **lines = (char **)malloc(num_processes * sizeof(char *));
    unsigned int *line_sizes = (unsigned int *)malloc(num_processes * sizeof(unsigned int));
    struct PCB *processes = (struct PCB *)malloc(num_processes * sizeof(struct PCB));

    readLines(FILENAME, lines, line_sizes, num_processes);

    BuildPageTable(processes, num_processes, lines, line_sizes);

    struct TLB *tlb = (struct TLB *)malloc(sizeof(struct TLB));
    // Initialize TLB
    initTLB(tlb);

    FILE *file = fopen(ACTUAL_OUTPUT_FILE, "w");
    int error = 0;

    // Simulate memory accesses
    for (unsigned int i = 0; i < 100000; i++) {
        // Randomly select a process
        unsigned int process_index = generateRandom(0, num_processes - 1);

        // randomly select an address
        unsigned int address = generateRandom(0, processes[process_index].limit-1);
        char data = getDataTLB(processes, tlb, process_index, address, i);

        if (data == lines[process_index][address]){
            fprintf(file, "Process %d accessing address %d correctly\n", process_index, address);
        }
        else {
            fprintf(file, "Process %d failed accessing address %d. Expected %c Read %c\n", process_index, address, data, lines[process_index][address]);
            error++;
        }
    }
    fclose(file);
}

FailReason test__student() {
	char** actual_lines = NULL;
	int num_actual_lines = 0;
	__test_student();
	file_read_lines(ACTUAL_OUTPUT_FILE, &actual_lines, &num_actual_lines);
	for (int i = 0; i < num_actual_lines; i++) {
		ASSERT_LINE_CONTAINS(actual_lines[i], "correctly");
	}

	return NULL;
}
int main() {

	RUN_TEST(2, VISIBLE, test_strings__equal);
	RUN_TEST(2, VISIBLE, test_strings__not_equal);
	RUN_TEST(2, VISIBLE, test_lines__equal);
	RUN_TEST(2, VISIBLE, test_lines__same_length_but_not_equal);
	RUN_TEST(2, VISIBLE, test_lines__actual_missing_lines);
	RUN_TEST(2, VISIBLE, test_lines__actual_has_too_many_lines);

	RUN_TEST(2, VISIBLE, test__student);

	write_to_file(RESULTS_DEST, result_to_string()); // Required
}

// ========================
// ========================
// Internal: Do not touch!
// ========================
// ========================

// ============
// To string
// ============
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

char* result_to_string() {
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
// Internal Utils
// ===========

char* escape_as_json(char* s) {
	if (s == NULL) {
		return strdup("");
	}
	char* escaped = malloc(strlen(s) * 2 + 1); // Allocate enough space for worst-case scenario
	if (!escaped) {
		EXIT("malloc");
	}
	char* dest = escaped;
	while (*s) {
		switch (*s) {
			case '\\':
				*dest++ = '\\';
				*dest++ = '\\';
				break;
			case '\"':
				*dest++ = '\\';
				*dest++ = '\"';
				break;
			case '\n':
				*dest++ = '\\';
				*dest++ = 'n';
				break;
			case '\r':
				*dest++ = '\\';
				*dest++ = 'r';
				break;
			default:
				*dest++ = *s;
				break;
		}
		s++;
	}
	*dest = '\0';
	return escaped;
}

int min_int(int i1, int i2) {
	return i1 > i2 ? i2: i1;
}

int max_int(int i1, int i2) {
	return i1 > i2 ? i1: i2;
}

bool file_exists(char *path) {
	return access(path, F_OK) == 0;
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

// ============
// Assertion Library Internals
// ============
void __run_test(float max_score, enum Visibility visibility, char* test_name, FailReason (*test_fn)()) {
	struct Test *test = create_not_yet_run_test(max_score, visibility, test_name);
	FailReason fail_reason = test_fn();
	if (fail_reason) {
		test->maybe_score = 0;
		test->maybe_status = FAILED;
		test->maybe_output = escape_as_json(fail_reason);
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

char* __assert_equal_lines(char** actual_lines, int actual_lines_count, char** expected_lines, int expected_lines_count) {
	char *error = NULL;
	size_t error_size = 0;
	int min_lines = min_int(actual_lines_count, expected_lines_count);
	for (int i = 0; i < min_lines; i++) {
		if (strcmp(actual_lines[i], expected_lines[i]) != 0) {
			char *line_error = NULL;
			asprintf(&line_error, "Expected '%s' on line %d, but found '%s'.\n", expected_lines[i], i + 1, actual_lines[i]);
			if (line_error) {
				size_t len = strlen(line_error);
				error = realloc(error, error_size + len + 1);
				memcpy(error + error_size, line_error, len);
				error_size += len;
				error[error_size] = '\0';
				free(line_error);
			}
		}
	}
	if (actual_lines_count > expected_lines_count) {
		for (int i = expected_lines_count; i < actual_lines_count; i++) {
			char *line_error = NULL;
			asprintf(&line_error, "Extra line in actual output at line %d: '%s'.\n", i + 1, actual_lines[i]);
			if (line_error) {
				size_t len = strlen(line_error);
				error = realloc(error, error_size + len + 1);
				memcpy(error + error_size, line_error, len);
				error_size += len;
				error[error_size] = '\0';
				free(line_error);
			}
		}
	} else if (expected_lines_count > actual_lines_count) {
		for (int i = actual_lines_count; i < expected_lines_count; i++) {
			char *line_error = NULL;
			asprintf(&line_error, "Missing line in actual output at line %d: '%s'.\n", i + 1, expected_lines[i]);
			if (line_error) {
				size_t len = strlen(line_error);
				error = realloc(error, error_size + len + 1);
				memcpy(error + error_size, line_error, len);
				error_size += len;
				error[error_size] = '\0';
				free(line_error);
			}
		}
	}
	return error;
}