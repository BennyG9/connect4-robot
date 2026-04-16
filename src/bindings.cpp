#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <limits>

#include "Connect4AI.h"

namespace py = pybind11;

PYBIND11_MODULE(connect4ai, m){
    py::class_<Connect4AI>(m, "Connect4AI")
    .def(py::init<>())

    .def("get_move", &Connect4AI::get_move)
    .def("minimax", &Connect4AI::minimax,
        py::arg("player"),
        py::arg("opponent"),
        py::arg("weights"),
        py::arg("depth"),
        py::arg("alpha")=-std::numeric_limits<double>::infinity(),
        py::arg("beta")=std::numeric_limits<double>::infinity(),
        py::arg("root")=true)
    .def("minimax_root", &Connect4AI::minimax_root, py::call_guard<py::gil_scoped_release>())
    .def("check_win", &Connect4AI::check_win)
    .def("print_board", &Connect4AI::print_board)
    .def("get_def_weights", &Connect4AI::get_def_weights)
    .def("fill_feat_vec", &Connect4AI::fill_feat_vec)
    .def("wins", &Connect4AI::wins)
    .def("debug", &Connect4AI::debug)
    .def("moves_available", &Connect4AI::moves_available)
    .def("model_query", &Connect4AI::model_query);
}
