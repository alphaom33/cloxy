#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
  (type*)allocateObject(sizeof(type), objectType)

#define ALLOCATE_STRING(type, objectType, length) \
  (type*)allocateObject(sizeof(type) + length * sizeof(char) + 1, objectType)

static Obj* allocateObject(size_t size, ObjType type) {
  Obj* object = (Obj*)reallocate(NULL, 0, size);
  object->type = type;

  object->next = vm.objects;
  vm.objects = object;
  return object;
}

static ObjString* allocateString(char* chars, int length, StringType type) {
  ObjString* string = ALLOCATE_STRING(ObjString, OBJ_STRING, length);
  string->length = length;
  string->type = type;
  for (int i = 0; i < string->length; i++) {
    string->chars[i] = chars[i];
  }
  return string;
}

ObjString* takeString(char* chars, int length, StringType type) {
  return allocateString(chars, length, type); 
}

ObjString* copyString(const char* chars, int length, StringType type) {
  char* heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length, type);
}

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_STRING:
      printf("%s", AS_CSTRING(value));
      break;
  }
}