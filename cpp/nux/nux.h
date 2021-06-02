#pragma once
/**
 \file
*/
#ifndef furiosa_libnux_bindings_h
#define furiosa_libnux_bindings_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum Axis {
  axis_width = 0,
  axis_height = 1,
  /**
   * For convolution weights, this axis is also known as an input channel.
   */
  axis_channel = 2,
  /**
   * For convolution weights, this axis is also known as an output channel or feature map.
   */
  axis_batch = 3,
  axis_width_outer = 4,
  axis_height_outer = 5,
  axis_channel_outer = 6,
  axis_batch_outer = 7,
  axis_unknown = 8,
} Axis;

/**
 * \brief Represent tensor data type
 */
typedef enum TensorDataType {
  tensor_data_type_float32 = 0,
  tensor_data_type_uint8 = 1,
  tensor_data_type_int8 = 2,
  tensor_data_type_int32 = 3,
} TensorDataType;

/**
 * \brief Represent a return status of a function that can fail.
 */
typedef enum nux_error_t {
  /**
   * When a function call is successful
   */
  nux_error_t_success = 0,
  /**
   * When #create_nux fails to initialize
   */
  nux_error_t_nux_creation_failed = 1,
  /**
   * When deploying a model to an executor fails.
   * See also #nux_create_sync_model, #nux_create_task_model for more details.
   */
  nux_error_t_model_deploy_failed = 2,
  /**
   * When running an inference task fails.
   * See also #model_run, #task_execute for more details.
   */
  nux_error_t_model_execution_failed = 3,
  /**
   * When the given index of an input tensor is invalid. See also #model_input_tensor
   * for more details.
   */
  nux_error_t_invalid_input_index = 4,
  /**
   * When the given index of an output tensor is invalid. See also #model_output_tensor
   * for more details.
   */
  nux_error_t_invalid_output_index = 5,
  /**
   * When a data buffer is invalid. See #tensor_set_buffer for more details.
   */
  nux_error_t_invalid_buffer = 6,
  /**
   * When the input tensors are invalid
   */
  nux_error_t_invalid_inputs = 7,
  /**
   * When the output tensors are invalid
   */
  nux_error_t_invalid_outputs = 8,
  /**
   * When #task_model_get_task or #task_model_try_get_task fails to return an available
   * task slot. This error code can have different meanings in both functions.
   * Please see #task_model_get_task and #task_model_try_get_task for more details.
   */
  nux_error_t_get_task_failed = 9,
  /**
   * reserved
   */
  nux_error_t_dump_profile_failed = 10,
  /**
   * When waiting for completion queue is timed out
   */
  nux_error_t_queue_wait_timeout = 11,
  /**
   * When there's no data in completion queue
   */
  nux_error_t_queue_no_data = 12,
  /**
   * When loading a model image fails
   */
  nux_error_t_incompatible_model = 13,
  /**
   * When the compiler fails to transform a given model image to a binary for NPU
   */
  nux_error_t_compilation_failed = 14,
  /**
   * Internal error
   */
  nux_error_t_internal_error = 15,
  /**
   * Invalid yaml syntax
   */
  nux_error_t_invalid_yaml = 16,
  /**
   * incompatible api client error
   */
  nux_error_t_incompatible_api_client_error = 17,
  /**
   * when api client fails to initialize
   */
  nux_error_t_api_client_init_failed = 18,
  /**
   * No API Key
   */
  nux_error_t_no_api_key = 19,
  /**
   * Null pointer exception
   */
  nux_error_t_null_pointer_exception = 20,
} nux_error_t;

typedef struct BlockingSession BlockingSession;

typedef struct CompletionQueue_ContextPtr CompletionQueue_ContextPtr;

typedef struct Model Model;

typedef struct Nux Nux;

typedef struct NuxModel NuxModel;

typedef struct OpaqueTensor OpaqueTensor;

typedef struct SessionOptions SessionOptions;

