# api – C++-ähnliche Lesbarkeit in C

Header-basierte Makro-Bibliothek, die C **C++-ähnliche Vorteile** verschafft –
zumindest was die **Lesbarkeit** angeht: `private`/`protected`-Sichtbarkeit
und überschreibbare `callback`s machen klassen-typische Kapselung und
virtuelle Hooks ohne OOP-Overhead direkt im Quelltext ausdrückbar. Rein C,
keine Laufzeit, kein Overhead.

## Umfang

- **`private`** → `static`: nur in der eigenen Übersetzungseinheit (`this.c`)
  sichtbar (Hilfsfunktionen/-typen).
- **`protected`** → (leer): Kennzeichnung „bibliotheksintern" in der
  Definition (`*.c`).
- **`protected_import(type, function)`** → `extern type function`: Import
  einer geschützten Funktion in die Übersetzungseinheit.
- **`callback`** → (leer): virtueller Callback, den die Anwendung liefert.
- **`callback_declaration(type, function)`** → `extern type function
  __attribute__((weak))`: voreinstellbare (weak) Callback-Deklaration des
  Moduls (`*.h`) – von der Anwendung überschreibbar.
- **`PRIVATE_FUNC(type, function)`** → `static type _##function`: Forward-
  Deklaration einer dateiinternen Funktion (`_name`) am Dateikopf — die
  Definition steht gesammelt am Dateiende.
- **`CLASS(name)`**, **`CLASS_METHOD_PTR_DECL(...)`**, **`CLASS_METHOD(...)`**:
  „C-Klassen" aus Datenstruktur + Methodenzeigern (siehe unten).
- **Stack-/Heap-Pointer-Typen** für Primitive: kodieren Nullability im Typ.

## Stack-/Heap-Pointer-Semantik

```c
typedef bool* const bool_stack_t, *bool_heap_t;
typedef int16_t* const int16_stack_t, *int16_heap_t;
typedef uint16_t* const uint16_stack_t, *uint16_heap_t;
typedef int32_t* const int32_stack_t, *int32_heap_t;
typedef uint32_t* const uint32_stack_t, *uint32_heap_t;
typedef float* const float_stack_t, *float_heap_t;
typedef double* const double_stack_t, *double_heap_t;
```

- `x_stack_t` (const): zeigt auf Caller-allokierten Speicher (Stack).
  Per Definition **nie NULL** → keine NULL-Prüfung erforderlich.
- `x_heap_t` (non-const): zeigt auf Heap-Speicher. **Kann NULL sein** →
  NULL-Prüfung Pflicht.
- **Promotion:** Aus einem `x_heap_t` kann nach bestandener NULL-Prüfung
  ein `x_stack_t` werden — für den Konsumenten ist dann garantiert:
  gültig, kein Check nötig.
- Out-Parameter (Aufrufer-Stack) verwenden die `_stack_t`-Varianten.

Strukturen folgen demselben Muster paarweise:
`typedef struct foo { ... }* const foo_stack_t, *foo_heap_t;`

## Klassen (CLASS-Makros)

Für „C-Klassen" — Datenstruktur plus Methodenzeiger, die der Konstruktor
installiert. Beispiel: `vulkan_gui/textures/class_texture.h`.

```c
// *.h
struct class_texture;                                                   // Forward-Deklaration
CLASS_METHOD_PTR_DECL(texture, bool, load, struct class_texture* object,
                      class_texture_context_stack_t context, const char* path);
CLASS_METHOD_PTR_DECL(texture, void, destroy, struct class_texture* object);

CLASS(texture)          // struct class_texture + typedef class_texture_heap_t
{
	class_texture_heap_t this;
	class_texture_load_stack_t load;      // Methoden, vom Konstruktor installiert
	class_texture_destroy_stack_t destroy;
};

CLASS_METHOD(texture, class_texture_heap_t, new());   // class_texture_new()

// *.c — Standardimplementierung selbst anmelden
PRIVATE_FUNC(bool, load(struct class_texture* object, ..., const char* path));
```

Regeln, die aus den Makros folgen:

- `CLASS(name)` erzeugt **nur** `class_<name>_heap_t` plus die Struktur. Ein
  Stack-Typ muss bei Bedarf von Hand ergänzt werden:
  `typedef struct class_foo* const class_foo_stack_t;`
- `CLASS_METHOD_PTR_DECL` ist **variadisch** und erzeugt **nur** den
  Zeigertyp `class_<name>_<f>_stack_t` — keine Deklaration. Mehrparametrige
  Methoden deshalb **ohne** eigene Klammern notieren.
- Die Standardimplementierung meldet die `.c`-Datei per `PRIVATE_FUNC` an.
  Eine `static`-Deklaration im Header ist verboten: sie kollidiert, sobald
  zwei Klassen eine gleichnamige Methode haben (`_destroy` gibt es in
  `class_texture` und `class_object`).
- `CLASS_METHOD(name, type, f)` liefert den Namen für die öffentliche
  Definition (`class_<name>_<f>`).

## Nutzung

```c
#include <api/api.h>

private void helfer(void) { /* nur in dieser .c */ }
protected void kern(void) { /* bibliotheksintern */ }

callback void app_hook(void);          /* von der Anwendung definiert */
callback_declaration(void, app_hook);  /* weak-Default im Modul */
```

## Abhängigkeiten

Keine weiteren Projekt-Bibliotheken.

## Build

```bash
cmake -S . -B build
cmake --build build
```

In ein Projekt einbinden: `add_subdirectory(../../libraries/api …)`,
Einbindung des Headers über den Include-Pfad `<api/api.h>`.

## Wiki

Dokumentation: [api – C++-ähnliche Lesbarkeit in C](https://czybor.i234.me/wiki/sw-module/api/) (Quartz-Wiki)