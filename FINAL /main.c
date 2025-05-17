//
//  main.c
//  facebook-application final project
//
//  Created by Rrita Hajrizi on 28.11.23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

//structure that saves the name of the user
typedef struct {
    char *name;
    char *surname;
    char *username;
    char *invited_by;
}nameData;

//structure to save a date
typedef struct {
    int day;
    int month;
    int year;
}date;
//member structure which saves the name of user, birthdate, and birthplace
typedef struct {
    nameData memberName;
    date birthdate;
    char *birthplace;
}member;

//linked list structure
typedef struct linkedLIST{
    member data;
    struct linkedLIST *next;
}linkedLIST;
//Binary search tree structure
typedef struct BST{
    member data;
    struct BST *right;
    struct BST *left;
}BST;

void PRINT_ERROR_MEMORY(void){
    fprintf(stderr, "Failed to allocate the memory!\n");
}

void PRINT_ERROR_FILE(void){
    fprintf(stderr, "Cannot open the file!\n");
}


//function that allocated memory for a new node in linked list
linkedLIST *createHead(member member_data){
    linkedLIST *newNode = (linkedLIST*)malloc(sizeof(linkedLIST));//dynamically alloctae memory for the new node
    if(newNode){
        newNode->data = member_data;//assign the data of the member to the node
        newNode->next = NULL;//make the next pointer NULL
    } else{
        PRINT_ERROR_MEMORY();//if data couldnt be allocated print the error
        return NULL;
    }
    return newNode;//return the new node;
}
//function that allocates memory for a new node in bst
BST *createRoot(member member_data){
    BST *newNode = (BST*)malloc(sizeof(BST));
    if(newNode){ //IF THE MEMORY COULD BE ALLOCATED ASSIGN THE MEMBER DATA TO THE NODE
        newNode->data = member_data;
        newNode->left = NULL;
        newNode->right = NULL;
    } else {//if not print the error and return
        PRINT_ERROR_MEMORY();
        return NULL;
    }
    return newNode;
}
//function that creates date
date Date(int d, int m , int y){
    date new;
    new.day = d;
    new.month = m;
    new.year = y;
    return new;
}
//function that allocates memory for the name of the member
nameData allocateName(char *n, char *sn, char *un, char *i_by){
    nameData new;
    new.name = strdup(n);
    new.surname = strdup(sn);
    new.username = strdup(un);
    new.invited_by = strdup(i_by);
    return new;
}
//function that allocates memory for the user data
member allocateMember(char *name, char *surname, char *username, char *invited,char*birthplace, int day, int month, int year){
    member x;
    x.memberName = allocateName(name, surname, username, invited);
    x.birthdate = Date(day, month, year);
    x.birthplace = strdup(birthplace);
    return x;
}
//function that serves nodes at the beginning of the linked list
linkedLIST *insertAtFront(linkedLIST *head, member userData){
    linkedLIST *newUser = createHead(userData);
    if(newUser == NULL){
        PRINT_ERROR_MEMORY();
        return NULL;
    }
    newUser->next = head;
    return newUser;
}
//function that inserts nodes at the end of linked list
linkedLIST *insertAtEnd(linkedLIST *head, member userData){
    linkedLIST *newNode = createHead(userData);
    if(newNode == NULL){
        PRINT_ERROR_MEMORY();
        return head;
    }
    newNode -> next = NULL;
    if(head == NULL){
        return newNode;
    } else {
        linkedLIST *curr = head;
        while(curr->next!=NULL){
            curr = curr->next;
        }
        curr->next = newNode;
        return head;
    }
}
//function that inserts nodes in the bst recursively
BST *insertNodesinBST(BST *root, member memberData){
    if(root == NULL){
            return createRoot(memberData);
        }
    //comparing the name of the user to the data at the current node in bst
        int cmp = strcmp(memberData.memberName.name, root->data.memberName.name);
        if(cmp < 0){
            root->left = insertNodesinBST(root->left, memberData);
        } else if (cmp > 0){//on the right side
            root->right = insertNodesinBST(root->right, memberData);
        }
        return root;
}
//freeing the memrory allocated for the name
void freeName(nameData name) {
    free(name.name);
    free(name.surname);
    free(name.username);
    free(name.invited_by);
}

