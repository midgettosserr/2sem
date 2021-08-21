#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//необходимые типы данных
typedef char* InfoType;
typedef unsigned long KeyType1;
typedef unsigned long KeyType2;
typedef unsigned long RelType1;
typedef unsigned long RelType2;
typedef unsigned long IndexType1;
typedef unsigned long IndexType2;

//вспомогательные функции для работы с InfoType
char* get_str(void);
InfoType* inputInfo(void);
InfoType* copyInfo(InfoType *info);
void printInfo(InfoType *info);
void freeInfo(InfoType *info);

typedef struct Item {   //элемент таблицы
    InfoType *info;
    KeyType1 key1;
    KeyType2 key2;
} Item;

// первое пространство ключей (просматриваемая таблица список)
typedef struct Node1 {
    RelType1 release;       //номер версии
    Item *info;             //указатель на информацию
    struct Node1 *next;     //указатель на следующий элемент
} Node1;

typedef struct KeySpace1 {
    KeyType1 key;           //ключ элемента
    Node1 *node;            //указатель на информацию
    struct KeySpace1 *link; //указатель на следующий элемент
} KeySpace1;

// второе пространство ключей (перемешанная таблица, перемешивание сцеплением)
typedef struct KeySpace2 {
    KeyType2 key;               //ключ элемента
    RelType2 release;           //номер версии элемента
    Item *info;                 //указатель на информацию
    struct KeySpace2 *next;     //указатель на следующий элемент
} KeySpace2;

typedef struct Table {      //таблица
    KeySpace1     *ks1;     //указатель на первое пространство ключей
    KeySpace2     **ks2;    //указатель на второе пространство ключей
    IndexType2     msize2;  //размер области 2-го пространства ключей
} Table;

Item* makeItem(InfoType *info, KeyType1 key1, KeyType2 key2);
Node1* findKeyKS1(KeySpace1 *ks1, KeyType1 key);
void addElementKS1(KeySpace1 **ks1, Item *item);
void removeKS1(KeySpace1 *ks1);
void printKS1(KeySpace1 *ks1);
void cleanKS1(KeySpace1 **ks1);
KeySpace1* getKey1(KeySpace1 *ks1, KeyType1 key);
KeySpace1* getKeyRel1(KeySpace1 *ks1, KeyType1 key, RelType1 release);
int removeKey(KeySpace1 **ks1, KeyType1 key);
KeyType2 removeKeyRel(KeySpace1 **ks1, KeyType1 key, RelType1 release);
int addElementKS2(KeySpace2 **ks2, Item *item, IndexType2 size);
void removeKS2(KeySpace2 **ks2, IndexType2 size);
void printKS2(KeySpace2 **ks2, IndexType2 size);
KeySpace2** getKey2(KeySpace2 **ks2, KeyType2 key, IndexType2 size);
KeySpace2** getKeyRel2(KeySpace2 **ks2, KeyType2 key, RelType2 release, IndexType2 size);
void cleanKs2(KeySpace2 **ks2, IndexType2 size);
Table* initTable(IndexType2 msize2);
void removeTable(Table *table);
Item* findElement(Table *table, KeyType1 key1, KeyType2 key2);
int addElement(Table *table, Item *item);
int removeElement(Table *table, KeyType1 key1, KeyType2 key2);
Node1* getNext1(Table *table, KeyType1 key1, Node1 *prev);
KeySpace2* getNext2(Table *table, KeyType2 key2, KeySpace2 *prev);
void removeAll1(Table *table, KeyType1 key1);
void removeAll2(Table *table, KeyType2 key2);
void printTable(Table *table, int type);
int removeKeyRel1(Table *table, KeyType1 key1, RelType1 rel);
void cleanTableKs2(Table *table);

#endif
