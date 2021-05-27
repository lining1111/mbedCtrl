//
// Created by root on 2021/2/28.
//

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rb {

    pthread_mutex_t rwlock;
    pthread_cond_t cond;
    uint8_t *buff;

    size_t capacity;
    int read_pos;
    int write_pos;
    int available_for_read;
    //int available_for_write = rb_capacity - available_for_read;
} RingBuffer;

RingBuffer *RingBuffer_New(size_t capacity);

void RingBuffer_Delete(RingBuffer *rb);

size_t RingBuffer_Read(RingBuffer *rb, void *data, size_t count);

size_t RingBuffer_Write(RingBuffer *rb, const void *data, size_t count);

#ifdef __cplusplus
}
#endif

#endif //DRIVER_RINGBUFFER_H
