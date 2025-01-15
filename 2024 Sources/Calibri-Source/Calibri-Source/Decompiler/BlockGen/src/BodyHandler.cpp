//
// Created by xgladius on 8/7/22.
//
#include "../include/BodyHandler.h"

namespace Luau::Decompiler::BlockGen {

    BodyType BodyHandler::getType() {
        return bodyTypeStack[bodyTypeStack.size() - 1];
    }

    AstStat* BodyHandler::get() {
        auto ret = generators[generators.size() - 1]->generate();
        generators.pop_back();
        bodyTypeStack.pop_back();
        return ret;
    }

    void BodyHandler::addStat(AstStat* stat) {
        generators[generators.size() - 1]->body->push_back(stat);
    }

    void BodyHandler::makeIf(AstExpr* condition) {
        bodyTypeStack.push_back(BodyType::IF);
        generators.push_back(new AstGen::AstStatIfGenerator(condition));
    }

    void BodyHandler::makeFor(AstLocal* var, AstExpr* from, AstExpr* to, AstExpr* step) {
        bodyTypeStack.push_back(BodyType::FORPREP);
        generators.push_back(new AstGen::AstStatForGenerator(
                var, from, to, step
                ));
    }

    void BodyHandler::makeWhile() {
        bodyTypeStack.push_back(BodyType::WHILE);
        generators.push_back(new AstGen::AstStatWhileGenerator());
    }

    void BodyHandler::makeMain() {
        bodyTypeStack.push_back(BodyType::MAIN);
        generators.push_back(new AstGen::BodyGenerator());
    }

    void BodyHandler::updateWhileCond(AstExpr *condition) {
        ((AstGen::AstStatWhileGenerator*)generators[generators.size() - 1])->updateCondition(condition);
    }
}
