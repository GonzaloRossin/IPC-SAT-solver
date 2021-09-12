#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#define SHRD_MEM_OBJ "/SHM"
#define BLOCK_SIZE 4096
#define SYS_FAILURE (-1)

typedef struct {
    char name[FILENAME_MAX];
    int fd;
    int size;
    int rIndex;
    int wIndex;
    char* address;
} shmem_t;

shmem_t createSharedMem(char* name, int size);
void writeSharedMem(shmem_t *shmem, char* buffer, int size, int offset);
void deleteSharedMem(shmem_t *shmem);
shmem_t joinSharedMem(char* name, int size);
char *readSharedMem(shmem_t *shmem);
void closeSharedMed(shmem_t *shmem);


void init_semaphore(sem_t *semaphore, int pshared, unsigned int value);
void wait_semaphore(sem_t *semaphore);
void post_semaphore(sem_t *semaphore);
void destroy_semaphore(sem_t* semaphore);

#define HANDLE_ERROR(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)             \

#endif