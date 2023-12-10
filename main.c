#include "doTheWorld.h"
#include "clinput.h"
DtwResourceModule resource;
DtwNamespace dtw;

#include "users/users.h"
#include "actions/actions.h"

#include "users/users.c"
#include "actions/actions.c"



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

    if(action == FIND){
        find_user(database,&interface);
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