typedef struct Submitter_ContextPtr Submitter_ContextPtr;

typedef struct Task Task;

typedef struct TaskModel TaskModel;

typedef struct TensorArray TensorArray;

typedef struct TensorDesc TensorDesc;

/**
 * \brief Nux handle
 *
 * #create_nux initializes an instance of #nux_handle_t.
 */
typedef struct Nux *nux_handle_t;

/**
 * \brief Byte length of a data buffer
 */
typedef uintptr_t nux_buffer_len_t;

/**
 * \brief Handle of a synchronous model
 *
 * #nux_create_sync_model initializes an instance of #nux_sync_model_t.
 */
typedef struct Model *nux_sync_model_t;

/**
 * \brief Handle of tensor
 *
 * #model_input_tensor or #model_output_tensor returns an instance of #nux_tensor_t.
 */
typedef struct OpaqueTensor *nux_tensor_t;

/**
 * \brief Pointer to a data buffer.
 *
 * In Nux, it is commonly used to contain any kind of binary data like ENF binary,
 * or a data buffer of input/output tensors.
 */
typedef uint8_t *nux_buffer_t;

typedef uintptr_t RequestId;

/**
 * \brief Identifier to distinguish tasks in a task model
 */
typedef RequestId nux_request_id_t;

typedef uint32_t OutputIndex;

/**
 * \brief Index of an output tensor, used in task model
 */
typedef OutputIndex nux_output_index_t;

/**
 * \brief Callback function called per tensor output
 * when a task of a task model is completed.
 *
 * For example, if one model has 2 output tensors,
 * this function will be called 2 times upon each task's completion.
 */
typedef void (*nux_output_callback_t)(nux_request_id_t, nux_output_index_t, nux_buffer_t, nux_buffer_len_t);

/**
 * \brief Callback function called when a task fails to complete.
 */
typedef void (*nux_error_callback_t)(nux_request_id_t, enum nux_error_t);

/**
 * \brief Callback function which is finally called when a task is completed.
 *
 * Unlike #nux_output_callback_t, this callback function is called only once
 * after #nux_output_callback_t is called for all output tensors.
 */
typedef void (*nux_finish_callback_t)(nux_request_id_t);

/**
 * \brief Handle of a task model
 *
 * #nux_create_task_model creates an instance of #nux_task_model_t.
 */
typedef struct TaskModel *nux_task_model_t;

/**
 * \brief Handle of a task of a task model
 *
 * #task_model_get_task or #task_model_try_get_task returns an instance of #nux_task_t.
 */
typedef struct Task *nux_task_t;

typedef uint32_t InputIndex;

/**
 * \brief Index of an input tensor, used in task model
 */
typedef InputIndex nux_input_index_t;

/**
 * \brief Session option
 */
typedef struct SessionOptions *nux_session_option_t;

/**
 * \brief Session
 */
typedef const struct BlockingSession *nux_session_t;

/**
 * \brief Model
 */
typedef const struct NuxModel *nux_model_t;

typedef struct TensorArray *nux_tensor_array_t;

/**
 * \brief Async Session
 */
typedef struct Submitter_ContextPtr *nux_async_session_t;

typedef struct CompletionQueue_ContextPtr *nux_completion_queue_t;

/**
 * \brief Context pointer
 */
typedef void *nux_context_t;

typedef const struct TensorDesc *nux_tensor_desc_t;

/**
 * \brief Represent a data type of tensor
 */
typedef enum TensorDataType nux_tensor_dtype_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * \brief Return a version of Nux native library
 */
const char *version(void);

/**
 * \brief Return a short Git hash of Nux native library
 */
const char *git_short_hash(void);

/**
 * \brief Return a build timestamp of Nux native library
 */
const char *build_timestamp(void);

/**
 * \brief Enable a logger.
 */
void enable_logging(void);

/**
 * \brief Enable a furiosa logger.
 */
void enable_furiosa_logging(void);

