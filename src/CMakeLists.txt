#FILE (GLOB srcFiles *.cc)
#MESSAGE (${srcFiles})

SET (FILES
    ParameterSet.cc
    ParameterSetParser.cc
    sha1.cpp
    )

FOREACH (FILE ${FILES})
    SET (srcFiles ${srcFiles} ${FILE})
ENDFOREACH (FILE)

ADD_LIBRARY (fhiclcppD SHARED ${srcFiles})  
ADD_LIBRARY (fhiclcpp         ${srcFiles})  

SET_TARGET_PROPERTIES (fhiclcppD
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

SET_TARGET_PROPERTIES (fhiclcpp
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

# making a copy of the source code
INSTALL (FILES ${srcFiles} CMakeLists.txt
    DESTINATION ${version}/src/fhiclcpp/src
    )

INSTALL (TARGETS fhiclcppD fhiclcpp 
    RUNTIME DESTINATION ${flavorqual_dir}/bin
    LIBRARY DESTINATION ${flavorqual_dir}/lib
    ARCHIVE DESTINATION ${flavorqual_dir}/lib
    ) 
