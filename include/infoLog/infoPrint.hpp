//
// Created by lei on 22-11-17.
//

#ifndef GPUPROGRAM_INFOPRINT_HPP
#define GPUPROGRAM_INFOPRINT_HPP

#include "consoleColor.hpp"

#define _FATAL(info) std::cout << RESET << PURPLE  << info << RESET
#define _ERROR(info) std::cout << RESET << RED     << info << RESET
#define _WARN(info)  std::cout << RESET << YELLOW  << info << RESET
#define _INFO(info)  std::cout << RESET << GREEN   << info << RESET
#define _DEBUG(info) std::cout << RESET << D_GREEN << info << RESET

#define FATAL(info)  std::cout << RESET << PURPLE  << info << RESET << std::endl
#define ERROR(info)  std::cout << RESET << RED     << info << RESET << std::endl
#define WARN(info)   std::cout << RESET << YELLOW  << info << RESET << std::endl
#define INFO(info)   std::cout << RESET << GREEN   << info << RESET << std::endl
#define DEBUG(info)  std::cout << RESET << D_GREEN << info << RESET << std::endl

#define _FATAL_P(pre, info)  std::cout << RESET << PURPLE  << pre << info << RESET
#define _ERROR_P(pre, info)  std::cout << RESET << RED     << pre << info << RESET
#define _WARN_P(pre, info)   std::cout << RESET << YELLOW  << pre << info << RESET
#define _INFO_P(pre, info)   std::cout << RESET << GREEN   << pre << info << RESET
#define _DEBUG_P(pre, info)  std::cout << RESET << D_GREEN << pre << info << RESET

#define FATAL_P(pre, info)  std::cout << RESET << PURPLE  << pre << info << RESET << std::endl
#define ERROR_P(pre, info)  std::cout << RESET << RED     << pre << info << RESET << std::endl
#define WARN_P(pre, info)   std::cout << RESET << YELLOW  << pre << info << RESET << std::endl
#define INFO_P(pre, info)   std::cout << RESET << GREEN   << pre << info << RESET << std::endl
#define DEBUG_P(pre, info)  std::cout << RESET << D_GREEN << pre << info << RESET << std::endl

#define DIG(info)    std::cout << RESET << BLUE    << info << RESET

#endif //GPUPROGRAM_INFOPRINT_HPP