/**
 * \brief Creates a new nux handle.
 *
 * @param[out] nux Mutable pointer to receive a created Nux handle
 * @return #nux_error_t_success if successful, or
 * it will return #nux_error_t_nux_creation_failed.
 */
enum nux_error_t create_nux(nux_handle_t *nux);

/**
 * \brief Destroy the Nux handle and release its resources.
 *
 * @param nux Nux handle to be destroyed. It must not be NULL.
 */
void destroy_nux(nux_handle_t nux);

/**
 * \brief Create an instance of a synchronous model.
 *
 * It creates a synchronous model to execute a compiled ENF binary.
 * The synchronous model provides APIs to run a single inference task
 * with one input batch at a time.
 *
 * @param nux Nux handle
 * @param buffer Byte buffer containing ENF binary
 * @param len Byte length of `buffer`
 * @param[out] model Mutable pointer to receive the handle of a created sync model
 */
enum nux_error_t nux_create_sync_model(nux_handle_t nux,
                                       const uint8_t *buffer,
                                       nux_buffer_len_t len,
                                       nux_sync_model_t *model);

/**
 * \brief Destroy the sync model and release its resources.
 *
 * @param model Sync model to be destroyed, and it must not be NULL.
 */
void destroy_sync_model(nux_sync_model_t model);

/**
 * \brief Return the number of input tensors of a given sync model.
 *
 * A model can have more than one input tensor.
 * This function returns how many input tensors this model has.
 *
 * @param model Sync model
 * @return Output tensors of this model
 */
uint32_t model_count_inputs(nux_sync_model_t model);

/**
 * \brief Return the number of output tensors of a given sync model.
 *
 * A model can have more than one output tensor.
 * This function returns how many output tensors this model has.
 *
 * @param model Sync model
 * @return Output tensors of this model
 */
uint32_t model_count_outputs(nux_sync_model_t model);

/**
 * \brief Get the handle of a specified input tensor from a given sync model.
 *
 * \remark An input tensor handle will be valid
 * until #destroy_sync_model with this `model` is called.
 *
 * @param model Sync model
 * @param index Input tensor index
 * @param[out] tensor Mutable pointer to receive the handle of a specified input tensor
 */
enum nux_error_t model_input_tensor(nux_sync_model_t model, uint32_t index, nux_tensor_t *tensor);

/**
 * \brief Get the handle of a specified output tensor from a given sync model.
 *
 * \remark Output tensor handle will be valid
 * until #destroy_sync_model with this `model` is called.
 *
 * @param model Sync model
 * @param index Output tensor index
 * @param[out] tensor Mutable pointer to receive a specified output tensor handle
 */
enum nux_error_t model_output_tensor(nux_sync_model_t model, uint32_t index, nux_tensor_t *tensor);

/**
 * \brief Run a single inference task
 *
 * Before calling this function, you must fill input tensors with proper data.
 * Please refer to #model_input_tensor and #tensor_set_buffer
 * to learn how to fill input tensors with data.
 */
enum nux_error_t model_run(nux_sync_model_t model);

/**
 * \brief Get a pointer to the data buffer of a given tensor.
 *
 * Once #model_run is called, the inference result will be written into output tensors.
 * This function returns a pointer to the data buffer of a specified output tensor.
 *
 * \remark the buffers of output tensors are valid until #destroy_sync_model is called.
 *
 * @param tensor Tensor from which you want to get a pointer of the data buffer
 * @param[out] buffer Mutable pointer to receive the pointer to the data buffer
 * @param[out] len Byte length of `buffer`
 * @return nux_error_t_success if successful
 */
enum nux_error_t tensor_get_buffer(nux_tensor_t tensor, nux_buffer_t *buffer, nux_buffer_len_t *len);

/**
 * \brief Copy data into the data buffer of a specified input tensor.
 *
 * To execute #model_run, you first need to fill input tensors with data.
 * #tensor_fill_buffer copies the data into the data buffer of a specified input tensor.
 *
 * @param tensor Tensor handle into which you want to copy input data
 * @param buffer Pointer to the data buffer
 * @param len Byte length of `buffer`
 * @return nux_error_t_success if successful,
 * or nux_error_t_invalid_buffer if `buffer` is invalid.
 */
