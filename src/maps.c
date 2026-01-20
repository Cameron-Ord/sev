#include "maps.h"
#include "util.h"
#include <math.h>
#include <stdlib.h>

u32 knuth_hash_i32(i32 val, i32 table_capacity){
    const i32 p = log2((f64)table_capacity);
    const u32 knu = 2654435769;
    const u32 uval = val;
    return (uval * knu) >> (32 - p);
}


// ==== TEXTURE MAP ====
void* tex_map_free(struct tex_map *m){
    for(i32 i = 0; i < m->capacity; i++){
        struct tex_node *node = m->nodes[i];
        if(node){
            sdl_texture_free(node->value);
            node->next = NULL;
        }
        sev_free(node);
        m->nodes[i] = NULL;
    }
    sev_free(m->nodes);
    sev_free(m);
    return NULL;
}


struct tex_map *tex_map_init(i32 capacity){
    struct tex_map *map = malloc(sizeof(struct tex_map));
    map->capacity = capacity;
    map->elements = 0;
    map->nodes = malloc(sizeof(struct tex_node *) * capacity);
    for(i32 i = 0; i < capacity; i++){
        map->nodes[i] = NULL;
    }
    return map;
}

struct tex_node *tex_node_create(i32 key, SDL_Texture *value){
    struct tex_node *node = malloc(sizeof(struct tex_node));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

bool tex_map_insert(struct tex_map *m, i32 key, SDL_Texture *value){

}

bool tex_map_delete(struct tex_map *m, i32 key){

}

SDL_Texture *tex_map_search(struct tex_map *m, i32 key){

}

// BUFFER MAP
struct buf_map *buf_map_init(i32 capacity){
    struct buf_map *map = malloc(sizeof(struct buf_map));
    map->capacity = capacity;
    map->elements = 0;
    map->nodes = malloc(sizeof(struct buf_node *) * capacity);
    for(i32 i = 0; i < capacity; i++){
        map->nodes[i] = NULL;
    }
    return map;
}

struct buf_node *buf_node_create(i32 key, buf_inst *value){
    struct buf_node *node = malloc(sizeof(struct buf_node));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

bool buf_map_insert(struct buf_map *m, i32 key, SDL_Texture *value){

}

bool buf_map_delete(struct buf_map *m, i32 key){

}

buf_inst *buf_map_search(struct buf_map *m, i32 key){

}

void* buf_map_free(struct buf_map *m){
    for(i32 i = 0; i < m->capacity; i++){
        struct buf_node *node = m->nodes[i];
        if(node){
            sev_free(node->value);
            node->next = NULL;
        }
        sev_free(node);
        m->nodes[i] = NULL;
    }
    sev_free(m->nodes);
    sev_free(m);
    return NULL;
}