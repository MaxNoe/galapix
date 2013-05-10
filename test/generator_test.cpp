/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2013 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include "archive/archive_manager.hpp"
#include "generator/generator.hpp"
#include "generator/image_data.hpp"
#include "generator/generator_callbacks.hpp"
#include "network/download_manager.hpp"
#include "resource/blob_manager.hpp"
#include "resource/resource_info.hpp"
#include "util/software_surface_factory.hpp"

class TestGeneratorCallbacks final : public GeneratorCallbacks
{
private:
  ResourceLocator m_locator;
  std::function<void ()> m_done_function;

public:
  TestGeneratorCallbacks(const ResourceLocator& locator,
                         const std::function<void ()>& done_function) :
    m_locator(locator),
    m_done_function(done_function)
  {
  }

  virtual ~TestGeneratorCallbacks() 
  {
  }

  GeneratorCallbacksPtr on_child_resource(const ResourceLocator& locator) override
  {
    return std::make_shared<TestGeneratorCallbacks>(locator, std::function<void ()>());
  }

  void on_blob_info(const BlobInfo& blob_info) override
  {
    std::cout << "on_blob_info: " << blob_info.get_size() << " " << blob_info.get_sha1().str() << std::endl;
  }

  void on_resource_info(const ResourceInfo& resource_info) override
  {
    std::cout << "on_resource_info: " << resource_info.get_resource_name().str() << std::endl;
  }


  void on_archive_data(const ArchiveInfo& archive_info) override
  {
  }

  void on_image_data(const ImageData& image_data) override
  {
    std::cout << "on_image_data: "
              << image_data.get_image_info().get_width() << "x" << image_data.get_image_info().get_height()
              << " tiles: " << image_data.get_image_tiles().size()
              << std::endl;
  }

  void on_success(ResourceStatus status) override
  {
    std::cout << "on_success: " << std::endl;
    m_done_function();
  }

  void on_error(ResourceStatus status, const std::string& err) override
  {
    std::cout << "on_error: " << std::endl;
    m_done_function();
  }
};

int main(int argc, char** argv)
{
  SoftwareSurfaceFactory surface_factory;
  DownloadManager download_mgr;
  ArchiveManager archive_mgr;
  BlobManager blob_mgr(download_mgr, archive_mgr);
  Generator   generator(blob_mgr, archive_mgr);

  int count = 0;
  for(int i = 1; i < argc; ++i)
  {
    ResourceLocator locator = ResourceLocator::from_string(argv[i]);
    std::cout << "requesting: " << locator.str() << std::endl;
    count += 1;
    generator.request_resource_processing(locator, std::make_shared<TestGeneratorCallbacks>(locator, 
                                                                                            [&count]{
                                                                                              count -= 1;
                                                                                            }));
  }

  // FIXME: we need to manually wait for the Generator to be done,
  // else things will crash as callbacks are thrown around when the
  // Generator is destructing itself
  while(count > 0)
  { 
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << "shutting down" << std::endl;

  return 0;
}

/* EOF */