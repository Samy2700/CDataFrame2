#ifndef COLUMN_H
#define COLUMN_H

#include <stdlib.h>

typedef struct {
    int id;
    double value;
} CustomStructure;


// ENUM_TYPE to identify the type of data in the column
typedef enum {
    NULLVAL = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
} ENUM_TYPE;

// Union to store different data types
typedef union column_type {
    unsigned int uint_value;
    int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
} COL_TYPE;

// Structure for a column
typedef struct column {
    char *title;
    unsigned int size; // Logical size
    unsigned int max_size; // Physical size
    ENUM_TYPE column_type;
    COL_TYPE **data; // Array of pointers to stored data
    unsigned long long int *index; // Array of integers for advanced indexing
} COLUMN;

// Function prototypes
COLUMN *create_column(ENUM_TYPE type, const char *title);
int insert_value(COLUMN *col, void *value);
void delete_column(COLUMN **col);
void convert_value(COLUMN *col, unsigned long long int index, char *str, int size);
void print_col(COLUMN *col);

// Additional function prototypes for value comparisons and searches
int count_occurrences(COLUMN *col, void *value);
void *get_value_at(COLUMN *col, unsigned int index);
int count_greater_than(COLUMN *col, void *value);
int count_less_than(COLUMN *col, void *value);
int count_equal_to(COLUMN *col, void *value);
int compare_values(ENUM_TYPE type, void *data1, void *data2);


#endif // COLUMN_H
