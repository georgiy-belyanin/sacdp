#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fatal(const char *msg) {
    printf("Fatal error: %s\n", msg);
    exit(-1);
}
static void usage(void) {
    printf("Usage: dpll <DIMACS file>\n");
    exit(-2);
}

struct vec {
    size_t size;
    size_t length;
    int8_t *data;
};
typedef struct vec *vec_t;
typedef struct vec const *cvec_t;

static vec_t create_vec(size_t size) {
    vec_t vec = malloc(sizeof(*vec));
    if (vec == NULL)
        fatal("No memory left");
    vec->size = size;
    vec->length = 0;
    vec->data = calloc(size, sizeof(*(vec->data)));
    if (vec->data == NULL)
        fatal("No memory left");
    return vec;
}

static void destroy_vec(vec_t vec) {
    if (vec == NULL) return;
    free(vec->data);
    free(vec);
}

static vec_t vec_copy(cvec_t another_vec) {
    if (another_vec == NULL) return NULL;
    vec_t vec = malloc(sizeof(*vec));
    if (vec == NULL)
        fatal("No memory left");
    vec->length = another_vec->length;
    vec->size = another_vec->size;
    vec->data = calloc(vec->size, sizeof(*(vec->data)));
    if (vec->data == NULL)
        fatal("No memory left");
    memcpy(vec->data, another_vec->data, sizeof(*(vec->data)) * vec->size);
    return vec;
}
static void vec_resize(vec_t vec, size_t size) {
    if (size == 0) {
        vec->length = 0;
        return;
    }
    vec->data = realloc(vec->data, sizeof(*(vec->data)) * size);
    if (vec->data == NULL)
        fatal("No memory left");
    vec->size = size;
    vec->length = size < vec->length ? size : vec->length;
}

static void vec_push(vec_t vec, int8_t element) {
    if (vec->length >= vec->size)
        vec_resize(vec, vec->size + 3);
    vec->data[vec->length++] = element;
}
static void vec_erase(vec_t vec, size_t i) {
    if (vec == NULL) return;
    int8_t last = vec->data[vec->length - 1];
    vec->data[i] = last;
    vec->length--;
    vec_resize(vec, vec->size - 1);
}
static void vec_remove(vec_t vec, int8_t value) {
    if (vec == NULL) return;
    for (size_t i = 0; i < vec->length; i++)
        if (vec->data[i] == value) {
            vec_erase(vec, i);
            return;
        }
}
static int8_t vec_get(cvec_t vec, size_t i) {
    return vec->data[i];
}
static bool vec_contains(cvec_t vec, int8_t value) {
    if (vec == NULL) return false;
    for (size_t i = 0; i < vec->length; i++)
        if (vec->data[i] == value) return true;
    return false;
}

typedef vec_t *cnf_t;

cnf_t create_cnf(size_t clauses) {
    cnf_t cnf = calloc(clauses, sizeof(*cnf));
    if (cnf == NULL)
        fatal("No memory left");
    return cnf;
}

cnf_t cnf_copy(cnf_t another_cnf, size_t clauses) {
    cnf_t cnf = calloc(clauses, sizeof(*cnf));
    if (cnf == NULL)
        fatal("No memory left");
    for (size_t clause = 0; clause < clauses; clause++)
        cnf[clause] = vec_copy(another_cnf[clause]);
    return cnf;
}

void destroy_cnf(cnf_t cnf, size_t clauses) {
    if (cnf == NULL) return;
    for (size_t clause = 0; clause < clauses; clause++)
        destroy_vec(cnf[clause]);
    free(cnf);
}

typedef int8_t *valuation_t;

static valuation_t create_valuation(size_t vars) {
    valuation_t valuation = calloc(vars + 1, sizeof(*valuation));
    if (valuation == NULL)
        fatal("No memory left");
    return valuation;
}
static void destroy_valuation(valuation_t valuation) {
    if (valuation == NULL) return;
    free(valuation);
}

static valuation_t valuation_copy(valuation_t another_valuation, size_t vars) {
    if (another_valuation == NULL) return NULL;

    valuation_t valuation = calloc(vars + 1, sizeof(*valuation));
    if (valuation == NULL)
        fatal("No memory left");
    memcpy(valuation, another_valuation, sizeof(*valuation) * (vars + 1));
    return valuation;
}

