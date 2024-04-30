#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "column.h"

// Structure for a dataframe
typedef struct dataframe {
    COLUMN **columns; // Array of pointers to COLUMN
    unsigned int column_count; // Number of columns in the dataframe
    unsigned int max_columns; // Maximum capacity of columns array
} DATAFRAME;

// Function prototypes for managing the dataframe
DATAFRAME *create_dataframe();
void add_column_to_dataframe(DATAFRAME *df, COLUMN *col);
void remove_column_from_dataframe(DATAFRAME *df, unsigned int index);
void free_dataframe(DATAFRAME *df);
COLUMN *get_column(DATAFRAME *df, unsigned int index);

// Advanced functionality prototypes
void print_dataframe(DATAFRAME *df);

// Filling
void initialize_empty_dataframe(DATAFRAME *df);
void fill_dataframe_from_user(DATAFRAME *df);
void hard_fill_dataframe(DATAFRAME *df, void **data, unsigned int num_rows, unsigned int num_columns);

// Displaying
void display_full_dataframe(DATAFRAME *df);
void display_dataframe_rows(DATAFRAME *df, unsigned int rows);
void display_dataframe_columns(DATAFRAME *df, unsigned int columns);

// Usual operations
void add_row_to_dataframe(DATAFRAME *df, void **row_data);
void delete_row_from_dataframe(DATAFRAME *df, unsigned int row_index);
void rename_column_title(DATAFRAME *df, unsigned int column_index, const char *new_title);
int check_value_existence(DATAFRAME *df, void *value);
void *get_cell_value(DATAFRAME *df, unsigned int row, unsigned int column);
void set_cell_value(DATAFRAME *df, unsigned int row, unsigned int column, void *value);
void display_column_names(DATAFRAME *df);

// Analysis and statistics
void display_row_count(DATAFRAME *df);
void display_column_count(DATAFRAME *df);
int count_cells_equal_to(DATAFRAME *df, void *value);
int count_cells_greater_than(DATAFRAME *df, void *value);
int count_cells_less_than(DATAFRAME *df, void *value);

#endif // CDATAFRAME_H

