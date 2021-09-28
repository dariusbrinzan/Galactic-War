// Copyright 2021 - 312CA Brinzan Darius-Ionut
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "function.h"

#define MAX_STR 32

// functie pentru erori
size_t out_of_bounds(size_t ind, size_t size, size_t planet){
    if (ind >= size){
        if (planet)
            printf("Planet out of bounds!\n");
        else
            printf("Shield out of bounds!\n");
        return 1;
    }
    return 0;
}
// functia pentru a da free planetelor
void free_planet(struct planet *p){
    struct shield *s = p->first_shield;
    do{
        struct shield *tmp = s;
        s = s->next;
        free(tmp);
    }while(s != p->first_shield);
    free(p->nume);
    free(p);
}
// functia pentru a sterge planeta
void delete_planet(struct galaxy *G, struct planet *p){
    if (G->nr_planets == 1){
        free_planet(p);
        G->nr_planets = 0;
        G->first = NULL;
        return;
    }

    if (G->first == p){
        G->first = p->next;
    }

    p->prev->next = p->next;
    p->next->prev = p->prev;
    free_planet(p);
    --G->nr_planets;
}
// functia pentru a adauga elemente in lista noastra
void add(struct galaxy *G, char *nume, size_t ind, const size_t nr_scuturi){
    if (out_of_bounds(ind, G->nr_planets + 1, 1)){
        free(nume);
        return;
    }
    // aloc shield-urile si le initializez pe toate cu valoarea 1
    struct shield *first_scut = (struct shield*) malloc(sizeof(struct shield));
    first_scut->val = 1;
    struct shield *last_scut = first_scut;
    for ( size_t i = 1 ; i < nr_scuturi ; ++i ){
        struct shield *new_s = (struct shield*) malloc(sizeof(struct shield));
        new_s->val = 1;
        last_scut->next = new_s;
        last_scut = new_s;
    }
    last_scut->next = first_scut;
    // aloc planeta si o initializez corespunzator
    struct planet *new_p = (struct planet *) malloc(sizeof(struct planet));
    new_p->first_shield = first_scut;
    new_p->last_shield = last_scut;
    new_p->nr_shields = nr_scuturi;
    new_p->kills = 0;
    new_p->nume = nume;
    // incrementez numarul planetelor din galaxie
    ++G->nr_planets;
    printf("The planet %s has joined the galaxy.\n", nume);

    // conditie daca e prima planeta din galaxie

    if (G->nr_planets == 1){
        new_p->next = new_p;
        new_p->prev = new_p;
        G->first = new_p;
        return;
    }
    // daca este prima planeta trebuie modificat si g->first
    if (ind == 0){
        new_p->next = G->first;
        new_p->prev = G->first->prev;
        G->first->prev->next = new_p;
        G->first->prev = new_p;
        G->first = new_p;
        return;
    }
    // adauga planeta in galaxie ( o leaga la lista )
    struct planet *curr = G->first->prev;
    while (ind){
        curr = curr->next;
        --ind;
    }

    new_p->next = curr->next;
    new_p->prev = curr;
    curr->next->prev = new_p;
    curr->next = new_p;
}
// functia de remove a planetei din galaxie
void black_hole(struct galaxy *G, size_t ind){
    if (out_of_bounds(ind, G->nr_planets, 1)) return;

    struct planet *p = G->first;
    while (ind){
        p = p->next;
        --ind;
    }
    printf("The planet %s has been eaten by the vortex.\n", p->nume);
    delete_planet(G, p);
}
// functia de upgrade a scutului planetei
void upgrade_shield(const struct galaxy *G, size_t ind_p, size_t ind_s,
const size_t val_upg){
    if (out_of_bounds(ind_p, G->nr_planets, 1)) return;
    // parcurg lista de planete
    struct planet *p = G->first;
    while (ind_p){
        p = p->next;
        --ind_p;
    }

    if (out_of_bounds(ind_s, p->nr_shields, 0)) return;
    // parcurg lista de shield-uri
    struct shield *s = p->first_shield;
    while (ind_s){
        s = s->next;
        --ind_s;
    }
    // upgradez valoarea shield-ului
    s->val += val_upg;
}
// functia pentru a mari scutul planetei
void expand_shield(const struct galaxy *G, size_t ind_p, size_t val_shld){
    if (out_of_bounds(ind_p, G->nr_planets, 1)) return;
    // parcurg lista de planete
    struct planet *p = G->first;
    while (ind_p){
        p = p->next;
        --ind_p;
    }
    // adaug un shield nou la finalul listei
    struct shield *new_s = (struct shield*) malloc(sizeof(struct shield));
    new_s->val = val_shld;
    new_s->next = p->first_shield;
    p->last_shield->next = new_s;
    p->last_shield = new_s;
    ++p->nr_shields;
}
// functia de remove a scutului
void remove_shield(const struct galaxy *G, size_t ind_p, size_t ind_s){
    if (out_of_bounds(ind_p, G->nr_planets, 1)) return;
    // parcurg lista de planete
    struct planet *p = G->first;
    while (ind_p){
        p = p->next;
        --ind_p;
    }

    if (out_of_bounds(ind_s, p->nr_shields, 0)) return;
    // conditie daca o planeta are mai putin de 4 shield-uri
    if (p->nr_shields == 4){
        printf("A planet cannot have less than 4 shields!\n");
        return;
    }
    // parcurg shield-urile
    struct shield *s = p->first_shield, *prev = p->last_shield;
    while (ind_s){
        prev = s;
        s = s->next;
        --ind_s;
    }
    // leg vecinii intre ei
    prev->next = s->next;
    if (s == p->first_shield){
        p->first_shield = p->first_shield->next;
    }
    if (s == p->last_shield){
        p->last_shield = prev;
    }
    free(s);
    --p->nr_shields;
}
// functia de ciocnire a planetelor
void collide(struct galaxy *G, size_t ind1, size_t ind2){
    if (out_of_bounds(ind1, G->nr_planets, 1)) return;
    if (out_of_bounds(ind2, G->nr_planets, 1)) return;
    // parcurg planetele
    struct planet *p = G->first;
    while (ind1){
        p = p->next;
        --ind1;
    }
    // initializez indicii si parcurg ambele liste
    int ind_s1 = p->nr_shields / 4, ind_s2 = p->next->nr_shields / 4 * 3;
    struct shield *s1 = p->first_shield, *s2 = p->next->first_shield;
    while (ind_s1){
        s1 = s1->next;
        --ind_s1;
    }
    while (ind_s2){
        s2 = s2->next;
        --ind_s2;
    }
    // conditii pentru fiecare caz de colision
    if (s1->val == 0 && s2->val){
        printf("The planet %s has imploded.\n", p->nume);
        ++p->next->kills;
        delete_planet(G, p);
        --s2->val;
    } else if (s1->val && s2->val == 0){
        printf("The planet %s has imploded.\n", p->next->nume);
        ++p->kills;
        delete_planet(G, p->next);
        --s1->val;
    } else if (s1->val == 0 && s2->val == 0){
        printf("The planet %s has imploded.\n", p->nume);
        printf("The planet %s has imploded.\n", p->next->nume);
        delete_planet(G, p->next);
        delete_planet(G, p);
    } else {
        --s1->val;
        --s2->val;
    }
}
// functia de rotire a planetei : sens trigonometric + acelor de ceasornic
void rotate(const struct galaxy *G, size_t ind, char sens, size_t nr){
    if (out_of_bounds(ind, G->nr_planets, 1)) return;
    // parcurg lista de planete
    struct planet *p = G->first;
    while (ind){
        p = p->next;
        --ind;
    }

    nr %= p->nr_shields;
    // conditii pentru rotatia in ambele sensuri
    if (sens == 'c'){
        nr = p->nr_shields - nr;
    } else if (sens != 't'){
        printf("Not a valid direction!\n");
        return;
    }

    while (nr){
        if (nr == 1){
            p->last_shield = p->first_shield;
        }
        p->first_shield = p->first_shield->next;
        --nr;
    }
}
// functia de afisare a informatiilor despre respectiva planeta
void show_info(const struct galaxy *G, const size_t indice){
    if ( out_of_bounds(indice, G->nr_planets, 1)) return;

    struct planet *p = G->first;
    for ( size_t i = 0 ; i < indice ; ++i ){
        p = p->next;
    }

    printf("NAME: %s\n", p->nume);

    if ( G->nr_planets == 1)
        printf("CLOSEST: none\n");
    else if ( G->nr_planets == 2)
        printf("CLOSEST: %s\n", p->prev->nume);
    else
        printf("CLOSEST: %s and %s\n", p->prev->nume, p->next->nume);


    printf("SHIELDS: ");
    struct shield *s = p->first_shield;
    for ( size_t i = 0;  i < p->nr_shields ; ++i ){
        printf("%zu ", s->val);
        s = s->next;
    }
    printf("\n");

    printf("KILLED: %zu\n", p->kills);
}
// functia care ne ajuta sa executam programul nostru
void execute(struct galaxy *G, const char cmd[]){
    if (!strcmp(cmd, "ADD")){
        char *nume = (char *) malloc(MAX_STR);
        size_t ind, nr_sct;
        scanf("%s %zu %zu", nume, &ind, &nr_sct);
        add(G, nume, ind, nr_sct);
    } else if (!strcmp(cmd, "BLH")){
        size_t ind;
        scanf("%zu", &ind);
        black_hole(G, ind);
    } else if (!strcmp(cmd, "UPG")){
        size_t ind_p, ind_s, val_upg;
        scanf("%zu %zu %zu", &ind_p, &ind_s, &val_upg);
        upgrade_shield(G, ind_p, ind_s, val_upg);
    } else if (!strcmp(cmd, "EXP")){
        size_t ind, val;
        scanf("%zu %zu", &ind, &val);
        expand_shield(G, ind, val);

    } else if (!strcmp(cmd, "RMV")){
        size_t ind_p, ind_s;
        scanf("%zu %zu", &ind_p, &ind_s);
        remove_shield(G, ind_p, ind_s);

    } else if (!strcmp(cmd, "COL")){
        size_t ind1, ind2;
        scanf("%zu %zu", &ind1, &ind2);
        collide(G, ind1, ind2);

    } else if (!strcmp(cmd, "ROT")){
        size_t ind, nr;
        char sens;
        scanf("%zu %c %zu", &ind, &sens, &nr);
        rotate(G, ind, sens, nr);
    } else if (!strcmp(cmd, "SHW")){
        size_t ind;
        scanf("%zu", &ind);
        show_info(G, ind);
    }
}

int main(void)
{   struct galaxy G = {0, NULL};

    size_t nr_comenzi;
    scanf("%zu", &nr_comenzi);

    char cmd[4];
    cmd[3] = '\0';
    for (size_t i = 0; i < nr_comenzi; ++i){
        scanf("%3s", cmd);
        execute(&G, cmd);
    }

    while (G.nr_planets){
        struct planet *tmp = G.first;
        G.first = G.first->next;
        free_planet(tmp);
        --G.nr_planets;
    }

    return 0;
}
