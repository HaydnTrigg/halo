/*
STACK_WALK_WINDOWS.H
*/

#ifndef __STACK_WALK_WINDOWS_H
#define __STACK_WALK_WINDOWS_H
#pragma once

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

typedef struct 
{
  unsigned int address;
  unsigned int rva_base;
  unsigned int name_string_offset;
  unsigned int library_object_string_offset;
} debug_symbol;

typedef struct 
{
  int number_of_symbols;
  char *string_storage;
  debug_symbol *symbols;
} debug_symbol_table;

typedef struct 
{
  int fixup;
  unsigned __int8 disregard_symbol_names;
  debug_symbol_table symbol_table;
} _stack_walk_globals;

/* ---------- prototypes/STACK_WALK_WINDOWS.C */

void stack_walk_disregard_symbol_names(boolean disregard);

/* ---------- globals */

extern _stack_walk_globals stack_walk_globals;

/* ---------- public code */

#endif // __STACK_WALK_WINDOWS_H
