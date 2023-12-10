#include "doTheWorld.h"
#include "clinput.h"
DtwResourceModule resource;
DtwNamespace dtw;


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



void User_set_email(User *user,const char *email){
    resource.set_string_in_sub_resource(user,email,"email");
}

void User_set_password(User *user,const char *password){
    char *generated_sha_password = dtw.generate_sha_from_string(password);
    resource.set_string_in_sub_resource(user,generated_sha_password,"password");
    free(generated_sha_password);
}

UserOrError User_create(Database *database, char *username,const char *password,const char *email){
    
    char *generated_sha = dtw.generate_sha_from_string(username);
    User *user_created = resource.sub_resource(database,generated_sha);

    if(resource.type(user_created) != DTW_NOT_FOUND){
        free(generated_sha);
        return (UserOrError){.error = USER_ALREADY_EXISTS};
    }

    resource.set_string_in_sub_resource(user_created,username,"username");

    User_set_password(user_created,password);
    User_set_email(user_created,email);

    free(generated_sha);
    return (UserOrError){.user = user_created};

}



char *User_get_email(User *user){
    return resource.get_string_from_sub_resource(user,"email");
}

char *User_get_password(User *user){
    return resource.get_string_from_sub_resource(user,"password");
}

char *User_get_username(User *user){
    return resource.get_string_from_sub_resource(user,"username");
}


UserOrError User_find(Database *database,const  char *username){
    char *generated_sha = dtw.generate_sha_from_string(username);
    User *user  = resource.sub_resource(database,generated_sha);

    if(resource.type(user) == DTW_NOT_FOUND){
        free(generated_sha);
        return (UserOrError){.error = USER_NOT_FOUND};
    }

    free(generated_sha);
    return (UserOrError){.user = user};
}



int  create_user(Database *database,CliInterface *interface){
    char *username = interface->ask_string(interface,"username",true);
    char *password = interface->ask_string(interface,"password",true);
    char *email = interface->ask_string(interface,"email",true);

    
    UserOrError user_created_or_error = User_create(database,username,password,email);

    bool modified = false;
    if(user_created_or_error.error == NO_ERROR){
        interface->print(interface,"User %s created\n",username);
        modified = true;
    }

    if(user_created_or_error.error == USER_ALREADY_EXISTS){
        interface->print(interface,"User %s already exists\n",username);
    }
    free(username);
    free(password);
    free(email);
    return modified;


}

void list_all_users(Database *database,CliInterface *interface){

    DtwResourceArray *users =  resource.sub_resources(database);
    for(int i = 0; i < users->size; i++){
        User *user = users->resources[i];
        char *username = User_get_username(user);
        char *email = User_get_email(user);
        char *password = User_get_password(user);
        DtwResource_protected(database){
            interface->print(interface,"------------------------------------\n");
            interface->print(interface,"%susername: %s%s\n",CLI_GREEN,CLI_BLUE, username);
            interface->print(interface,"%semail: %s%s\n",CLI_GREEN,CLI_BLUE, email);
            interface->print(interface,"%spassword: %s%s\n",CLI_GREEN,CLI_BLUE,password);

        }
    }
    


}



int main(void){
    dtw = newDtwNamespace();
    resource = newDtwResourceModule();
    
    Database *database = resource.newResource("database");
    CliInterface  interface = newCliInterface();

    int action =  interface.ask_option(&interface,"type the action","create | list | find | delete");
    const int CREATE =0;
    const int LIST =1;
    const int FIND =2;
    const int DELETE =3;

    bool modified = false;

    if(action == CREATE){
       modified= create_user(database,&interface);
    }

    if(action == LIST){
        list_all_users(database,&interface);
    }

     
    if(modified){
        bool commit  = interface.ask_option(&interface,"commit?","no | yes");
        if(commit){
            resource.commit(database);
        }
    }




    resource.free(database);
    return 0;
}