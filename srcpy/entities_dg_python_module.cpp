/**
 * @file
 * @license BSD 3-clause
 * @copyright Copyright (c) 2020, New York University and Max Planck
 * Gesellschaft
 *
 * @brief Expose the vicon_client_entity dynamic_graph module to python.
 */

#include "dg_optitrack_sdk/optitrack_client_entity.hpp"
#include "dynamic-graph/python/module.hh"
#include "dynamic-graph/python/signal.hh"

namespace dg = dynamicgraph;

typedef bp::return_value_policy<bp::reference_existing_object>
    reference_existing_object;

BOOST_PYTHON_MODULE(entities)
{
    bp::import("dynamic_graph");

    using dg_optitrack_sdk::OptitrackClientEntity;
    dynamicgraph::python::exposeEntity<OptitrackClientEntity>().def(
        "add_object_to_track",
        +[](boost::python::object obj, const std::string& object_name) {
            OptitrackClientEntity* entity =
                boost::python::extract<OptitrackClientEntity*>(obj);
            entity->add_object_to_track(object_name);
            return dynamicgraph::python::entity::addSignals(obj);
        },
        "Create a new signal that will attempt to get the vicon "
        "pause object.",
        bp::arg("string: name of the vicon object i.e. "
                "[subject_name]/[segment_name]."));
}
