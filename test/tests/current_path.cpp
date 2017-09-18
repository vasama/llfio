/* Integration test kernel for whether current path works
(C) 2017 Niall Douglas <http://www.nedproductions.biz/> (2 commits)
File Created: Aug 2017


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
    (See accompanying file Licence.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

#include "../../include/afio/afio.hpp"
#include "kerneltest/include/kerneltest.hpp"

template <class FileHandleType, class DirectoryHandleType> static inline void TestHandleCurrentPath()
{
  namespace afio = AFIO_V2_NAMESPACE;
  {
    std::error_code ec;
    afio::filesystem::remove_all("tempfile", ec);
    afio::filesystem::remove_all("tempfile2", ec);
    afio::filesystem::remove_all("tempdir", ec);
    afio::filesystem::remove_all("tempdir2", ec);
  }
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif
  FileHandleType h1 = afio::construct<FileHandleType>{afio::path_handle(), "tempfile", afio::file_handle::mode::write, afio::file_handle::creation::if_needed, afio::file_handle::caching::temporary, afio::file_handle::flag::unlink_on_close}().value();     // NOLINT
  DirectoryHandleType h2 = afio::construct<DirectoryHandleType>{afio::path_handle(), "tempdir", afio::file_handle::mode::write, afio::file_handle::creation::if_needed, afio::file_handle::caching::all, afio::file_handle::flag::unlink_on_close}().value();  // NOLINT
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  {
    auto h1path = h1.current_path();
    BOOST_CHECK(h1path);
    if(!h1path)
    {
      std::cerr << "Getting the current path of a file FAILED due to " << h1path.error().message() << std::endl;
    }
    else if(h1path.value().empty())
    {
      BOOST_CHECK(!h1path.value().empty());
      std::cerr << "Getting the current path of a file FAILED due to the returned path being empty" << std::endl;
    }
    else
    {
      std::cout << "The path of the file is " << h1path.value() << std::endl;
    }

    auto h2path = h2.current_path();
    BOOST_CHECK(h2path);
    if(!h2path)
    {
      std::cerr << "Getting the current path of a directory FAILED due to " << h2path.error().message() << std::endl;
    }
    else if(h2path.value().empty())
    {
      BOOST_CHECK(!h2path.value().empty());
      std::cerr << "Getting the current path of a directory FAILED due to the returned path being empty" << std::endl;
    }
    else
    {
      std::cout << "The path of the directory is " << h2path.value() << std::endl;
    }
  }

  h1.relink({}, "tempfile2").value();
  h2.relink({}, "tempdir2").value();

  {
    auto h1path = h1.current_path();
    BOOST_CHECK(h1path);
    if(!h1path)
    {
      std::cerr << "Getting the current path of a file FAILED due to " << h1path.error().message() << std::endl;
    }
    else if(h1path.value().empty())
    {
      BOOST_CHECK(!h1path.value().empty());
      std::cerr << "Getting the current path of a file FAILED due to the returned path being empty" << std::endl;
    }
    else
    {
      std::cout << "The path of the file is " << h1path.value() << std::endl;
    }

    auto h2path = h2.current_path();
    BOOST_CHECK(h2path);
    if(!h2path)
    {
      std::cerr << "Getting the current path of a directory FAILED due to " << h2path.error().message() << std::endl;
    }
    else if(h2path.value().empty())
    {
      BOOST_CHECK(!h2path.value().empty());
      std::cerr << "Getting the current path of a directory FAILED due to the returned path being empty" << std::endl;
    }
    else
    {
      std::cout << "The path of the directory is " << h2path.value() << std::endl;
    }
  }
}

KERNELTEST_TEST_KERNEL(integration, afio, current_path, handle, "Tests that afio::handle::current_path() works as expected", TestHandleCurrentPath<AFIO_V2_NAMESPACE::file_handle, AFIO_V2_NAMESPACE::directory_handle>())
KERNELTEST_TEST_KERNEL(integration, afio, current_path, cached_parent_handle_adapter, "Tests that afio::cached_parent_handle_adapter::current_path() works as expected",
                       TestHandleCurrentPath<AFIO_V2_NAMESPACE::algorithm::cached_parent_handle_adapter<AFIO_V2_NAMESPACE::file_handle>, AFIO_V2_NAMESPACE::algorithm::cached_parent_handle_adapter<AFIO_V2_NAMESPACE::directory_handle>>())