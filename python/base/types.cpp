//
// Created by thomas on 09/12/2025.
//
#include <nanobind/nanobind.h>
#include <poutre/base/types.hpp>

namespace nb = nanobind;

void init_base_types(nb::module_ &mod)
{
  nb::enum_<poutre::PType>(mod, "PType")
    .value("GrayUINT8", poutre::PType::PType_GrayUINT8)
    .value("GrayINT32", poutre::PType::PType_GrayINT32)
    .value("GrayINT64", poutre::PType::PType_GrayINT64)
    .value("F32", poutre::PType::PType_F32)
    .value("D64", poutre::PType::PType_D64)
    .export_values();

  nb::enum_<poutre::CompoundType>(mod, "CType")
    .value("Scalar", poutre::CompoundType::CompoundType_Scalar)
    .value("3Planes", poutre::CompoundType::CompoundType_3Planes)
    .value("4Planes", poutre::CompoundType::CompoundType_4Planes)
    .export_values();

  nb::enum_<poutre::CompOpType>(mod, "Comp")
    .value("Diff", poutre::CompOpType::CompOpDiff)
    .value("Equal", poutre::CompOpType::CompOpEqual)
    .value("Inf", poutre::CompOpType::CompOpInf)
    .value("Sup", poutre::CompOpType::CompOpSup)
    .value("InfEqual", poutre::CompOpType::CompOpInfEqual)
    .value("SupEqual", poutre::CompOpType::CompOpSupEqual)
    .export_values();
}