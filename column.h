#ifndef COLUMN_H
#define COLUMN_H

#include <stdlib.h>

// In column.h or a similar header file
typedef struct CustomStructure {
    int id;
    double value;
    char description[100];  // Example field
} CustomStructure;


// Enumeration for column types
enum enum_type {
    NULLVAL = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum enum_type ENUM_TYPE;

// Union to store different data types
union column_type {
    unsigned int uint_value;
    unsigned int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
};
typedef union column_type COL_TYPE;

// Structure for a column
struct column {
    char *title;
    unsigned int size;  // Logical size
    unsigned int max_size;  // Physical size
    ENUM_TYPE column_type;
    COL_TYPE **data;  // Array of pointers to stored data
    unsigned long long int *index;  // Array of integers
};
typedef struct column COLUMN;

// Function prototypes for managing columns

// Create a new column with specified type and title
COLUMN *create_column(ENUM_TYPE type, char *title);

// Insert a value into the column
int insert_value(COLUMN *col, void *value);

// Free the memory allocated for a column
void delete_column(COLUMN **col);

// Convert a value at a specified index in a column to a string
void convert_value(COLUMN *col, unsigned long long int index, char *str, int size);

// Print the contents of a column
void print_col(COLUMN *col);


// Function prototypes for accessing and analyzing column data
int count_occurrences(COLUMN *col, void *value);
void *get_value_at(COLUMN *col, unsigned int index);
int count_greater_than(COLUMN *col, void *value);
int count_less_than(COLUMN *col, void *value);
int count_equal_to(COLUMN *col, void *value);
int compare_values(ENUM_TYPE type, void *data1, void *data2);


#endif // COLUMN_H
