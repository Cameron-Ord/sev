#ifndef MAPS_H
#define MAPS_H

#define TABLE_BASE_CAPAC 256

// There is repition here but its small so I don't care for now -- it keeps things explicitly seperate.
#include "typedefs.h"
#include "errdef.h"

typedef struct SDL_Texture SDL_Texture;
typedef struct buf_inst buf_inst;

struct tex_node_ret {

};

struct tex_map_ret {

};

struct buf_node_ret {

};

struct buf_map_ret {

};

struct tex_map {
    u32 elements;
    u32 capacity;
    struct tex_node **nodes;
};

struct buf_map {
    u32 elements;
    u32 capacity;
    struct buf_node **nodes;
};

//Values in these nodes are heap allocated and are the owners of the pointers
//after they are initially created and assigned -- so they must also be freed when the map is freed.
struct tex_node {
    i32 key;
    SDL_Texture *value;
    struct tex_node *next;
};

struct buf_node {
    i32 key;
    buf_inst *value;
    struct buf_node *next;
};

//USING KNUTHS THE CAPACITY **MUST** ALWAYS BE A POWER OF 2
u32 knuth_hash_i32(i32 val, u32 table_capacity);

// TEXTURE MAP
struct tex_map *tex_map_init(u32 capacity);
struct tex_node *tex_node_create(i32 key, SDL_Texture *value);
const struct tex_node *tex_map_insert(struct tex_map *m, i32 key, SDL_Texture *value);
SDL_Texture *tex_map_search(struct tex_map *m, i32 key);

bool tex_map_delete(struct tex_map *m, i32 key);
void* tex_map_free(struct tex_map *m);

// BUFFER MAP
struct buf_map *buf_map_init(u32 capacity);
struct buf_node *buf_node_create(i32 key, buf_inst *value);
const struct buf_node *buf_map_insert(struct buf_map *m, i32 key, buf_inst *value);
buf_inst *buf_map_search(struct buf_map *m, i32 key);

bool buf_map_delete(struct buf_map *m, i32 key);
void* buf_map_free(struct buf_map *m);

#endif // MAPS_H