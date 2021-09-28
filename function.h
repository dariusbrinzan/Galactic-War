// Copyright 2021 - 312CA Brinzan Darius-Ionut
#ifndef FUNCTION_H_
#define FUNCTION_H_

struct shield {
    struct shield *next;
    size_t val;
};

struct planet {
    struct planet *prev, *next;
    struct shield *first_shield, *last_shield;
    size_t nr_shields, kills;
    char *nume;
};

struct galaxy {
    size_t nr_planets;
    struct planet *first;
};

size_t out_of_bounds(size_t ind, size_t size, size_t planet);

void free_planet(struct planet *p);

void delete_planet(struct galaxy *G, struct planet *p);

void add(struct galaxy *G, char *nume, size_t ind, const size_t nr_scuturi);

void black_hole(struct galaxy *G, size_t ind);

void upgrade_shield(const struct galaxy *G, size_t ind_p, size_t ind_s,
const size_t val_upg);

void expand_shield(const struct galaxy *G, size_t ind_p, size_t val_shld);

void remove_shield(const struct galaxy *G, size_t ind_p, size_t ind_s);

void collide(struct galaxy *G, size_t ind1, size_t ind2);

void rotate(const struct galaxy *G, size_t ind, char sens, size_t nr);

void show_info(const struct galaxy *G, const size_t indice);

void execute(struct galaxy *G, const char cmd[]);

#endif  // FUNCTION_H_
