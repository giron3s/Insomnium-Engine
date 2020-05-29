/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#pragma once

#ifdef ENVIRONMENT64
    #include "optick.h"
#endif

#ifdef ENVIRONMENT64 && _DEBUG
    #define PROFILER_EVENT(...)                                                                                 OPTICK_EVENT(__VA_ARGS__)
    #define PROFILER_CATEGORY(NAME, CATEGORY)                                                                   OPTICK_CATEGORY(NAME, CATEGORY)
    #define PROFILER_FRAME(NAME)                                                                                OPTICK_FRAME(NAME)
    #define PROFILER_THREAD(THREAD_NAME)                                                                        OPTICK_THREAD(THREAD_NAME)
    #define PROFILER_START_THREAD(THREAD_NAME)                                                                  OPTICK_START_THREAD(THREAD_NAME)
    #define PROFILER_STOP_THREAD()                                                                              OPTICK_STOP_THREAD()
    #define PROFILER_TAG(NAME, DATA)                                                                            OPTICK_TAG(NAME, DATA)
    #define PROFILER_EVENT_DYNAMIC(NAME)	                                                                    OPTICK_EVENT_DYNAMIC(NAME)
    #define PROFILER_PUSH_DYNAMIC(NAME)		                                                                    OPTICK_PUSH_DYNAMIC(NAME)
    #define PROFILER_PUSH(NAME)				                                                                    OPTICK_PUSH(NAME)
    #define PROFILER_POP()		                                                                                OPTICK_POP()
    #define PROFILER_CUSTOM_EVENT(DESCRIPTION)                                                                  OPTICK_CUSTOM_EVENT(DESCRIPTION)
    #define PROFILER_STORAGE_REGISTER(STORAGE_NAME)                                                             OPTICK_STORAGE_REGISTER(STORAGE_NAME)
    #define PROFILER_STORAGE_EVENT(STORAGE, DESCRIPTION, CPU_TIMESTAMP_START, CPU_TIMESTAMP_FINISH)             OPTICK_STORAGE_EVENT(STORAGE, DESCRIPTION, CPU_TIMESTAMP_START, CPU_TIMESTAMP_FINISH)
    #define PROFILER_STORAGE_PUSH(STORAGE, DESCRIPTION, CPU_TIMESTAMP_START)                                    OPTICK_STORAGE_PUSH(STORAGE, DESCRIPTION, CPU_TIMESTAMP_START)
    #define PROFILER_STORAGE_POP(STORAGE, CPU_TIMESTAMP_FINISH)				                                    OPTICK_STORAGE_POP(STORAGE, CPU_TIMESTAMP_FINISH)
    #define PROFILER_SET_STATE_CHANGED_CALLBACK(CALLBACK)                                                       OPTICK_SET_STATE_CHANGED_CALLBACK(CALLBACK)
    #define PROFILER_SET_MEMORY_ALLOCATOR(ALLOCATE_FUNCTION, DEALLOCATE_FUNCTION, INIT_THREAD_CALLBACK)	        OPTICK_SET_MEMORY_ALLOCATOR(ALLOCATE_FUNCTION, DEALLOCATE_FUNCTION, INIT_THREAD_CALLBACK)
    #define PROFILER_SHUTDOWN()                                                                                 OPTICK_SHUTDOWN()
    #define PROFILER_GPU_INIT_D3D12(DEVICE, CMD_QUEUES, NUM_CMD_QUEUS)                                          OPTICK_GPU_INIT_D3D12(DEVICE, CMD_QUEUES, NUM_CMD_QUEUS)
    #define PROFILER_GPU_INIT_VULKAN(DEVICES, PHYSICAL_DEVICES, CMD_QUEUES, CMD_QUEUES_FAMILY, NUM_CMD_QUEUS)   OPTICK_GPU_INIT_VULKAN(DEVICES, PHYSICAL_DEVICES, CMD_QUEUES, CMD_QUEUES_FAMILY, NUM_CMD_QUEUS)
    #define PROFILER_GPU_CONTEXT(...)                                                                           OPTICK_GPU_CONTEXT(...)
    #define PROFILER_GPU_EVENT(NAME)                                                                            OPTICK_GPU_EVENT(NAME)
    #define PROFILER_GPU_FLIP(SWAP_CHAIN)                                                                       OPTICK_GPU_FLIP(SWAP_CHAIN)
    #define PROFILER_UPDATE()                                                                                   OPTICK_UPDATE()
    #define PROFILER_FRAME_FLIP(...)                                                                            OPTICK_FRAME_FLIP(__VA_ARGS__)
    #define PROFILER_START_CAPTURE(...)                                                                         OPTICK_START_CAPTURE(__VA_ARGS__)
    #define PROFILER_STOP_CAPTURE()                                                                             OPTICK_STOP_CAPTURE()
    #define PROFILER_SAVE_CAPTURE(...)                                                                          OPTICK_SAVE_CAPTURE(__VA_ARGS__)
    #define PROFILER_APP(NAME)                                                                                  OPTICK_APP(NAME)
#else
    #define PROFILER_EVENT(...)
    #define PROFILER_CATEGORY(NAME, CATEGORY)
    #define PROFILER_FRAME(NAME)
    #define PROFILER_THREAD(THREAD_NAME)
    #define PROFILER_START_THREAD(THREAD_NAME)
    #define PROFILER_STOP_THREAD()
    #define PROFILER_TAG(NAME, DATA)
    #define PROFILER_EVENT_DYNAMIC(NAME)
    #define PROFILER_PUSH_DYNAMIC(NAME)
    #define PROFILER_PUSH(NAME)
    #define PROFILER_POP()
    #define PROFILER_CUSTOM_EVENT(DESCRIPTION)
    #define PROFILER_STORAGE_REGISTER(STORAGE_NAME)
    #define PROFILER_STORAGE_EVENT(STORAGE, DESCRIPTION, CPU_TIMESTAMP_START, CPU_TIMESTAMP_FINISH)
    #define PROFILER_STORAGE_PUSH(STORAGE, DESCRIPTION, CPU_TIMESTAMP_START)
    #define PROFILER_STORAGE_POP(STORAGE, CPU_TIMESTAMP_FINISH)
    #define PROFILER_SET_STATE_CHANGED_CALLBACK(CALLBACK)
    #define PROFILER_SET_MEMORY_ALLOCATOR(ALLOCATE_FUNCTION, DEALLOCATE_FUNCTION, INIT_THREAD_CALLBACK)
    #define PROFILER_SHUTDOWN()
    #define PROFILER_GPU_INIT_D3D12(DEVICE, CMD_QUEUES, NUM_CMD_QUEUS)
    #define PROFILER_GPU_INIT_VULKAN(DEVICES, PHYSICAL_DEVICES, CMD_QUEUES, CMD_QUEUES_FAMILY, NUM_CMD_QUEUS)
    #define PROFILER_GPU_CONTEXT(...)
    #define PROFILER_GPU_EVENT(NAME)
    #define PROFILER_GPU_FLIP(SWAP_CHAIN)
    #define PROFILER_UPDATE()
    #define PROFILER_FRAME_FLIP(...)
    #define PROFILER_START_CAPTURE(...)
    #define PROFILER_STOP_CAPTURE()
    #define PROFILER_SAVE_CAPTURE(...)
    #define PROFILER_APP(NAME)
#endif
