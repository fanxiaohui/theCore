add_subdirectory(${PLATFORM_NAME})

# Order matters. Adding `common` module after a platform allows to reference
# platform info, like IRQ count, from inner common modules.
add_subdirectory(common)
