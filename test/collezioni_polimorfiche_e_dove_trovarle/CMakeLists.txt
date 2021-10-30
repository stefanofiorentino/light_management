#
# collezioni_polimorfiche_e_dove_trovarle
#

if (${BUILD_GOOGLE_BASED_TESTS})
    ADD_UNIT_TEST(runtime_polymorphism runtime_polymorphism.cpp)
    ADD_UNIT_TEST(std_variant std_variant.cpp)
    ADD_UNIT_TEST(entt entt.cpp)
    ADD_UNIT_TEST(pragmatic_type_erasure pragmatic_type_erasure.cpp)
endif (${BUILD_GOOGLE_BASED_TESTS})
