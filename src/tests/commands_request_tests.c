#include "commands/request.h"
#include "commands/impl/request.h"

#include "unity.h"

#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef TEST_ITERATIONS
#    define TEST_ITERATIONS 0x1000
#endif

#define BUFFER_LEN 0x100

void
setUp(void)
{}

void
tearDown(void)
{}

static void
command_request_read_test(void)
{
    int      i, rc;
    char     buf[BUFFER_LEN];
    char     strbuf1[0x40], strbuf2[0x40];
    FILE *   fbuf;
    uint8_t  b1, b2;
    uint16_t w1, w2;
    uint32_t d1, d2, len;
    uint64_t q1, q2;
    time_t   start_seed;

    struct ts_command_request req;

    start_seed = time(NULL);
    srand(start_seed);

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        b1 = rand();
        w1 = rand();
        d1 = rand();
        q1 = rand();
        snprintf(strbuf1, sizeof(strbuf1), "Test string #%d", i);

        fbuf = fmemopen(buf, sizeof(buf), "w");
        TEST_ASSERT_NOT_NULL(fbuf);

        fwrite((const void *)&b1, sizeof(b1), 1, fbuf);
        fwrite((const void *)&w1, sizeof(w1), 1, fbuf);
        fwrite((const void *)&d1, sizeof(d1), 1, fbuf);
        fwrite((const void *)&q1, sizeof(q1), 1, fbuf);

        len = htobe32(strnlen(strbuf1, sizeof(strbuf1)));
        fwrite((const void *)&len, sizeof(len), 1, fbuf);
        fwrite(
            (const void *)strbuf1, 1, strnlen(strbuf1, sizeof(strbuf1)), fbuf);

        req.body_buffer = (const void *)buf;
        req.body_length = ftell(fbuf);
        req.current_pos = 0;
        req.flags       = 0;
        len             = sizeof(strbuf2);
        fclose(fbuf);

        rc = ts_request_read_byte(&req, &b2);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_request_read_uint16(&req, &w2);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_request_read_uint32(&req, &d2);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_request_read_uint64(&req, &q2);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_request_read_string(&req, strbuf2, &len);
        TEST_ASSERT_EQUAL(0, rc);

        TEST_ASSERT_EQUAL(b1, b2);
        TEST_ASSERT_EQUAL(w1, htobe16(w2));
        TEST_ASSERT_EQUAL(d1, htobe32(d2));
        TEST_ASSERT_EQUAL(q1, htobe64(q2));
        TEST_ASSERT_EQUAL(len, strnlen(strbuf1, sizeof(strbuf1)));
        TEST_ASSERT_EQUAL_STRING(strbuf1, strbuf2);
    }
}

int
main(int argc, char *argv[])
{

    UNITY_BEGIN();

    RUN_TEST(command_request_read_test);

    return UNITY_END();
}