//freeing memory allocated for the member
void freeUserData(member m) {
    freeName(m.memberName);
    free(m.birthplace);
}
//function to free memory allocated for linked list
void freeLinkedList(linkedLIST *head){
    linkedLIST *pointer = head;
    while(pointer!=NULL){
        linkedLIST *temp= pointer ->next;
        freeUserData(pointer->data);
        free(pointer);
        pointer = temp;
    }
}

void destroyBST(BST *root){
    if(root!= NULL){
        destroyBST(root->left);
        destroyBST(root->right);
        freeUserData(root->data);
        free(root);
    }
}


//function that prints all the data of the linked list
void printList(linkedLIST *head){
    linkedLIST *point;
    for(point = head; point != NULL; point = point -> next){
        printf("Name: %s.\n", point->data.memberName.name);
        printf("Surname: %s.\n", point->data.memberName.surname);
        printf("Username: %s.\n", point->data.memberName.username);
        printf("Invited by %s.\n", point->data.memberName.invited_by);
        printf("Birthdate: %02d.%02d.%04d\n", point->data.birthdate.day,point->data.birthdate.month, point->data.birthdate.year);
        printf("Birthplace: %s", point->data.birthplace);
        printf("\n\n");
    }
}
//function to search for the name in the bst
BST *searchNameInBST(BST *root, char *what){
    if(root == NULL){
        printf("Reached NULL\n");
        return NULL;
    }
    int cmp = strcmp(what, root->data.memberName.name);
    printf("Comparing %s with %s, Result: %d\n", what, root->data.memberName.name, cmp);

    if(cmp == 0){
        printf("Found\n");
        return root;
    } else if(cmp < 0){
        printf("Going left\n");
        return searchNameInBST(root->left, what);
    } else {
        printf("Going right\n");
        return searchNameInBST(root->right, what);
    }
}
//function to print member if found in the node
void printUser(BST *root){
    if(root == NULL){
        printf("The User wasn't found n the database!\n");
    } else{
        printf("Name: %s.\n", root->data.memberName.name);
        printf("Surname: %s.\n", root->data.memberName.surname);
        printf("Username: %s.\n", root->data.memberName.username);
        printf("Invited by %s.\n", root->data.memberName.invited_by);
        printf("Birthdate: %02d.%02d.%02d\n",root->data.birthdate.day,root->data.birthdate.month, root->data.birthdate.year);
        printf("Birthplace: %s", root->data.birthplace);
        printf("\n\n");
    }
}

//function that serializes data from linked list to insert into a CSV file
void encodeData(FILE *file, member data){
    fprintf(file, "%s,%s,%s,%s,%d,%d,%d,%s\n", data.memberName.name, data.memberName.surname, data.memberName.username, data.memberName.invited_by,
            data.birthdate.day, data.birthdate.month, data.birthdate.year, data.birthplace);//creates the format of saving data with a comma
  fflush(file);
}


void loadDatainFile(char *filename, linkedLIST *head) {
    linkedLIST *current = head;//make the current pointer point at the beginnig of the list
    FILE *file = fopen(filename, "a");
    if(file == NULL){
        PRINT_ERROR_FILE();
        return;
    }
    while (current != NULL) {//loop through list till the end
        encodeData(file, current->data); //print the data into file in the CSV format
        current = current->next;//move to the next node
    }
  fclose(file);
}

member decodeUser(char *bufferLine){
    member new;
    int readUserData = sscanf(bufferLine, "%50[^,],%50[^,],%50[^,],%50[^,],%d,%d,%d,%50[^\n]",
                              new.memberName.name, new.memberName.surname, new.memberName.username, new.memberName.invited_by, &new.birthdate.day, &new.birthdate.month, &new.birthdate.year,new.birthplace);
    if(readUserData != 8){
        fprintf(stderr, "Not enough data.\n");
        strcpy(new.memberName.name, "NO DATA");
                strcpy(new.memberName.surname, "NO DATA");
                strcpy(new.memberName.username, "NO DATA");
                strcpy(new.memberName.invited_by, "NO DATA");
                new.birthdate.day = 0;
                new.birthdate.month = 0;
                new.birthdate.year = 0;
                strcpy(new.birthplace, "NO DATA");
    }
    return new;
}
//Function that reads from the file, and saves the data into a linked list
linkedLIST *readLinkedList(char *filename){
    FILE *file =fopen(filename, "r");
    if(file == NULL){
        PRINT_ERROR_FILE();
        return NULL;
    }
    linkedLIST *listHead = NULL;
    char bufferLine[MAX];
    while(fgets(bufferLine, sizeof(bufferLine), file)!=NULL){
        member newUser = decodeUser(bufferLine);

        linkedLIST *newN = createHead(newUser);
        if(newN == NULL){
            PRINT_ERROR_MEMORY();
            fclose(file);
            freeLinkedList(listHead);
            return NULL;
        }
        listHead = insertAtEnd(listHead, newN ->data);
    }
    fclose(file);
    return listHead;
}
//function to delete the node in bst and returns back the new bst without the node
BST *deleteNodeInBST(BST *root, char *what){
    if(root == NULL){
        return root;
    }
    int cmp = strcmp(what, root->data.memberName.name);

    if(cmp < 0){
        root->left = deleteNodeInBST(root->left, what);
    } else if(cmp>0){
        root->right = deleteNodeInBST(root->right, what);
    } else {
        if(root->left == NULL){
            BST *temp = root->right;
            free(root);
            return temp;
        } else if(root->right == NULL){
            BST* temp = root->left;
            free(root);
            return temp;
        }

        BST *temp = root->right;
        while(temp -> left != NULL){
            temp = temp->left;
        }
        root->data = temp ->data;
        root->right = deleteNodeInBST(root->right, temp->data.memberName.name);
    }
    return root;
}

