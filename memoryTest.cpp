#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "debugConsole.hpp"

extern unsigned int __heap_start;
extern void *__brkval;

/*
 * The free list structure as maintained by the
 * avr-libc memory allocation routines.
 */
struct __freelist
{
  size_t sz;
  struct __freelist *nx;
};

/* The head of the free list structure */
extern struct __freelist *__flp;

#include "memoryTest.h"

static unsigned int max_ram = 0;
static unsigned int stack_top = 0;
static unsigned int stack_bottom = 0;
static unsigned int stack_size = 0;
static unsigned int heap_start = 0;
static unsigned int heap_end = 0;
static unsigned int heap_size = 0;
static unsigned int free_size = 0;

static unsigned int mark_stack = 0;
static unsigned int mark_heap = 0;

struct __freelist* current;
int total = 0;

void initFreeMemory()
{
 char *__stack = 0;

 max_ram = RAMEND - RAMSTART + 1;

 stack_top = max_ram;
 stack_bottom = ((unsigned int)&__stack - RAMSTART) + 1;
 stack_size = stack_top - stack_bottom;
 mark_stack = stack_size;

 heap_start = (unsigned int)__malloc_heap_start - RAMSTART;
 void *__heap = malloc(1);
 heap_end = (unsigned int)__heap - RAMSTART;
 heap_end -= 2;
 free(__heap);
 heap_size = heap_end - heap_start;
 mark_heap = heap_size;

 free_size = (unsigned int)stack_top - (unsigned int)heap_end + freeListSize();

 debugConsole.print(F("bss_size = "));
 debugConsole.println(heap_start);

 debugConsole.print(F("\nstack_top = "));
 debugConsole.println(stack_top);
 debugConsole.print(F("stack_bottom = "));
 debugConsole.println(stack_bottom);
 debugConsole.print(F("stack_size = "));
 debugConsole.println(stack_size);

 debugConsole.print(F("\nheap start = "));
 debugConsole.println(heap_start);
 debugConsole.print(F("heap end = "));
 debugConsole.println(heap_end);
 debugConsole.print(F("heap size = "));
 debugConsole.println(heap_size);

 debugConsole.print(F("\nfree_size = "));
 debugConsole.println(free_size);
 debugConsole.print(F("used_size = "));
 debugConsole.println(max_ram - free_size);

 debugConsole.print(F("\n"));
}


void setMark(const __FlashStringHelper *s, int line)
{
 char __stack = 0;

 debugConsole.print(s);
 debugConsole.print(":");
 debugConsole.print(line);
 debugConsole.println(": Mark set");

 mark_stack = ((unsigned int)&__stack - RAMSTART) + 1;
 mark_stack = stack_top - mark_stack;

 mark_heap = 0;
 if(__brkval != 0)
 {
 mark_heap = (unsigned int)__brkval - RAMSTART - heap_start;
 }
}


void showMark(const __FlashStringHelper *s, int line)
{
 char __stack = 0;

 debugConsole.print(s);
 debugConsole.print(":");
 debugConsole.print(line);
 debugConsole.print(": Mark");

 stack_bottom = ((unsigned int)&__stack - RAMSTART) + 1;
 stack_size = stack_top - stack_bottom;

 if(__brkval != 0)
 {
 heap_end = (unsigned int)__brkval - RAMSTART;
 }
 else
 {
 heap_end = heap_start - RAMSTART;
 }
 heap_size = heap_end - heap_start;

 debugConsole.print(F(" stack: "));
 debugConsole.print((int)stack_size - (int)mark_stack);
 debugConsole.print(F("/heap: "));
 debugConsole.println((int)heap_size - (int)mark_heap);
}


void showSizes(const __FlashStringHelper *s, int line)
{
 char __stack = 0;

 debugConsole.print(s);
 debugConsole.print(":");
 debugConsole.print(line);
 debugConsole.print(":");

 stack_bottom = (unsigned int)&__stack - RAMSTART + 1;
 stack_size = stack_top - stack_bottom;

 if(__brkval != 0)
 {
 heap_end = (unsigned int)__brkval - RAMSTART;
 }
 else
 {
 heap_end = heap_start - RAMSTART;
 }
 heap_size = heap_end - heap_start;
 free_size = stack_bottom - heap_end + freeListSize();

 debugConsole.print(F(" free:"));
 debugConsole.print(free_size);
 debugConsole.print(F(" used:"));
 debugConsole.print(max_ram - free_size);
 debugConsole.print(F(" ("));
 debugConsole.print(stack_size);
 debugConsole.print(F("/"));
 debugConsole.print(heap_size);
 debugConsole.print(F(")"));
 debugConsole.print(F("\n"));
}


int freeListSize()
{
  total = 0;

  for (current = __flp; current; current = current->nx)
  {
    total += 2; /* Add two bytes for the memory block's header  */
    total += (int) current->sz;
  }

  return total;
}


int freeMemory()
{
  int free_memory;
  if ((int)__brkval == 0)
  {
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
  }
  else
  {
    free_memory = ((int)&free_memory) - ((int)__brkval);
    free_memory += freeListSize();
  }
  return free_memory;
}


void showMem(void)
{
 char __stack = 0;

 debugConsole.print(F("__brkval="));
 debugConsole.println((unsigned int)__brkval);
 debugConsole.print(F("__heap_start="));
 debugConsole.println((unsigned int)__heap_start);
 debugConsole.print(F("__malloc_heap_start="));
 debugConsole.println((unsigned int)__malloc_heap_start);
 debugConsole.print(F("__malloc_margin="));
 debugConsole.println((unsigned int)__malloc_margin);
 debugConsole.print(F("__flp="));
 debugConsole.println((unsigned int)__flp);
 debugConsole.print(F("__stack="));
 debugConsole.println((unsigned int)&__stack);
 debugConsole.print(F("stack size="));
 debugConsole.println(RAMEND - (unsigned int)&__stack);
 debugConsole.print(F("Heap size="));
 debugConsole.println((unsigned int)__brkval - RAMSTART);
 
 total = 0;
 for (current = __flp; current; current = current->nx)
 {
 total += 2; /* Add two bytes for the memory block's header  */
 total += (int) current->sz;
 debugConsole.print(F("mblk: sz="));
 debugConsole.print((unsigned int)current->sz);
 debugConsole.print(F(" nx="));
 debugConsole.println((unsigned int)current->nx);
 debugConsole.print(F("Total: "));
 debugConsole.println(total);
 }
 debugConsole.println(F("\n"));
}


void showMemory(const char *s)
{
 char __stack = 0;
 
 debugConsole.println(s);
 showMem();
}


void showMemory_P(const __FlashStringHelper *s)
{
 char __stack = 0;
 
 debugConsole.println(s);
 showMem();
}
