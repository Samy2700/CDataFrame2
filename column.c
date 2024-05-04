#include "column.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define REALOC_SIZE 256



// Create a new column with specified type and title
COLUMN *create_column(ENUM_TYPE type, char *title) {
    COLUMN *col = (COLUMN *)malloc(sizeof(COLUMN));
    if (col == NULL) {
        fprintf(stderr, "Failed to allocate memory for column structure.\n");
        return NULL;
    }

    col->title = strdup(title); // Duplicate the title for the column
    if (col->title == NULL) {
        free(col);
        fprintf(stderr, "Failed to allocate memory for column title.\n");
        return NULL;
    }

    col->size = 0;
    col->max_size = 0;
    col->column_type = type;
    col->data = NULL; // Initialize data pointer as NULL
    col->index = NULL; // Indexing not handled at creation

    return col;
}

// Function to insert a value into the column
int insert_value(COLUMN *col, void *value) {
    if (col->size >= col->max_size) {
        size_t new_max_size = col->max_size == 0 ? REALOC_SIZE : col->max_size + REALOC_SIZE;
        COL_TYPE **new_data = (COL_TYPE **)realloc(col->data, new_max_size * sizeof(COL_TYPE *));
        if (!new_data) {
            fprintf(stderr, "Memory reallocation failed.\n");
            return 0;
        }
        col->data = new_data;
        col->max_size = new_max_size;
    }

    // Allocate memory for the new value based on the type of the column
    void *new_value = NULL;
    switch (col->column_type) {
        case UINT:
            new_value = malloc(sizeof(unsigned int));
            if (new_value) *(unsigned int *)new_value = *(unsigned int *)value;
            break;
        case INT:
            new_value = malloc(sizeof(int));
            if (new_value) *(int *)new_value = *(int *)value;
            break;
        case CHAR:
            new_value = malloc(sizeof(char));
            if (new_value) *(char *)new_value = *(char *)value;
            break;
        case FLOAT:
            new_value = malloc(sizeof(float));
            if (new_value) *(float *)new_value = *(float *)value;
            break;
        case DOUBLE:
            new_value = malloc(sizeof(double));
            if (new_value) *(double *)new_value = *(double *)value;
            break;
        case STRING:
            new_value = strdup((char *)value);
            break;
        case STRUCTURE:
            new_value = malloc(sizeof(struct CustomStructure));  // Assuming CustomStructure is defined elsewhere
            if (new_value) memcpy(new_value, value, sizeof(struct CustomStructure));
            break;
        default:
            fprintf(stderr, "Unsupported type for insertion.\n");
            return 0;
    }

    if (!new_value) {
        fprintf(stderr, "Memory allocation for new value failed.\n");
        return 0;
    }

    col->data[col->size++] = new_value;
    return 1;
}

// Function to free the memory allocated for a column
void delete_column(COLUMN **col_ptr) {
    if (col_ptr == NULL || *col_ptr == NULL) {
        fprintf(stderr, "Attempt to delete a null column pointer.\n");
        return;
    }

    COLUMN *col = *col_ptr;

    // Free each element in the data array
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i] != NULL) {
            if (col->column_type == STRING) {
                free(*(char **)(col->data[i]));  // Free the string before the pointer
            }
            free(col->data[i]);  // Free the data pointer itself
        }
    }

    // Free the data array pointer
    free(col->data);

    // Free the column title and the column struct itself
    free(col->title);
    free(col);

    // Set the pointer in the original reference to NULL
    *col_ptr = NULL;
}


void CustomStructureToString(CustomStructure *cs, char *str, size_t size) {
    if (cs == NULL || str == NULL) {
        snprintf(str, size, "NULL");
        return;
    }
    // Create a formatted string with structure's fields
    snprintf(str, size, "ID: %d, Value: %.2f, Description: %s", cs->id, cs->value, cs->description);
}


// Function to convert a column value to a string based on its data type
void convert_value(COLUMN *col, unsigned long long int index, char *str, int size) {
    if (col == NULL || str == NULL) {
        snprintf(str, size, "NULL");
        return;
    }
    if (index >= col->size || col->data[index] == NULL) {
        snprintf(str, size, "NULL");
        return;
    }

    // Convert value based on its type
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
        case STRING:
            snprintf(str, size, "%s", (char *)(col->data[index]));
            break;
        case STRUCTURE:
            CustomStructureToString((CustomStructure *)(col->data[index]), str, size);
            break;
        default:
            snprintf(str, size, "Unsupported Type");
            break;
    }
}

// Function to print the contents of a column
void print_col(COLUMN *col) {
    if (col == NULL) {
        printf("Attempt to print a null column.\n");
        return;
    }

    printf("Column '%s':\n", col->title);
    char buffer[256]; // Buffer to hold the string representation of each value

    for (unsigned int i = 0; i < col->size; i++) {
        convert_value(col, i, buffer, sizeof(buffer));
        printf("[%u] %s\n", i, buffer);
    }
}


// Function to count the number of occurrences of a value
int count_occurrences(COLUMN *col, void *value) {
    if (col == NULL || value == NULL) return 0;

    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (compare_values(col->column_type, col->data[i], value) == 0) {
            count++;
        }
    }
    return count;
}

// Function to get the value at a given position
void *get_value_at(COLUMN *col, unsigned int index) {
    if (col == NULL || index >= col->size) return NULL;
    return col->data[index];
}

// Function to count the number of values greater than a given value
int count_greater_than(COLUMN *col, void *value) {
    if (col == NULL || value == NULL) return 0;

    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (compare_values(col->column_type, col->data[i], value) > 0) {
            count++;
        }
    }
    return count;
}

// Function to count the number of values less than a given value
int count_less_than(COLUMN *col, void *value) {
    if (col == NULL || value == NULL) return 0;

    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (compare_values(col->column_type, col->data[i], value) < 0) {
            count++;
        }
    }
    return count;
}

// Function to count the number of values equal to a given value
int count_equal_to(COLUMN *col, void *value) {
    return count_occurrences(col, value);
}

// Helper function to compare two values based on the column type
int compare_values(ENUM_TYPE type, void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) return -1; // Indicating invalid comparison

    switch (type) {
        case INT:
            return (*(int *)data1 > *(int *)data2) - (*(int *)data1 < *(int *)data2);
        case FLOAT:
            return (*(float *)data1 > *(float *)data2) - (*(float *)data1 < *(float *)data2);
        case DOUBLE:
            return (*(double *)data1 > *(double *)data2) - (*(double *)data1 < *(double *)data2);
        case CHAR:
            return (*(char *)data1 > *(char *)data2) - (*(char *)data1 < *(char *)data2);
        case STRING:
            return strcmp((char *)data1, (char *)data2);
        case STRUCTURE:
            return ((CustomStructure *)data1)->value > ((CustomStructure *)data2)->value ? 1 :
                   ((CustomStructure *)data1)->value < ((CustomStructure *)data2)->value ? -1 : 0;
        default:
            fprintf(stderr, "Unsupported type for comparison.\n");
            return -1; // Unsupported type
    }
}


