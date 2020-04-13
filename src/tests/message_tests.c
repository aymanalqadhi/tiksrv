#include "net/message.h"

#include "unity.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef TEST_ITERATIONS
#    define TEST_ITERATIONS 0x1000
#endif

void
setUp(void)
{}

void
tearDown(void)
{}

static void
request_message_decode_test(void)
{
    int      i, rc;
    uint32_t command, flags, seq_number, body_length;
    time_t   start_seed;
    char     buf[TS_MESSAGE_REQUEST_HEADER_SIZE];

    struct ts_request_header header;

    start_seed = time(NULL);
    srand(time(NULL));

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        command     = rand();
        flags       = rand();
        seq_number  = rand();
        body_length = rand();

        header.command     = command;
        header.flags       = flags;
        header.seq_number  = seq_number;
        header.body_length = body_length;

        rc = ts_encode_request_header(&header, (void *)buf, sizeof(buf) - 1);
        TEST_ASSERT_LESS_THAN(0, rc);

        rc = ts_encode_request_header(&header, (void *)buf, sizeof(buf));

        memset((void *)&header, 0, sizeof(header));

        rc = ts_decode_request_header(&header, (const void *)buf, sizeof(buf));
        TEST_ASSERT_EQUAL(0, rc);

        TEST_ASSERT_EQUAL(command, header.command);
        TEST_ASSERT_EQUAL(flags, header.flags);
        TEST_ASSERT_EQUAL(seq_number, header.seq_number);
        TEST_ASSERT_EQUAL(body_length, header.body_length);
    }
}

static void
response_message_decode_test(void)
{
    int      i, rc;
    uint16_t code;
    uint32_t flags, seq_number, body_length;
    time_t   start_seed;
    char     buf[TS_MESSAGE_RESPONSE_HEADER_SIZE];

    struct ts_response_header header;

    start_seed = time(NULL);
    srand(time(NULL));

    for (i = 0; i < TEST_ITERATIONS; ++i) {
        code        = rand();
        flags       = rand();
        seq_number  = rand();
        body_length = rand();

        header.code        = code;
        header.flags       = flags;
        header.seq_number  = seq_number;
        header.body_length = body_length;

        rc = ts_encode_response_header(&header, (void *)buf, sizeof(buf) - 1);
        TEST_ASSERT_LESS_THAN(0, rc);

        rc = ts_encode_response_header(&header, (void *)buf, sizeof(buf));

        memset((void *)&header, 0, sizeof(header));

        rc = ts_decode_response_header(&header, (const void *)buf, sizeof(buf));
        TEST_ASSERT_EQUAL(0, rc);

        TEST_ASSERT_EQUAL(code, header.code);
        TEST_ASSERT_EQUAL(flags, header.flags);
        TEST_ASSERT_EQUAL(seq_number, header.seq_number);
        TEST_ASSERT_EQUAL(body_length, header.body_length);
    }
}

int
main(int argc, char *argv[])
{

    UNITY_BEGIN();

    RUN_TEST(request_message_decode_test);
    RUN_TEST(response_message_decode_test);

    return UNITY_END();
}
