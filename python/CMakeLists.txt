include_directories($ENV{PYTHON_INCLUDE})
cet_make(LIBRARY_NAME fhiclmodule LIBRARIES cetlib cetlib_except fhiclcpp ${PYTHON})
