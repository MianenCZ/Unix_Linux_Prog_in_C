
#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


void worker_run();
void supply_run();

struct path
{
    char* s;
    // This macro does the magic to point to other nextarg
    TAILQ_ENTRY(path) nextpath;
};

typedef struct {
    //Or whatever information that you need
    int t_id;
    char* path;
    int index;
} t_arg;

pthread_mutex_t paths_mutex;
pthread_cond_t paths_condvar;
TAILQ_HEAD(paths, path) paths;
size_t qsize = 0;
bool path_done = false;

pthread_mutex_t result_mutex;
size_t zero_bytes = 0;
size_t f_processed = 0;
size_t f_skipped = 0;

pthread_mutex_t non_mutex;
pthread_mutex_t non_mutex2;

pthread_cond_t pool_condvar;
pthread_t* t_pool;
pthread_mutex_t pool_mutex;
int pool_report;

pthread_mutex_t tdebug_mutex;
int* tdebug;
int* tdebug2;

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <thread_count>\n", *argv);
        return 1;
    }
    int threads = (int) strtol(*(argv + 1), (char **)NULL, 10);
    if(errno != 0 && threads == 0 || threads <= 0)
    {
        fprintf(stderr,"%s IS NOT A VALID NUMBER\n", *(argv + 1));
    }

    pthread_mutex_lock(&pool_mutex);


    t_pool = malloc(sizeof(pthread_t) * threads);
    tdebug = malloc(sizeof(int) * threads);
    tdebug2 = malloc(sizeof(int) * threads);

    for (size_t i = 0; i < 16; i++)
    {
        tdebug2[i] = -1;
    }
    

    pthread_create(&(t_pool[0]), NULL, supply_run, NULL );
    for (int i = 1; i < threads; i++)
    {
        set_work(i);
        // pthread_create(&(t_pool[i]), NULL, worker_run, (void*)i );
    }
    struct path* e = NULL;

    // printf("[main] threads created\n");

    while(1)
    {
        // printf("[main] while starts\n", pool_report);
        // pthread_mutex_lock(&pool_mutex);
        pthread_cond_wait(&pool_condvar, &pool_mutex);
        // printf("[main] for [%d] pthread_cond_wait(&pool_condvar, &pool_mutex);\n", pool_report);
        pthread_join(t_pool[pool_report], NULL);
        int sret = set_work(pool_report);
        if(sret == -1)
        {
            pthread_join(t_pool[pool_report], NULL);
            // printf("[main]: t_pool[%d] joined\n", pool_report);
            threads--;
            // printf("[main]: remaining threads: %d\n", pool_report, threads);
        }
        if(threads == 0)
            break;
    }

    //There is only one thread now
    // printf("Precessed: %zu files (%zu skipped)\n", f_processed, f_skipped);
    // printf("Zero bytes: %zu\n", zero_bytes);
    return 0;
}

int set_work(int tid)
{
    // printf("[main]: int set_work(int tid = %d)\n", tid);
    int val = 0;
    pthread_mutex_lock(&paths_mutex);
    // printf("[main]: qsize = %zu\n", qsize);
    if(qsize > 0 || path_done == false)
    {
        if(qsize == 0)
        {
            // pthread_mutex_unlock(&paths_mutex);
            // printf("[main]: for [%d] pthread_cond_wait(&paths_condvar, &non_mutex2);\n", tid);
            pthread_cond_wait(&paths_condvar, &paths_mutex);
            // pthread_mutex_lock(&paths_mutex);
        }

        struct path* e = NULL;
        e = TAILQ_FIRST(&paths);
        TAILQ_REMOVE(&paths, e, nextpath);
        --qsize;

        t_arg *args = malloc(sizeof *args);
        args->t_id = tid;
        args->path = e->s;
        args->index = f_processed;

        int iret = pthread_create(&(t_pool[tid]), NULL, worker_run, args );
        if(iret != 0)
            exit(145);
    }
    else
    {
        val = -1;
    }
    
    pthread_mutex_unlock(&paths_mutex);
    
    return val;
}

void supply_run()
{
    int thread_id = 0;
    TAILQ_INIT(&paths);
    while(1)
    // for (size_t i = 0; i < 1000; i++)
    {
        char* line = NULL;
        size_t len = 0;
        int c = getline(&line, &len, stdin);
        if(c == -1)
            break;
        line[c-1] = '\0';
        struct path* p = NULL;
        p = malloc(sizeof(struct path));
        p->s = line;
        pthread_mutex_lock(&paths_mutex);
        TAILQ_INSERT_TAIL(&paths, p, nextpath);        
        ++qsize;
        pthread_mutex_unlock(&paths_mutex);
        
        pthread_cond_signal(&paths_condvar);
        // printf("[%d]: pthread_cond_signal(&paths_condvar);\n",thread_id);
    }
    // printf("SUPPLY_END\n");
    path_done = true;

    // printf("[%d]: pthread_mutex_lock(&pool_mutex);\n",thread_id);
    pthread_mutex_lock(&pool_mutex);
    pool_report = thread_id;
    // printf("[%d]: pthread_cond_signal(&pool_condvar);\n",thread_id);
    pthread_cond_signal(&pool_condvar);
    pthread_mutex_unlock(&pool_mutex);
}

void worker_run(void *args)
{
    t_arg *actual_args = args;
    int thread_id = actual_args->t_id;
    char* path = actual_args->path;

    if(tdebug2[thread_id] == actual_args->index)
    {
        exit(145);
    }

    tdebug2[thread_id] = actual_args->index;


    int fd = open(path, O_RDONLY); //fd value is checked by cycle
    if(fd == -1)
    {
        exit(145);
    }
    size_t nul = 0;
    int err = fd;
    char buff[16384];
    while(err >= -1)
    {
        int r = read(fd, buff, 16384);

        pthread_mutex_lock(&tdebug_mutex);
        
        tdebug[thread_id] = r;
        printf("\r r=%d",r);
        for (size_t i = 0; i < 8; i++)
        {
            printf("[%d|%d]", tdebug2[i], tdebug[i]);
        }
        printf(" (%zu)                 ", qsize);
        fflush(stdout);               


        pthread_mutex_unlock(&tdebug_mutex);

        
        if(r == -1)
        {
            break;
        }
        else if (r == 0)
        {
            break;
        }
        else
        {
            for (size_t i = 0; i < 16384; i++)
            {
                nul += (buff[i] == 0)?1:0;
            }            
        }        
    }
    close(fd);
    // printf("[%d]: Work finished\n", thread_id);
    
    pthread_mutex_lock(&result_mutex);
    f_processed++;
    // printf("err= %d\n", err);
    f_skipped += (err)?1:0;
    zero_bytes += (err)?0:nul;

    pthread_mutex_unlock(&result_mutex);
    


    pthread_mutex_lock(&pool_mutex);    
    // printf("[%d]: pthread_mutex_lock(&pool_mutex);\n", thread_id);
    pool_report = thread_id;

    pthread_cond_signal(&pool_condvar);

    pthread_mutex_unlock(&pool_mutex);
    // printf("[%d]: return;\n",thread_id);
    return;
}
