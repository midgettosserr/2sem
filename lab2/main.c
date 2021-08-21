#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"

typedef struct Data { // структура с данными одной записи после разбора строки
    char *id;
    int ta;
    int ts;
    int priority; //метка о том, что пациенту "только спросить"
    int neok; //наличие ошибки (запись не соответствует формату)
} Data;

char *get_str() {                     // ввод строки произвольной длины
        char buf[81] = {0};
        char *str = NULL;
        int len = 0;
        int n = 0;
        do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
                if (!str) {
                return NULL;
                }
        } else if (n > 0) {
                int chunk_len = strlen(buf);
                int str_len = len + chunk_len;
                str = realloc(str, str_len + 1);
                memcpy(str + len, buf, chunk_len);
                len = str_len;
        } else {
                scanf("%*c");
        }

        } while (n > 0);
        if (len > 0) {
        str[len] = '\0';
        } else {
        str = calloc(1, sizeof(char));
        }
        return str;
}

Data errorData(Data *data) {  // сброс Data (при ошибке)
    free(data->id);
    data->id = NULL;
    data->neok = 0;
    return *data;
}

Data getPatient(const char* str) {  // разбор одной записи
    Data data;
    data.id = NULL;
    int ind = 0;
    if (str == NULL) {
        return errorData(&data);
    }
    if (str[ind] == '*') {
        data.priority = 1;
        ind++;
    }
    else {
        data.priority = 0;
    }
    // получаем строку-идентификатор
    int len = 0;
    data.id = (char*)malloc(sizeof(char));
    while (str[ind] != '\0' && str[ind] != '/') {
        data.id[len++] = str[ind++];
        data.id = (char*)realloc(data.id, sizeof(char)*len+1);
    }
    data.id[len] = '\0';
    if (str[ind] != '/' || len == 0) {
        return errorData(&data);
    }
    ind++;
    // получаем число - время появления
    char buf[20];
    int i = 0;
    while (str[ind] != '\0' && str[ind] != '/' && i < 20) {
        buf[i++] = str[ind++];
    }
    buf[i] = '\0';
    data.ta = atoi(buf);
    if (str[ind] != '/' || data.ta < 1) {
        return errorData(&data);
    }
    ind++; 
    // получаем число - время обслуживания
    i = 0;
    while (str[ind] != '\0' && i < 20) {
        buf[i++] = str[ind++];
    }
    buf[i] = '\0';
    data.ts = atoi(buf);
    if (str[ind] != '\0' || data.ts < 1) {
        return errorData(&data);
    }
    data.neok = 1;
    return data;
}

void process(char* str) { // моделирование функционирования очереди
    int time = 1;
    Dequeue *q = createQueue();
    char *patient = strtok(str, " \t");
    Data data = getPatient(patient);
    Patient *current_patient = NULL;
    while (patient || !isempty(q) || current_patient) {
        if (patient && !data.neok) {
            printf("Ошибка ввода\n");
            break;
        }
        if (patient && data.ta == time) {
            if (data.priority)
                addValueToBegin(q, data.id, data.ts);
            else
                addValueToEnd(q, data.id, data.ts);
            patient = strtok(NULL, " \t");
            free(data.id);
            data = getPatient(patient);
            if (!current_patient) {
                current_patient = removeBegin(q);
            }
        }
        else if (!patient || data.ta > time) {
            printf("Время: %d\n", time);
            printf("На приеме: %s\n", current_patient ? current_patient->id : "Никого нет");
            printf("Очередь: ");
            printQueue(q);
            time++;
            if (current_patient) {
                current_patient->time--;
                if (current_patient->time == 0) {
                    free(current_patient->id);
                    free(current_patient);
                    current_patient = removeBegin(q);
                }
            }
            else {
                current_patient = removeBegin(q);
            }
        }
        else {
            printf("Ошибка времени прихода\n");
            break;
        }
    }
    if (current_patient) {
      free(current_patient->id);
    }
    free(current_patient);
    free(data.id);
    free(q);
}

int main() {
    char *str = get_str();
    while (str) {
        printf("---Моделирование очереди---\n");
        process(str);
        free(str);
        printf("---Конец моделирования---\n");
        str = get_str();
    }
    return 0;
}
