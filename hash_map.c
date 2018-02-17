#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct {
  size_t size;
  size_t capacity;
  void **keys;
  void **values;
} HashTable;


HashTable *HashTable_new(size_t const capacity) {
  HashTable *hash_table = malloc(sizeof(HashTable));
  hash_table->capacity = capacity;
  hash_table->size = 0;
  hash_table->keys = calloc(capacity, sizeof(void *));
  hash_table->values = calloc(capacity, sizeof(void *));

  return hash_table;
}

void HashTable_destroy(HashTable *hash_table) {
  free(hash_table->values);
  free(hash_table->keys);
  free(hash_table);
}

size_t HashTable_index (HashTable const *hash_table, void *const key) {
  size_t index = ((size_t) key) % hash_table->capacity; // TODO Hashing function!
  while (hash_table->keys[index] && hash_table->keys[index] != key) {
    index = (index + 1) % hash_table->capacity;
  }
  return index;
}

void HashTable_insert(HashTable *hash_table, void *const key, void *const value) {
  size_t const index = HashTable_index(hash_table, key);
  hash_table->keys[index] = key;
  hash_table->values[index] = value;
  // TODO resizing.

  ++(hash_table->size);
  if(hash_table->size * 4 >= hash_table->capacity * 3) { // 3/4 maximum load factor.
    HashTable *larger_table = HashTable_new(hash_table->capacity << 2);
    for(size_t iter_index = 0; iter_index < hash_table->capacity; ++iter_index) {
      if(hash_table->keys[iter_index] != NULL){
        HashTable_insert(larger_table, hash_table->keys[iter_index], hash_table->values[iter_index]);
      }
    }
    free(hash_table->values);
    free(hash_table->keys);
    HashTable *tmp = hash_table;
    *hash_table = *larger_table;
    HashTable_destroy(tmp);
  }
}

void *HashTable_lookup(HashTable *hash_table, void *const key) {
  size_t const index = HashTable_index(hash_table, key);
  return hash_table->values[index];
}


int main() {
  HashTable *hash_table = HashTable_new(10);
  HashTable_insert(hash_table, "hello", "world");
  HashTable_insert(hash_table, "foo", "bar");
  HashTable_insert(hash_table, "foo2", "bar2");
  HashTable_insert(hash_table, "foo3", "bar3");
  HashTable_insert(hash_table, "foo4", "bar4");
  HashTable_insert(hash_table, "foo5", "bar5");
  printf("hello => %s\n", HashTable_lookup(hash_table, "hello"));
  printf("foo => %s\n", HashTable_lookup(hash_table, "foo"));
  printf("foo2 => %s\n", HashTable_lookup(hash_table, "foo2"));
  printf("foo3 => %s\n", HashTable_lookup(hash_table, "foo3"));
  printf("foo4 => %s\n", HashTable_lookup(hash_table, "foo4"));
  printf("foo5 => %s\n", HashTable_lookup(hash_table, "foo5"));
  HashTable_destroy(hash_table);
}
