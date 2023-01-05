#ifndef STATUS_H
#define STATUS_H

#define OK 0
#define EIO 1
#define EINVARG 2
#define ENOMEM 3
#define EBADPATH 4
#define EFSNOTUS 5
#define ERDONLY 6
#define EUNIMP 7
#define EISTKN 8
#define EBADFORMAT 9

#define CHECK(condition, err) \
    if (!(condition))         \
    {                         \
        res = err;            \
        goto out;             \
    }

#define CHECK_ARG(condition) CHECK(condition, -EINVARG)

#define CHECK_ERR(expr)                   \
    {                                     \
        int _expr_res = (expr);           \
        CHECK(_expr_res >= 0, _expr_res); \
    }
#endif