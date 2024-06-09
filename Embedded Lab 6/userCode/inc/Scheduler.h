#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MaxTasks 5

typedef void (*TaskFunction)(void);

typedef struct {
    TaskFunction function;
    uint16_t interval;
    uint16_t counter;
} Task;

extern void initScheduler(void);
extern void addTaskToScheduler(TaskFunction function, uint16_t interval);
extern void runScheduler(void);

#endif /* SCHEDULER_H */
