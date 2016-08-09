#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <libmemcached/memcached.h>

#define IP1  "vm1"
#define IP2  "vm2"
#define PORT 11211

void usage(void) {
    fprintf(stderr, "memcached_sample [get|set] [key] [value]\n");
    exit(0);
}

void add_server(memcached_st *memc, const char *IP) {
    memcached_return rc;
    memcached_server_st *servers = NULL;

    servers = memcached_server_list_append(servers, IP, PORT, &rc);
    rc = memcached_server_push(memc, servers);
    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Added server successfully\n");
    else
        fprintf(stderr, "Couldn't add server: %s\n",
                memcached_strerror(memc, rc));
}

memcached_st *add_memcache_server(void) {
    memcached_st *memc;

    memc = memcached_create(NULL);
    add_server(memc, IP1);
    add_server(memc, IP2);

    return memc;
}

void do_get(memcached_st *memc, char **argv) {
    memcached_return rc;
    char *retrieved_value;
    size_t value_length;
    uint32_t flags;

    retrieved_value = memcached_get(memc, argv[2], strlen(argv[2]), &value_length, &flags, &rc);

    if (rc == MEMCACHED_SUCCESS) {
        fprintf(stderr, "Key retrieved successfully\n");
        printf("The key '%s' returned value '%s'.\n", argv[2], retrieved_value);
        free(retrieved_value);
    }
    else
        fprintf(stderr, "Couldn't retrieve key: %s\n", memcached_strerror(memc, rc));
}

void do_set(memcached_st *memc, char **argv) {
    memcached_return rc;
    char *retrieved_value;
    size_t value_length;
    uint32_t flags;

    rc = memcached_set(memc, argv[2], strlen(argv[2]), argv[3],
                       strlen(argv[3]), (time_t)0, (uint32_t)0);

    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Key stored successfully\n");
    else
        fprintf(stderr, "Couldn't store key: %s\n", memcached_strerror(memc, rc));

    retrieved_value = memcached_get(memc, argv[2], strlen(argv[2]), &value_length, &flags, &rc);

    if (rc == MEMCACHED_SUCCESS) {
        fprintf(stderr, "Key retrieved successfully\n");
        printf("The key '%s' returned value '%s'.\n", argv[2], retrieved_value);
        free(retrieved_value);
    }
    else
        fprintf(stderr, "Couldn't retrieve key: %s\n", memcached_strerror(memc, rc));
}

void do_op(memcached_st *memc, char **argv) {
    if (!strcmp(argv[1], "get")) {
        do_get(memc, argv);
    } else if (!strcmp(argv[1], "set")) {
        do_set(memc, argv);
    } else {
        usage();
    }
}

int main(int argc, char **argv) {

    if (argc < 2) {
        usage();
    }

    memcached_st *memc;

    memc = add_memcache_server();

    do_op(memc, argv);

    return 0;
}
