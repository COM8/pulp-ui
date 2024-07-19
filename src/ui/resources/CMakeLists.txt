cmake_minimum_required(VERSION 3.27)

set(RESOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/ui_resources.c.xml PARENT_SCOPE)
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(RESOURCE_DEPS ${CMAKE_CURRENT_SOURCE_DIR}/theme.css
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/dock-left-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/cloud-outline-thin-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/list-compact-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/package-x-generic-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/update-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/share-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/view-sort-ascending-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/actions/view-sort-descending-symbolic.svg
    PARENT_SCOPE)