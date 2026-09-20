/*
 * examples.c - usage examples for strvw.h
 *
 * Build:
 * cc -std=c99 -Wall -Wextra -o examples examples.c
 */

#define STRVW_IMPLEMENTATION
#include "strvw.h"

#include <stdio.h>

static void example_chop_left(void) {
    strview s = sv_init("chop me from the left");
    printf("orig=\"" SV_FMT "\", len=%zu\n", SV_ARG(s), s.len);
    // chop 3 chars from the left
    sv_chop_left(&s, 3);
    printf("  after chop=\"" SV_FMT "\", len=%zu\n", SV_ARG(s), s.len);
}

static void example_chop_right(void) {
    strview s = sv_init("chop me from the right");
    printf("orig=\"" SV_FMT "\", len=%zu\n", SV_ARG(s), s.len);
    // chop 3 chars from the right
    sv_chop_right(&s, 3);
    printf("  after chop=\"" SV_FMT "\", len=%zu\n", SV_ARG(s), s.len);
}

static void example_chop_too_much(void) {
    strview s = sv_init("short");
    // n larger than the view is clamped to len
    sv_chop_left(&s, 999);
    printf("chopped 999 of 5: len=%zu, empty=%d\n", s.len, s.len == 0);
}

static void example_trim(void) {
    strview s = sv_init("  \t padded with space \n ");
    printf("before trim=\"" SV_FMT "\" (len=%zu)\n", SV_ARG(s), s.len);
    sv_trim(&s);
    printf("  after trim=\"" SV_FMT "\" (len=%zu)\n", SV_ARG(s), s.len);

    // trim_left and trim_right are available on their own
    strview one_side = sv_init("   only the left   ");
    sv_trim_left(&one_side);
    printf("  trim_left only=\"" SV_FMT "\"\n", SV_ARG(one_side));
}

static void example_key_value(void) {
    strview line = sv_init("  name =  strvw  ");
    strview key;

    // chop_by_delim returns everything before the delim and advances
    // the view past it
    key = sv_chop_by_delim(&line, '=');
    sv_trim(&key);
    sv_trim(&line);

    printf("key=\"" SV_FMT "\" value=\"" SV_FMT "\"\n", SV_ARG(key), SV_ARG(line));
}

static void example_split_fields(void) {
    strview csv = sv_init("a,b,,c");

    printf("fields:");
    while (csv.len > 0) {
        strview field = sv_chop_by_delim(&csv, ',');
        printf(" [" SV_FMT "]", SV_ARG(field));
    }
    printf("\n");
}

static void example_split_lines(void) {
    strview text = sv_init("first line\nsecond line\n\nfourth");
    int n = 0;

    while (text.len > 0) {
        strview line = sv_chop_by_delim(&text, '\n');
        printf("line %d: \"" SV_FMT "\"\n", ++n, SV_ARG(line));
    }
}

static void example_eq(void) {
    strview a = sv_init("strvw");
    strview b = sv_init("strvw");
    strview c = sv_init("strvü");

    printf("eq(a,b)=%d eq(a,c)=%d\n", sv_eq(a, b), sv_eq(a, c));

    // comparing a chopped field against a literal is the common case
    strview line = sv_init("mode=fast");
    strview key = sv_chop_by_delim(&line, '=');
    if (sv_eq(key, sv_init("mode")))
        printf("mode is \"" SV_FMT "\"\n", SV_ARG(line));
}

static void example_from_parts(void) {
    const char raw[6] = { 'n', 'o', ' ', 'n', 'u', 'l' };
    strview s = sv_from_parts(raw, sizeof raw);

    printf("raw view=\"" SV_FMT "\", len=%zu\n", SV_ARG(s), s.len);

    // views into the middle of a buffer are just pointer arithmetic
    strview middle = sv_from_parts(raw + 3, 3);
    printf("middle=\"" SV_FMT "\"\n", SV_ARG(middle));
}

static void example_no_copy(void) {
    const char *buf = "shared backing buffer";
    strview whole = sv_init(buf);
    strview word = whole;

    sv_chop_right(&word, whole.len - 6);

    printf("whole=\"" SV_FMT "\" word=\"" SV_FMT "\"\n",
           SV_ARG(whole), SV_ARG(word));
    printf("same buffer: %d\n", whole.data == word.data);

    printf("whole len still %zu\n", whole.len);
}

int main(void)
{
    example_chop_left();
    example_chop_right();
    example_chop_too_much();
    example_trim();
    example_key_value();
    example_split_fields();
    example_split_lines();
    example_eq();
    example_from_parts();
    example_no_copy();
    return 0;
}
