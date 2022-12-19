#ifndef STATUS_H
#define STATUS_H

#define OK 0
#define EIO 1
#define EINVARG 2
#define ENOMEM 3
#define EBADPATH 4
#define EFSNOTUS 5

#define panic(msg) \
    print(msg);    \
    while (1)      \
    {              \
    }

#define check(condition, err) \
    if (!(condition))         \
    {                         \
        res = err;            \
        goto out;             \
    }

#define check_arg(condition) check(condition, -EINVARG)

#define check_err(res) check(res >= 0, res);
#endif