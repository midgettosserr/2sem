#include <stdio.h>
#include <malloc.h>

typedef struct Line {  //структура для задания строки матрицы
    int n;  //количество элементов в строке матрицы
    int* a;  //массив элементов, указатель на первый элемент массива - динамический массив
} Line;

typedef struct Matrix {  //структура для задания самой матрицы
    int lines;  //количество строк матрицы
    Line* matr; //динамический массив строк матрицы
} Matrix;

int getInt(int*);  //ввод целого числа
int input(Matrix* a); //ввод матрицы
void output(const char* msg, Matrix a); //вывод матрицы
void erase(Matrix* a);  //освобождение зан€той памяти

void makeNewMatrix(Matrix *a, Matrix *b); //создание новой матрицы

int main()
{
    Matrix matr = { 0, NULL };
    Matrix newMatr = {0, NULL};
    if (input(&matr) == 0) {
        printf("%s\n", "End of file occured");
        return 1;
    }
    output("Source matrix", matr);
    makeNewMatrix(&matr, &newMatr);
    output("New matrix", newMatr);
    erase(&matr);
    erase(&newMatr);
    return 0;
}

//ввод целого числа
//при обнаружении ошибки ввод повторяется
//функция возвращает 1, если ввод корректен и 0 при обнаружении конца файла
int getInt(int* a)
{
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)  //обнаружен конец файла
            return 0;
        if (n == 0) {  //обнаружен некорректный символ-ошибка
            printf("%s\n", "Error! Repeat input");
            scanf("%*c");
        }
    } while (n == 0);
    return 1;
}

//функция ввода матрицы
//функция возвращает 1, если воод корректен и 0, при обнаружении конца файла
//если конец файла обнаружен в середине воода-нужно освободить выделенную пам€ть
int input(Matrix* rm)
{
    const char* pr = "";  //будущее сообщение об ошибке
    int m;  //количество строк матрицы
    int i, j;
    int* p;
    //сначала вводим кол-во строк
    do {
        printf("%s\n", pr);
        printf("Enter number of lines: --> ");
        pr = "You are wrong; repeat, please!";
        if (getInt(&m) == 0)
            return 0; //обнаружен конец файла, память не выделилась
    } while (m < 1);
    rm->lines = m;
    
    //выделяем память под массив структур - строк матрицы
    rm->matr = (Line*)calloc(m, sizeof(Line));
    for (i = 0; i < rm->lines; ++i) {
        //теперь для каждой строки матрицы вводим количество столбцов
        pr = "";
        do {
            printf("%s\n", pr);
            printf("Enter number of items in line %d: --> ", i + 1);
            pr = "You are wrong; repeat, please!";
            if (getInt(&m) == 0) { //обнаружен конец файла
                //освобождение выделенной пам€ти
                rm->lines = i;  //сколько строк сформировано, память выделена
                erase(rm);
                return 0; //обнаружен конец файла
            }
        } while (m < 1);
        rm->matr[i].n = m;
        //и выделяем необходимую память под элементы строки
        p = (int*)malloc(sizeof(int) * m);
        rm->matr[i].a = p;
        //и теперь можно вводить сами элементы данной строки матрицы
        printf("Enter items for matrix line #%d:\n", i + 1);
        for (j = 0; j < m; ++j, ++p)
            if (getInt(p) == 0) {
                //освобождение выделенной памяти
                rm->lines = i + 1;
                erase(rm);
                return 0;
            }
    }
    return 1;
}

void output(const char* msg, Matrix a)  //функция вывода
{
    int i, j;
    int* p;
    printf("%s:\n", msg);
    for (i = 0; i < a.lines; ++i) {
        p = a.matr[i].a;
        for (j = 0; j < a.matr[i].n; ++j, ++p)
            printf("%d ", *p);
        printf("\n");
    }
}

void erase(Matrix* a) //функция освобождения занятой памяти
{
    int i;
    for (i = 0; i < a->lines; ++i)
        free(a->matr[i].a);
    free(a->matr);
    a->lines = 0;
    a->matr = NULL;
}

int max(Line *a) //функция вычисления max элемента вектора
{
    int i;
    int *p = a->a;
    int res = *p;
    for (i = 0; i < a->n; ++i, ++p)
    {
        if (*p > res)
            res = *p;
    }
    return res;
}

int min(Line *a) //функция вычисления min элемента вектора
{
    int i;
    int *p = a->a;
    int res = *p;
    for (i = 0; i < a->n; ++i, ++p)
    {
        if (*p < res)
            res = *p;
    }
    return res;
}

void makeNewLine(Line *a, Line *b) //функция формирования строки новой  матрицы
{
    int i, mi, ma;
    int *pa, *pb;
    
    b->n = a->n;
    b->a = (int*)malloc(sizeof(int) * b->n);
    
    pa = a->a;
    pb = b->a;
    
    mi = min(a);
    ma = max(a);
    
    for (i = 0; i < a->n; ++i, ++pa, ++pb)
    {
        if (*pa == mi)
            *pb = ma;
        else if (*pa == ma)
            *pb = mi;
        else
            *pb = *pa;
    }
}

void makeNewMatrix(Matrix *a, Matrix *b)
{
    int i;
    
    b->lines = a->lines;
    b->matr = (Line*)calloc(b->lines, sizeof(Line));
    
    for (i = 0; i < a->lines; ++i)
    {
        makeNewLine(&(a->matr[i]), &(b->matr[i]));
    }
}
