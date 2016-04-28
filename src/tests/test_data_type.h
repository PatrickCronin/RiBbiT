#ifndef TEST_DATA_TYPE
#define TEST_DATA_TYPE

struct _id_name {
    int id;
    char * name;   
};
typedef struct _id_name id_name;
typedef struct _id_name* t_id_name;

t_id_name id_name_init (void);
void id_name_free (t_id_name);
void id_name_setid (t_id_name, int);
int id_name_getid (t_id_name);
void id_name_setname (t_id_name, char *);
char * id_name_getname (t_id_name);
void id_name_display (t_id_name);
void * id_name_copy (t_id_name);
int id_name_compare (t_id_name, t_id_name);

#endif

