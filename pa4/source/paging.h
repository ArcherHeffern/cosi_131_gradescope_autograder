#define PAGE_SIZE 8
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

struct PCB
{
    unsigned int pid;

    char ** page_table;
    unsigned int page_table_length;

    unsigned int limit; // The limit of the process
};

#define TLB_ENTRY_SIZE 4
struct TLB_ENTRY
{
    bool is_valid;
    unsigned int pid;
    unsigned int page_number;
    char *frame_addr;
    unsigned int time_stamp;
};

struct TLB{
    struct TLB_ENTRY *tlb;
    unsigned int hits;
    unsigned int misses;
};

void BuildPageTable(struct PCB *processes, unsigned int num_processes, char **lines, unsigned int *line_sizes);
char getDataTLB(struct PCB *processes, struct TLB *tlb, unsigned int pid, unsigned int address, unsigned int time_stamp);

void initTLB(struct TLB *tlb);
char *getTLB(struct TLB *tlb, unsigned int pid, unsigned int page_number, unsigned int time_stamp);
void addTLB(struct TLB *tlb, unsigned int pid, unsigned int page_number, char *frame_addr, unsigned int time_stamp);
