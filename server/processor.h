#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "workqueue.h"

struct processor {
	struct workqueue *input;
	struct workqueue *output;
};

void processor_init(struct processor *p, struct workqueue *i, struct workqueue *o);
int processor_execute(struct processor *p);

#endif
