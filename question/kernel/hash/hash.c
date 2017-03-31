#include "hash/hash.h"

void HASH_INIT(pid_table_t * table) {
    for (int i = 0; i < SIZE; i++) {
        INIT_LIST_HEAD(&table->entry[i]);
    }
}

int hash_PID(pid_t pid) {
    return pid % SIZE;
}

pid_node_t * find_node_pid(pid_table_t * table, pid_t pid) {
    list_head * node;
    pid_node_t * current;
    list_head * head = &table->entry[hash_PID(pid)];
    list_for_each(node, head) {
        current = list_entry(node, pid_node_t, node);
        if (current->task->pid == pid) {
            return current;
        }
    }
    return NULL;
}

task_t * find_task_pid(pid_table_t * table, pid_t pid) {
    return find_node_pid(table, pid)->task;
}

void add_hash_entry(pid_table_t * table, task_t * entry) {
    pid_t pid = entry->pid;
    pid_node_t * new_entry = malloc(sizeof(pid_node_t));

    new_entry->task = entry;
    INIT_LIST_HEAD(&new_entry->node);

    list_add_tail(&new_entry->node, &table->entry[hash_PID(pid)]);
}

void remove_hash_entry(pid_table_t * table, pid_t pid) {
    pid_node_t * task = find_node_pid(table, pid);
    list_del(&task->node);
    free(task);
    return;

}

