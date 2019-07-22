/* author: snikons */

/* glib */
#include <glib.h>

#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void enable_logging();
void disable_logging();
gboolean logging_enabled();
void log_msg(const char *format, ...);
void log_error(const char *file_name, int line_number, const char *format, ...);
#define LOG_MSG(...) log_msg(__VA_ARGS__)
#define LOG_ERROR(...) log_error(__FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _LOG_H_ */