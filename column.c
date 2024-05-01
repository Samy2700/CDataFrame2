#include "column.h"
#include <stdio.h>
#include <string.h>

#define REALOC_SIZE 256


// Function to create a column with a specified type
COLUMN *create_column(ENUM_TYPE type, const char *title) {
    COLUMN *col = (COLUMN *)malloc(sizeof(COLUMN));

    if (!col) {
        fprintf(stderr, "Memory allocation failed for column creation.\n");
        return NULL;
    }

    col->title = strdup(title); // Allocate and copy the title
    col->size = 0;
    col->max_size = 0;
    col->column_type = type;
    col->data = NULL; // Start with an empty data array
    col->index = NULL; // Start without an index


    return col;
}


// Function to insert a value into the column
int insert_value(COLUMN *col, void *value) {
    if (col->size == col->max_size) {
        size_t new_max_size = col->max_size + REALOC_SIZE;
        void **new_data = realloc(col->data, new_max_size * sizeof(void*));

        if (!new_data) {
            fprintf(stderr, "Memory reallocation failed.\n");
            return 0;
        }

        col->data = new_data;
        col->max_size = new_max_size;
    }

    // Allocate memory and insert the value based on the type
    switch (col->column_type) {
        case UINT:
            col->data[col->size] = value ? malloc(sizeof(unsigned int)) : NULL;
            if (value && col->data[col->size]) {
                *(unsigned int *)(col->data[col->size]) = *(unsigned int *)value;
            }
            break;
        case INT:
            col->data[col->size] = value ? malloc(sizeof(int)) : NULL;
            if (value && col->data[col->size]) {
                *(int *)(col->data[col->size]) = *(int *)value;
            }
            break;
        case CHAR:
            col->data[col->size] = value ? malloc(sizeof(char)) : NULL;
            if (value && col->data[col->size]) {
                *(char *)(col->data[col->size]) = *(char *)value;
            }
            break;
        case FLOAT:
            col->data[col->size] = value ? malloc(sizeof(float)) : NULL;
            if (value && col->data[col->size]) {
                *(float *)(col->data[col->size]) = *(float *)value;
            }
            break;
        case DOUBLE:
            col->data[col->size] = value ? malloc(sizeof(double)) : NULL;
            if (value && col->data[col->size]) {
                *(double *)(col->data[col->size]) = *(double *)value;
            }
            break;
        case STRING:
            col->data[col->size] = value ? strdup(*(char **)value) : NULL;
            break;
        case STRUCTURE:
            col->data[col->size] = value ? malloc(sizeof(CustomStructure)) : NULL;
            if (value && col->data[col->size]) {
                memcpy(col->data[col->size], value, sizeof(CustomStructure));
            }
            break;
        default:
            fprintf(stderr, "Unsupported data type for insertion.\n");
            return 0;
    }


    if (value && col->data[col->size] == NULL) {
        fprintf(stderr, "Memory allocation failed for data insertion.\n");
        return 0;
    }

    col->size++;
    return 1;
}


void delete_column(COLUMN **col) {
    if (col == NULL || *col == NULL) return;

    // Free each data element based on the column type
    for (unsigned int i = 0; i < (*col)->size; i++) {
        if ((*col)->data[i] == NULL) continue;

        switch ((*col)->column_type) {
            case UINT:
            case INT:
            case FLOAT:
            case DOUBLE:
                free((*col)->data[i]); // For primitive data types, a simple free is sufficient
                break;
            case STRING:
                free((char *)(*col)->data[i]); // Cast to char* before freeing
                break;
            case STRUCTURE:
                free((*col)->data[i]); // Free the allocated structure memory
                break;
            default:
                // If there are any other types that require special handling, add cases for them
                break;
        }
    }

    // Free the data array itself
    free((*col)->data);

    // Free the index array if it exists
    if ((*col)->index != NULL) {
        free((*col)->index);
    }

    // Free the title string
    free((*col)->title);

    // Finally, free the column structure
    free(*col);
    *col = NULL;
}

