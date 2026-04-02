#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Connect4AI.h"

namespace py = pybind11;

PYBIND11_MODULE(connect4ai, m){
    py::class_<Connect4AI>(m, "Connect4AI")
    .def(py::init<>())

    .def("get_move", &Connect4AI::get_move)
    .def("minimax", &Connect4AI::minimax)
    .def("check_win", &Connect4AI::check_win)
    .def("print_board", &Connect4AI::print_board);
    .def("get_def_weights", &Connect4AI::get_def_weights);
}
