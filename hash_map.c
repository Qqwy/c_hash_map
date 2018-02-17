#include <stdio.h>
#include "hash_map.h"

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
