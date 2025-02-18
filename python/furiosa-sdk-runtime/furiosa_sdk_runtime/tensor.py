"""Tensor object and its utilities"""

import ctypes
from ctypes import c_void_p, POINTER, c_uint64, c_uint8, byref
from enum import IntEnum
from typing import Union

import numpy as np

from ._util import list_to_dict
from .errors import UnsupportedTensorType
from ._api import LIBNUX


class Axis(IntEnum):
    """Axis of Tensor"""
    WIDTH = 0
    HEIGHT = 1
    CHANNEL = 2
    BATCH = 3
    WIDTH_OUTER = 4
    HEIGHT_OUTER = 5
    CHANNEL_OUTER = 6
    BATCH_OUTER = 7
    UNKNOWN = 8

    @classmethod
    def _names(cls):
        return ["W", "H", "C", "N", "Wo", "Ho", "Co", "No", "?"]

    def __repr__(self):
        return self._names()[self]


class DataType(IntEnum):
    """Tensor data type"""
    FLOAT32 = 0
    UINT8 = 1
    INT8 = 2
    INT32 = 3

    @classmethod
    def _names(cls):
        return ["float32", "uint8", "int8", "int32"]

    @classmethod
    def _numpy_dtypes(cls):
        return [np.float32, np.uint8, np.int8, np.int32]

    def __repr__(self) -> str:
        return self._names()[self]

    def numpy_dtype(self):
        """Return the numpy dtype corresponding to this DataType"""
        return self._numpy_dtypes()[self]


class TensorDesc:
    """Tensor description including dimension, shape, and data type"""
    ref = c_void_p(None)

    def __init__(self, ref):
        self.ref = ref
        self._as_parameter_ = ref

    def ndim(self) -> int:
        """Number of dimensions"""
        return LIBNUX.nux_tensor_dim_num(self)

    def dim(self, idx) -> int:
        """Size of i-th dimension"""
        return LIBNUX.nux_tensor_dim(self, idx)

    def shape(self) -> tuple:
        """tensor shape"""
        dims = []
        for i in range(self.ndim()):
            dims.append(self.dim(i))

        return tuple(dims)

    def axis(self, idx) -> Axis:
        """Axis type of i-th dimension (e.g., width, height, channel)"""
        return Axis(LIBNUX.nux_tensor_axis(self, idx))

    def size(self) -> int:
        """Size in bytes"""
        return LIBNUX.nux_tensor_size(self)

    def length(self) -> int:
        """Number of all elements across all dimensions"""
        return LIBNUX.nux_tensor_len(self)

    def format(self) -> str:
        """Tensor memory layout (e.g., NHWC, NCHW)"""
        tensor_format = str()
        for idx in range(self.ndim()):
            tensor_format += self.axis(idx).__repr__()

        return tensor_format

    def dtype(self) -> DataType:
        """Data type of tensor"""
        return DataType(LIBNUX.nux_tensor_dtype(self))

    def numpy_dtype(self):
        """Return numpy dtype"""
        return self.dtype().numpy_dtype()

    def __repr__(self) -> str:
        return self.__class__.__name__ + \
               ': shape=' + str(self.shape()) + \
               ', dtype=' + self.dtype().__repr__() + \
               ', format=' + self.format() + \
               ', size=' + str(self.size()) + \
               ', len=' + str(self.length())


