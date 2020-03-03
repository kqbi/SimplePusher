aux_source_directory(. current_source_list)

add_executable(SimplePusher ${current_source_list} ${SimplePusher_src_list})

if(ANDROID)
    target_link_libraries(SimplePusher S_Pusher mp4v2 ${LINK_LIB_LIST})
elseif(WIN32)
    target_link_libraries(SimplePusher S_Pusher mp4v2 ${LINK_LIB_LIST})
else()
    target_link_libraries(SimplePusher S_Pusher mp4v2 ${LINK_LIB_LIST} pthread)
endif()
