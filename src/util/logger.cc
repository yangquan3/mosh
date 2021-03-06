/*
Copyright (c) 2007, 2008 by Juliusz Chroboczek
Copyright (c) 2014 by Matthieu Boutier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include "logger.h"

int log_level = 0;
int log_indent_level = 0;
FILE *log_output = NULL;

void
log_msg(int level, const char *format, ...)
{
  int i;
  va_list args;
  if (UNLIKELY(log_output == NULL)) {
    log_output = stderr;
  }
  if (LOG_GET_LEVEL(log_level) < LOG_GET_LEVEL(level))
    return;
  va_start(args, format);
  for (i = 0; i < log_indent_level; i ++)
    fprintf(log_output, "  ");
  vfprintf(log_output, format, args);
  if (level & LOG_PRINT_ERROR)
    fprintf(log_output, ": %s.\n", strerror(errno));
  fflush(log_output);
  va_end(args);
}

void
log_parse_level(const char *string) {
  log_level = 0;
  while (*string) {
    if (*string == 'e') {
      log_level |= LOG_ERROR;
    } else if (*string == 'd') {
      log_level |= LOG_DEBUG;
      log_level |= LOG_DEBUG_ALL;
      /* There is only one debug level currently.  with more, the syntax should be
         dcommon, dall, etc:
      string ++;
      if (*string == 'c') {
        log_level |= LOG_DEBUG_COMMON;
      } else if (*string == 'a') {
        log_level |= LOG_DEBUG_ALL;
      }
      */
    } else if (*string == 'a') {
      log_level |= LOG_MAX;
    } else {
      break;
    }
    while (*string && *string != '|') {
      string++;
    }
  }
}
