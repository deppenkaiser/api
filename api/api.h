#pragma once

#include <stdint.h>
#include <stdbool.h>

#define protected_import(type, function) extern type function							// protected in this.lib declaration (*.h)

#define callback																		// virtual callback in application
#define callback_declaration(type, function) extern type function __attribute__((weak))	// virtual callback declaration in this.lib (*.h)

#define PRIVATE_FUNC(type, function) static type _##function

typedef bool* const bool_stack_t, *bool_heap_t;
typedef int16_t* const int16_stack_t, *int16_heap_t;
typedef uint16_t* const uint16_stack_t, *uint16_heap_t;
typedef int32_t* const int32_stack_t, *int32_heap_t;
typedef uint32_t* const uint32_stack_t, *uint32_heap_t;
typedef float* const float_stack_t, *float_heap_t;
typedef double* const double_stack_t, *double_heap_t;

// Aus einem x_heap_t Pointer kann nach NULL-Prüfung ein x_stack_t werden!

#define CLASS(class_name) typedef struct class_##class_name* class_##class_name##_heap_t;\
					struct class_##class_name

#define CLASS_METHOD_PTR_DECL(class_name, type, function, arguments)	typedef type (*class_##class_name##_##function##_stack_t)(arguments);\
																		PRIVATE_FUNC(type, function(arguments))

#define CLASS_METHOD(class_name, type, function) type class_##class_name##_##function
