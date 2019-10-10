#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void clearMyMem(char* username){                                                    //function to clear up shared memory
    char buffer[200];                                                                       //holds the line of the file
    char *word, lastWord[25], removeMem[50];                                      //word holds words seperated by spaces
    int count = 0;                                                                        //initialize action count to 0
    strcpy(removeMem, "ipcrm -m ");                               //copy the command to remove shared memory to a string
    remove("tempIPCSFile.txt");                                         //remove the temp file if it exists upon running
    system("ipcs >> tempIPCSFile.txt");                       //run the system command that pipes ipcs results into file
    FILE* fptr = fopen("tempIPCSFile.txt", "r");                                   //open the temp file we created above
    fgets(buffer, sizeof(buffer), fptr);                            //get the first line of the file that we do not need
    while(fgets(buffer, sizeof(buffer), fptr)) {                                      //go through the file line by line
        word = strtok(buffer, " ");                                         //tokenize the line of the file we retrieved
        while(word != NULL){                                                               //while the token has a value
            if(strcmp(username, word) == 0 ){                       //if the token is the same as the username we passed
                strcat(removeMem, lastWord);                         //concatenate the ID of memory to the command above
                system(removeMem);                                                       //run the command on the system
                count++;                                                   //increase the count because we did an action
            }
            strcpy(lastWord, word);                           //copy the last value we have which is used to hold the ID
            word = strtok(NULL, " ");
        }
    }
    if(count == 0){                                //if count did not increase, tell the user no shared memory was found
        printf("No shared memory found.\n");
    }
    else{                                                                      //otherwise report how much we cleared up
        printf("%d areas of shared memory cleared\n", count);
    }
    fclose(fptr);                                                                                //close out of the file
    remove("tempIPCSFile.txt");                                                                   //delete the temp file
}

void killMyChildren(char* command){                                       //function to kill child processes by its name
    char buffer[200];                                                                       //holds the line of the file
    char removePs[50], field1[32], field2[32], field3[32], field4[32];                  //fields hold each field of line
    int count = 0;                                                                               //initialize count to 0
    strcpy(removePs, "kill -9 ");                                                 //copy the killing command to a string
    remove("tempPSFile.txt");                                   //remove the temp file we will create if it exists prior
    system("ps >> tempPSFile.txt");                                               //pipe the output of ps to a temp file
    FILE* fptr = fopen("tempPSFile.txt", "r");                                           //open the temp file we created
    while(fgets(buffer, sizeof(buffer), fptr) != NULL){                               //go through the file line by line
        sscanf(buffer, "%s%s%s%s", field1, field2, field3, field4);                         //get each field on the line
        if(strcmp(field4, command) == 0){             //if the field that holds command names is the same as the process
            strcat(removePs, field1);                                   //concatenate it to the end of the command above
            system(removePs);                                       //run the command on the system that kills processes
            count++;                                                                         //increase the action count
        }
    }
    if(count == 0){                                       //if the count is 0 than report there were no processes killed
        printf("No processes found for that name.\n");
    }
    else{                                                              //otherwise report how many processes were killed
        printf("%d processes killed\n");
    }
    fclose(fptr);                                                                               //close the file pointer
    remove("tempPSFile.txt");                                                                     //remove the temp file
}

int main(int argc, char *argv[]){
    char userName[100], command[100];               //username holds the username, command holds the command user inputs
    int response, check = 0;          //response holds the user selection in menu, check allows us to control while loop
    printf("USER ID: ");
    while(check != 1){
        printf("---------------------------------------\n");
        printf("Select one of the following:\n");
        printf("1 Remove Shared Memory\n");
        printf("2 Kill open processes\n");
        printf("3 Exit\n");
        printf("Selection: ");
        scanf("%d", &response);
        printf("---------------------------------------\n");
        switch(response){
            case 1:
                printf("What is your Username or UMSL ID: ");
                scanf("%s", userName);                                                 //get the user's ID from the user
                clearMyMem(userName);                                            //calls function to clear shared memory
                break;
            case 2:
                printf("What is the name of your program that you think has open processes: ");
                scanf("%s", command);                                       //get the name of the program with the child
                killMyChildren(command);                                          //calls function to kill child process
                break;
            case 3:
                printf("Goodbye\n");
                check = 1;                                   //to exit, set check to one to end the while loop and break
                break;
            default:
                printf("\nInvalid input\n");                //any other input isn't recognized so display that and break
                break;
        }
    }
}