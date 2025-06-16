// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/config.hpp>
#include <poutre/structuring_element/se_interface.hpp>
#include <poutre/structuring_element/se_chained.hpp>
#include <cstdio>
#include <memory>
#include <vector>
#include <utility>

namespace poutre::se
{
ChainedStructuringElement::ChainedStructuringElement(): m_vect_se(){}

void ChainedStructuringElement::append(
  std::unique_ptr<IStructuringElement> strel)
{
  m_vect_se.push_back(std::move(strel));
}

std::unique_ptr<IStructuringElement> ChainedStructuringElement::Transpose() const
{
  std::unique_ptr<ChainedStructuringElement> output_se = std::make_unique<ChainedStructuringElement>();
  for(const auto & strel : m_vect_se) {
    output_se->m_vect_se.push_back(strel->Transpose());
  }
  return output_se;
}

std::size_t ChainedStructuringElement::GetSize() const
{
  return 0;
}

std::unique_ptr<IStructuringElement> ChainedStructuringElement::RemoveCenter() const
{
  POUTRE_RUNTIME_ERROR("RemoveCenter not implemented for chained se");
}

std::unique_ptr<IStructuringElement> ChainedStructuringElement::Clone() const
{
  std::unique_ptr<ChainedStructuringElement> output_se = std::make_unique<ChainedStructuringElement>();
  for(const auto & strel : m_vect_se) {
    output_se->m_vect_se.push_back(strel->Clone());
  }
  return output_se;
}

bool ChainedStructuringElement::is_equal(const IStructuringElement& strel) const noexcept
{
  const auto *se_t = dynamic_cast<const ChainedStructuringElement *>(&strel);
  if  (se_t == nullptr) {
    return false;
  }
  if (se_t->m_vect_se.size() != m_vect_se.size()) {return false;}
  for (std::size_t i = 0; i < m_vect_se.size(); ++i) {
    if (!se_t->m_vect_se[i]->is_equal(*m_vect_se[i])) {
      return false;
    }
  }
  return true;
}

bool ChainedStructuringElement::is_equal_unordered(const IStructuringElement& strel) const noexcept
{
  const auto *se_t = dynamic_cast<const ChainedStructuringElement *>(&strel);
  if  (se_t == nullptr) {
    return false;
  }
  if (se_t->m_vect_se.size() != m_vect_se.size()) {return false;}
  for (std::size_t i = 0; i < m_vect_se.size(); ++i) {
    if (!se_t->m_vect_se[i]->is_equal_unordered(*m_vect_se[i])) {
      return false;
    }
  }
  return true;
}

}