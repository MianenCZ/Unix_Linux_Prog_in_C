
#define _POSIX_C_SOURCE 200809L

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
TAILQ_HEAD(paths, path) paths;
size_t qsize = 0;

size_t zero_bytes = 0;
size_t f_processed = 0;
size_t f_skipped = 0;


int main(int argc, char const *argv[])
{
    supply_run();

    struct path* e = NULL;





    while(qsize > 0)
    {
        
    }





    printf("Precessed: %zu files (%zu skipped)\n", f_processed, f_skipped);
    printf("Zero bytes: %zu\n", zero_bytes);
    return 0;
}

void supply_run()
{
    TAILQ_INIT(&paths);
    while(1)
    // for (size_t i = 0; i < 10; i++)
    {
        char* line = NULL;
        size_t len = 0;
        int c = getline(&line, &len, stdin);
        if(c == -1)
            return;
        line[c-1] = '\0';
        struct path* p = NULL;
        p = malloc(sizeof(struct path));
        p->s = line;
        TAILQ_INSERT_TAIL(&paths, p, nextpath);        
        ++qsize;
    }
}

void worker_run()
{
    struct path* e = NULL;
    e = TAILQ_FIRST(&paths);
    TAILQ_REMOVE(&paths, e, nextpath);
    --qsize;
    int fd = open(e->s, O_RDONLY);
    if(fd == -1)
    {
        printf("fd error: %s\n", strerror(errno));
        ++f_skipped;
        ++f_processed;
        return;
    }
    size_t nul = 0;
    while(1)
    {
        char buff[64];
        int r = read(fd, buff, 64);
        if(r == -1)
        {
            ++f_skipped;
            ++f_processed;
            break;
        }
        else if (r == 0)
        {
            zero_bytes += nul;
            ++f_processed;
            break;
        }
        else
        {
            for (size_t i = 0; i < 64; i++)
            {
                nul += (buff[i] == 0)?1:0;
            }            
        }        
    }
    close(fd);
}
