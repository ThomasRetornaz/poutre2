//
// Created by thomas on 11/12/2025.
//

#include <nanobind/nanobind.h>

namespace nb = nanobind;

void init_base_types(nb::module_ &);
void init_base_image(nb::module_ &);

void init_io(nb::module_ &);

void init_pp_arith(nb::module_ &);
void init_pp_copy_convert(nb::module_ &);
void init_pp_compare(nb::module_ &);

void init_se_types(nb::module_ &);
void init_se_interface(nb::module_ &);

void init_llm_ero_dil(nb::module_ &);

void init_geodesy(nb::module_ &);

void init_label(nb::module_ &);

NB_MODULE(pypoutre, mod)
{
  mod.doc() = "This is poutre python bindings";

  init_base_types(mod);
  init_base_image(mod);
  init_pp_arith(mod);
  init_pp_copy_convert(mod);
  init_pp_compare(mod);
  init_io(mod);
  init_se_types(mod);
  init_se_interface(mod);
  init_llm_ero_dil(mod);
  init_geodesy(mod);
  init_label(mod);
}