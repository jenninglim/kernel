#include "hash/hash.h"

void HASH_INIT(pid_table_t * table) {
    for (int i = 0; i < SIZE; i++) {
        INIT_LIST_HEAD(&table->entry[i]);
    }
}

int hash_PID(pid_t pid) {
    return pid % SIZE;
}

task_t * find_task_PID(pid_table_t * table, pid_t pid) {
    list_head * node;
    task_t * current;
    list_head * head = &table->entry[hash_PID(pid)];
    list_for_each(node, head) {
        current = list_entry(node, pid_node_t, node)->task;
        if (current->pid == pid) {
            return current;
        }
    }
    return NULL;
}

void add_hash_entry(pid_table_t * table, task_t * entry) {
    pid_t pid = entry->pid;
    // Initialise PID
    pid_node_t * new_entry = malloc(sizeof(pid_node_t));

    new_entry->task = entry;
    INIT_LIST_HEAD(&new_entry->node);

    list_add_tail(&new_entry->node, &table->entry[hash_PID(pid)]);
}

void remove_head_entry(pid_table_t * table, pid_t pid) {
    return;
}

