/*
 * @author 	Alexander Rüedlinger <a.rueedlinger@gmail.com>
 * @date 	22.02.2015
 *
 * Python bindings for the TSL2561 driver written in C.
 *
 */

#include <Python.h>
#include <structmember.h>
#include "tsl2561.h"

typedef struct {
	PyObject_HEAD
	void *tsl2561;
} TSL2561_Object;


static void TSL2561_dealloc(TSL2561_Object *self) {
	tsl2561_close(self->tsl2561);
    self->tsl2561 = NULL;
    self->ob_type->tp_free((PyObject*)self);
}


static PyObject *TSL2561_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	TSL2561_Object *self;
	self = (TSL2561_Object *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}


static int TSL2561_init(TSL2561_Object *self, PyObject *args, PyObject *kwds) {
	int address;
	const char *i2c_device;
    static char *kwlist[] = {"address", "i2c_devcie", NULL};

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "is", kwlist, &address, &i2c_device))
		return -1;

	if(i2c_device) {
		self->tsl2561 = tsl2561_init(address, i2c_device);
		if(self->tsl2561 == NULL) {
			PyErr_SetString(PyExc_RuntimeError, "Cannot initialize sensor. Run program as root and check i2c device / address.");
			return -1;
		}
	}
	return 0;
}


static PyObject *TSL2561_lux(TSL2561_Object *self) {
	PyObject *result;
	long lux = tsl2561_lux(self->tsl2561);
	result = PyInt_FromLong(lux);
	return result;
}


static PyObject *TSL2561_enable(TSL2561_Object *self) {
	tsl2561_enable(self->tsl2561);
	return Py_None;
}


static PyObject *TSL2561_disable(TSL2561_Object *self) {
	tsl2561_disable(self->tsl2561);
	return Py_None;
}


static PyObject *TSL2561_enable_autogain(TSL2561_Object *self) {
	tsl2561_enable_autogain(self->tsl2561);
	return Py_None;
}


static PyObject *TSL2561_disable_autogain(TSL2561_Object *self) {
	tsl2561_disable_autogain(self->tsl2561);
	return Py_None;
}


static PyObject *TSL2561_set_timing(TSL2561_Object *self, PyObject *args, PyObject *kwds) {
	int time, gain;
	static char *kwlist[] = {"time", "gain", NULL};

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "ii", kwlist, &time, &gain))
		return NULL;

	tsl2561_set_timing(self->tsl2561, time, gain);
	return Py_None;
}


static PyObject *TSL2561_set_gain(TSL2561_Object *self, PyObject *args) {
	int gain;
    if(!PyArg_ParseTuple(args, "i", &gain))
		return NULL;

	tsl2561_set_gain(self->tsl2561, gain);
	return Py_None;
}


static PyObject *TSL2561_set_time(TSL2561_Object *self, PyObject *args) {
	int time;
    if(!PyArg_ParseTuple(args, "i", &time))
		return NULL;

	tsl2561_set_integration_time(self->tsl2561, time);
	return Py_None;
}


static PyObject *TSL2561_set_type(TSL2561_Object *self, PyObject *args) {
	int type;
    if(!PyArg_ParseTuple(args, "i", &type))
		return NULL;

	tsl2561_set_type(self->tsl2561, type);
	return Py_None;
}


static PyMethodDef TSL2561_methods[] = {
	{"lux", (PyCFunction) TSL2561_lux, METH_NOARGS, "Returns a lx value"},
	{"enable_autogain", (PyCFunction) TSL2561_enable_autogain, METH_NOARGS, "Enables autogain"},
	{"disable_autogain", (PyCFunction) TSL2561_disable_autogain, METH_NOARGS, "Disables autogain"},
	{"enable", (PyCFunction) TSL2561_enable, METH_NOARGS, "Enables this sensor"},
	{"disable", (PyCFunction) TSL2561_disable, METH_NOARGS, "Disables this sensor"},
	{"set_gain", (PyCFunction) TSL2561_set_gain, METH_VARARGS, "Sets gain"},
	{"set_time", (PyCFunction) TSL2561_set_time, METH_VARARGS, "Sets time"},
	{"set_type", (PyCFunction) TSL2561_set_type, METH_VARARGS, "Sets type"},
	{"set_timing", (PyCFunction) TSL2561_set_timing, METH_VARARGS, "Sets timing"},
    {NULL}  /* Sentinel */
};


static PyMemberDef TSL2561_members[] = {
    {NULL}  /* Sentinel */
};


static PyTypeObject TSL2561_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "tentacle_pi.TSL2561",             /*tp_name*/
    sizeof(TSL2561_Object),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)TSL2561_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "TSL2561 objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    TSL2561_methods,             /* tp_methods */
    TSL2561_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)TSL2561_init,      /* tp_init */
    0,                         /* tp_alloc */
    TSL2561_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};



PyMODINIT_FUNC initTSL2561(void) {
	PyObject *m;

	if(PyType_Ready(&TSL2561_Type) < 0)
		return;

	m = Py_InitModule3("TSL2561", module_methods, "TSL2561 extension module");

	if(m == NULL)
		return;

	Py_INCREF(&TSL2561_Type);
	PyModule_AddObject(m, "TSL2561", (PyObject *)&TSL2561_Type);

}
