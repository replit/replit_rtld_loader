#define _GNU_SOURCE

#include <link.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include "syscalls.h"
#include "string_funs.h"
#include "consts.h"
#include "env_parser.h"
#include "dynamic_lookup.h"
#include "lookup_by_channel.generated.h"