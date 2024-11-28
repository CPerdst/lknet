//
// Created by l1Akr on 24-8-11.
//

#include "packer.h"

const char* packer::RootFormatStr               = "[%level] [%s {%Y-%m-%d %H:%M:%S}] %filepath:%line (%func): %message\n";
const packer::level packer::RootLevel           = packer::level::Debug;