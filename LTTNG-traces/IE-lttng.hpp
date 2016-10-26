//
// Created by charles on 10/25/16.
//

#ifndef __IE_LTTNG_HPP__
#define __IE_LTTNG_HPP__

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER IE_test

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "IE-lttng.hpp"

#if !defined(_MY_TP_HPP) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _MY_TP_HPP

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
        IE_test,
        tracing_IE,
        TP_ARGS(
                size_t, input
        ),
        TP_FIELDS(
                ctf_integer(size_t, input_field, intput)
        )
)

#endif /*MY_TO_HPP*/
#include <lttng/tracepoint-event.h>

#endif
