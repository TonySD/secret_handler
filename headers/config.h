#pragma once

#define SECRET_FILENAME "secrets"
#define INDEX_FILENAME "indexes"
#define DEBUG 1

#if DEBUG == 0
#define FILENAME_FORMAT "/proc/%d/%s"
#elif DEBUG == 1
#define FILENAME_FORMAT "/tmp/%d_%s"
#endif