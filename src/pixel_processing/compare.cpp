
// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <cstddef>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <poutre/pixel_processing/compare.hpp>
#include <poutre/pixel_processing/details/compare_op_t.hpp>
#include <variant>

namespace {
using namespace poutre;

// sss
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_sss_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,// NOLINT
  const ScalarTypeVariant &i_valtrue,// NOLINT
  const ScalarTypeVariant &i_valfalse,// NOLINT
  IInterface &o_img)// NOLINT
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_sss_Dispatch i_img downcast fail"); }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;
  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_sss_Dispatch o_img downcast fail"); }

  pTypeIn comp_value = { 0 };
  pTypeOut true_value = { 0 };
  pTypeOut false_value = { 0 };
  try {
    comp_value = std::get<pTypeIn>(i_comp);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_sss_Dispatch unable to extract suitable type from i_comp");
  }
  try {
    true_value = std::get<pTypeOut>(i_valtrue);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_sss_Dispatch unable to extract suitable type from i_valtrue");
  }
  try {
    false_value = std::get<pTypeOut>(i_valfalse);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_sss_Dispatch unable to extract suitable type from i_valfalse");
  }
  details::t_CompareImage_sss(*i_img_t, compOpType, comp_value, true_value, false_value, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_sss_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,// NOLINT
  const ScalarTypeVariant &i_valtrue,// NOLINT
  const ScalarTypeVariant &i_valfalse,// NOLINT
  IInterface &o_img)// NOLINT
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_sss_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_sss_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_sss_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_sss_Dispatch<NumDims, Pin, PType::PType_F32>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_sss_Dispatch<NumDims, Pin, PType::PType_D64>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

// iii
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_iii_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,// NOLINT
  const IInterface &i_imgtrue,// NOLINT
  const IInterface &i_imgfalse,// NOLINT
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_iii_Dispatch i_img downcast fail"); }
  const auto *i_imgcomp_t = dynamic_cast<const ImgTypeIn *>(&i_imgcomp);
  if (!i_imgcomp_t) { POUTRE_RUNTIME_ERROR("CompareImage_iii_Dispatch i_imgcomp downcast fail"); }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  const auto *i_imgtrue_t = dynamic_cast<const ImgTypeOut *>(&i_imgtrue);
  if (!i_imgtrue_t) { POUTRE_RUNTIME_ERROR("CompareImage_iii_Dispatch i_imgtrue downcast fail"); }

  const auto *i_imgfalse_t = dynamic_cast<const ImgTypeOut *>(&i_imgfalse);
  if (!i_imgfalse_t) { POUTRE_RUNTIME_ERROR("CompareImage_iii_Dispatch i_imgfalse downcast fail"); }

  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_iii_Dispatch o_img downcast fail"); }

  details::t_CompareImage_iii(*i_img_t, compOpType, *i_imgcomp_t, *i_imgtrue_t, *i_imgfalse_t, *o_img_t);
}


template<size_t NumDims, PType Pin>
void CompareImage_iii_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const IInterface &i_imgtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_iii_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_iii_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_iii_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_iii_Dispatch<NumDims, Pin, PType::PType_F32>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_iii_Dispatch<NumDims, Pin, PType::PType_D64>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

