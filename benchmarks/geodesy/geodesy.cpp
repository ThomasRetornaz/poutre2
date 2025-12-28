#include "benchmark/benchmark.h"
#include <filesystem>
#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/geodesy/extrema.hpp>
#include <poutre/geodesy/leveling.hpp>
#include <poutre/io/loader.hpp>
#include <poutre/pixel_processing/arith.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>

// NOLINTBEGIN

namespace fs = std::filesystem;

class FixtureDynOpen : public ::benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State & /*unused*/) override
  {
    const fs::path root_data_path(DATA_DIR);
    if (!fs::exists(root_data_path)) {
      POUTRE_RUNTIME_ERROR(std::format("folder not found {}", root_data_path.string()));
    }
    const fs::path file("gray/cameraman.png");
    const fs::path full_path = root_data_path / file;
    if (!fs::exists(full_path)) { POUTRE_RUNTIME_ERROR(std::format("file not found {}", full_path.string())); }
    m_img = poutre::io::ImageLoader().SetPath(full_path.string()).Load();
    m_out = poutre::CloneGeometry(*m_img);
  }
  void TearDown(const ::benchmark::State & /*unused*/) override
  {
    m_img.reset();
    m_out.reset();
  }

  std::unique_ptr<poutre::IInterface> m_img;
  std::unique_ptr<poutre::IInterface> m_out;
};

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(FixtureDynOpen, dyn_open)(benchmark::State &state)
{
  for (auto _ : state) {
    constexpr auto constant = static_cast<poutre::pUINT8>(10);
    poutre::geo::dynamic_pseudo_opening(*m_img, constant, poutre::se::Common_NL_SE::SESquare2D, *m_out);
  }
}
BENCHMARK_REGISTER_F(FixtureDynOpen, dyn_open)->Unit(benchmark::kMicrosecond);

class FixtureLeveling : public ::benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State & /*unused*/) override
  {
    const fs::path root_data_path(DATA_DIR);
    if (!fs::exists(root_data_path)) {
      POUTRE_RUNTIME_ERROR(std::format("folder not found {}", root_data_path.string()));
    }
    const fs::path file("gray/cameraman.png");
    const fs::path full_path = root_data_path / file;
    if (!fs::exists(full_path)) { POUTRE_RUNTIME_ERROR(std::format("file not found {}", full_path.string())); }
    m_img = poutre::io::ImageLoader().SetPath(full_path.string()).Load();
    m_marker = poutre::CloneGeometry(*m_img);
    m_out = poutre::CloneGeometry(*m_img);
  }
  void TearDown(const ::benchmark::State & /*unused*/) override
  {
    m_img.reset();
    m_marker.reset();
    m_out.reset();
  }

  std::unique_ptr<poutre::IInterface> m_img;
  std::unique_ptr<poutre::IInterface> m_marker;
  std::unique_ptr<poutre::IInterface> m_out;
};

// cppcheck-suppress unknownMacro
BENCHMARK_DEFINE_F(FixtureLeveling, leveling)(benchmark::State &state)
{
  for (auto _ : state) {
    constexpr auto constant = static_cast<poutre::pUINT8>(10);
    poutre::ArithSaturatedSubConstant(*m_img, constant, *m_marker);
    poutre::geo::leveling(*m_img, *m_marker, poutre::se::Common_NL_SE::SESquare2D, *m_out);
  }
}
BENCHMARK_REGISTER_F(FixtureLeveling, leveling)->Unit(benchmark::kMicrosecond);

// NOLINTEND