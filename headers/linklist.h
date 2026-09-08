// linklist.h

#ifndef LINKLIST_H
#define LINKLIST_H

#include <stddef.h> //size_t

typedef struct linklist_t linklist_t;

#ifndef DSA_LL_NODE_DEFINED
#define DSA_LL_NODE_DEFINED
typedef struct ll_node ll_node;
struct ll_node {
        void    *data;
        struct ll_node *next;
};
#endif

struct linklist_t {
        ll_node *head;
        size_t  elem_sz;
        void (*__free_ele)(void *);
};

int 	ll_init(linklist_t *l, size_t elem_sz, void (*free_ele)(void *));
void 	ll_free(linklist_t *l);

int	ll_isempty(const linklist_t *l);
size_t	ll_size(const linklist_t *l);

int 	ll_insert(linklist_t *l, size_t i, const void *elem);
int	ll_inserthead(linklist_t *l, const void *elem);
int 	ll_inserttail(linklist_t *l, const void *elem);

int	ll_delete(linklist_t *l, size_t i, void *out_elem);
int	ll_deletehead(linklist_t *l, void *out_elem);
int	ll_deletetail(linklist_t *l, void *out_elem);

int 	ll_getelem(const linklist_t *l, size_t i, void *out_elem);
int 	ll_gethead(const linklist_t *l, void *out_elem);
int	ll_gettail(const linklist_t *l, void *out_elem);

#endif