// sii
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_sii_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,// NOLINT
  const IInterface &i_imgfalse,// NOLINT
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_sii_Dispatch i_img downcast fail"); }

  pTypeIn comp_value = { 0 };
  try {
    comp_value = std::get<pTypeIn>(i_comp);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_sii_Dispatch unable to extract suitable type from i_comp");
  }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  const auto *i_imgtrue_t = dynamic_cast<const ImgTypeOut *>(&i_imgtrue);
  if (!i_imgtrue_t) { POUTRE_RUNTIME_ERROR("CompareImage_sii_Dispatch i_imgtrue downcast fail"); }

  const auto *i_imgfalse_t = dynamic_cast<const ImgTypeOut *>(&i_imgfalse);
  if (!i_imgfalse_t) { POUTRE_RUNTIME_ERROR("CompareImage_sii_Dispatch i_imgfalse downcast fail"); }

  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_sii_Dispatch o_img downcast fail"); }

  details::t_CompareImage_sii(*i_img_t, compOpType, comp_value, *i_imgtrue_t, *i_imgfalse_t, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_sii_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_sii_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_sii_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_sii_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_sii_Dispatch<NumDims, Pin, PType::PType_F32>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_sii_Dispatch<NumDims, Pin, PType::PType_D64>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

// sis
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_sis_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_sis_Dispatch i_img downcast fail"); }

  pTypeIn comp_value = { 0 };
  try {
    comp_value = std::get<pTypeIn>(i_comp);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_sis_Dispatch unable to extract suitable type from i_comp");
  }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  const auto *i_imgtrue_t = dynamic_cast<const ImgTypeOut *>(&i_imgtrue);
  if (!i_imgtrue_t) { POUTRE_RUNTIME_ERROR("CompareImage_sis_Dispatch i_imgtrue downcast fail"); }

  pTypeOut false_value = { 0 };
  try {
    false_value = std::get<pTypeOut>(i_valfalse);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_sis_Dispatch unable to extract suitable type from i_valfalse");
  }

  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_sis_Dispatch o_img downcast fail"); }

  details::t_CompareImage_sis(*i_img_t, compOpType, comp_value, *i_imgtrue_t, false_value, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_sis_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_sis_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_sis_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_sis_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_sis_Dispatch<NumDims, Pin, PType::PType_F32>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_sis_Dispatch<NumDims, Pin, PType::PType_D64>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

/// isi
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_isi_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_isi_Dispatch i_img downcast fail"); }

  const auto *i_imgcomp_t = dynamic_cast<const ImgTypeIn *>(&i_imgcomp);
  if (!i_imgcomp_t) { POUTRE_RUNTIME_ERROR("CompareImage_isi_Dispatch i_imgcomp downcast fail"); }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  pTypeOut true_value = { 0 };
  try {
    true_value = std::get<pTypeOut>(i_valtrue);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_isi_Dispatch unable to extract suitable type from i_valtrue");
  }
  const auto *i_imgfalse_t = dynamic_cast<const ImgTypeOut *>(&i_imgfalse);
  if (!i_imgfalse_t) { POUTRE_RUNTIME_ERROR("CompareImage_isi_Dispatch i_imgfalse downcast fail"); }

  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_isi_Dispatch o_img downcast fail"); }

  details::t_CompareImage_isi(*i_img_t, compOpType, *i_imgcomp_t, true_value, *i_imgfalse_t, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_isi_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_isi_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_isi_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_isi_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_isi_Dispatch<NumDims, Pin, PType::PType_F32>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_isi_Dispatch<NumDims, Pin, PType::PType_D64>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}
// iss
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_iss_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,// NOLINT
  const ScalarTypeVariant &i_valfalse,// NOLINT
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_iss_Dispatch i_img downcast fail"); }

  const auto *i_imgcomp_t = dynamic_cast<const ImgTypeIn *>(&i_imgcomp);
  if (!i_imgcomp_t) { POUTRE_RUNTIME_ERROR("CompareImage_iss_Dispatch i_imgcomp downcast fail"); }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  pTypeOut true_value = { 0 };
  try {
    true_value = std::get<pTypeOut>(i_valtrue);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_iss_Dispatch unable to extract suitable type from i_valtrue");
  }

  pTypeOut false_value = { 0 };
  try {
    false_value = std::get<pTypeOut>(i_valfalse);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_iss_Dispatch unable to extract suitable type from i_valfalse");
  }

  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_iss_Dispatch o_img downcast fail"); }

  details::t_CompareImage_iss(*i_img_t, compOpType, *i_imgcomp_t, true_value, false_value, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_iss_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_iss_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_iss_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_iss_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_iss_Dispatch<NumDims, Pin, PType::PType_F32>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_iss_Dispatch<NumDims, Pin, PType::PType_D64>(
      i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

// ssi
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_ssi_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,// NOLINT
  const ScalarTypeVariant &i_valtrue,// NOLINT
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_ssi_Dispatch i_img downcast fail"); }

  pTypeIn comp_value = { 0 };
  try {
    comp_value = std::get<pTypeIn>(i_comp);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_ssi_Dispatch unable to extract suitable type from i_comp");
  }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  pTypeOut true_value = { 0 };
  try {
    true_value = std::get<pTypeOut>(i_valtrue);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_ssi_Dispatch unable to extract suitable type from i_valtrue");
  }

  const auto *i_imgfalse_t = dynamic_cast<const ImgTypeOut *>(&i_imgfalse);
  if (!i_imgfalse_t) { POUTRE_RUNTIME_ERROR("CompareImage_ssi_Dispatch i_imgfalse downcast fail"); }

  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_ssi_Dispatch o_img downcast fail"); }

  details::t_CompareImage_ssi(*i_img_t, compOpType, comp_value, true_value, *i_imgfalse_t, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_ssi_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_ssi_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_ssi_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_ssi_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_ssi_Dispatch<NumDims, Pin, PType::PType_F32>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_ssi_Dispatch<NumDims, Pin, PType::PType_D64>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

// iis
template<size_t NumDims, PType Pin, PType Pout>
void CompareImage_iis_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp, //NOLINT
  const IInterface &i_imgtrue, //NOLINT
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  using pTypeIn = typename enum_to_type<CompoundType::CompoundType_Scalar, Pin>::type;
  using ImgTypeIn = details::image_t<pTypeIn, NumDims>;
  const auto *i_img_t = dynamic_cast<const ImgTypeIn *>(&i_img);
  if (!i_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_iis_Dispatch i_img downcast fail"); }

  const auto *i_imgcomp_t = dynamic_cast<const ImgTypeIn *>(&i_imgcomp);
  if (!i_imgcomp_t) { POUTRE_RUNTIME_ERROR("CompareImage_iis_Dispatch i_imgcomp downcast fail"); }

  using pTypeOut = typename enum_to_type<CompoundType::CompoundType_Scalar, Pout>::type;
  using ImgTypeOut = details::image_t<pTypeOut, NumDims>;

  const auto *i_imgtrue_t = dynamic_cast<const ImgTypeOut *>(&i_imgtrue);
  if (!i_imgtrue_t) { POUTRE_RUNTIME_ERROR("CompareImage_iis_Dispatch i_imgtrue downcast fail"); }

  pTypeOut false_value = { 0 };
  try {
    false_value = std::get<pTypeOut>(i_valfalse);
  } catch (std::bad_variant_access &) {
    POUTRE_RUNTIME_ERROR("CompareImage_iis_Dispatch unable to extract suitable type from i_valfalse");
  }
  auto *o_img_t = dynamic_cast<ImgTypeOut *>(&o_img);
  if (!o_img_t) { POUTRE_RUNTIME_ERROR("CompareImage_iis_Dispatch o_img downcast fail"); }

  details::t_CompareImage_iis(*i_img_t, compOpType, *i_imgcomp_t, *i_imgtrue_t, false_value, *o_img_t);
}

template<size_t NumDims, PType Pin>
void CompareImage_iis_Pin_Dispatch(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp, //NOLINT
  const IInterface &i_imgtrue, //NOLINT
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  switch (o_img.GetPType()) {
  case PType::PType_GrayUINT8: {
    CompareImage_iis_Dispatch<NumDims, Pin, PType::PType_GrayUINT8>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT32: {
    CompareImage_iis_Dispatch<NumDims, Pin, PType::PType_GrayINT32>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_GrayINT64: {
    CompareImage_iis_Dispatch<NumDims, Pin, PType::PType_GrayINT64>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_F32: {
    CompareImage_iis_Dispatch<NumDims, Pin, PType::PType_F32>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
  } break;
  case PType::PType_D64: {
    CompareImage_iis_Dispatch<NumDims, Pin, PType::PType_D64>(
      i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
  } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
  }
  }
}

}// namespace

namespace poutre {

void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const ScalarTypeVariant &i_valtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  POUTRE_ENTERING("CompareImage sss variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_sss_Pin_Dispatch<2, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_sss_Pin_Dispatch<2, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_sss_Pin_Dispatch<2, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_sss_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_sss_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_sss_Pin_Dispatch<3, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_sss_Pin_Dispatch<3, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_sss_Pin_Dispatch<3, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_sss_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_sss_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_comp, i_valtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}


void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const IInterface &i_imgtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  POUTRE_ENTERING("CompareImage iii variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgcomp, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgtrue, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgfalse, o_img, "CompareImage images have not compatible sizes");

  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgcomp.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgtrue.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgfalse.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_iii_Pin_Dispatch<2, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_iii_Pin_Dispatch<2, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_iii_Pin_Dispatch<2, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_iii_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_iii_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_iii_Pin_Dispatch<3, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_iii_Pin_Dispatch<3, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_iii_Pin_Dispatch<3, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_iii_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_iii_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_imgtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}

void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{

  POUTRE_ENTERING("CompareImage sii variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgtrue, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgfalse, o_img, "CompareImage images have not compatible sizes");

  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgtrue.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgfalse.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_sii_Pin_Dispatch<2, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_sii_Pin_Dispatch<2, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_sii_Pin_Dispatch<2, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_sii_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_sii_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_sii_Pin_Dispatch<3, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_sii_Pin_Dispatch<3, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_sii_Pin_Dispatch<3, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_sii_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_sii_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_comp, i_imgtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}


void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const IInterface &i_imgtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  POUTRE_ENTERING("CompareImage sis variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgtrue, o_img, "CompareImage images have not compatible sizes");

  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgtrue.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_sis_Pin_Dispatch<2, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_sis_Pin_Dispatch<2, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_sis_Pin_Dispatch<2, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_sis_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_sis_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_sis_Pin_Dispatch<3, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_sis_Pin_Dispatch<3, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_sis_Pin_Dispatch<3, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_sis_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_sis_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_comp, i_imgtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}

void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{
  POUTRE_ENTERING("CompareImage isi variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgcomp, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgfalse, o_img, "CompareImage images have not compatible sizes");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgcomp.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgfalse.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_isi_Pin_Dispatch<2, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_isi_Pin_Dispatch<2, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_isi_Pin_Dispatch<2, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_isi_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_isi_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_isi_Pin_Dispatch<3, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_isi_Pin_Dispatch<3, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_isi_Pin_Dispatch<3, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_isi_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_isi_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_valtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}


void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const ScalarTypeVariant &i_valtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  POUTRE_ENTERING("CompareImage iss variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgcomp, o_img, "CompareImage images have not compatible sizes");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgcomp.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_iss_Pin_Dispatch<2, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_iss_Pin_Dispatch<2, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_iss_Pin_Dispatch<2, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_iss_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_iss_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_iss_Pin_Dispatch<3, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_iss_Pin_Dispatch<3, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_iss_Pin_Dispatch<3, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_iss_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_iss_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_valtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}


void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const ScalarTypeVariant &i_comp,
  const ScalarTypeVariant &i_valtrue,
  const IInterface &i_imgfalse,
  IInterface &o_img)
{

  POUTRE_ENTERING("CompareImage ssi variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgfalse, o_img, "CompareImage images have not compatible sizes");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgfalse.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_ssi_Pin_Dispatch<2, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_ssi_Pin_Dispatch<2, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_ssi_Pin_Dispatch<2, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_ssi_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_ssi_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_ssi_Pin_Dispatch<3, PType::PType_GrayUINT8>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_ssi_Pin_Dispatch<3, PType::PType_GrayINT32>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_ssi_Pin_Dispatch<3, PType::PType_GrayINT64>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_ssi_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_ssi_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_comp, i_valtrue, i_imgfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}

void CompareImage(const IInterface &i_img,
  CompOpType compOpType,
  const IInterface &i_imgcomp,
  const IInterface &i_imgtrue,
  const ScalarTypeVariant &i_valfalse,
  IInterface &o_img)
{
  POUTRE_ENTERING("CompareImage ssi variant");
  AssertSizesCompatible(i_img, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgcomp, o_img, "CompareImage images have not compatible sizes");
  AssertSizesCompatible(i_imgtrue, o_img, "CompareImage images have not compatible sizes");
  POUTRE_CHECK(i_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgcomp.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(i_imgtrue.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");
  POUTRE_CHECK(o_img.GetCType() == CompoundType::CompoundType_Scalar, "CompareImage images must be scalar");

  const auto &numDims = i_img.GetRank();
  switch (numDims) {
  case 0: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims:0");
  } break;
  // case 1: {
  //   ConvertInto1DDispatch<1>(i_img, o_img);
  // } break;
  case 2: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_iis_Pin_Dispatch<2, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_iis_Pin_Dispatch<2, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_iis_Pin_Dispatch<2, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_iis_Pin_Dispatch<2, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_iis_Pin_Dispatch<2, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  case 3: {
    switch (i_img.GetPType()) {
    case PType::PType_GrayUINT8: {
      CompareImage_iis_Pin_Dispatch<3, PType::PType_GrayUINT8>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT32: {
      CompareImage_iis_Pin_Dispatch<3, PType::PType_GrayINT32>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_GrayINT64: {
      CompareImage_iis_Pin_Dispatch<3, PType::PType_GrayINT64>(
        i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_F32: {
      CompareImage_iis_Pin_Dispatch<3, PType::PType_F32>(i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    case PType::PType_D64: {
      CompareImage_iis_Pin_Dispatch<3, PType::PType_D64>(i_img, compOpType, i_imgcomp, i_imgtrue, i_valfalse, o_img);
    } break;
    default: {
      POUTRE_RUNTIME_ERROR("CompareImage unsupported PTYPE");
    }
    }
  } break;
  // case 4: {
  //   // ConvertIntoDispatchDims<4>(i_img1, o_img2);
  // } break;
  default: {
    POUTRE_RUNTIME_ERROR("CompareImage Unsupported number of dims");
  }
  }
}
}// namespace poutre