#include "benchmark/benchmark.h"
#include <filesystem>
#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/io/loader.hpp>
#include <poutre/label/label_binary.hpp>
#include <poutre/label/label_flat_zones.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>

// NOLINTBEGIN

namespace fs = std::filesystem;

class FixtureLabelBinary : public ::benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State & /*unused*/) override
  {
    const fs::path root_data_path(DATA_DIR);
    if (!fs::exists(root_data_path)) {
      POUTRE_RUNTIME_ERROR(std::format("folder not found {}", root_data_path.string()));
    }
    const fs::path file("binary/eutectic.png");
    const fs::path full_path = root_data_path / file;
    if (!fs::exists(full_path)) { POUTRE_RUNTIME_ERROR(std::format("file not found {}", full_path.string())); }
    m_img = poutre::io::ImageLoader().SetPath(full_path.string()).Load();
    m_out = poutre::ConvertGeometry(*m_img, poutre::PType::PType_GrayINT64);
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
BENCHMARK_DEFINE_F(FixtureLabelBinary, label_binary)(benchmark::State &state)
{
  for (auto _ : state) { poutre::label::label_binary(*m_img, poutre::se::Common_NL_SE::SESquare2D, *m_out); }
}
BENCHMARK_REGISTER_F(FixtureLabelBinary, label_binary)->Unit(benchmark::kMicrosecond);

class FixtureLabelFlatZones : public ::benchmark::Fixture
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
    m_out = poutre::ConvertGeometry(*m_img, poutre::PType::PType_GrayINT64);
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
BENCHMARK_DEFINE_F(FixtureLabelFlatZones, label_flat_zones)(benchmark::State &state)
{
  for (auto _ : state) { poutre::label::label_flat_zones(*m_img, poutre::se::Common_NL_SE::SESquare2D, *m_out); }
}
BENCHMARK_REGISTER_F(FixtureLabelFlatZones, label_flat_zones)->Unit(benchmark::kMicrosecond);


// NOLINTEND