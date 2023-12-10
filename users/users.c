



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

void User_delete(User *user){
    resource.destroy(user);
}