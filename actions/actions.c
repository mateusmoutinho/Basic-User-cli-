

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
        interface->warning(interface,"User %s already exists\n",username);
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

void find_user(Database *database,CliInterface *interface){
    char *username = interface->ask_string(interface,"username",true);
    UserOrError user_or_error = User_find(database,username);
    if(user_or_error.error == NO_ERROR){
        User *user = user_or_error.user;
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
    if(user_or_error.error == USER_NOT_FOUND){
        interface->warning(interface,"User %s not found\n",username);
    }
    free(username);

}

bool delete_user(Database *database,CliInterface *interface){
    char *username = interface->ask_string(interface,"username",true);
    UserOrError user_or_error = User_find(database,username);
    bool modified = false;
    if(user_or_error.error == NO_ERROR){
       User_delete(user_or_error.user);
       interface->print(interface,"User %s deleted\n",username); 
        modified= true;
    }

    if(user_or_error.error == USER_NOT_FOUND){
        interface->warning(interface,"User %s not found\n",username);
    }
    free(username);
    return modified;
}