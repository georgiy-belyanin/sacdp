#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(a) (((a) >= 0) ? (a) : -(a))

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
    int16_t *data;
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

static void vec_push(vec_t vec, int16_t element) {
    if (vec->length >= vec->size)
        vec_resize(vec, vec->size + 3);
    vec->data[vec->length++] = element;
}
static void vec_erase(vec_t vec, size_t i) {
    if (vec == NULL) return;
    int16_t last = vec->data[vec->length - 1];
    vec->data[i] = last;
    vec->length--;
    vec_resize(vec, vec->size - 1);
}
static void vec_remove(vec_t vec, int16_t value) {
    if (vec == NULL) return;
    for (size_t i = 0; i < vec->length; i++)
        if (vec->data[i] == value) {
            vec_erase(vec, i);
            return;
        }
}
static int16_t vec_get(cvec_t vec, size_t i) {
    return vec->data[i];
}
static bool vec_contains(cvec_t vec, int16_t value) {
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
void cnf_print(cnf_t cnf, size_t clauses) {
    printf("c ");
    for (size_t clause = 0; clause < clauses; clause++) {
        if (cnf[clause] == NULL) continue;
        printf("( ");
        for (size_t var = 0; var < cnf[clause]->length; var++)
            printf("%d ", vec_get(cnf[clause], var));
        printf(")");
    }
    printf("\n");
}

void cnf_assign(cnf_t cnf, size_t clauses, int16_t var) {
    for (size_t clause = 0; clause < clauses; clause++) {
        if (vec_contains(cnf[clause], var)) {
            destroy_vec(cnf[clause]);
            cnf[clause] = NULL;
        }
        vec_remove(cnf[clause], -var);
    }
}

void destroy_cnf(cnf_t cnf, size_t clauses) {
    if (cnf == NULL) return;
    for (size_t clause = 0; clause < clauses; clause++)
        destroy_vec(cnf[clause]);
    free(cnf);
}

typedef int16_t *valuation_t;

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
static void valuation_assign(valuation_t valuation, int16_t var) {
    valuation[abs(var)] = var > 0 ? 1 : -1;
}
static void valuation_print(valuation_t valuation, size_t vars) {
    printf("v ( ");
    for (size_t var = 1; var <= vars; var++)
        printf("%d ", valuation[var]);
    printf(")\n");
}

static cnf_t prev_cnf;
static valuation_t dpll(cnf_t cnf, size_t clauses, int16_t *valuation, size_t vars) {
    bool fuck = true;

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

    for (size_t clause = 0; clause < clauses; clause++) {
        if (cnf[clause] == NULL || cnf[clause]->length != 1) continue;

        int16_t var = vec_get(cnf[clause], 0);

        valuation_assign(valuation, var);
        cnf_assign(cnf, clauses, var);
    }

    // Pure literal
    for (size_t var = 1; var < vars; var++) {
        if (valuation[var]) continue;
        int8_t pure = 0;
        for (size_t clause = 0; clause < clauses; clause++) {
            if (vec_contains(cnf[clause], var)) {
                if (pure == 0) pure = 1;
                else if (pure == -1) {
                    pure = 0;
                    break;
                }
            } else if (vec_contains(cnf[clause], -var)) {
                if (pure == 0) pure = -1;
                else if (pure == 1) {
                    pure = 0;
                    break;
                }
            }
        }
        if (pure) {
            valuation_assign(valuation, var * pure);
            cnf_assign(cnf, clauses, var * pure);
        }
    }

    // New variable
    size_t var = 0;
    while (var < vars && valuation[++var] != 0);
    if (var == vars && valuation[var]) var = 0;

    cnf_t new_cnf = cnf_copy(cnf, clauses);
    valuation_t new_valuation = valuation_copy(valuation, vars);

    valuation_assign(new_valuation, var);
    cnf_assign(new_cnf, clauses, var);

    valuation_t res = dpll(new_cnf, clauses, new_valuation, vars);
    destroy_cnf(new_cnf, clauses);
    if (res != NULL) {
        if (res != new_valuation) destroy_valuation(new_valuation);
        return res;
    }
    destroy_valuation(new_valuation);

    new_valuation = valuation_copy(valuation, vars);
    new_cnf = cnf_copy(cnf, clauses);

    valuation_assign(new_valuation, -var);
    cnf_assign(new_cnf, clauses, -var);

    res = dpll(new_cnf, clauses, new_valuation, vars);
    destroy_cnf(new_cnf, clauses);
    if (res != NULL) {
        if (res != new_valuation) destroy_valuation(new_valuation);
        return res;
    }
    destroy_valuation(new_valuation);
    //valuation[var] = 0;

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
            if (strcmp(line, "cnf")) {
                fclose(f);
                fatal("Error reading DIMACS header: no cnf signature");
            }
            if (fscanf(f, "%ld %ld", &vars, &clauses) != 2) {
                fclose(f);
                fatal("Error reading DIMACS header: clauses and vars not found");
            }
            break;
        }
    }

    if (vars == 0 || clauses == 0)
        fatal("Error reading DIMACS: clause before header");
    cnf_t cnf = create_cnf(clauses);
    vec_t clause = create_vec(3);
    size_t i = 0;
    int16_t var = 0;
    while (fscanf(f, "%hd", &var) != 0 && i < clauses) {
        if (var == 0) {
            cnf[i] = clause;
            i++;
            clause = NULL;
        } else {
            if (clause == NULL) clause = create_vec(3);
            vec_push(clause, var);
        }
    }

    if (vars < 0 || clauses < 0) {
        fclose(f);
        fatal("Error reading DIMACS header");
    }

    if (out_clauses != NULL)
        *out_clauses = clauses;
    if (out_vars != NULL)
        *out_vars = vars;

    fclose(f);

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
        printf("s SATISFIABLE\nv ");
        for (int32_t var = 1; var <= vars; var++)
            printf("%d ", result[var] >= 0 ? var : -var);
        printf("\n");
    } else {
        printf("s UNSATISFIABLE\n");
    }
    destroy_valuation(result);
    destroy_valuation(valuation);
    destroy_cnf(cnf, clauses);
    return 0;
}

