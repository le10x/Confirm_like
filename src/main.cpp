#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    void onMyCustomLevel(CCObject* sender) {
    // Usamos la utilidad de Geode para leer desde el directorio de recursos
    // Esto es compatible con Android y Windows
    auto path = Mod::get()->getResourcesDir() / "mi_nivel.txt";
    auto data = file::readString(path);

    if (data.isErr()) {
        FLAlertLayer::create("Error", "No se pudo leer mi_nivel.txt", "OK")->show();
        return;
    }

    std::string levelString = data.unwrap();

    // Crear el objeto del nivel
    auto level = GJGameLevel::create();
    level->m_levelName = "Nivel Embebido";
    level->m_levelID = 100;
    level->m_creatorName = "le10x";
    level->m_levelString = levelString;
    
    // Configuración visual segura
    level->m_difficulty = GJDifficulty::Insane; 
    level->m_stars = 0; 
    level->m_starsRequested = 0;

    // Abrir la pantalla del nivel
    auto scene = LevelInfoLayer::scene(level, false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    
}


    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("side-menu");
        
        // 2. Corregido: Creamos el sprite y el botón de forma estándar
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        sprite->setScale(0.4f);

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::onMyCustomLevel)
        );

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }
};