void convert_value(COLUMN *col, unsigned long long int index, char *str, int size) {
    if (index >= col->size || col->data[index] == NULL) {
        snprintf(str, size, "NULL");
        return;
    }

    switch (col->column_type) {
        case UINT:
            snprintf(str, size, "%u", *(unsigned int *)(col->data[index]));
            break;
        case INT:
            snprintf(str, size, "%d", *(int *)(col->data[index]));
            break;
        case CHAR:
            snprintf(str, size, "%c", *(char *)(col->data[index]));
            break;
        case FLOAT:
            snprintf(str, size, "%.2f", *(float *)(col->data[index]));
            break;
        case DOUBLE:
            snprintf(str, size, "%.2lf", *(double *)(col->data[index]));
            break;
        case STRUCTURE:
        {
            CustomStructure *cs = (CustomStructure *)(col->data[index]);
            snprintf(str, size, "ID: %d, Value: %.2f", cs->id, cs->value);
        }
            break;
        default:
            snprintf(str, size, "UNKNOWN");
            break;
    }
}

void print_col(COLUMN *col) {
    if (!col) {
        printf("Column is NULL\n");
        return;
    }

    printf("Column '%s':\n", col->title);
    char value_str[256]; // Buffer for converted value string
    for (unsigned int i = 0; i < col->size; ++i) {
        convert_value(col, i, value_str, sizeof(value_str));
        printf("[%u] %s\n", i, value_str);
    }
}

// New function to count occurrences of a value
int count_occurrences(COLUMN *col, void *value) {
    if (!col || !value) return 0;
    int count = 0;
    for (unsigned int i = 0; i < col->size; ++i) {
        if (compare_values(col->column_type, col->data[i], value) == 0) {
            count++;
        }
    }
    return count;
}

// New function to get the value at a specific index
void *get_value_at(COLUMN *col, unsigned int index) {
    if (index >= col->size || !col->data) return NULL;
    return col->data[index];
}

// New function to count values greater than a given value
int count_greater_than(COLUMN *col, void *value) {
    if (!col || !value) return 0;
    int count = 0;
    for (unsigned int i = 0; i < col->size; ++i) {
        if (compare_values(col->column_type, col->data[i], value) > 0) {
            count++;
        }
    }
    return count;
}

// New function to count values less than a given value
int count_less_than(COLUMN *col, void *value) {
    if (!col || !value) return 0;
    int count = 0;
    for (unsigned int i = 0; i < col->size; ++i) {
        if (compare_values(col->column_type, col->data[i], value) < 0) {
            count++;
        }
    }
    return count;
}

// New function to count values equal to a given value
int count_equal_to(COLUMN *col, void *value) {
    return count_occurrences(col, value);
}

// Helper function for value comparison
int compare_values(ENUM_TYPE type, void *data1, void *data2) {
    if (!data1 || !data2) return -2; // Return -2 for invalid comparison
    switch (type) {
        case UINT:
            return (*(unsigned int *)data1 > *(unsigned int *)data2) - (*(unsigned int *)data1 < *(unsigned int *)data2);
        case INT:
            return (*(int *)data1 > *(int *)data2) - (*(int *)data1 < *(int *)data2);
        case CHAR:
            return (*(char *)data1 > *(char *)data2) - (*(char *)data1 < *(char *)data2);
        case FLOAT:
            return (*(float *)data1 > *(float *)data2) - (*(float *)data1 < *(float *)data2);
        case DOUBLE:
            return (*(double *)data1 > *(double *)data2) - (*(double *)data1 < *(double *)data2);
        case STRING:
            return strcmp(*(char **)data1, *(char **)data2);
        case STRUCTURE:
            // Custom comparison for CustomStructure, assuming comparison based on 'value'
            return (((CustomStructure *)data1)->value > ((CustomStructure *)data2)->value) - (((CustomStructure *)data1)->value < ((CustomStructure *)data2)->value);
        default:
            return -2; // Unsupported type comparison
    }
}