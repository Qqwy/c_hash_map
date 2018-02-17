#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct {
  size_t size;
  size_t capacity;
  void **keys;
  void **values;
} HashMap;


HashMap *HashMap_new(size_t capacity) {
  capacity = capacity > 2 ? capacity : 2;
  HashMap *hash_map = calloc(1, sizeof(HashMap));
  hash_map->capacity = capacity;
  hash_map->size = 0;
  hash_map->keys = calloc(capacity, sizeof(void *));
  hash_map->values = calloc(capacity, sizeof(void *));

  return hash_map;
}

void HashMap_destroy(HashMap *hash_map) {
  free(hash_map->values);
  free(hash_map->keys);
  free(hash_map);
}

size_t HashMap_index (HashMap const *hash_map, void *const key) {
  size_t index = ((size_t) key) % hash_map->capacity; // TODO Hashing function!
  while (hash_map->keys[index] != NULL && hash_map->keys[index] != key) {
    /* printf("HTC: %lu, Not storing %s at key %lu because already filled by %s => %s\n", hash_map->capacity, key, index, hash_map->keys[index], hash_map->values[index]); */
    index = (index + 1) % hash_map->capacity;
  }
  return index;
}

void HashMap_insert(HashMap **hash_map_ref, void *const key, void *const value) {
  HashMap *hash_map = *hash_map_ref;
  size_t const index = HashMap_index(hash_map, key);
  /* printf("HTC: %lu, Inserting %s at index %lu\n", hash_map->capacity, key, index); */
  hash_map->keys[index] = key;
  if(hash_map->values[index] == NULL) // Table increases in size only if previous key was not replaced.
    ++(hash_map->size);
  hash_map->values[index] = value;

  if(hash_map->size * 4 >= hash_map->capacity * 3) { // 3/4 maximum load factor.
    printf("Resizing HashMap...\n");
    HashMap *larger_table = HashMap_new(hash_map->capacity << 1);
    for(size_t iter_index = 0; iter_index < hash_map->capacity; ++iter_index) {
      if(hash_map->keys[iter_index] != NULL){
        HashMap_insert(&larger_table, hash_map->keys[iter_index], hash_map->values[iter_index]);
      }
    }
    *hash_map_ref = larger_table;
    HashMap_destroy(hash_map);
    printf("Finished Resizing HashMap.\n");
  }
}

void *HashMap_lookup(HashMap *hash_map, void *const key) {
  size_t const index = HashMap_index(hash_map, key);
  return hash_map->values[index];
}

/* Testing function */
int main() {
  HashMap *hash_map = HashMap_new(1);
  HashMap_insert(&hash_map, "hello", "world");
  HashMap_insert(&hash_map, "foo", "bar");
  HashMap_insert(&hash_map, "foo2", "bar2");
  HashMap_insert(&hash_map, "foo3", "bar3");
  HashMap_insert(&hash_map, "foo4", "bar4");
  HashMap_insert(&hash_map, "foo5", "bar5");
  printf("hello => %s\n", HashMap_lookup(hash_map, "hello"));
  printf("foo => %s\n", HashMap_lookup(hash_map, "foo"));
  printf("foo2 => %s\n", HashMap_lookup(hash_map, "foo2"));
  printf("foo3 => %s\n", HashMap_lookup(hash_map, "foo3"));
  printf("foo4 => %s\n", HashMap_lookup(hash_map, "foo4"));
  printf("foo5 => %s\n", HashMap_lookup(hash_map, "foo5"));
  for(size_t index = 0; index < 1000; ++index) {
    HashMap_insert(&hash_map, (void *) index, (void *) index);
    printf("%lu => %lu\n", index, (size_t) HashMap_lookup(hash_map, (void *)index));
  }
  HashMap_destroy(hash_map);
}
