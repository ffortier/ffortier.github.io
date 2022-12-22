#ifndef STATUS_H
#define STATUS_H

#define OK 0
#define EIO 1
#define EINVARG 2
#define ENOMEM 3
#define EBADPATH 4
#define EFSNOTUS 5
#define ERDONLY 6

#define check(condition, err) \
    if (!(condition))         \
    {                         \
        res = err;            \
        goto out;             \
    }

#define check_arg(condition) check(condition, -EINVARG)

#define check_err(expr)                   \
    {                                     \
        int _expr_res = (expr);           \
        check(_expr_res >= 0, _expr_res); \
    }
#endif