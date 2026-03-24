#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "infradb/core/Engine.hpp"
#include "infradb/execution/VectorBatch.hpp"

namespace py = pybind11;

/**
 * Pybind11 Python Bridge definition for InfraDB Core.
 */
PYBIND11_MODULE(infradb_core, m) {
    m.doc() = "InfraDB Native High-Performance Core Engine Wrapper";

    // Expose VectorBatch to Python
    py::class_<infradb::execution::VectorBatch>(m, "VectorBatch")
        .def_property_readonly("row_count", &infradb::execution::VectorBatch::num_rows)
        .def("__repr__", [](const infradb::execution::VectorBatch& v) {
            return "<infradb_core.VectorBatch rows=" + std::to_string(v.num_rows()) + ">";
        });

    // Expose Core Engine to Python
    py::class_<infradb::core::Engine>(m, "Engine")
        .def(py::init<>())
        .def("execute_optimized_scan", [](infradb::core::Engine& self, const std::string& path) {
            // RELEASE THE GIL (Global Interpreter Lock)
            // This allows Django to continue handling requests while C++ scans on all CPU cores
            py::gil_scoped_release release;
            return self.scan_file(path);
        }, py::arg("file_path"), "Parallel file scan with GIL release")
        .def("optimize_plan", &infradb::core::Engine::optimize_plan, py::arg("logical_plan"));
}
