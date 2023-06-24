#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

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

 Serial.print(F("bss_size = "));
 Serial.println(heap_start);

 Serial.print(F("\nstack_top = "));
 Serial.println(stack_top);
 Serial.print(F("stack_bottom = "));
 Serial.println(stack_bottom);
 Serial.print(F("stack_size = "));
 Serial.println(stack_size);

 Serial.print(F("\nheap start = "));
 Serial.println(heap_start);
 Serial.print(F("heap end = "));
 Serial.println(heap_end);
 Serial.print(F("heap size = "));
 Serial.println(heap_size);

 Serial.print(F("\nfree_size = "));
 Serial.println(free_size);
 Serial.print(F("used_size = "));
 Serial.println(max_ram - free_size);

 Serial.print(F("\n"));
}


void setMark(const __FlashStringHelper *s, int line)
{
 char __stack = 0;

 Serial.print(s);
 Serial.print(":");
 Serial.print(line);
 Serial.println(": Mark set");

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

 Serial.print(s);
 Serial.print(":");
 Serial.print(line);
 Serial.print(": Mark");

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

 Serial.print(F(" stack: "));
 Serial.print((int)stack_size - (int)mark_stack);
 Serial.print(F("/heap: "));
 Serial.println((int)heap_size - (int)mark_heap);
}


void showSizes(const __FlashStringHelper *s, int line)
{
 char __stack = 0;

 Serial.print(s);
 Serial.print(":");
 Serial.print(line);
 Serial.print(":");

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

 Serial.print(F(" free:"));
 Serial.print(free_size);
 Serial.print(F(" used:"));
 Serial.print(max_ram - free_size);
 Serial.print(F(" ("));
 Serial.print(stack_size);
 Serial.print(F("/"));
 Serial.print(heap_size);
 Serial.print(F(")"));
 Serial.print(F("\n"));
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

 Serial.print(F("__brkval="));
 Serial.println((unsigned int)__brkval);
 Serial.print(F("__heap_start="));
 Serial.println((unsigned int)__heap_start);
 Serial.print(F("__malloc_heap_start="));
 Serial.println((unsigned int)__malloc_heap_start);
 Serial.print(F("__malloc_margin="));
 Serial.println((unsigned int)__malloc_margin);
 Serial.print(F("__flp="));
 Serial.println((unsigned int)__flp);
 Serial.print(F("__stack="));
 Serial.println((unsigned int)&__stack);
 Serial.print(F("stack size="));
 Serial.println(RAMEND - (unsigned int)&__stack);
 Serial.print(F("Heap size="));
 Serial.println((unsigned int)__brkval - RAMSTART);
 
 total = 0;
 for (current = __flp; current; current = current->nx)
 {
 total += 2; /* Add two bytes for the memory block's header  */
 total += (int) current->sz;
 Serial.print(F("mblk: sz="));
 Serial.print((unsigned int)current->sz);
 Serial.print(F(" nx="));
 Serial.println((unsigned int)current->nx);
 Serial.print(F("Total: "));
 Serial.println(total);
 }
 Serial.println(F("\n"));
}


void showMemory(const char *s)
{
 char __stack = 0;
 
 Serial.println(s);
 showMem();
}


void showMemory_P(const __FlashStringHelper *s)
{
 char __stack = 0;
 
 Serial.println(s);
 showMem();
}