class Tensor:
    """A tensor which contains data and tensor description including shape"""
    ref = c_void_p(None)
    allocated: bool = False
    desc = TensorDesc

    def __init__(self, ref, desc: TensorDesc, allocated=False):
        self.ref = ref
        self.desc = desc
        self.allocated = allocated
        self._as_parameter_ = ref

    def shape(self) -> tuple:
        """Return the tensor shape

        Returns:
            Tensor shape. An example shape is
            ```(1, 28, 28, 1)```.
        """
        return self.desc.shape()

    def dtype(self) -> DataType:
        """Data type of tensor"""
        return self.desc.dtype()

    def numpy_dtype(self):
        """Return numpy dtype"""
        return self.desc.numpy_dtype()

    def copy_from(self, data: Union[np.ndarray, np.generic]):
        """Copy the contents of Numpy ndarray to this tensor"""
        if isinstance(data, np.ndarray):
            data = np.ascontiguousarray(data)
        if isinstance(data, (np.ndarray, np.generic)):
            buf_ptr = data.ctypes.data_as(POINTER(c_uint8))
            buf_size_in_bytes = data.nbytes
            LIBNUX.tensor_fill_buffer(self, buf_ptr, buf_size_in_bytes)
        else:
            raise UnsupportedTensorType()

    def numpy(self) -> np.ndarray:
        """Return numpy.ndarray converted from this tensor"""
        itemsize = np.dtype(self.numpy_dtype()).itemsize
        arr_size = np.prod(self.shape()[:]) * itemsize

        buf_ptr = POINTER(c_uint8)()
        buf_len = c_uint64(0)
        LIBNUX.tensor_get_buffer(self, byref(buf_ptr), byref(buf_len))

        buf_from_mem = ctypes.pythonapi.PyMemoryView_FromMemory
        buf_from_mem.restype = ctypes.py_object
        buf_from_mem.argtypes = (ctypes.c_void_p, ctypes.c_int, ctypes.c_int)
        buffer = buf_from_mem(buf_ptr, arr_size, 0x100)

        arr = np.ndarray(tuple(self.shape()[:]), self.numpy_dtype(), buffer, order="C")
        return arr.copy()

    def __repr__(self):
        return '<' + self.__class__.__name__ + \
               ': shape=' + str(self.desc.shape()) + \
               ', dtype=' + str(self.desc.dtype()) + \
               ', numpy=' + str(self.numpy()) + '>'

    def __del__(self):
        if self.allocated and self.ref:
            LIBNUX.nux_tensor_destroy(self.ref)
            self.allocated = False

    def __eq__(self, other):
        if isinstance(other, Tensor):
            return np.array_equal(self.numpy(), other.numpy())

        return False


class TensorArray:
    """A list of tensors

    It is used for input and output values of model inferences.
    """
    ref = c_void_p(None)
    should_drop: bool = False
    descs: [TensorDesc]
    len: int

    def __init__(self, ref, descs: [TensorDesc], allocated=False):
        self.ref = ref
        self.descs = descs
        self.should_drop = allocated
        self._as_parameter_ = ref
        self.len = LIBNUX.nux_tensor_array_len(self.ref)

    def is_empty(self) -> bool:
        """True if it has no Tensor"""
        return self.len == 0

    def __len__(self):
        return self.len

    def __getitem__(self, key):
        if isinstance(key, int):
            while key < 0:
                # Index is a negative, so addition will subtract.
                key += len(self)

            if key >= len(self):
                raise IndexError("tensor index (%d) out of range [0, %d)" % (key, len(self)))

            return Tensor(LIBNUX.nux_tensor_array_get(self, key),
                          desc=self.descs[key], allocated=False)

        if isinstance(key, slice):
            start, stop, step = key.indices(len(self))
            return [self[i] for i in range(start, stop, step)]

        raise TypeError

    def __setitem__(self, key, data):
        if isinstance(key, int):
            while key < 0:
                # Index is a negative, so addition will subtract.
                key += len(self)

            if key >= len(self):
                raise IndexError("tensor index (%d) out of range [0, %d)" % (key, len(self)))

            self[key].copy_from(data)
            return

        raise TypeError

    def numpy(self) -> [np.ndarray]:
        """Convert TensorArray to a list of numpy.ndarray"""
        array = []
        for idx in range(self.len):
            array.append(self[idx].numpy())

        return array

    def __del__(self):
        if self.should_drop and self.ref:
            LIBNUX.nux_tensor_array_destroy(self.ref)
            self.should_drop = False

    def __repr__(self):
        return list_to_dict(self).__repr__()


def numpy_dtype(value):
    """Return numpy dtype from any eligible object of Nux"""
    if isinstance(value, (np.ndarray, np.generic)):
        return value.dtype()
    if isinstance(value, Tensor):
        return value.numpy_dtype()
    if isinstance(value, TensorDesc):
        return value.numpy_dtype()
    if isinstance(value, DataType):
        return value.numpy_dtype()

    raise TypeError