enum nux_error_t tensor_fill_buffer(nux_tensor_t tensor, nux_buffer_t buffer, nux_buffer_len_t len);

/**
 * \brief Specify a pointer to a data buffer to a specified input tensor.
 *
 * This function is almost same to #tensor_fill_buffer.
 * However, `tensor` takes the ownership of the buffer with `free` function
 * to deallocate the buffer. Ensure that nothing else uses the pointer after calling this function.
 *
 * if `free` is `null`, `tensor` will not deallocate the buffer.
 * It can be useful when you want to reuse the buffer or buffer is on a stack area of a function.
 * Please ensure that the buffer must be deallocated manually by users
 * in order to prevent the memory leak.
 *
 * @param tensor Tensor handle into which you want to copy input data
 * @param buffer Pointer to the data buffer
 * @param len Byte length of `buffer`
 * @param free a function pointer to deallocate a buffer. If `free` is `null`,
 * `tensor` will not deallocate the data.
 * @return nux_error_t_success if successful,
 * or nux_error_t_invalid_buffer if `buffer` is invalid.
 */
enum nux_error_t tensor_set_buffer(nux_tensor_t tensor,
                                   nux_buffer_t buffer,
                                   nux_buffer_len_t len,
                                   void (*free)(uint8_t*, uintptr_t));

/**
 * \brief Create an instance of a task model.
 *
 * It allows users to run multiple inference tasks asynchronously and simultaneously.
 * When each task is completed or failed, corresponding callback functions will be called
 * with #nux_request_id_t, an identifier of a task request.
 * See also #task_execute for more details.
 *
 * @param nux Nux handle
 * @param buffer Byte buffer containing ENF binary
 * @param len Byte length of `buffer`
 * @param max_batch Number of concurrent running tasks
 * It can be limited according to internal configurations and HW capacity.
 * @param output_callback Callback function invoked when a task is completed.
 * It will be called per output tensor. Please see #nux_output_callback_t for more details.
 * @param error_callback Callback function invoked when a task is failed.
 * @param finish_callback Callback function which will be called finally
 * after the output_callback is called after all output tensors.
 * @param task_model Mutable pointer to receive the handle of a created task model
 */
enum nux_error_t nux_create_task_model(nux_handle_t nux,
                                       const uint8_t *buffer,
                                       nux_buffer_len_t len,
                                       uint32_t max_batch,
                                       nux_output_callback_t output_callback,
                                       nux_error_callback_t error_callback,
                                       nux_finish_callback_t finish_callback,
                                       nux_task_model_t *task_model);

/**
 * \brief Destroy the task model and release its resources.
 *
 * @param task_model Task model to be destroyed
 */
void destroy_task_model(nux_task_model_t task_model);

/**
 * \brief Return `true` if there's no running tasks, or `false` if any task is still running.
 *
 * @param task_model Task handle
 */
bool task_model_is_all_task_done(nux_task_model_t task_model);

/**
 * \brief Get a task handle from a specified task model.
 *
 * When there's no available task in a given task model,
 * it will block until new task is available.
 *
 * @param task_model Handle of a task model
 * @param[out] task Mutable pointer to receive the handle of a created task
 */
enum nux_error_t task_model_get_task(nux_task_model_t task_model, nux_task_t *task);

/**
 * \brief (non-blocking) Get a task handle from the specified task model.
 *
 * It's the non-blocking version of #task_model_get_task.
 * This function returns an available task, or
 * it will return immediately #nux_error_t_get_task_failed
 * if there's no available task in a given task model.
 *
 * @param task_model Handle of a task model
 * @param[out] task Mutable pointer to receive the handle of a created task
 */
enum nux_error_t task_model_try_get_task(nux_task_model_t task_model, nux_task_t *task);

