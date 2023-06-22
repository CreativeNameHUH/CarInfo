//
// Created by Hubert on 22.06.2023.
//

#ifndef CARINFO_BOOTLEGUNIQUEPTR_H
#define CARINFO_BOOTLEGUNIQUEPTR_H

#include <memory>

/***
 * sketchy std::make_unique implementation for the unique pointer in C++ 14
 * @tparam T
 * @tparam Args
 * @param args
 * @return std::unique_ptr<T>
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif //CARINFO_BOOTLEGUNIQUEPTR_H
