rm -rf Coverage
mkdir Coverage
cd Coverage

TARGET_DIR=../Binaries/Debug/Intermediate
INFO_DIR=$PWD
SOURCE_DIR=../Libraries

lcov -c -d $TARGET_DIR/ -o $INFO_DIR/coverage.info
lcov --remove $INFO_DIR/coverage.info "*/v1/*" "*/CommandLineTools/*" "*/gtest/*" "*/googletest-src/*" -o $INFO_DIR/coverage_filtered.info
genhtml -o report $INFO_DIR/coverage_filtered.info