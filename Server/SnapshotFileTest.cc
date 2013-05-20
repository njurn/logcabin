/* Copyright (c) 2013 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

#include "Server/SnapshotFile.h"

namespace LogCabin {
namespace Server {
namespace SnapshotFile {
namespace {

namespace FilesystemUtil = Storage::FilesystemUtil;

class ServerSnapshotFileTest : public ::testing::Test {
  public:
    ServerSnapshotFileTest()
        : tmpdir()
    {
        std::string path = FilesystemUtil::mkdtemp();
        tmpdir = FilesystemUtil::File(open(path.c_str(), O_RDONLY|O_DIRECTORY),
                                      path);
    }
    ~ServerSnapshotFileTest() {
        tmpdir.close();
        FilesystemUtil::remove(tmpdir.path);
    }
    FilesystemUtil::File tmpdir;
};


TEST_F(ServerSnapshotFileTest, basic)
{
    {
        Writer writer(tmpdir);
        writer.getStream().WriteVarint32(482);
        writer.close();
    }
    {
        Reader reader(tmpdir);
        uint32_t x = 0;
        reader.getStream().ReadVarint32(&x);
        EXPECT_EQ(482U, x);
    }
}

TEST_F(ServerSnapshotFileTest, reader_fileNotFound)
{
    EXPECT_THROW(Reader reader(tmpdir), std::runtime_error);
}

} // namespace LogCabin::Server::SnapshotFile::<anonymous>
} // namespace LogCabin::Server::SnapshotFile
} // namespace LogCabin::Server
} // namespace LogCabin
