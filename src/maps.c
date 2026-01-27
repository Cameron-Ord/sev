#include "maps.h"
#include "errdef.h"
#include "util.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

u32 knuth_hash_i32(i32 val, u32 table_capacity) {
  const u32 p = log2u32(table_capacity);
  const u32 knu = 2654435769;
  const u32 uval = (u32)val;
  return (uval * knu) >> ((u32)32 - p);
}

// ==== TEXTURE MAP ====
void *tex_map_free(struct tex_map *m) {
  for (u32 i = 0; i < m->capacity; i++) {
    struct tex_node *node = m->nodes[i];
    if (node) {
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

// Enforce size is a power of 2 and greater than 0
struct tex_map_ret tex_map_init(u32 capacity) {
  assert(capacity > 0);
  assert((capacity & (capacity - 1)) == 0);
  struct mem_ret result = {NULL, UNSET};

  result = sev_malloc(sizeof(struct tex_map));
  if (result.err != OK) {
    return (struct tex_map_ret){NULL, result.err};
  }
  struct tex_map *map = (struct tex_map *)result.allocated;

  map->capacity = capacity;
  map->elements = 0;

  result = sev_malloc(sizeof(struct tex_node *) * capacity);
  if (result.err != OK) {
    sev_free(map);
    return (struct tex_map_ret){NULL, result.err};
  }
  // reinterpret
  map->nodes = (struct tex_node **)result.allocated;

  for (u32 i = 0; i < capacity; i++) {
    map->nodes[i] = NULL;
  }

  return (struct tex_map_ret){map, OK};
}

struct tex_node_ret tex_node_create(i32 key, SDL_Texture *value) {
  struct mem_ret result = {NULL, UNSET};
  // struct tex_node *node
  result = sev_malloc(sizeof(struct tex_node));
  if (result.err != OK) {
    return (struct tex_node_ret){NULL, result.err};
  }
  struct tex_node *node = (struct tex_node *)result.allocated;

  node->key = key;
  node->value = value;
  node->next = NULL;
  return (struct tex_node_ret){node, OK};
}

struct tex_node_ret tex_map_insert(struct tex_map *m, i32 key,
                                   SDL_Texture *value) {
  if (!value) {
    printf("Failed to insert: Bad Param\n");
    return (struct tex_node_ret){NULL, BAD_PARAM};
  }
  const u32 bucket = knuth_hash_i32(key, m->capacity);
  // struct tex_node *node
  struct tex_node_ret result = tex_node_create(key, value);
  if (result.err != OK) {
    return (struct tex_node_ret){NULL, result.err};
  }
  struct tex_node *node = result.node;

  printf("Inserting at bucket: %d\n", bucket);
  if (!m->nodes[bucket]) {
    m->nodes[bucket] = node;
  } else {
    node->next = m->nodes[bucket];
    m->nodes[bucket] = node;
  }

  return (struct tex_node_ret){node, OK};
}

bool tex_map_delete(struct tex_map *m, i32 key) {
  (void)m;
  (void)key;
  return false;
}

SDL_Texture *tex_map_search(struct tex_map *m, i32 key) {
  const u32 bucket = knuth_hash_i32(key, m->capacity);
  struct tex_node *head = m->nodes[bucket];
  while (head) {
    if (head->key == key) {
      return head->value;
    }
    head = head->next;
  }
  return NULL;
}

// BUFFER MAP
struct buf_map_ret buf_map_init(u32 capacity) {
  // Enforce size is a power of 2 and greater than 0
  assert(capacity > 0);
  assert((capacity & (capacity - 1)) == 0);
  struct mem_ret result = {NULL, UNSET};

  result = sev_malloc(sizeof(struct buf_map));
  if (result.err != OK) {
    return (struct buf_map_ret){NULL, result.err};
  }
  struct buf_map *map = (struct buf_map *)result.allocated;

  map->capacity = capacity;
  map->elements = 0;

  result = sev_malloc(sizeof(struct buf_node *) * capacity);
  if (result.err != OK) {
    sev_free(map);
    return (struct buf_map_ret){NULL, result.err};
  }
  // reinterpret
  map->nodes = (struct buf_node **)result.allocated;

  for (u32 i = 0; i < capacity; i++) {
    map->nodes[i] = NULL;
  }

  return (struct buf_map_ret){map, OK};
}

struct buf_node_ret buf_node_create(i32 key, struct buf_inst *value) {
  struct mem_ret result = {NULL, UNSET};
  result = sev_malloc(sizeof(struct buf_node));
  if (result.err != OK) {
    return (struct buf_node_ret){NULL, result.err};
  }
  struct buf_node *node = (struct buf_node *)result.allocated;

  node->key = key;
  node->value = value;
  node->next = NULL;

  return (struct buf_node_ret){node, OK};
}

struct buf_node_ret buf_map_insert(struct buf_map *m, i32 key,
                                   struct buf_inst *value) {
  if (!value) {
    printf("Failed to insert: Bad Param\n");
    return (struct buf_node_ret){NULL, BAD_PARAM};
  }
  const u32 bucket = knuth_hash_i32(key, m->capacity);
  struct buf_node_ret result = buf_node_create(key, value);
  if (result.err != OK) {
    return (struct buf_node_ret){NULL, result.err};
  }
  struct buf_node *node = result.node;

  printf("Inserting at bucket: %d\n", bucket);
  if (!m->nodes[bucket]) {
    m->nodes[bucket] = node;
  } else {
    node->next = m->nodes[bucket];
    m->nodes[bucket] = node;
  }

  return (struct buf_node_ret){node, OK};
}

bool buf_map_delete(struct buf_map *m, i32 key) {
  (void)m;
  (void)key;
  return false;
}

struct buf_inst *buf_map_search(struct buf_map *m, i32 key) {
  const u32 bucket = knuth_hash_i32(key, m->capacity);
  struct buf_node *head = m->nodes[bucket];
  while (head) {
    if (head->key == key) {
      return head->value;
    }
    head = head->next;
  }
  return NULL;
}

void *buf_map_free(struct buf_map *m) {
  for (u32 i = 0; i < m->capacity; i++) {
    struct buf_node *node = m->nodes[i];
    if (node) {
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
