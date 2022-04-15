# TQt/TDE
find_package( TQt )
find_package( TDE )

# Additional checks
tde_setup_architecture_flags( )

include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)

tde_setup_largefiles( )