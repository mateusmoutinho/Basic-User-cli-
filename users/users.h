

typedef enum {
    NO_ERROR,
    USER_ALREADY_EXISTS,
    USER_NOT_FOUND,
}database_error;

typedef DtwResource User;

typedef struct {
    User *user;
    database_error error;
} UserOrError;


typedef DtwResource Database;


void User_set_email(User *user,const char *email);

void User_set_password(User *user,const char *password);

UserOrError User_create(Database *database, char *username,const char *password,const char *email);


char *User_get_email(User *user);

char *User_get_password(User *user);
char *User_get_username(User *user);


UserOrError User_find(Database *database,const  char *username);

void User_delete(User *user);