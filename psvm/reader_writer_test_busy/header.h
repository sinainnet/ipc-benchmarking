#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdatomic.h>

struct Data {
  atomic_int state;
};

#define DATA_SIZE (sizeof(struct Data))

#endif  /* PROTOCOL_H */