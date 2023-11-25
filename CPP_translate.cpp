#include <Python.h>
#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <csignal>

int distance(std::array<std::array<double, 2>, 21> lm) {
    double dx = lm[8][0] - lm[4][0];
    double dy = lm[8][1] - lm[4][1];
    return int(std::sqrt(dx * dx + dy * dy));
}

volatile sig_atomic_t gSignalStatus = 0; // flag to indicate if signal was received

void handle_signal(int signal) {
    gSignalStatus = signal;
}

int main(int argc, char* argv[])
{
    // Initialize the Python interpreter
    Py_Initialize();

    // Run a Python script
    FILE* fp;
    PyObject* pModule;
    PyObject* pFunc_run;
    PyObject* pFunc_distance;

    fp = fopen("hand_gesture_noloop.py", "r");
    pModule = PyImport_ImportModule("__main__");
    PyModule_AddObject(pModule, "__builtins__", PyEval_GetBuiltins());
    PyRun_SimpleFile(fp, "hand_gesture_noloop.py");
    pFunc_run = PyObject_GetAttrString(pModule, "run");
    // pFunc_distance = PyObject_GetAttrString(pModule, "fingerDistance");

    std::signal(SIGINT, handle_signal);

    std::array<std::array<double, 2>, 21> landmarks;

    while (gSignalStatus == 0) {
        PyObject* pValue = PyObject_CallObject(pFunc_run, NULL);
        PyObject* pList = PyList_AsTuple(pValue);
        Py_DECREF(pValue);
        // Extract the x and y coordinates from the tuple and print them
        for (int i = 0; i < 21; i++) {
            PyObject* pItem = PyTuple_GetItem(pList, i);
            double x = PyFloat_AsDouble(PyTuple_GetItem(PyList_AsTuple(pItem), 0));
            double y = PyFloat_AsDouble(PyTuple_GetItem(PyList_AsTuple(pItem), 1));
            // Py_DECREF(pItem);
            // std::cout << "Coordinate " << i << ": (" << x << ", " << y << ")" << std::endl;
            landmarks[i][0] = x;
            landmarks[i][1] = y;
        }

        // std::cout << "Coordinate " << landmarks[5][0] << ", " << landmarks[5][1] << std::endl;
        int dist = distance(landmarks);
        std::cout << "Coordinate " << dist << std::endl;

    }

    std::cout << "Loop interrupted with signal: " << gSignalStatus << std::endl;
    // Clean up
    // Py_DECREF(pValue);
    Py_DECREF(pFunc_run);
    Py_DECREF(pModule);
    fclose(fp);
    Py_Finalize();

    return 0;
}