//function that changes the bst to a linked list
linkedLIST *BSTtoLinkedList(BST *root, linkedLIST *head){
    if(root == NULL){ //if theres no bst return head of the list
        return head;
    }
    head = BSTtoLinkedList(root->left, head);
    head = insertAtEnd(head, root->data);
    head = BSTtoLinkedList(root->right, head);

    return head;
}

//function that changes linked list to bst
BST* linkedListToBST(linkedLIST* head) {
    BST* root = NULL;
    linkedLIST* current = head;

    while (current != NULL) {
        root = insertNodesinBST(root, current->data);
        current = current->next;
    }
    return root;
}
//function to scan member info by user input
linkedLIST* scanMember(linkedLIST *head){
    char name[51];
    char surname[51];
    char username[51];
    char invitedBY[51];
    int day, month, year;
    char birthplace[51];

    printf("Enter member information:\n");
    printf("Name: ");
    scanf("%s", name);
    printf("Surname: ");
    scanf("%s", surname);
    printf("Username: ");
    scanf("%s", username);
    printf("Invited by: ");
    scanf("%s", invitedBY);
    printf("Birthdate (day month year): ");
    scanf("%d %d %d", &day, &month, &year);
    printf("Birthplace: ");
    scanf("%s", birthplace);

    member new = allocateMember(name, surname, username, invitedBY, birthplace, day, month, year);
    head = insertAtEnd(head, new);
    return head;

}

int userChoice(void){
    int choice;
    printf("\nMenu:\n");
    printf("1. Add a new user\n");
    printf("2. Search for a user\n");
    printf("3. Delete a user\n");
    printf("4. Display all users\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}




int main(int argc, const char * argv[]) {
    //load list from the file
    linkedLIST *loadedlist = readLinkedList("database.csv");
    //make the bst from the list
    BST *searchtree = linkedListToBST(loadedlist);
    
    char searchName[50];
    char deleteName[50];
    while(1){
          int choice = userChoice();
            switch(choice){
                case 1:
                    //scan the member from the user
                    loadedlist = scanMember(loadedlist);
                    loadDatainFile("database.csv", loadedlist); //load it into the file
                    break;
                case 2:
                  printf("Enter the first name of the user you want to search: ");
                  scanf("%s", searchName);//scan name to be searched
                  BST *foundUser = searchNameInBST(searchtree, searchName);
                  if(foundUser){
                      printf("User found:\n");
                      printUser(foundUser);
                  } else{
                      printf("User %s not found in the database!\n", searchName);
                  }
                    break;
                case 3:
                    printf("Enter the first name of the user you want to delete: ");
                    scanf("%s", deleteName);//sacn name to be deleted

                    BST *deleteUSer = deleteNodeInBST(searchtree, deleteName);//delete the name and pass back the new bst
                    loadedlist = BSTtoLinkedList(deleteUSer, loadedlist);
                    loadDatainFile("database.csv", loadedlist);//load data in the file
                    break;
                case 4:
                    printList(loadedlist);
                    break;
                case 5:
                    printf("Exiting the program.\n");
                    destroyBST(searchtree);//free the memory for bst
                    freeLinkedList(loadedlist);//free the memory for the linked list
                    exit(0);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;

            }
    }

    return 0;
}