/**
 * \brief Destroy the task and release its resources.
 *
 * \remark Once you call #task_execute with a `task`,
 * the `task` will get destroyed automatically. Please do not call #destroy_task with
 * the `task` which is already passed to #task_execute.
 *
 * @param task Task to be destroyed
 */
void destroy_task(nux_task_t task);

/**
 * \brief Request one asynchronous inference task.
 *
 * #task_execute submits a request for an inference task identified by `task`.
 * Once a task is completed, `output_callback` function passed to #nux_create_task_model
 * will be called with a distinct output index per output tensor.
 * `finish_callback` function will be also called
 * after `output_callback` function is called for all output tensors.
 *
 * \remark Once you call #task_execute with a `task`,
 * the `task` will get destroyed automatically. Please do not call #destroy_task with
 * the `task` which is already passed to #task_execute.
 *
 * @param task Task handle obtained from calling #task_model_get_task
 * or #task_model_try_get_task.
 * @param request_id An positive integer to distinguish task requests.
 * The behavior of #task_execute doesn't rely on a `request_id` value at all.
 * `request_id` will be just passed to callback functions.
 * @return #nux_error_t_success if successful.
 * or #nux_error_t_model_execution_failed if failed.
 */
enum nux_error_t task_execute(nux_task_t task, nux_request_id_t request_id);

/**
 * \brief Return a mutable pointer to the buffer of the specified input tensor.
 *
 * @param task Task handle
 * @param index Index of the input tensor
 * @return A mutable pointer to the data buffer of the given input tensor
 */
nux_buffer_t task_input(nux_task_t task, nux_input_index_t index);

/**
 * \brief Return the buffer length in bytes of the specified input tensor.
 *
 * @param task Task handle
 * @param index Index of the input tensor
 */
nux_buffer_len_t task_input_size(nux_task_t task, nux_input_index_t index);

/**
 * \brief Create a session option
 *
 * It allows to specify more options for a session.
 *
 * @return Session option
 */
nux_session_option_t nux_session_option_create(void);

/**
 * \brief Set a NPU device to the given session option
 *
 * If NPU device is not set, Session will use the default NPU device `npu0pe0`.
 *
 * @param option Session option
 * @param devname A NPU device name (e.g., npu0pe0, npu1pe0)
 */
void nux_session_option_set_device(nux_session_option_t builder, const char *devname);

/**
 * \brief Set a compiler config to the given session option
 *
 * If compiler config is not set, Session will use a default compiler config.
 *
 * @param option Session option
 * @param yaml A yaml formatted string of compiler option
 * @return #nux_error_t_success if successful.
 * or #nux_error_t_invalid_yaml if `yaml` is not a valid yaml document.
 */
enum nux_error_t nux_session_option_set_compiler_config(nux_session_option_t option, const char *yaml);

/**
 * \brief Destroy a session option
 */
void nux_session_option_destroy(nux_session_option_t option);

/**
 * \brief Create a session
 *
 * Session provides simple and synchronous APIs to run inference tasks from a loaded model.
 *
 * @param model_binary Buffer which contains a model binary
 * @param model_binary_len Length of `model_binary`
 * @param option Session option. If it is null, the default option will be used.
 * #nux_session_create internally releases the session option pointed to by `option`,
 * so `option` must not be dereferenced or destroyed manually (via #nux_session_option_destroy)
 * after calling #nux_session_create.
 * @param[out] sess Mutable pointer to receive a created Nux handle
 * @return #nux_error_t_success if successful.
 * or it will return other error value.
 */
enum nux_error_t nux_session_create(nux_buffer_t model_binary,
                                    nux_buffer_len_t model_binary_len,
                                    nux_session_option_t option,
                                    nux_session_t *out_sess);

/**
 * \brief Get the pointer of the Model of a given session
 *
 * @param sess Handle of session instance. It must not be `NULL`.
 * @return Pointer of the Model of a given session
 */
