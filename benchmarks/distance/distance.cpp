#include "../../include/poutre/distance/distance.hpp"


#include "../../include/poutre/distance/mdistance.hpp"
#include "benchmark/benchmark.h"
#include <filesystem>
#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/distance/mdistance.hpp>
#include <poutre/io/loader.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>

// NOLINTBEGIN

namespace fs = std::filesystem;

class FixtureDistance : public ::benchmark::Fixture
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
BENCHMARK_DEFINE_F(FixtureDistance, mdistance)(benchmark::State &state)
{
  for (auto _ : state) { poutre::dist::mdistance(*m_img, poutre::se::Common_NL_SE::SESquare2D, *m_out); }
}
BENCHMARK_REGISTER_F(FixtureDistance, mdistance)->Unit(benchmark::kMicrosecond);


// NOLINTEND