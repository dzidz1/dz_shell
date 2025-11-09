#include "dz_shell.h"


// cd, cd [path], cd - (prev dir), cd ~, cd .., handling non existing dirs and permission issues
int command_cd(char** args, char* initial_directory) 
{
    (void) initial_directory; // not needed yet
    if(args[1] == NULL) {
        printf("cd expects path as an argument\n");
    } else if(chdir(args[1]) == 0) {
        printf("CD worked\n");
    } else {
        perror("CD failed");
    }
    return 0;
}


int command_pwd() 
{
    char* cwd = getcwd(NULL, 0);

    if(cwd == NULL) {
        perror("PWD failed");
    } else {
        printf("%s\n", cwd);
    }
    free(cwd);
    
    return 0;
}

// echo hello, echo $PATH, echo -n hello
int command_echo(char** args, char** env)
{
    (void) env;
    if(args[1] == NULL) {
        printf("\n");
    } else {
        char no_newline = 0;
        size_t i = 1;
        if(my_strcmp(args[1], "-n") == 0) {
            no_newline = 1;
            i = 2;
        }

        for(; args[i] != NULL; i++) {
            if(args[i][0] == '$') {
                char* variable_value = my_getenv(&args[i][1], env);
                if(variable_value) {
                    printf("%s", variable_value);
                } 
            } else {
                printf("%s", args[i]);
            }
            
            if(args[i + 1] != NULL) {
                printf(" ");
            }
        }

        if(no_newline == 0) {
            printf("\n");
        }
    }

    return 0;
}


int command_env(char** env);
int command_which(char** args, char** env);

char** command_setenv(char** args, char** env);
char** command_unsetenv(char** args, char** env);