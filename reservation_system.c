#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int date;
    char name[21];
    struct node* next;
}
node;

FILE *file = NULL;

int limit;

node* queue_front = NULL;
node* queue_rear = NULL;

node* create_node();

int count(int);

void print_guests(int);
void enqueue();
void make_reservation();
void change_date_reservation();
void delete_reservation();

int main()
{
   file = fopen("amusement.dat", "r");
   
   if (file == NULL)
   {
       printf("FILE does not exist\n");
       return 1;
   }

   fscanf(file, "%d", &limit);

   while(!feof(file))
   {
       int choice;
       fscanf(file, "%d", &choice);
       switch(choice)
       {
           case 1:
            {
                make_reservation();
                break;
            }
           case 2:
           {
               change_date_reservation();
               break;
           }
           case 3:
           {
               delete_reservation();
               break;
           }
           case 4:
           {
               int date;
               fscanf(file,"%d", &date);
               print_guests(date);
               break;
           }
           case 5:
           {
               return 0;
           }
           default :
           printf("Code %d invalid\n", choice);
       }
   }

   fclose(file);
   return 0;
}

node* create_node()
{
    node* n = (node*)malloc(sizeof(node));
    if (n == NULL)
    {
        printf("Memory Full\nExiting\n");
        exit(1);
    }
    n->next = NULL;
    return n;
}

int count(int date)
{   
    int ctr = 0;
    for (node* temp = queue_front; temp != NULL; temp = temp->next)
    {
        if(temp->date == date)
            ctr++;
    }
    return ctr;
}

void enqueue(int date, char name[])
{
    node* new = create_node();
    new->date = date;
    strcpy(new->name,name);
    new->next = NULL;

    if (queue_front == NULL)
    {
        queue_front = new;
        queue_rear = new;
    }
    else
    {
        queue_rear->next = new;
        queue_rear = new;
    }
}

void make_reservation()
{
    int date_buf;
    char name_buf[21];

    fscanf(file, "%d", &date_buf);
    fscanf(file, "%s", name_buf);

    if (count(date_buf) >= limit)
    {
        printf("SORRY, QUOTA OVER\n");
    }
    else
    {
        enqueue(date_buf,name_buf);
    }
}

void change_date_reservation()
{
    int old_date_buf;
    char name_buf[21];
    int new_date_buf;

    fscanf(file, "%d", &old_date_buf);
    fscanf(file, "%s", name_buf);
    fscanf(file, "%d", &new_date_buf);

    for (node* temp = queue_front; temp != NULL; temp = temp->next)
    {
        if (temp->date == old_date_buf && !strcmp(temp->name, name_buf))
            temp->date = new_date_buf;
    }
}

void delete_reservation()
{
    int date_buf;
    char name_buf[21];

    fscanf(file, "%d", &date_buf);
    fscanf(file, "%s", name_buf);

    node* temp = queue_front;
    node* ite = temp;

    int found = 0;
    while (temp->next != NULL)
    {
        if (temp->next->date == date_buf && !strcmp(temp->next->name, name_buf))
        {
           ite = temp;
           temp = temp->next;
           found = 1;
           break;
        }
        temp = temp->next;
    }
    if (found == 1)
    {
        ite->next = temp->next;
        free(temp);
    }
    else if (ite->date == date_buf && !strcmp(ite->name, name_buf))
    {
        queue_front = ite->next;
        free(ite);
    }
}

void print_guests(int date)
{
    int ctr = 0;
    printf("%d : ", date);
    for (node* temp = queue_front; temp != NULL; temp = temp->next)
    {
        if (temp->date == date)
        {
            ctr++;
            printf("%s ", temp->name);
        }
    }
    if (ctr == 0)
    {
        printf("NO GUESTS");
    }
    printf("\n");
}
