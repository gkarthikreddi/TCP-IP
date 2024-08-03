#ifndef __GLTHREADS__
#define __GLTHREADS__

typedef struct _glthread {
    struct _glthread *left;
    struct _glthread *right;
} glthread;

void glthread_add_next(glthread *base, glthread *node);
void glthread_add_before(glthread *base, glthread *node);
void glthread_add_last(glthread *base, glthread *node);
void remove_glthread(glthread *node);
void init_glthread(glthread *node);

#define IS_GLTHREAD_LIST_EMPTY(ptr)             \
    (ptr->right == 0 && ptr->left == 0)

#define GLTHREAD_TO_STRUCT(fn_name, struct_name, field_name)                                \
    static inline struct_name* fn_name(glthread *ptr) {                                     \
        return (struct_name *)((char *)(prt) - (char *)&(((struct_name *)0)->fieldname));   \
    }

#define BASE(ptr) (ptr->right)

#define ITERATE_GLTHREAD_BEGIN(start_ptr, ptr)      \
{                                                   \
    glthread *_glthread_ptr = NULL;                 \
    _glthread_ptr = BASE(start_ptr);                \
    for(; ptr; ptr = _glthread_ptr) {               \
        _glthread_ptr = ptr->right;

#define ITERATE_GLTHREAD_END(start_ptr, ptr) }}

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(ptr, offset)     \
    (void *)((char *)(ptr-offset))

void delete_glthread_lst(glthread *start_ptr);
unsigned int get_glthread_lst_count(glthread *base);
void glthread_priority_insert(glthread *base, glthread *node, int (*comp_fn)(void *, void *), int offset);

#if 0
void * gl_thread_search(glthread *base,
        void *(*thread_to_struct_fn)(glthread *),
        void *key,
        int (*comparison_fn)(void *, void *));
#endif

#endif
