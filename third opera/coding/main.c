#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "init.h"

#define MAX_USERS 100

typedef struct {
	char username[50];
	char password[50];
}User;

void create_and_store_account(User users[], int* num_users){
	if(*num_users>=MAX_USERS){
		printf("linmited.\n");
		return;
	}

	User new_user;
	printf("printf name: ");
	scanf("%s", new_user.username);
	printf("printf coding: ");
	scanf("%s",new_user.password);

	users[*num_users] =new_user;
	(*num_users)++;
	printf("success!\n");
}

int login(User users[],int num_users, User* current_user){
	char input_username[50], input_password[50];
	printf("printf name: ");
	scanf("%s", input_username);
	printf("printf coding");
	scanf("%s", input_password);

	for(int i=0;i<num_users;i++){
		if(strcmp(input_username,users[i].username)==0&&strcmp(input_password,users[i].password)==0){
				*current_user=users[i];
				return 1;
		}
	}
	return 0;
}

User users[MAX_USERS];
User current_user;
int num_users =0;
int is;

char current_path[256];
int main(int argc,char **argv){
    if(num_users==0){
    	create_and_store_account(&users,&num_users);
    }

    char command[10],temp[9];
    initialize_memory();
    while(1)
    {
	if(is==1){
		;
	}
	else if(is=login(users,num_users,&current_user)&&is==0){
		
	}
	else{
		printf("wrong\n");
		continue;
	}

        printf("%s]#",current_path);
        scanf("%s",command);
       
        if(!strcmp(command,"cd"))
        {
            scanf("%s",temp);
            cd(temp);
        }
        else if(!strcmp(command,"created"))
        {
            scanf("%s",temp);
            mkdir(temp,2);
        }
        else if(!strcmp(command,"createc"))
        {
            scanf("%s",temp);
            cat(temp,1);
        }

        else if(!strcmp(command,"deleted"))
        {
            scanf("%s",temp);
            rmdir(temp);
        }
        else if(!strcmp(command,"deletec"))
        {
            scanf("%s",temp);
            del(temp);
        }
        else if(!strcmp(command,"open"))
        {
            scanf("%s",temp);
            open_file(temp);
        }
        else if(!strcmp(command,"close"))
        {
            scanf("%s",temp);
            close_file(temp);
        }
        else if(!strcmp(command,"read"))
        {
            scanf("%s",temp);
            read_file(temp);
        }
        else if(!strcmp(command,"write"))
        {
            scanf("%s\n",temp);
            write_file(temp);
        }
        else if(!strcmp(command,"ls"))
        {
        	ls();
        }
        else if(!strcmp(command,"format"))
        {
            char tempch;
            printf("Format will erase all the data in the Disk\n");
            printf("Are you sure?y/n:\n");
            fflush(stdin);
            scanf(" %c",&tempch);
            if(tempch=='Y'||tempch=='y')
            {
                format();
            }
            else
            {
            	printf("Format Disk canceled\n");
            }
        }
        else if(!strcmp(command,"ckdisk"))
        {
        	check_disk();
        }
        else if(!strcmp(command,"quit"))
        {
        	break;
        }
        else printf("No this Command,Please check!\n");
        getchar();
        //while((getchar())!='\n');
    }
    return 0;
}
