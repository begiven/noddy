//
//  noddymodule.c
//  noddy
//
//  Created by lattesir on 15/12/28.
//  Copyright © 2015年 lattesir. All rights reserved.
//

#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    PyObject *first;    /* first name */
    PyObject *last;     /* last name */
    int number;
} Noddy;

static void
Noddy_dealloc(Noddy *self)
{
    Py_XDECREF(self->first);
    Py_XDECREF(self->last);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
Noddy_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    Noddy *self;
    
    self = (Noddy *)type->tp_alloc(type, 0);
    if (self) {
        self->first = PyUnicode_FromString("");
        if (!self->first) {
            Py_DECREF(self);
            return NULL;
        }
        
        self->last = PyUnicode_FromString("");
        if (!self->last) {
            Py_DECREF(self);
            return NULL;
        }
        
        self->number = 0;
    }
    
    return (PyObject *)self;
}

static int
Noddy_init(Noddy *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {
      "first", "last", "number", NULL
    };
    
    PyObject *first = NULL;
    PyObject *last = NULL;
    PyObject *tmp;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|SSi", kwlist,
                                     &first, &last, &self->number))
        return -1;
    
    if (first) {
        tmp = self->first;
        Py_INCREF(first);
        self->first = first;
        Py_XDECREF(tmp);
    }
    
    if (last) {
        tmp = self->first;
        Py_INCREF(last);
        self->last = last;
        Py_XDECREF(tmp);
    }
    
    return 0;
}

static PyMemberDef Noddy_members[] = {
    {"number", T_INT, offsetof(Noddy, number), 0, "noddy number"},
    {NULL}
};

static PyObject *
Noddy_getfirst(Noddy *self, void *closure)
{
    Py_INCREF(self->first);
    return self->first;
}

static int
Noddy_setfirst(Noddy *self, PyObject *value, void *closure)
{
    if (!value) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }
    
    if (!PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The first attribute value must be a string");
        return -1;
    }
    
    Py_INCREF(value);
    Py_DECREF(self->first);
    self->first = value;
    
    return 0;
}

static PyObject *
Noddy_getlast(Noddy *self, void *closure)
{
    Py_INCREF(self->last);
    return self->last;
}

static int
Noddy_setlast(Noddy *self, PyObject *value, void *closure)
{
    if (!value) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the last attribute");
        return -1;
    }
    
    if (!PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The last attribute must be a string");
        return -1;
    }
    
    Py_INCREF(value);
    Py_DECREF(self->last);
    self->last = value;
    
    return 0;
}

static PyGetSetDef Noddy_getseters[] = {
    {"first", (getter)Noddy_getfirst, (setter)Noddy_setfirst, "first name", NULL},
    {"last", (getter)Noddy_getlast, (setter)Noddy_setlast, "last name", NULL},
    {NULL}
};

static PyObject *
Noddy_name(Noddy *self)
{
    if (!self->first) {
        PyErr_SetString(PyExc_AttributeError, "first");
        return NULL;
    }
    
    if (!self->last) {
        PyErr_SetString(PyExc_AttributeError, "last");
        return NULL;
    }
    
    return PyUnicode_FromFormat("%S %S", self->first, self->last);
}

static PyMethodDef Noddy_methods[] = {
    {"name", (PyCFunction)Noddy_name, METH_NOARGS,
     "Return the name, combining the first and last name"
    },
    {NULL}
};

static PyTypeObject noddy_NoddyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "noddy.Noddy",             /* tp_name */
    sizeof(Noddy), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Noddy_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Noddy objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Noddy_methods,             /* tp_methods */
    Noddy_members,             /* tp_members */
    Noddy_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
};

static PyModuleDef noddymodule = {
    PyModuleDef_HEAD_INIT,
    "noddy",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_noddy(void)
{
    PyObject *module;
    
    if (PyType_Ready(&noddy_NoddyType) < 0)
        return NULL;
    
    module = PyModule_Create(&noddymodule);
    if (!module)
        return NULL;
    
    Py_INCREF(&noddy_NoddyType);
    PyModule_AddObject(module, "Noddy", (PyObject *)&noddy_NoddyType);
    
    return module;
}
