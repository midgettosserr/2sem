#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "common.h"
#include "queue.h"

int getMenuItem() {
    printf("1 - сгенерировать случайный граф\n");
    printf("2 - добавить вершину\n");
    printf("3 - добавить ребро\n");
    printf("4 - удалить вершину\n");
    printf("5 - удалить ребро\n");
    printf("6 - вывод графа\n");
    printf("7 - поиск в ширину\n");
    printf("71 - поиск в ширину (рекурсия)\n");
    printf("8 - кратчайший путь\n");
    printf("9 - проверка на ацикличность\n");
    printf("10 - топологическая сортировка\n");
    printf("0 - выход\n");
    printf("> ");
    int menu;
    menu = getInt();
    return menu;
}

int main() {
    Graph *graph = NULL;
    int menu;
    char *name1;
    char *name2;
    double x, y;
    clock_t t;
    Path *path;
    ShortestPath *spath;
    unsigned int n, m;
    printf("Введите имя файла: ");
    char *filename = get_str(stdin);
    FILE *file = fopen(filename, "rt");
    if (!file || !(graph = inputFromFile(file))) {
        printf("Не удалось открыть/прочитать файл, создан пустой граф\n");
        graph = initGraph();
    }
    if (file) {
        fclose(file);
    }
    do {
        menu = getMenuItem();
        switch (menu) {
            case 1:
                removeGraph(graph);
                printf("Введите число вершин: ");
                n = getInt();
                if (n == 0) {
                    printf("Создан пустой граф\n");
                    graph = initGraph();
                }
                else {
                    printf("Введите число ребер: ");
                    m = getInt();
                    t = clock();
                    graph = makeRand(n, m);
                    t = clock() - t;
                    printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                }
                break;
            case 2:
                printf("Введите имя: ");
                name1 = get_str(stdin);
                printf("Введите координаты: ");
                x = getFloat();
                y = getFloat();
                t = clock();
                addNode(graph, name1, x, y);
                t = clock() - t;
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                break;
            case 3:
                printf("Введите имя первой вершины: ");
                name1 = get_str(stdin);
                printf("Введите имя второй вершины: ");
                name2 = get_str(stdin);
                t = clock();
                addEdge(graph, name1, name2);
                t = clock() - t;
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                free(name2);
                break;
            case 4:
                printf("Введите имя: ");
                name1 = get_str(stdin);
                t = clock();
                removeNode(graph, name1);
                t = clock() - t;
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                break;
            case 5:
                printf("Введите имя первой вершины: ");
                name1 = get_str(stdin);
                printf("Введите имя второй вершины: ");
                name2 = get_str(stdin);
                t = clock();
                removeEdge(graph, name1, name2);
                t = clock() - t;
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                free(name2);
                break;
            case 6:
                t = clock();
                print(graph);
                t = clock() - t;
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                break;
            case 7:
                printf("Введите имя первой вершины: ");
                name1 = get_str(stdin);
                printf("Введите имя второй вершины: ");
                name2 = get_str(stdin);
                t = clock();
                path = BFS(graph, name1, name2);
                t = clock() - t;
                if (path) {
                    printPath(path);
                }
                else {
                    printf("Путь не найден\n");
                }
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                free(name2);
                removePath(path);
                break;
            case 71:
                printf("Введите имя первой вершины: ");
                name1 = get_str(stdin);
                printf("Введите имя второй вершины: ");
                name2 = get_str(stdin);
                t = clock();
                path = BFSRecursive(graph, name1, name2);
                t = clock() - t;
                if (path) {
                    printPath(path);
                }
                else {
                    printf("Путь не найден\n");
                }
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                free(name2);
                removePath(path);
                break;
            case 8:
                printf("Введите имя первой вершины: ");
                name1 = get_str(stdin);
                printf("Введите имя второй вершины: ");
                name2 = get_str(stdin);
                t = clock();
                spath = BellmanFord(graph, name1, name2);
                t = clock() - t;
                if (spath) {
                    printf("Кратчайший путь равен %lf\n", spath->dist);
                    for (int i = spath->size-1; i >= 0; i--) {
                        printf("%s", spath->path[i]);
                        if (i == 0) {
                            printf("\n");
                        }
                        else {
                            printf(" -> ");
                        }
                    }
                    free(spath->path);
                    free(spath);
                }
                else {
                    printf("Путь не найден\n");
                }
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                free(name1);
                free(name2);
                break;
            case 9:
                t = clock();
                if (isCycle(graph)) {
                    printf("Граф не ацикличен\n");
                }
                else {
                    printf("Граф ацикличен\n");
                }
                t = clock() - t;
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                break;
            case 10:
                t = clock();
                path = sorting(graph);
                t = clock() - t;
                if (path) {
                    printPath(path);
                    removePath(path);
                }
                else {
                    printf("Топологическая сортирока не существует\n");
                }
                printf("Время: %.2lf мс\n", (double)t/CLOCKS_PER_SEC * 1000);
                break;
            default:
                break;
        }
    } while (menu != 0);
    file = fopen(filename, "wt");
    outputToFile(file, graph);
    fclose(file);
    printf("Граф сохранен в файл %s\n", filename);
    removeGraph(graph);
    free(filename);
    return 0;
}
