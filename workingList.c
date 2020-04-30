#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* CURRENT IDEA:
   numbers are stored using a singly linked list where the orders of magnitude are backwards

   ex. 1024 would be stored as [ 4, 2, 0, 1 ]

   it should be stored backwards because in order for a recursive function to carry a remainder
   to the next, higher order of magnitude place, it needs to traverse up the int, and since it's
   singly linked it would be best to store it in reverse order.

   should they be stored the other direction?
*/

//list to store numbers
struct list {
   int digit;
   struct list *next;
};
//datatype to hold numbers
struct num {
   struct list *head;
   struct list *tail;
};

//functions
bool isEmpty(struct list *); //passed : head
int length(struct list *, struct list *); //passed : head, tail
void insertFront(struct list **, struct list **, int); //passed : head, tail, digit
void insertBack(struct list **, struct list **, int); //passed : head, tail, digit
struct list * deleteFront(struct list **); //passed : head
void printList(struct list **, struct list **); //passed : head, tail
void free_list(struct list **, struct list **); //passed : head, tail
void my_add(struct list **, struct list **, struct list **, struct list **); //passed: head, tail for num1, and head,tail for num2

int main(int argc, char * argv[])
{
   struct num *x, *y;
   x = (struct num*) malloc(sizeof(struct num));
   y = (struct num*) malloc(sizeof(struct num));
   x->head = x->tail = y->head = y->tail = NULL;

   int i = 0, count = 10;

   for (i; i < count; i++)
   {
      insertFront(&x->head, &x->tail, (rand() % 10));
      insertBack(&y->head, &y->tail, (rand() % 10));
   }

   my_add(&x->head,&x->tail,&y->head,&y->tail);
   //addition(&x->head, &x->tail, &y->head, &y->tail, 1 , 2);
   printf("\nOriginal Lists: ");

   //prints list
   printList(&x->head, &x->tail);
   printf("x Length: %d", length(x->head, x->tail));
   printList(&y->head, &y->tail);
   printf("y Length: %d", length(x->head, x->tail));

   //clears list
   free_list(&x->head, &x->tail);
   free_list(&y->head, &y->tail);

   printf("\nLists after deleting all items:\nx: ");
   printList(&x->head, &x->tail);
   printf("y:");
   printList(&y->head, &y->tail);

   free(x);
   free(y);

   return 0;
}

bool isEmpty(struct list *ptr)
{
   return ptr  == NULL;
}

int length(struct list *start, struct list *end)
{
   int length = 0;
   //if list is empty
   if(isEmpty(start)) {
      return 0;
   }
   struct list *current = start;

   while(current != end) {
      length++;
      current = current->next;
   }
   length++;

   return length;
}

//insert link at the first location
void insertFront(struct list **head, struct list **tail, int digit)
{

   //creates a link
   struct list *node = (struct list*) malloc(sizeof(struct list));
      node -> digit = digit;

   if (isEmpty(*head)) {
      *head = *tail = node;
      (*head) -> next = (*tail) -> next = NULL;
   } else {
      //point it to old first list
      node -> next = *head;

      //point first to new first list
      *head = node;
   }

}

void insertBack(struct list **head, struct list **tail, int digit)
{
   struct list *node = (struct list*) malloc(sizeof(struct list));
   node -> digit = digit;

   if (isEmpty(*head))
   {
      *head = *tail = node;
      (*head) -> next = (*tail) -> next = NULL;
   }
   else
   {
      //point it to NULL
      node -> next = NULL;

      //update old tail
      (*tail) -> next = node;
	   *tail = node;
   }
}

//delete first item
struct list * deleteFront(struct list **head)
{
   if(isEmpty((*head)->next)) {
	printf("\nDeleted value:(%d)", (*head) -> digit);
	free(head);
	*head = NULL;
      return NULL;
   }
   //save reference to first link
	struct list *old_head = *head;

   //mark next to first link as first
   *head = (*head) -> next;
	printf("\nDeleted value:(%d)", old_head -> digit);
   free(old_head);
   //return the new head
   return *head;
}

//display the list
void printList(struct list **head, struct list **tail)
{
   printf("\n[ ");

   //start from the beginning
   if(!isEmpty(*head)) {
      struct list *ptr = *head;
      while(ptr != *tail) {
         printf("(%d) ", ptr -> digit);
         ptr = ptr -> next;
      }
        printf("(%d) ", ptr -> digit);
   }

   printf(" ]\n");
}


// destructor
void free_list(struct list **head, struct list **tail)
{
   while(!isEmpty(*head)) {
      struct list *temp = deleteFront(head);
   }
    printf("\n");
}

//note carry can only ever be one
void my_add(struct list **head1, struct list **tail1, struct list **head2, struct list **tail2){

    int val;
    int carry = 0;
    struct list *ptr1 = *head1;              //just creating like a temp ptr to 1st number's head
    struct list *ptr2 = *head2;              //same for above but for 2nd number
    struct list *first_tail = *tail1;        //wouldnt work unless i created a temp ptr to the tail, wouldnt let me use original
    struct list *second_tail = *tail2;       //same for above but for 2nd number
    int num1,num2;                           //values for the digit's used to calculate the value
    struct num *result;                      //where we will store our answer
    result = (struct num*) malloc(sizeof(struct num)); //not sure if this is needed, did it cause mike did
    result->head = result->tail = NULL;       //initialized

    //this loop makes sure we still have elements
    while(ptr1 != first_tail && ptr2 != second_tail){
        //for the case where num2 has more digits than num1
        if(ptr1 == first_tail && ptr2 != second_tail){
            num1 = 0;
            num2 = ptr2->digit;
            ptr2 = ptr2->next;
        }
        //for the case where num1 has more digits than num2
        else if(ptr2 == second_tail && ptr1 != first_tail){
            num2 = 0;
            num1 = ptr1->digit;
            ptr1 = ptr1->next;
        }
        else{
            num1 = ptr1 -> digit;
            num2 = ptr2 -> digit;
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }

        val  = num1 + num2 + carry;  //calculating val
        if(val > 9){                 //if val is greater than 9 we will have a carry of 1, and we need to mod val to get
            carry = 1;               //the least sig bit ex. say we have 5+7==12, 12 mod 10 = 2 which we would insert
            val = val % 10;
            insertBack(&result->head, &result->tail, val);
        }else{                       //case where we can just insert and not have a carry
            carry = 0;
            insertBack(&result->head, &result->tail, val);
        }
    }

    //all this stuff is basically what we did above but for the last element in tail, wouldnt let me do it in loop
    val = ptr1->digit + ptr2->digit + carry;
    if(val > 9){
        carry = 1;
        val = val % 10;
        insertBack(&result->head, &result->tail, val);
        insertBack(&result->head, &result->tail, carry);
    }else{
        carry = 0;
        insertBack(&result->head, & result->tail, val);
    }

    printList(&result->head, &result->tail);

}
