/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <cstdio>
#include <memory>
#include <poutre/base/config.hpp>
#include <poutre/structuring_element/se_chained.hpp>
#include <poutre/structuring_element/se_interface.hpp>
#include <utility>
#include <vector>

namespace poutre::se {
void ChainedStructuringElement::append(std::unique_ptr< IStructuringElement > strel)
{
  m_vect_se.push_back(std::move(strel));
}

// NOLINTBEGIN
std::unique_ptr< IStructuringElement > ChainedStructuringElement::Transpose() const
{
  std::unique_ptr< ChainedStructuringElement > output_se = std::make_unique< ChainedStructuringElement >();
  for (const auto &strel : m_vect_se) {
    output_se->m_vect_se.push_back(strel->Transpose());
  }
  return output_se;
}


std::size_t ChainedStructuringElement::GetSize() const { return 0; }

std::unique_ptr< IStructuringElement > ChainedStructuringElement::RemoveCenter() const
{
  POUTRE_RUNTIME_ERROR("RemoveCenter not implemented for chained se");
}

std::unique_ptr< IStructuringElement > ChainedStructuringElement::Clone() const
{
  std::unique_ptr< ChainedStructuringElement > output_se = std::make_unique< ChainedStructuringElement >();
  for (const auto &strel : m_vect_se) {
    output_se->m_vect_se.push_back(strel->Clone());
  }
  return output_se;
}
// NOLINTEND

bool ChainedStructuringElement::is_equal(const IStructuringElement &strel) const noexcept
{
  const auto *se_t = dynamic_cast< const ChainedStructuringElement * >(&strel);
  if (se_t == nullptr) {
    return false;
  }
  if (se_t->m_vect_se.size() != m_vect_se.size()) {
    return false;
  }
  for (std::size_t i = 0; i < m_vect_se.size(); ++i) {
    // NOLINTNEXTLINE
    if (!se_t->m_vect_se[i]->is_equal(*m_vect_se[i])) {
      return false;
    }
  }
  return true;
}

bool ChainedStructuringElement::is_equal_unordered(const IStructuringElement &strel) const noexcept
{
  const auto *se_t = dynamic_cast< const ChainedStructuringElement * >(&strel);
  if (se_t == nullptr) {
    return false;
  }
  if (se_t->m_vect_se.size() != m_vect_se.size()) {
    return false;
  }
  for (std::size_t i = 0; i < m_vect_se.size(); ++i) {
    // NOLINTNEXTLINE
    if (!se_t->m_vect_se[i]->is_equal_unordered(*m_vect_se[i])) {
      return false;
    }
  }
  return true;
}

}// namespace poutre::se