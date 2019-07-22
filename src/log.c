/* author: snikons */

#include "log.h"

/* va_list, va_start(), va_end() */
#include <stdarg.h>

/* glib */
#include <glib/gprintf.h>

static gboolean enabled = FALSE;

gboolean logging_enabled() {
  return enabled;
}

void enable_logging() {
  enabled = TRUE;
}

void disable_logging() {
  enabled = FALSE;
}

void log_msg(const char *format, ...) {
  va_list args;
  if (!enabled) return;
  va_start(args, format);
  g_vfprintf(stdout, format, args);
  g_fprintf(stdout, "\n");
  va_end(args);
}

void log_error(const char *file_name, int line_number, const char *format, ...) {
  va_list args;
  if (!enabled) return;
  va_start(args, format);
  g_fprintf(stderr, "ERROR: ");
  g_vfprintf(stderr, format, args);
  g_fprintf(stderr, "\n  %s (%d)\n", file_name, line_number);
  va_end(args);
}