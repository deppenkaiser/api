#pragma once

#define protected																		// protected in this.lib definition (*.c)
#define protected_import(type, function) extern type function							// protected in this.lib declaration (*.h)

#define callback																		// virtual callback in application
#define callback_declaration(type, function) extern type function __attribute__((weak))	// virtual callback declaration in this.lib (*.h)

#define PRIVATE_FUNC(type, function)        static type _##function
#define PUBLIC_FUNC(type, modul, function)  type modul##_##function
