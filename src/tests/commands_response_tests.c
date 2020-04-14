#include "commands/response.h"

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
command_response_write_test(void)
{
    int      i, rc;
    char     strbuf1[0x40], strbuf2[0x40];
    FILE *   fbuf;
    uint8_t  b1, b2;
    uint16_t w1, w2;
    uint32_t d1, d2, len;
    uint64_t q1, q2;
    time_t   start_seed;

    struct ts_command_response resp;

    start_seed = time(NULL);
    srand(start_seed);

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        b1 = rand();
        w1 = rand();
        d1 = rand();
        q1 = rand();
        snprintf(strbuf1, sizeof(strbuf1), "Test string #%d", i);

        memset((void *)&resp, 0, sizeof(resp));

        rc = ts_response_write_byte(&resp, b1);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_response_write_uint16(&resp, w1);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_response_write_uint32(&resp, d1);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_response_write_uint64(&resp, q1);
        TEST_ASSERT_EQUAL(0, rc);
        rc = ts_response_write_string(
            &resp, strbuf1, strnlen(strbuf1, sizeof(strbuf1)));
        TEST_ASSERT_EQUAL(0, rc);
        TEST_ASSERT_NOT_NULL(resp.body_buffer);

        fbuf = fmemopen(resp.body_buffer, resp.buffer_length, "r");
        TEST_ASSERT_NOT_NULL(fbuf);

        fread((void *)&b2, sizeof(b2), 1, fbuf);
        fread((void *)&w2, sizeof(w2), 1, fbuf);
        fread((void *)&d2, sizeof(d2), 1, fbuf);
        fread((void *)&q2, sizeof(q2), 1, fbuf);
        fread((void *)&len, sizeof(len), 1, fbuf);

        len = be32toh(len);
        fread((void *)strbuf2, 1, len, fbuf);

        fclose(fbuf);
        free(resp.body_buffer);

        TEST_ASSERT_EQUAL(b1, b2);
        TEST_ASSERT_EQUAL(w1, be16toh(w2));
        TEST_ASSERT_EQUAL(d1, be32toh(d2));
        TEST_ASSERT_EQUAL(q1, be64toh(q2));
        TEST_ASSERT_EQUAL(strnlen(strbuf1, sizeof(strbuf1)), len);
        TEST_ASSERT_EQUAL_STRING_LEN(strbuf1, strbuf2, len);
    }
}

int
main(int argc, char *argv[])
{

    UNITY_BEGIN();

    RUN_TEST(command_response_write_test);

    return UNITY_END();
}
