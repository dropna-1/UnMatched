#pragma once 

#include <memory>

class Hero ;

class HeroFactory
{
    public : 
        static std::shared_ptr<Hero> createSherlock() ;
        static std::shared_ptr<Hero> createDracula() ;
};