static valuation_t dpll(cnf_t cnf, size_t clauses, int8_t *valuation, size_t vars) {
    bool sat = true;
    bool empty = true;
    for (size_t clause = 0; clause < clauses; clause++)
        if (cnf[clause] != NULL) empty = false;
    if (empty) {
        return valuation;
    }

    for (size_t clause = 0; clause < clauses; clause++) {
        if (cnf[clause] != NULL && cnf[clause]->length == 0) {
            return NULL;
        }
    }

    // Unit propagation

    // Pure literal

    // New variable
    size_t var = 0;
    while (valuation[++var] != 0 && var < vars);

    cnf_t new_cnf = cnf_copy(cnf, clauses);
    valuation[var] = 1;

    for (size_t clause = 0; clause < clauses; clause++) {
        if (vec_contains(new_cnf[clause], var)) {
            destroy_vec(new_cnf[clause]);
            new_cnf[clause] = NULL;
        } else if (vec_contains(new_cnf[clause], -var))
            vec_remove(new_cnf[clause], -var);
    }

    valuation_t res = dpll(new_cnf, clauses, valuation, vars);
    destroy_cnf(new_cnf, clauses);
    if (res != NULL) return res;

    new_cnf = cnf_copy(cnf, clauses);
    valuation[var] = -1;

    for (size_t clause = 0; clause < clauses; clause++) {
        if (vec_contains(new_cnf[clause], -var)) {
            destroy_vec(new_cnf[clause]);
            new_cnf[clause] = NULL;
        } else if (vec_contains(new_cnf[clause], var))
            vec_remove(new_cnf[clause], var);
    }

    res = dpll(new_cnf, clauses, valuation, vars);

    destroy_cnf(new_cnf, clauses);
    if (res != NULL) return res;
    valuation[var] = 0;

    return NULL;
}

cnf_t read_dimacs(const char *path, size_t *out_clauses, size_t *out_vars) {
    FILE *f = fopen(path, "r");
    if (f == NULL)
        fatal("Error reading input DIMACS file");

    size_t clauses = 0;
    size_t vars = 0;

    char line[512];
    char sym;
    while (fscanf(f, "%c", &sym)) {
        if (sym == 'c') fgets(line, sizeof(line), f);
        else if (sym == 'p') {
            fscanf(f, "%s", line);
            if (strcmp(line, "cnf"))
                fatal("Error reading DIMACS header: no cnf signature");
            if (fscanf(f, "%ld %ld", &vars, &clauses) != 2)
                fatal("Error reading DIMACS header: clauses and vars not found");
            break;
        }
    }

    if (vars == 0 || clauses == 0)
        fatal("Error reading DIMACS: clause before header");
    cnf_t cnf = create_cnf(clauses);
    vec_t clause = create_vec(3);
    size_t i = 0;
    int8_t var = 0;
    while (fscanf(f, "%hhd", &var) != 0 && i < clauses) {
        if (var == 0) {
            cnf[i] = clause;
            i++;
            clause = NULL;
        } else {
            if (clause == NULL) clause = create_vec(3);
            vec_push(clause, var);
        }
    }

    if (vars < 0 || clauses < 0)
        fatal("Error reading DIMACS header");

    if (out_clauses != NULL)
        *out_clauses = clauses;
    if (out_vars != NULL)
        *out_vars = vars;

    return cnf;
}

int main(int argc, char **argv) {
    if (argc < 2)
        usage();
    size_t clauses;
    size_t vars;
    cnf_t cnf = read_dimacs(argv[1], &clauses, &vars);
    valuation_t valuation = create_valuation(vars);
    valuation_t result = dpll(cnf, clauses, valuation, vars);
    if (result != NULL) {
        printf("SATISFIABLE\n");
        for (int32_t var = 1; var <= vars; var++)
            printf("%d ", result[var] >= 0 ? var : -var);
    } else {
        printf("UNSAT\n");
    }
    destroy_valuation(valuation);
    destroy_cnf(cnf, clauses);
    return 0;
}

