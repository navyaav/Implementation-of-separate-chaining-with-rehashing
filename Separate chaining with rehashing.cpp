#include<iostream>
#include <cstdlib>
#define LOAD_FACTOR 1

using namespace std;

typedef struct listNode{
  int key;
  int data;
  struct listNode *next;
}ListNode;
typedef struct hashTableNode{
  int bCount;          // count of elements in a block or chain
  ListNode *next;
}HashTableNode;
typedef struct hashTable{
  int countOfElement;  //count of elements in the hash table
  int tCapacity;       // table capacity/size/slots
  HashTableNode *table;
}HashTable;

HashTable *h = NULL;

int hashFun(int data, int tCapacity){
  return data % tCapacity;
}
void createHashTable(int noOfEelement){

  h = (HashTable*)malloc(sizeof(HashTable));
  h->countOfElement = 0;
  h->tCapacity = noOfEelement/LOAD_FACTOR;
  h->table = (HashTableNode *)malloc(h->tCapacity * sizeof(HashTableNode));

  for(int i = 0; i<h->tCapacity; i++){
    h->table[i].next = NULL;
    h->table[i].bCount = 0;
  }
}
ListNode *createNode(int data){
  ListNode *temp;
  temp = (ListNode *)malloc(sizeof(ListNode));

  int index = hashFun(data, h->tCapacity);
  temp->key = index;
  temp->data = data;
  temp->next = NULL;

  return temp;
}
int searchToHashTable(int data){
  int index = hashFun(data, h->tCapacity);
  ListNode *temp = h->table[index].next;

  while(temp){
    if(temp->data == data)
      return 1;
    temp = temp->next;
  }
  return 0;
}
void reHash(){
  HashTableNode *oldTable;
  int oldCapacity = h->tCapacity;
  oldTable = h->table; h->tCapacity = (h->tCapacity*2)+1;

  h->table = (HashTableNode *)malloc(sizeof(HashTableNode) * h->tCapacity);
  if(!h->table){
    cout<<"Allocation Failed";
    return;
  }
  for(int i = 0; i<h->tCapacity; i++){
    h->table[i].next = NULL;
    h->table[i].bCount = 0;
  }

  for(int i = 0; i<oldCapacity; i++){
    ListNode *temp = oldTable[i].next, *temp2;
    while(temp){
      int index = hashFun(temp->data, h->tCapacity);
      temp2 = temp; temp = temp->next;

      if(h->table[index].bCount){
        temp2->next = h->table[index].next;
        h->table[index].next = temp2;
        h->table[index].bCount++;
      }
      else{
        h->table[index].next = temp2; temp2->next = NULL;
        h->table[index].bCount++;
      }

    }
  }
}
int insertToHashTable(ListNode *newNode){

  int index = newNode->key;
  int data = newNode->data;
  if(searchToHashTable(data))
    return -1;

  newNode->next = h->table[index].next;
  h->table[index].next = newNode;
  h->table[index].bCount++;
  h->countOfElement++;

  if(h->countOfElement > h->tCapacity * LOAD_FACTOR)
    reHash();

  return 1;
  }
int deleteToHashTable(int data){

  int index = hashFun(data, h->tCapacity);
  ListNode *temp = h->table[index].next, *prev = NULL;
  while(temp){
    if(temp->data == data){
      if(prev != NULL){
        prev->next = temp->next;
            } else{
        h->table[index].next = temp->next;
            }

      free(temp);
      h->table[index].bCount--;
      h->countOfElement--;
      return 1;
    }
    prev = temp; temp = temp->next;
  }
  return 0;
}
int main(void){
  int tosear,toin,todel,n,a[300],choice;
  while(1)
  {
      cout<<"1.Create hash table\n"<<"2.Insert key\n"<<"3.Delete\n"<<"4.Search\n"<<"5.Display\n"<<"6.Exit\n";
      cout<<"Enter your choice:";
      cin>>choice;
      switch(choice)
      {
        case 1:
  cout<<"Enter the number of keys to insert in hash table:";
  cin>>n;
  createHashTable(n);
  cout<<"Enter "<<n<<" Keys\n";
  for(int i=0;i<n;i++)
  {
      cin>>a[i];
  insertToHashTable(createNode(a[i]));
  }
  cout<<"Successfully inserted elements!\n";
  break;
      case 2:

  cout<<"\nEnter any element to insert:";
    cin>>toin;

  insertToHashTable(createNode(toin));
  break;
      case 3:
        cout<<"\nEnter an element to delete:";
  cin>>todel;
  if(deleteToHashTable(todel))
    cout<<"\nElement "<<todel<<" deleted\n";
    break;
  case 4:
      cout<<"\nEnter element to search:";
  cin>>tosear;
  if(searchToHashTable(tosear))
    cout<<"\nFound Element\n";
  else
    cout<<"\nNot found\n";
  break;
  case 5:
   cout<<"\nHash table contents";
  cout<<"\nTable Capacity ="<< h->tCapacity<<"\tTotal elements ="<<h->countOfElement<<"\n";
  for(int i = 0; i< h->tCapacity; i++){
    cout<<"["<<i<<"]";
    for(ListNode *temp = h->table[i].next; temp; temp=temp->next){
      if(temp->next)
        cout<<"->"<<temp->data;
      else
        cout<<"->"<<temp->data;
      }
    cout<<"\n";
    }
    cout<<"\n";
  break;
  case 6: cout<<"\nThank you!";
  exit(0);
  break;
  default: cout<<"\nPlease enter a valid choice!";
  break;
  }
  }

  return 0;

}
