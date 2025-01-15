//
// Created by xgladius on 8/7/22.
//
#include "../../Common.h"
#include "../../AstGen/include/Generator.h"

namespace Luau::Decompiler::BlockGen {
    enum BodyType {
        MAIN = -1,
        FORPREP,
        FORGPREP,
        IF,
        WHILE
    };

    class BodyHandler {
        std::vector<BodyType> bodyTypeStack;
        std::vector<AstGen::Generator*> generators;
    public:
        BodyType getType();

        AstStat* get();

        void addStat(AstStat* stat);

        void makeIf(AstExpr* condition);

        void makeFor(AstLocal* var, AstExpr* from, AstExpr* to, AstExpr* step);

        void makeMain();

        void makeWhile();

        void updateWhileCond(AstExpr* condition);
    };
}