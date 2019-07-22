/* author: snikons */

/* glib */
#include <glib.h>
#include <gmodule.h>

/* assert() */
#include <assert.h>

/* memset() */
#include <string.h>

/* logging api */
#include "log.h"

/* modbus api */
#include "modbus.h"

/* command line options */
static struct {
  gboolean log;
  gchar *pcap_path;
  gchar *find;
  gboolean list;
} options;

/* command line option scheme */
static GOptionEntry goptions[] = {
  { "log", 'l', 0, G_OPTION_ARG_NONE, &options.log, "Enable logging", NULL },
  { "pcap-path", 'p', 0, G_OPTION_ARG_STRING, &options.pcap_path, "PCAP file path", "P" },
  { "find", 'f', 0, G_OPTION_ARG_STRING, &options.find, "search target", "F" },
  { "list", 't', 0, G_OPTION_ARG_NONE, &options.list, "List available targets to find", NULL },
  { NULL }
};

/* available search targets */
#define TCP_MB_DEVICE_INFO "tcp-mb-device-info"

int print_targets() {
  g_print("Available search targets:\n");
  g_print("  " TCP_MB_DEVICE_INFO ": MODBUS device vendor, product and version\n");
  return 0;
}

/* print printable chars and print hex values of non-printable chars */
void print_data(const gchar *prefix, const GByteArray *byte_array) {
  guint i = 0;
  g_print(prefix);
  
  if (!byte_array) {
    g_print("NULL\n");
    return;
  }

  for (i = 0; i < byte_array->len; ++i) {
    if (byte_array->data[i] >= 32 && byte_array->data[i] <= 126)
      g_print("%c", (gchar)(byte_array->data[i]));
    else
      g_print(".");
      /*g_print("{0x%02X}", byte_array->data[i]);*/
  }

  g_print("\n");
}

void print_device_info(mb_device_info *device_info) {
  assert(device_info);
  if (device_info->vendor_name.data)
    print_data("Vendor         : ", &(device_info->vendor_name));
  if (device_info->product_code.data)
    print_data("Product code   : ", &(device_info->product_code));
  if (device_info->product_version.data)
    print_data("Product version: ", &(device_info->product_version));
  if (device_info->vendor_url.data)
    print_data("Vendor URL     : ", &(device_info->vendor_url));
  if (device_info->product_name.data)
    print_data("Product name   : ", &(device_info->product_name));
  if (device_info->model_name.data)
    print_data("Model name     : ", &(device_info->model_name));
  if (device_info->server_id.data)
    print_data("Server ID      : ", &(device_info->server_id));
}

int main(int argc, char* argv[]) {
  GError *error = NULL;
  GOptionContext *context = NULL;
  const gchar *glib_version_error = NULL;
  pcap_handler packet_handler = NULL;
  u_char *user = NULL;

  /* prevent issues due to glib incompatibility */
  glib_version_error = glib_check_version(GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
  if (glib_version_error)
    g_error("%s\n", glib_version_error);

  /* parse command line options */
  context = g_option_context_new("- Packet Analyzer");
  if (!context) g_error("g_option_context_new() failed\n");
  g_option_context_add_main_entries(context, goptions, NULL);
  if (!g_option_context_parse(context, &argc, &argv, &error))
      g_error("option parsing failed: %s\n", error->message);

  /* Enable / disable logging */
  if (options.log) enable_logging();
  else disable_logging();

  /* print debug info */
  LOG_MSG("%s", pcap_lib_version());
  LOG_MSG("glib version %u.%u.%u", glib_major_version, glib_minor_version, glib_micro_version);

  /* handle command line options */
  if (options.list) return print_targets();

  if (options.find) {
    if (!options.pcap_path)
      g_error("PCAP file path missing\n");

    if (g_strcmp0(options.find, TCP_MB_DEVICE_INFO) == 0) {
      mb_device_info device_info;
      memset(&device_info, 0, sizeof(mb_device_info));
      mb_find_device_info_in_pcap(options.pcap_path, &device_info);
      print_device_info(&device_info);
    } else
      g_error("Unsupported search target %s\n", options.find);
  }

  return 0;
}