nux_model_t nux_session_get_model(nux_session_t sess);

/**
 * \brief Run an inference task
 *
 * @param sess Handle of Session instance. It must not be `NULL`.
 * @param inputs Input tensors
 * @param outputs Output tensors
 * @return #nux_error_t_success if successful.
 * or it will return other error value.
 */
enum nux_error_t nux_session_run(nux_session_t sess, nux_tensor_array_t inputs, nux_tensor_array_t outputs);

/**
 * \brief Destroy a session
 * @param sess Handle of Session instance to be destroyed. It must not be `NULL`.
 */
void nux_session_destroy(nux_session_t sess);

/**
 * \brief Create an asynchronous session and a queue to receive the results.
 *
 * #nux_async_session_t allows users to submit inference tasks.
 * #nux_completion_queue_t is an iterator to receive the result in the asynchronous way.
 * See also #nux_async_session_run and #nux_completion_queue_next.
 *
 * @param model_binary Buffer which contains a model binary
 * @param model_binary_len Length of `model_binary`
 * @param option Session option. If it is null, the default option will be used.
 * nux_session_create internally releases the session option pointed to by option,
 * so option must not be dereferenced or destroyed manually (via nux_session_option_destroy)
 * after calling nux_async_session_create.
 * @param[out] out_sess Mutable pointer to an instance of async session
 * @param[out] out_queue Mutable pointer to an instance of async completion queue
 */
enum nux_error_t nux_async_session_create(nux_buffer_t model_binary,
                                          nux_buffer_len_t model_binary_len,
                                          nux_session_option_t option,
                                          nux_async_session_t *out_sess,
                                          nux_completion_queue_t *out_queue);

/**
 * \brief Submit an inference task asynchronously. It returns immediately.
 *
 * You can receive the completed inference results through #nux_completion_queue_t.
 * please refer to #nux_completion_queue_next to know how to receive the completed results.
 *
 * @param sess Handle of asynchronous session
 * @param `context` is a void pointer, so you can pass any data to context.
 * Generally, `context` can be used to identify what's the completion event.
 * Also, sophisticated data structure can be used to to do more as soon as
 * you receive the completion event. You must make sure the pointer must be valid as
 * long as you want to use a passed context even after you receive the completion event.
 * @param inputs an array of tensors
 */
enum nux_error_t nux_async_session_run(nux_async_session_t sess, nux_context_t context, nux_tensor_array_t inputs);

/**
 * \brief Wait until it receives any completion event, which can be success or fail.
 */
bool nux_completion_queue_next(nux_completion_queue_t queue,
                               nux_context_t *context,
                               nux_tensor_array_t *outputs,
                               enum nux_error_t *error);

/**
 * \brief It's almost the same as #nux_completion_queue_next,
 * but it immediately returns even if there hasn't been completion event after the last call.
 * It will set `error` to #nux_error_t_queue_no_data.
 * if this function returns without any completion event.
 */
bool nux_completion_queue_try_next(nux_completion_queue_t queue,
                                   nux_context_t *context,
                                   nux_tensor_array_t *outputs,
                                   enum nux_error_t *error);

/**
 * \brief It's almost the same as #nux_completion_queue_next,
 * but it will wait for a given timeout in milli seconds even if there's no completion event.
 * After the timeout is expired, it will set #nux_error_t_queue_wait_timeout to `error`.
 */
bool nux_completion_queue_next_timeout(nux_completion_queue_t queue,
                                       nux_context_t *context,
                                       nux_tensor_array_t *outputs,
                                       enum nux_error_t *error,
                                       uint64_t timeout_millis);

/**
 * \brief Get the pointer of the Model of a given asynchronous session
 *
 * @param sess Handle of asynchronous session instance. It must not be `NULL`.
 * @return Pointer of the Model of a given asynchronous session
 */
nux_model_t nux_async_session_get_model(nux_async_session_t sess);

/**
 * \brief Destroy an asynchronous session
 */
