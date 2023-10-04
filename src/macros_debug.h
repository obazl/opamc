#include <stdlib.h>
#include "log.h"

#define TRACE_ENTRY if (opamc_trace) log_trace(RED "ENTRY:" CRESET " %s", __func__);

#define TRACE_LOG(fmt, ...) if (opamc_trace) log_trace(fmt, __VA_ARGS__)

#define TRACE_EXIT if (opamc_trace) log_trace(RED "EXIT:" CRESET " %s", __func__);

#define TRACE_ENTRY_MSG(fmt, ...) \
    if (opamc_trace) log_trace(RED "ENTRY:" CRESET " %s, " fmt, __func__, __VA_ARGS__);

#define LOG_DEBUG(lvl, fmt, ...) if (opamc_debug>lvl) log_debug(fmt, __VA_ARGS__)
#define LOG_ERROR(lvl, fmt, ...) if (opamc_debug>lvl) log_error(fmt, __VA_ARGS__)
#define LOG_INFO(lvl, fmt, ...)  if (opamc_debug>lvl) log_info(fmt, __VA_ARGS__)
#define LOG_TRACE(lvl, fmt, ...) if (opamc_debug>lvl) log_trace(fmt, __VA_ARGS__)
#define LOG_WARN(lvl, fmt, ...)  if (opamc_debug>lvl) log_warn(fmt, __VA_ARGS__)
