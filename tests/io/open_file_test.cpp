#include <gtest/gtest.h>

#include <QTemporaryFile>
#include <QTextStream>

#include "OpenFile.h"

TEST(OpenFile, reads_single_line) {
  QTemporaryFile tmp;
  ASSERT_TRUE(tmp.open());

  QTextStream out(&tmp);
  out << "hello world";
  out.flush();

  OpenFile f(tmp.fileName());

  EXPECT_EQ(f.read(), "hello world");
}

TEST(OpenFile, preserves_newlines) {
  QTemporaryFile tmp;
  ASSERT_TRUE(tmp.open());

  QTextStream out(&tmp);
  out << "hello\n";
  out << "world\n";
  out << "qt";
  out.flush();

  OpenFile f(tmp.fileName());

  EXPECT_EQ(f.read(), "hello\nworld\nqt");
}

TEST(OpenFile, reads_empty_file) {
  QTemporaryFile tmp;
  ASSERT_TRUE(tmp.open());

  OpenFile f(tmp.fileName());

  EXPECT_EQ(f.read(), "");
}

TEST(OpenFile, preserves_whitespace) {
  QTemporaryFile tmp;
  ASSERT_TRUE(tmp.open());

  QTextStream out(&tmp);
  out << "   hello   \n";
  out << "\tworld\t";
  out.flush();

  OpenFile f(tmp.fileName());

  EXPECT_EQ(f.read(), "   hello   \n\tworld\t");
}

TEST(OpenFile, second_read_returns_empty) {
  QTemporaryFile tmp;
  ASSERT_TRUE(tmp.open());

  QTextStream out(&tmp);
  out << "abc";
  out.flush();

  OpenFile f(tmp.fileName());

  EXPECT_EQ(f.read(), "abc");
  EXPECT_EQ(f.read(), "");
}