void nux_async_session_destroy(nux_async_session_t sess);

/**
 * \brief Destroy a completion queue
 */
void nux_completion_queue_destroy(nux_completion_queue_t queue);

/**
 * \brief Return the number of input tensors of the given model
 */
uintptr_t nux_input_num(nux_model_t model);

/**
 * \brief Return the number of output tensors of the given model
 */
uintptr_t nux_output_num(nux_model_t model);

/**
 * \brief Return the description of i-th input tensor of the given model
 */
nux_tensor_desc_t nux_input_desc(nux_model_t model, uintptr_t index);

/**
 * \brief Return the description of i-th output tensor of the given model
 */
nux_tensor_desc_t nux_output_desc(nux_model_t model, uintptr_t index);

/**
 * \brief Return the number of dimensions of the given tensor
 */
uintptr_t nux_tensor_dim_num(nux_tensor_desc_t tensor);

/**
 * \brief Return the dimension size of the dimension index
 */
uintptr_t nux_tensor_dim(nux_tensor_desc_t tensor, uintptr_t index);

/**
 * \brief Return the axis type of i-th dimension (e.g., Width, Height, Channel)
 */
enum Axis nux_tensor_axis(nux_tensor_desc_t tensor, uintptr_t idx);

/**
 * \brief Return the tensor data type
 */
nux_tensor_dtype_t nux_tensor_dtype(nux_tensor_desc_t tensor);

/**
 * \brief Return the total number of all elements
 */
uintptr_t nux_tensor_len(nux_tensor_desc_t tensor);

/**
 * \brief Return the byte size of tensor
 *
 * It will be useful when you have to fill the tensor buffer with data.
 */
uintptr_t nux_tensor_size(nux_tensor_desc_t tensor);

/**
 * \brief Create a tensor according to a given tensor description
 *
 * You have to call `nux_tensor_destroy` to release the resource of a create tensor.
 */
nux_tensor_t nux_tensor_create(nux_tensor_desc_t desc);

/**
 * \brief Create a tensor with an exiting buffer. It doesn't copy the data.
 */
nux_tensor_t nux_tensor_create_with_buffer(nux_tensor_desc_t desc,
                                           nux_buffer_t buffer,
                                           nux_buffer_len_t len,
                                           void (*free)(uint8_t*, uintptr_t));

/**
 * \brief Destroy a tensor
 */
void nux_tensor_destroy(nux_tensor_t tensor);

/**
 * \brief Return the pointer to the buffer of this tensor
 */
uint8_t *nux_tensor_buffer(nux_tensor_t tensor);

/**
 * \brief Return the size of tensor buffer
 */
uintptr_t nux_tensor_buffer_size(nux_tensor_t tensor);

/**
 * \brief Return an array of input tensors with allocated buffers
 *
 * This function is generally used with #tensor_fill_buffer.
 */
nux_tensor_array_t nux_tensor_array_allocate_inputs(nux_model_t model);

/**
 * \brief Return an array of input tensors without allocated buffers
 *
 * This function is generally used with #tensor_set_buffer.
 */
nux_tensor_array_t nux_tensor_array_create_inputs(nux_model_t model);

/**
 * \brief Return an array of output tensors with allocated buffers
 */
nux_tensor_array_t nux_tensor_array_allocate_outputs(nux_model_t model);

/**
 * \brief Return an array of output tensors without allocated buffers
 */
nux_tensor_array_t nux_tensor_array_create_outputs(nux_model_t model);

/**
 * \brief Return a tensor from the given tensor array
 */
nux_tensor_t nux_tensor_array_get(nux_tensor_array_t array, uintptr_t index);

/**
 * \brief Return the number of tensors in the given tensor array
 */
uintptr_t nux_tensor_array_len(nux_tensor_array_t array);

/**
 * \brief Destroy a tensor array
 */
void nux_tensor_array_destroy(nux_tensor_array_t array);



#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* furiosa_libnux_bindings_h */