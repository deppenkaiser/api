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