#pragma once

#define private static
#define protected
#define protected_import(type, function) extern type function
#define callback
#define callback_declaration(type, function) extern type function __attribute__((weak))
