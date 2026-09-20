#ifndef STRVW_H
#define STRVW_H

#include <stddef.h>

#define SV_FMT "%.*s"
#define SV_ARG(s) (int)(s).len, (s).data
// printf(SV_FMT"\n", SV_ARG(name));

typedef struct {
    const char  *data;
    size_t      len;
} strview;

strview sv_from_parts(const char *data, size_t len);
strview sv_init(const char *data);
void sv_chop_left(strview *v, size_t n);
void sv_chop_right(strview *s, size_t n);
void sv_trim_left(strview *s);
void sv_trim_right(strview *s);
void sv_trim(strview *s);
strview sv_chop_by_delim(strview *s, char delim);
int sv_eq(strview a, strview b);

#endif

#ifdef STRVW_IMPLEMENTATION

#include <string.h>
#include <ctype.h>

strview sv_from_parts(const char *data, size_t len) {
    return (strview) {
        data,
        len,
    };
}

strview sv_init(const char *data) {
    return sv_from_parts(data, strlen(data));
}

void sv_chop_left(strview *s, size_t n) {
    if (n > s->len) n = s->len;
    s->len -= n;
    s->data  += n;
}

void sv_chop_right(strview *s, size_t n) {
    if (n > s->len) n = s->len;
    s->len -= n;
}

void sv_trim_left(strview *s) {
    while (s->len > 0 && isspace((unsigned char)s->data[0])) {
        sv_chop_left(s, 1);
    }
}

void sv_trim_right(strview *s) {
    while (s->len > 0 && isspace((unsigned char)s->data[s->len - 1])) {
        sv_chop_right(s, 1);
    }
}

void sv_trim(strview *s) {
    sv_trim_left(s);
    sv_trim_right(s);
}

strview sv_chop_by_delim(strview *s, char delim) {
    size_t i = 0;
    strview res;

    while (i < s->len && s->data[i] != delim)
        i += 1;
    res = sv_from_parts(s->data, i);
    sv_chop_left(s, i < s->len ? i + 1 : i);
    return res;
}

int sv_eq(strview a, strview b) {
    if (a.len != b.len) return 0;
    if (a.len == 0) return 1;
    return memcmp(a.data, b.data, a.len) == 0;
}

#endif
