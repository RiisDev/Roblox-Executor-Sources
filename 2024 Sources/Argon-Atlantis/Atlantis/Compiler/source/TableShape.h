// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#pragma once

#include "../include/Ast.h"
#include "../include/DenseHash.h"

namespace Luau
{
namespace Compile
{

struct TableShape
{
    unsigned int arraySize = 0;
    unsigned int hashSize = 0;
};

void predictTableShapes(DenseHashMap<AstExprTable*, TableShape>& shapes, AstNode* root);

} // namespace Compile
} // namespace Luau
